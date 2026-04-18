/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flima <flima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 20:33:58 by yulpark           #+#    #+#             */
/*   Updated: 2026/04/18 17:37:39 by flima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <iostream>
#include <map>
#include <string>
#include <sstream>

// #include "../io/Server.hpp"

//if you use enum it is anti-typo, so the compiler doesn't accept PST
//also more memory & speed efficient
//by using enum class, the names don't crash with other libraries because you must type METHOD::DELETE

// to be changed
enum class feedReturn
{
	INCOMPLETE,
	COMPLETE,
	ERROR,
	NO_HOST_ERROR,
	MAX_BODY_SIZE, // if we recieve bigger file then limits in config
	EXPECT_FAILED, // if we recieve Expect header with value other than 100-continue
	CGI_ERROR,
	UNSUPPORTED_HTTP,
	CGI_TIMEOUT,
	INTERNAL_ERROR,
	REQUEST_TIMEOUT,
	METHOD_ERROR
};

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
	//multimap allows duplicate keys
		std::multimap<std::string, std::string> _headerMap; // or maybe call it a map
	public:
		feedReturn	parseMap(std::string rawHeaderString);
		//std::string	getKeys();
		std::string	getValue(std::string key);
		void 		printHeader();
		int			countValues(std::string key);
};

enum class COMPONENTS
{
	REQUEST,
	HEADERS,
	CHUNK_BODY,
	BODY,
	COMPLETED
};


//optional body?

class HTTPrequests
{
	public:
		HTTPrequests();
		~HTTPrequests();

		enum class METHODS
		{
			GET,
			POST,
			DELETE,
			ERR
		};

		feedReturn	feed(std::string newChunk);
		feedReturn		parseRequest(std::string request);
		feedReturn		parseHeader(std::string header); // calls the Header class's function and validates?

		METHODS findMethods(std::string first);
		ProtocolV findVersion(std::string version);

		//testing
		void printRequest();
		void printMethod();
		void printVersion();
		void printHeader();
		void printBody();

		HTTPrequests::METHODS	getMethods() const;
		std::string				getPath() const;
		ProtocolV				getProtocol() const;
		Headers					getHeader() const;
		std::string				getBody() const;
		size_t					getContLen() const;
		size_t					getStatusCode() const;
		u_int32_t				getClientIP() const;
		std::string				getContType() const;
		std::string				getVersionStr();
		std::string				getMethodStr();

		void	setConectionInfo(uint32_t ClientIP);
		void	setStatusCode(feedReturn type);
		void	setMaxBodySize(size_t limit);
		feedReturn isHostValid(std::string contLen, std::string transfEncod, std::string expect);
		feedReturn handleChunkedBody();

	private:
		size_t		_maxBodySize;
		std::string _buffer;
		COMPONENTS	_components;

		METHODS		_methods;
		std::string	_path;
		ProtocolV	_protocolv;
		Headers		_header;
		std::string	_body;
		size_t		_contLen;
		int			_statusCode;
		uint32_t	_clientIP;
		std::string _contType;
		bool		_chunked;

		// Request line parser
		// header parser
		// body parser
};

bool 		isDigits(std::string str);
#endif
