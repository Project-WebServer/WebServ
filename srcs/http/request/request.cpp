/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flima <flima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 18:51:55 by yulpark           #+#    #+#             */
/*   Updated: 2026/04/08 20:10:26 by flima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/http/request.hpp"

HTTPrequests::HTTPrequests() : _maxBodySize(0), _buffer(""), 
	_components(COMPONENTS::REQUEST), 
	_methods(METHODS::ERR), _path(""), 
	_protocolv(ProtocolV::ERR),  
	_body(""), _contLen(0), 
	_statusCode(0), 
	_contType("")
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
        size_t end = _buffer.find("\r\n");
        if (end == std::string::npos)
			return feedReturn::INCOMPLETE;
        std::string request = _buffer.substr(0, end);
        status = parseRequest(request);
        if (status != feedReturn::COMPLETE)
            return (status);
        _buffer.erase(0, end + 2);
        _components = COMPONENTS::HEADERS;
    }

    if (_components == COMPONENTS::HEADERS)
    {
        size_t end = _buffer.find("\r\n\r\n");
        if (end == std::string::npos)
			return feedReturn::INCOMPLETE;
        std::string header = _buffer.substr(0, end + 4);
        status = parseHeader(header);
        if (status != feedReturn::COMPLETE)
            return (status);
        _buffer.erase(0, end + 4);
		if (_maxBodySize > 0 && _contLen > _maxBodySize)
			return feedReturn::MAX_BODY_SIZE;
        if (_contLen == 0)
            _components = COMPONENTS::COMPLETED;
        else
            _components = COMPONENTS::BODY;
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
        std::cout << "Buffer not empty though reached the end :(" << std::endl;
    return feedReturn::COMPLETE;
}

void HTTPrequests::setMaxBodySize(size_t limit)
{
    _maxBodySize = limit;
}

//when _components == COMPONENTS::COMPLETED check what is left in the buffer. WJAT IF SOMETHING LEFT IN THERE???


// after I use select() loop to tell me whether the data is already in the socket
// the you call recv(fd, buffer, ...)
// then call feed()
// once getState is completed, look at the path, if valid, build response
// then send it
