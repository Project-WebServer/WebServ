/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connectionInfo.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 16:26:38 by yulpark           #+#    #+#             */
/*   Updated: 2026/04/07 20:24:43 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/http/request.hpp"

void HTTPrequests::setConectionInfo(uint32_t ClientIP)
{
	_clientIP = ClientIP;
}
