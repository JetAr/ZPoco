﻿//
// HTTPServerConnectionFactory.cpp
//
// $Id: //poco/1.4/Net/src/HTTPServerConnectionFactory.cpp#1 $
//
// Library: Net
// Package: HTTPServer
// Module:  HTTPServerConnectionFactory
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Poco/Net/HTTPServerConnectionFactory.h"
#include "Poco/Net/HTTPServerConnection.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"


namespace Poco
{
namespace Net
{


HTTPServerConnectionFactory::HTTPServerConnectionFactory(HTTPServerParams::Ptr pParams, HTTPRequestHandlerFactory::Ptr pFactory):
    _pParams(pParams),
    _pFactory(pFactory)
{
    poco_check_ptr (pFactory);
}


HTTPServerConnectionFactory::~HTTPServerConnectionFactory()
{
}


TCPServerConnection* HTTPServerConnectionFactory::createConnection(const StreamSocket& socket)
{
    return new HTTPServerConnection(socket, _pParams, _pFactory);
}


}
} // namespace Poco::Net
