/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 18:51:55 by yulpark           #+#    #+#             */
/*   Updated: 2026/02/25 21:09:53 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

HTTPrequests::HTTPrequests() : _buffer(""), _body(""), _protocolv(ProtocolV::ERR), _components(COMPONENTS::REQUEST), _methods(METHODS::ERR),  _path("")
{
}

HTTPrequests::~HTTPrequests()
{
}

feedReturn	HTTPrequests::feed(std::string newChunk)
{
	feedReturn status;

	_buffer += newChunk;
	if (_components == COMPONENTS::REQUEST)
	{
		size_t end = _buffer.find("\r\n");
		if (end != std::string::npos)
		{
			std::string request = _buffer.substr(0, end);
			status = parseRequest(request);
			if (status != feedReturn::COMPLETE)
				return (status);
			_buffer.erase(0, end + 2);
		}
		_components = COMPONENTS::HEADERS;
	}
	else
		return feedReturn::INCOMPLETE;
	if (_components == COMPONENTS::HEADERS)
	{
		size_t end = _buffer.find("\r\n\r\n");
		if (end != std::string::npos)
		{
			std::string header = _buffer.substr(0, end + 4);
			status = parseHeader(header);
			if (status != feedReturn::COMPLETE)
				return (status);
			_buffer.erase(0, end + 4);
			if (_contLen == 0)
				_components = COMPONENTS::COMPLETED;
			else
				_components = COMPONENTS::BODY;
		}
	}
	else
		return feedReturn::INCOMPLETE;
	if (_components == COMPONENTS::BODY)
	{
		if (_buffer.length() >= _contLen)
		{
			std::string body = _buffer.substr(0, _contLen);
			_body = body;
			_buffer.erase(0, _contLen);
		}
		// only when actually done
		_components = COMPONENTS::COMPLETED;
	}
	else
		return feedReturn::INCOMPLETE;
	if (!_buffer.empty())
		std::cout << "Buffer not empty though reached the end :(" << std::endl;
	return feedReturn::COMPLETE;
}

//when _components == COMPONENTS::COMPLETED check what is left in the buffer. WJAT IF SOMETHING LEFT IN THERE???


// after I use select() loop to tell me whether the data is already in the socket
// the you call recv(fd, buffer, ...)
// then call feed()
// once getState is completed, look at the path, if valid, build response
// then send it
