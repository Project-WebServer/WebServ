/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 20:33:58 by yulpark           #+#    #+#             */
/*   Updated: 2026/02/13 16:58:44 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

//if you use enum it is anti-typo, so the compiler doesn't accept PST
//also more memory & speed efficient
//by using enum class, the names don't crash with other libraries because you must type METHOD::DELETE

enum class METHODS
{
	GET,
	POST,
	DELETE
};

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
	HTTP_2_0
};

class Headers
{
// takes in everything until \r\n -> double crlf
	private:
		std::string	host;
		std::string user_agent;
		std::string accept;
		std::string content_type;
		std::string content_len;
	public:
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
	private:
		std::string _buffer;
		COMPONENTS	_components; // zero at birth, maybe move it to constructor?

		METHODS		methods;
		std::string	path;
		ProtocolV	protocolv;
		Headers		headers;
		std::string	body;

		// Request line parser
		// header parser
		// body parser

	public:
		//HTTPrequests();
		//~HTTPrequests();

		void		feed(std::string newChunk);
		std::string	parseRequest(std::string request);
		std::string	parseHeader(std::string header);
		std::string	parseBody(std::string body);
		//parseSection(std::string chunk);

};
