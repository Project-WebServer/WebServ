/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serialise.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 17:48:12 by yulpark           #+#    #+#             */
/*   Updated: 2026/03/31 23:34:44 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/http/response.hpp"

// input: get response
// check if has status line, header, empty line, body all together
// put the string into buffer

HTTPresponse::HTTPresponse() : _protocolv(ProtocolV::ERR), _statusPhrase(""), _httpHeaders(""),  _msgbody(""), _response("")
{
}

HTTPrequests::~HTTPrequests()
{
}

std::string serialise(HTTPresponse response)
{
	return (_response);
}

// version -> status code -> status msg -> headers-> body
