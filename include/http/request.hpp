/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 20:33:58 by yulpark           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2026/03/20 15:49:19 by yulpark          ###   ########.fr       */
=======
/*   Updated: 2026/03/24 15:10:44 by ypark            ###   ########.fr       */
>>>>>>> 4594afe279bf2f63bb7a54ccd958e0708de7f422
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
	NO_HOST_ERROR
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
		std::map<std::string, std::string> _headerMap; // or maybe call it a map
	public:
		feedReturn	parseMap(std::string rawHeaderString);
		//std::string	getKeys();
		std::string	getValue(std::string key);
		void 		printHeader();
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
		int						getServerPort() const;
<<<<<<< HEAD
		std::string				getServerIP() const;
		std::string				getClientIP() const;
=======
		u_int32_t				getServerIP() const;
		u_int32_t				getClientIP() const;
		std::string				getContType() const;
>>>>>>> 4594afe279bf2f63bb7a54ccd958e0708de7f422

		void	setConectionInfo(uint32_t ServerIP, uint32_t ClientIP, int Port);
		void	statusCode(feedReturn type);


	private:
		std::string _buffer;
		COMPONENTS	_components;

		METHODS		_methods;
		std::string	_path;
		ProtocolV	_protocolv;
		Headers		_header;
		std::string	_body;
		size_t		_contLen;
<<<<<<< HEAD
		int			_statusCode;
=======
		size_t		_statusCode;
		std::string _contType;

>>>>>>> 4594afe279bf2f63bb7a54ccd958e0708de7f422
		int			_serverPort;
		uint32_t	_serverIP;
		uint32_t _clientIP;

		// Request line parser
		// header parser
		// body parser
};


#endif