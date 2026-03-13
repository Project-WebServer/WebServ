/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   statusCode.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 17:33:38 by yulpark           #+#    #+#             */
/*   Updated: 2026/03/13 16:30:28 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/http/request.hpp"

// if no host error => 400
// unsupported protocol version
// use static keyword for the map
//Now you need to make the server "speak".

//The Map: Inside a function (like getReasonPhrase), create a std::map<size_t, std::string>.

//The Pairing: Add the standard codes:

//200 -> "OK"

//400 -> "Bad Request"

//404 -> "Not Found"

//500 -> "Internal Server Error"

//The Static Trick: Use the static keyword for the map so that the "dictionary" is only built once in memory, rather than being recreated every time you have an error.

//The Search: Use the .find() method of the map to look up the phrase. If it’s not found, return a default "Internal Server Error".


//std::string getReasonPhrase(size_t code) {
//    static std::map<size_t, std::string> codes; // The 'static' makes it live forever
//    if (codes.empty()) {
//        // Fill the map only ONCE the first time the function is called
//        codes[200] = "OK";
//        codes[404] = "Not Found";
//    }
//    // ... search and return ...
//}
