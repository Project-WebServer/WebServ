/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseHeader.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 17:59:24 by yulpark           #+#    #+#             */
/*   Updated: 2026/02/14 19:11:39 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

//Host: localhost:8080\r\n                  <-- Headers
//User-Agent: curl/7.64.1\r\n
//Content-Length: 15\r\n
//\r\n      

//class Headers
//{
//// takes in everything until \r\n -> double crlf
//	private:
//		std::map<std::string, std::string> _headerMap;
//	public:
//};

void Headers::parseMap(std::string rawHeaderString)
{
	//until : is the key and from there to \r\n is the value
	// _headerMap[name] = value;
	
	std::string::iterator it = rawHeaderString.begin();
	std::string name;
	std::string value;

	while (it != rawHeaderString.end())
	{
		// what happens if I don't clear the name and value here?
		while (it != rawHeaderString.end() && *it != ':')
		{
			name += *it;
			it++;
		}
		// if we already reach the end before finding the colon -> error
		it++;
		// what happens if there are spaces?
		while (it != rawHeaderString.end() && *it != '\r')
		{
			value += *it;
			it++;
		}
		_headerMap[name] = value;
		while (it != rawHeaderString.end() && *it != '\r' || it != rawHeaderString.end() && *it != '\n')
			it++;
	}
}

std::string Headers::getValue(std::string key)
{
	return (_headerMap["key"]);
}

Headers HTTPrequests::parseHeader(std::string header)
{	
	_header.parseMap(header);
	if (_header.getValue("Host").empty())
	{
		//error, no host
		return _header; // just for now
	}
	// content length tells you how much body to read
	return _header; //for now
}

