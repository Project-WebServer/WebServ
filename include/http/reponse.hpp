/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reponse.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 17:48:05 by yulpark           #+#    #+#             */
/*   Updated: 2026/03/31 15:17:46 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <map>
#include <string>
#include <sstream>

#include "../handlers/Response.hpp"
#include "request.hpp"

class HTTPresponse
{
	public:

	private:
		// status line
		ProtocolV	_protocol_v;
		int			_statusCode;
		std::string	_statusPhrase;

		//headers
		std::map<std::string, std::string> _headerMap;

		//msg body
		std::string	_msgBody;
		// exception for msg body is when the server responds to the HEAD method but not part of the methods..
		
		std::string _response;
		
};