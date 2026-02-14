/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 17:52:06 by yulpark           #+#    #+#             */
/*   Updated: 2026/02/14 19:12:19 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

void Headers::print()
{
    std::cout << "--- Headers ---" << std::endl;
    for (std::map<std::string, std::string>::const_iterator it = _headerMap.begin(); it != _headerMap.end(); ++it)
    {
        std::cout << it->first << " : " << it->second << std::endl;
    }
}

void HTTPrequests::printRequest()
{
    std::cout << "--- Request Line ---" << std::endl;
    
    std::cout << "Method: ";
    if (_methods == METHODS::GET) std::cout << "GET";
    else if (_methods == METHODS::POST) std::cout << "POST";
    else if (_methods == METHODS::DELETE) std::cout << "DELETE";
    else std::cout << "UNKNOWN";
    std::cout << std::endl;

    std::cout << "Path: " << _path << std::endl;
    
    std::cout << "Version: ";
    if (_protocolv == ProtocolV::HTTP_1_1) std::cout << "HTTP/1.1";
    else std::cout << "Other";
    std::cout << std::endl;

    // Now print the headers
    _header.print();
}


int main()
{
    HTTPrequests req;
    std::string raw = 
        "GET /index.html HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "User-Agent: Mozilla/5.0\r\n"
        "Accept: text/html\r\n"
        "Content-Length: 0\r\n"
        "\r\n"; // The empty line marks the end of headers


    req.feed(raw);
    req.printRequest();

    return 0;
}