/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseHeader.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 17:59:24 by yulpark           #+#    #+#             */
/*   Updated: 2026/04/20 16:14:47 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/http/request.hpp"

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

std::string toLower(std::string name)
{
	std::string lowercased = "";
	int N = name.length();
	for (int i = 0; i < N; i++)
		lowercased += tolower(name[i]);
	return (lowercased);
}

feedReturn Headers::parseMap(std::string rawHeaderString)
{
	//until : is the key and from there to \r\n is the value

	std::string::iterator it = rawHeaderString.begin();
	while (it != rawHeaderString.end())
	{
		std::string name;
		std::string value;

		while (it != rawHeaderString.end() && (*it == ' ' || *it == '\r' || *it == '\n'))
			it++;
		if (it == rawHeaderString.end())
			break;
		while (it != rawHeaderString.end() && *it != ':')
		{
			if (*it == ' ')
				return feedReturn::ERROR;
			name += *it;
			it++;
		}
		if (it == rawHeaderString.end() || name.empty())
			return feedReturn::NO_HOST_ERROR;
		while (it != rawHeaderString.end() && (*it == ' ' || *it == ':'))
			it++;
		while (it != rawHeaderString.end() && *it != '\r' && *it != '\n')
		{
			value += *it;
			it++;
		}
		_headerMap.insert(std::make_pair(toLower(name), value));
	}
	return feedReturn::COMPLETE;
}
bool isDigits(std::string str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (!isdigit(str[i]))
			return false;
	}
	return true;
}

std::string Headers::getValue(std::string key)
{
	auto it = _headerMap.find(toLower(key));
	if (it != _headerMap.end())
		return it->second;
	return ("");
}

int Headers::countValues(std::string key)
{
	return (_headerMap.count(key));
}

feedReturn HTTPrequests::parseHeader(std::string header)
{
	if (_header.parseMap(header) != feedReturn::COMPLETE)
		return feedReturn::NO_HOST_ERROR;
	std::string contLen     = _header.getValue("content-length");
	if (!contLen.empty() && !isDigits(contLen))
		return feedReturn::NO_HOST_ERROR;
	std::string transfEncod = _header.getValue("transfer-encoding");
	_contType               = _header.getValue("content-type");
	std::string expect = _header.getValue("expect");
	if (!contLen.empty())
	{
		std::stringstream stream(contLen);
		if (!(stream >> _contLen) || _contLen < 0)
		{
			_statusCode = 400;
			return feedReturn::ERROR;
		}
	}
	else
		_contLen = 0;
	return isHostValid(contLen, transfEncod, expect);
}

feedReturn HTTPrequests::isHostValid(std::string contLen, std::string transfEncod, std::string expect)
{
	if (_header.countValues("host")!= 1)
   		return feedReturn::NO_HOST_ERROR;
	if (_header.getValue("host").empty())
		return feedReturn::NO_HOST_ERROR;
	if (_header.countValues("content-length") > 1 || _header.countValues("transfer-encoding") > 1)
		return feedReturn::NO_HOST_ERROR;
	if (!contLen.empty() && !transfEncod.empty())
    	return feedReturn::NO_HOST_ERROR;
	if (!transfEncod.empty() && transfEncod != "chunked")
   		return feedReturn::ERROR;
	if (transfEncod == "chunked")
		_chunked = true;
	if (!expect.empty())
		return feedReturn::EXPECT_FAILED;
	return feedReturn::COMPLETE;
}
