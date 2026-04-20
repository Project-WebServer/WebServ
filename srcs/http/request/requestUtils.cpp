/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 16:17:01 by yulpark           #+#    #+#             */
/*   Updated: 2026/04/20 16:30:34 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/http/request.hpp"

feedReturn HTTPrequests::handleChunkedBody()
{
	while (true)
	{
		if (_buffer.substr(0, 5) == "0\r\n\r\n")
		{
			_buffer.erase(0, 5);
			return feedReturn::COMPLETE;
		}
		size_t marker1 = _buffer.find("\r\n");
		if (marker1 == std::string::npos)
			return feedReturn::INCOMPLETE;
		size_t chunkSize;
		std::string size = _buffer.substr(0, marker1);
		try
		{
			chunkSize = std::stoul(size, nullptr, 16);
		}
		catch(const std::exception& e)
		{
			return feedReturn::NO_HOST_ERROR;
		}
		if (chunkSize == 0)
		{
			_buffer.erase(0, marker1+ 4);  //4 being 0\r\n\r\n
			return feedReturn::COMPLETE;
		}
		if (_buffer.size() < marker1 + 2 + chunkSize + 2)
        	return feedReturn::INCOMPLETE;
		_body += _buffer.substr(marker1 + 2, chunkSize);
		_buffer.erase(0, chunkSize + marker1 + 4);
	}
	return feedReturn::COMPLETE;
}

feedReturn HTTPrequests::handleHeaders()
{
	size_t end = _buffer.find("\r\n\r\n");
	if (end == std::string::npos)
		return feedReturn::NO_HOST_ERROR;
	std::string header = _buffer.substr(0, end + 4);
	feedReturn status = parseHeader(header);
	_buffer.erase(0, end + 4);
	return (status);
}

void HTTPrequests::setComponents()
{
	if (_chunked)
		_components = COMPONENTS::CHUNK_BODY;
	else if (_contLen == 0)
		_components = COMPONENTS::COMPLETED;
	else
		_components = COMPONENTS::BODY;
}

void HTTPrequests::setMaxBodySize(size_t limit)
{
	_maxBodySize = limit;
}

feedReturn HTTPrequests::setRequestLine()
{
	size_t end = _buffer.find("\r\n");
	if (end == std::string::npos)
		return feedReturn::NO_HOST_ERROR;
	std::string request = _buffer.substr(0, end);
	if (request.empty())
		return feedReturn::NO_HOST_ERROR;
	feedReturn status = parseRequest(request);
	_buffer.erase(0, end + 2);
	_components = COMPONENTS::HEADERS;
	return (status);
}
