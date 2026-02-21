/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 18:51:55 by yulpark           #+#    #+#             */
/*   Updated: 2026/02/21 17:22:54 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

HTTPrequests::HTTPrequests() : _buffer(""), _body(""), _protocolv(ProtocolV::ERR), _components(COMPONENTS::REQUEST), _methods(METHODS::ERR),  _path("")
{
}

HTTPrequests::~HTTPrequests()
{
}

void HTTPrequests::feed(std::string newChunk)
{
	_buffer += newChunk;
	if (_components == COMPONENTS::REQUEST)
	{
		size_t end = _buffer.find("\r\n");
		if (end != std::string::npos)
		{
			std::string request = _buffer.substr(0, end);
			parseRequest(request); // if it fails I return?
			_buffer.erase(0, end + 2);
		}
		_components = COMPONENTS::HEADERS;
	}
	else
	{
		//not enough data yet
		return ;
	}
	if (_components == COMPONENTS::HEADERS)
	{
		size_t end = _buffer.find("\r\n\r\n");
		if (end != std::string::npos)
		{
			std::string header = _buffer.substr(0, end + 4);
			parseHeader(header);
			_buffer.erase(0, end + 4);
			if (_contLen == 0)
				_components = COMPONENTS::COMPLETED;
			else
				_components = COMPONENTS::BODY;
		}
	}
	else
	{
		//not enough data yet
		return ;
	}
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
	{
		//not enough data yet
		return ;
	}
}

//when _components == COMPONENTS::COMPLETED check what is left in the buffer.

// after I use select() loop to tell me whether the data is already in the socket
// the you call recv(fd, buffer, ...)
// then call feed()
// once getState is completed, look at the path, if valid, build response
// then send it
