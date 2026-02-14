/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 20:33:58 by yulpark           #+#    #+#             */
/*   Updated: 2026/02/14 18:04:29 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <map>
#include <string>

//if you use enum it is anti-typo, so the compiler doesn't accept PST
//also more memory & speed efficient
//by using enum class, the names don't crash with other libraries because you must type METHOD::DELETE

enum class StatCode
//indicates whether the HTTP request has been succesfully completed
{
	INFORM, //100-199
	SUCCESS, //200-299
	REDIRECT, //300-399
	CLIENT_ERR, //400-499
	SERVER_ERR //500-599
};
//maybe a class for errors

enum class ProtocolV
{
	HTTP_1_0,
	HTTP_1_1,
	HTTP_2_0,
	ERR
};

// using map is like dict in python so you can map key and value
class Headers
{
// takes in everything until \r\n -> double crlf
	private:
		std::map<std::string, std::string> _headerMap; // or maybe call it a map
	public:
		void parseMap(std::string rawHeaderString);
};

enum class COMPONENTS
{
	REQUEST,
	HEADERS,
	BODY,
	COMPLETED
};


//optional body?

class HTTPrequests
{
	public:
		//HTTPrequests();
		//~HTTPrequests();

		enum class METHODS
		{
			GET,
			POST,
			DELETE,
			ERR
		};
		
		void		feed(std::string newChunk);
		void		parseRequest(std::string request);
		Headers		parseHeader(std::string header); // calls the Header class's function and validates?
		std::string	parseBody(std::string body);
		
		HTTPrequests::METHODS findMethods(std::string first);
		ProtocolV HTTPrequests::findVersion(std::string version);
		
	private:
		std::string _buffer;
		COMPONENTS	_components; // zero at birth, maybe move it to constructor?

		METHODS		_methods;
		std::string	_path;
		ProtocolV	_protocolv;
		Headers		_headers;
		std::string	_body;

		// Request line parser
		// header parser
		// body parser
};
