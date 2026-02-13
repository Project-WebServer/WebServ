/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 18:51:55 by yulpark           #+#    #+#             */
/*   Updated: 2026/02/13 17:04:36 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <request.hpp>

void HTTPrequests::feed(std::string newChunk)
{
	_buffer += newChunk;
	if (_components == COMPONENTS::REQUEST)  //maybe don't use if else here? but case?
	{
		size_t end = _buffer.find("\r\n");
		if (end == std::string::npos)
			return ; // get more later from IOa
		std::string request = _buffer.substr(end + 1);
		parseRequest(request); // if it fails I return?
		_buffer.erase(0, end);
		_components == COMPONENTS::HEADERS;
	}
	if (_components == COMPONENTS::HEADERS)
	{
		size_t end = _buffer.find("\r\n\r\n");
		if (end == std::string::npos)
			return ;
		std::string header = _buffer.substr(end + 1);
		parseHeader(header);
		_components == COMPONENTS::BODY;
	}
	if (_components == COMPONENTS::BODY)
	{
		_components == COMPONENTS::COMPLETED;
	}
}
