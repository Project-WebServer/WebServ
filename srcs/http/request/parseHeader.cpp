/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseHeader.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 17:59:24 by yulpark           #+#    #+#             */
/*   Updated: 2026/02/14 18:07:31 by yulpark          ###   ########.fr       */
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
}

Headers HTTPrequests::parseHeader(std::string header)
{
	// if "/" the default?
	
	return ;
}

