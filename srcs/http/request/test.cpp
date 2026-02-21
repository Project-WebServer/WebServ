/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 17:52:06 by yulpark           #+#    #+#             */
/*   Updated: 2026/02/21 15:19:36 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

void HTTPrequests::printRequest()
{
    std::cout << "Request Line:" << std::endl;

    std::cout << "Method: ";
    printMethod();
    std::cout << "Path: " << _path << std::endl;
    std::cout << "Version: ";
    printVersion();
    // Headers printing
}

void Headers::printHeader()
{
	std::map<std::string, std::string>::iterator it;
    std::cout << "Header Line:" << std::endl;
	for (it = _headerMap.begin() ; it != _headerMap.end(); it++)
    {
		std::cout << "" << it->first << ": " << it->second
             << "" << std::endl;
	}
	// Headers printing
}

void HTTPrequests::printHeader()
{
	_header.printHeader();
}

void HTTPrequests::printMethod()
{
    switch (_methods)
    {
        case METHODS::GET:    std::cout << "GET";    break;
        case METHODS::POST:   std::cout << "POST";   break;
        case METHODS::DELETE: std::cout << "DELETE"; break;
        case METHODS::ERR:    std::cout << "ERR";    break;
    }
    std::cout << std::endl;
}

void HTTPrequests::printVersion()
{
    switch (_protocolv)
    {
        case ProtocolV::HTTP_1_0:    std::cout << "1.0";    break;
        case ProtocolV::HTTP_1_1:    std::cout << "1.1";   break;
        case ProtocolV::HTTP_2_0:  std::cout << "2.0"; break;
        case ProtocolV::ERR:    std::cout << "ERR";    break;
    }
    std::cout << std::endl;
}

void HTTPrequests::printBody()
{
	std::cout << "Body Line:" << std::endl;
	std::cout << _body << std::endl;
}

int main()
{
	HTTPrequests req;
	std::string raw = "POST /users HTTP/1.1\r\n"
	"Host: example.com\r\n"
	"Content-Type: application/x-www-form-urlencoded\r\n"
	"Content-Length: 49\r\n"
	"\r\n"
	"name=FirstName+LastName&email=bsmth%40example.com";

	req.feed(raw);
	req.printRequest();
	std::cout << std::endl;
	req.printHeader();
	std::cout << std::endl;
	req.printBody();
	return 0;
}
