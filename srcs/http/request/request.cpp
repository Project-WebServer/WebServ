/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 18:51:55 by yulpark           #+#    #+#             */
/*   Updated: 2026/02/20 16:36:27 by yulpark          ###   ########.fr       */
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
			_components = COMPONENTS::HEADERS;
		}
	}
	if (_components == COMPONENTS::HEADERS)
	{
		size_t end = _buffer.find("\r\n\r\n");
		if (end != std::string::npos)
		{
			std::string header = _buffer.substr(0, end + 4);
			parseHeader(header);
			_buffer.erase(0, end + 4);
			_components = COMPONENTS::BODY;
		}
	}
	if (_components == COMPONENTS::BODY)
	{
		//
		_components = COMPONENTS::COMPLETED;
	}
}
