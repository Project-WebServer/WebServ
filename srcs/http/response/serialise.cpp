/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serialise.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 17:48:12 by yulpark           #+#    #+#             */
/*   Updated: 2026/03/31 23:56:22 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/http/response.hpp"

// input: get response
// check if has status line, header, empty line, body all together
// put the string into buffer

HTTPresponse::HTTPresponse() : _protocolV(ProtocolV::ERR), _statusPhrase(""), _msgBody(""), _response("")
{
}

HTTPresponse::~HTTPresponse()
{
}

std::string HTTPresponse::serialise(HTTPresponse response)
{
	(void)response; //temporarily
	return (_response);
}

// version -> status code -> status msg -> headers-> body
