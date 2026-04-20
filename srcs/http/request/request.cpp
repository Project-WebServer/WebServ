/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 18:51:55 by yulpark           #+#    #+#             */
/*   Updated: 2026/04/20 16:30:44 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/http/request.hpp"

HTTPrequests::HTTPrequests() : _maxBodySize(0), _buffer(""),
	_components(COMPONENTS::REQUEST),
	_methods(METHODS::ERR), _path(""),
	_protocolv(ProtocolV::ERR),
	_body(""), _contLen(0),
	_statusCode(200),
	_contType(""), _chunked(false)
{
}

HTTPrequests::~HTTPrequests()
{
}

feedReturn HTTPrequests::feed(std::string newChunk)
{
    feedReturn status;

    _buffer += newChunk;
    if (_components == COMPONENTS::REQUEST)
    {
        status = setRequestLine();
        if (status != feedReturn::COMPLETE)
            return (status);
    }
	if (_components == COMPONENTS::HEADERS)
	{
		status = handleHeaders();
		if (status != feedReturn::COMPLETE)
			return (status);
		if (_maxBodySize > 0 && _contLen > _maxBodySize)
			return feedReturn::MAX_BODY_SIZE;
		setComponents();
	}
	if (_components == COMPONENTS::CHUNK_BODY)
	{
		status = handleChunkedBody();
		if (status != feedReturn::COMPLETE)
			return status;
		_components = COMPONENTS::COMPLETED;
	}
    if (_components == COMPONENTS::BODY)
    {
        if (_buffer.length() < _contLen)
			return feedReturn::INCOMPLETE;
        _body = _buffer.substr(0, _contLen);
        _buffer.erase(0, _contLen);
        _components = COMPONENTS::COMPLETED;
    }
    if (!_buffer.empty())
       return feedReturn::NO_HOST_ERROR;
    return feedReturn::COMPLETE;
}
