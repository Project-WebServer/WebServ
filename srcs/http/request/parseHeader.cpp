/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseHeader.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 17:59:24 by yulpark           #+#    #+#             */
/*   Updated: 2026/03/06 17:19:56 by yulpark          ###   ########.fr       */
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

static std::string toLower(std::string name)
{
	std::string lowercased = "";
	int N = name.length();
	for (int i = 0; i < N; i++)
		lowercased += tolower(name[i]);
	return (lowercased);
}

void Headers::parseMap(std::string rawHeaderString)
{
	//until : is the key and from there to \r\n is the value
	// _headerMap[name] = value;

	std::string::iterator it = rawHeaderString.begin();

	while (it != rawHeaderString.end())
	{
		std::string name;
		std::string value;

		while (it != rawHeaderString.end() && (*it == ' ' || *it == '\r' || *it == '\n'))
			it++;
		if (it == rawHeaderString.end())
			break;
			// what happens if I don't clear the name and value here
		while (it != rawHeaderString.end() && *it != ':')
		{
			name += *it;
			it++;
		}
		if (it == rawHeaderString.end() || name.empty())
			break;
		while (it != rawHeaderString.end() && (*it == ' ' || *it == ':'))
			it++;
		while (it != rawHeaderString.end() && *it != '\r' && *it != '\n')
		{
			value += *it;
			it++;
		}
		_headerMap[toLower(name)] = value;

		//std::cout << "[DEBUG] Found Key: '" << name << "' | Found Value: '" << value << "'" << std::endl;
	}
}

std::string Headers::getValue(std::string key)
{
	return (_headerMap[key]);
}

feedReturn HTTPrequests::parseHeader(std::string header)
{
	_header.parseMap(header);
	if (_header.getValue("host").empty())
	{
		//error, no host
		return feedReturn::NO_HOST_ERROR;
	}
	// content length tells you how much body to read
	std::string contLen = _header.getValue("content-length");
	std::stringstream stream(contLen);
	stream >> _contLen;
	return feedReturn::COMPLETE; // for now, later add other error cases
}
