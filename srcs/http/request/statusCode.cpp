/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   statusCode.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 17:33:38 by yulpark           #+#    #+#             */
/*   Updated: 2026/04/17 16:59:01 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/http/request.hpp"

//parseHeader - feedReturn::NO_HOST_ERROR then 400
//            - return feedReturn::COMPLETE; then 200
//parseRL  - INCOMPLETE
//         - feedReturn::ERROR then either 405 method not allowed or 505 http version unsupported

void HTTPrequests::setStatusCode(feedReturn type)
{
	if (type == feedReturn::NO_HOST_ERROR)
		_statusCode = 400;
	else if (type == feedReturn::ERROR)
		_statusCode = 405;
	else if (type == feedReturn::MAX_BODY_SIZE)
		_statusCode = 413;
	else if (type == feedReturn::EXPECT_FAILED)
		_statusCode = 417;
	else
		_statusCode = 200;
}

//std::string Response::getHttpCode(int code)
//{
//	switch (code)
//	{
//		case 200: return " OK";                    // Request succeeded
//		case 201: return " Created";               // Resource created successfully
//		case 301: return " Moved Permanently";     // Resource moved to a new URL
//		case 400: return " Bad Request";           // Malformed or invalid request
//		case 403: return " Forbidden";             // No permission to access the resource
//		case 404: return " Not Found";             // Resource does not exist
//		case 405: return " Method Not Allowed";    // HTTP method not supported for this route
//		case 408: return " Request Timeout";       // Client took too long to send the request
//		case 409: return " Conflict";              // Request conflicts with current resource state
//		case 413: return " Payload Too Large";     // Body exceeds client_max_body_size
//		case 415: return " Unsupported Media Type"; // Content-Type not supported
//		case 500: return " Internal Server Error"; // Unexpected server-side failure
//		case 502: return " Bad Gateway";           // CGI returned an invalid response
//		case 504: return " Gateway Timeout";       // CGI took too long to respond
//	}
//	return "";
//}
