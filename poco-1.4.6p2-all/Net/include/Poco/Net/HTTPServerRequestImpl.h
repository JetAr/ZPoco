﻿//
// HTTPServerRequestImpl.h
//
// $Id: //poco/1.4/Net/include/Poco/Net/HTTPServerRequestImpl.h#2 $
//
// Library: Net
// Package: HTTPServer
// Module:  HTTPServerRequestImpl
//
// Definition of the HTTPServerRequestImpl class.
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


#ifndef Net_HTTPServerRequestImpl_INCLUDED
#define Net_HTTPServerRequestImpl_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponseImpl.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/AutoPtr.h"
#include <istream>


namespace Poco
{
namespace Net
{


class HTTPServerSession;
class HTTPServerParams;
class StreamSocket;


class Net_API HTTPServerRequestImpl: public HTTPServerRequest
    /// This subclass of HTTPServerRequest is used for
    /// representing server-side HTTP requests.
    ///
    /// A HTTPServerRequest is passed to the
    /// handleRequest() method of HTTPRequestHandler.
{
public:
    HTTPServerRequestImpl(HTTPServerResponseImpl& response, HTTPServerSession& session, HTTPServerParams* pParams);
    /// Creates the HTTPServerRequestImpl, using the
    /// given HTTPServerSession.

    ~HTTPServerRequestImpl();
    /// Destroys the HTTPServerRequestImpl.

    std::istream& stream();
    /// Returns the input stream for reading
    /// the request body.
    ///
    /// The stream is valid until the HTTPServerRequestImpl
    /// object is destroyed.

    bool expectContinue() const;
    /// Returns true if the client expects a
    /// 100 Continue response.

    const SocketAddress& clientAddress() const;
    /// Returns the client's address.

    const SocketAddress& serverAddress() const;
    /// Returns the server's address.

    const HTTPServerParams& serverParams() const;
    /// Returns a reference to the server parameters.

    HTTPServerResponse& response() const;
    /// Returns a reference to the associated response.

    StreamSocket& socket();
    /// Returns a reference to the underlying socket.

    StreamSocket detachSocket();
    /// Returns the underlying socket after detaching
    /// it from the server session.

protected:
    static const std::string EXPECT;

private:
    HTTPServerResponseImpl&         _response;
    HTTPServerSession&              _session;
    std::istream*                   _pStream;
    Poco::AutoPtr<HTTPServerParams> _pParams;
    SocketAddress                   _clientAddress;
    SocketAddress                   _serverAddress;
};


//
// inlines
//
inline std::istream& HTTPServerRequestImpl::stream()
{
    poco_check_ptr (_pStream);

    return *_pStream;
}


inline const SocketAddress& HTTPServerRequestImpl::clientAddress() const
{
    return _clientAddress;
}


inline const SocketAddress& HTTPServerRequestImpl::serverAddress() const
{
    return _serverAddress;
}


inline const HTTPServerParams& HTTPServerRequestImpl::serverParams() const
{
    return *_pParams;
}


inline HTTPServerResponse& HTTPServerRequestImpl::response() const
{
    return _response;
}


}
} // namespace Poco::Net


#endif // Net_HTTPServerRequestImpl_INCLUDED
