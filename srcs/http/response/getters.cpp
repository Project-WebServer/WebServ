/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 11:06:05 by yulpark           #+#    #+#             */
/*   Updated: 2026/03/31 23:48:18 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/http/response.hpp"

ProtocolV HTTPresponse::getProtocolV()
{
	return (_protocolV);
}
int HTTPresponse::getStatCode()
{
	return (_statusCode);
}

std::string HTTPresponse::getStatPhrase()
{
	return (_statusPhrase);
}

std::string HTTPresponse::getMsgBody()
{
	return (_msgBody);
}