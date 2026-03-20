/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reponse.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 17:48:05 by yulpark           #+#    #+#             */
/*   Updated: 2026/03/20 15:41:04 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <map>
#include <string>
#include <sstream>

#include "../handlers/Response.hpp"

//HTTP/1.1 201 Created                      <= status line: <protocol> <status-code> <reason-phrase>
//Content-Type: application/json
//Location: http://example.com/users/123

//{
//  "message": "New user created",
//  "user": {
//    "id": 123,
//    "firstName": "Example",
//    "lastName": "Person",
//    "email": "bsmth@example.com"
//  }
//}


//HTTP/1.1 200 OK
//Server: Apache                                <= Response headers
//Date: Fri, 21 Jun 2024 12:51:39 GMT
//Cache-Control:

//Content-Type:                                 <= Representation Headers
//ETag:
//Last-Modified:

//<!DOCTYPE html>
//<html lang="en"
//(more data)
