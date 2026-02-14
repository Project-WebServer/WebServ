/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 18:51:55 by yulpark           #+#    #+#             */
/*   Updated: 2026/02/13 17:34:38 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

void HTTPrequests::feed(std::string newChunk)
{
	_buffer += newChunk;
	if (_components == COMPONENTS::REQUEST)
	{
		size_t end = _buffer.find("\r\n");
		if (end != std::string::npos)
		{
			std::string request = _buffer.substr(0, end + 2);
			parseRequest(request); // if it fails I return?
			_buffer.erase(0, end + 2);
			_components == COMPONENTS::HEADERS;
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
			_components == COMPONENTS::BODY;
		}
	}
	if (_components == COMPONENTS::BODY)
	{
		_components == COMPONENTS::COMPLETED;
	}
}
