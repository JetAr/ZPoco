﻿//
// NetException.h
//
// $Id: //poco/1.4/Net/include/Poco/Net/NetException.h#4 $
//
// Library: Net
// Package: NetCore
// Module:  NetException
//
// Definition of the NetException class.
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


#ifndef Net_NetException_INCLUDED
#define Net_NetException_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Exception.h"


namespace Poco
{
namespace Net
{


POCO_DECLARE_EXCEPTION(Net_API, NetException, Poco::IOException)
POCO_DECLARE_EXCEPTION(Net_API, InvalidAddressException, NetException)
POCO_DECLARE_EXCEPTION(Net_API, InvalidSocketException, NetException)
POCO_DECLARE_EXCEPTION(Net_API, ServiceNotFoundException, NetException)
POCO_DECLARE_EXCEPTION(Net_API, ConnectionAbortedException, NetException)
POCO_DECLARE_EXCEPTION(Net_API, ConnectionResetException, NetException)
POCO_DECLARE_EXCEPTION(Net_API, ConnectionRefusedException, NetException)
POCO_DECLARE_EXCEPTION(Net_API, DNSException, NetException)
POCO_DECLARE_EXCEPTION(Net_API, HostNotFoundException, DNSException)
POCO_DECLARE_EXCEPTION(Net_API, NoAddressFoundException, DNSException)
POCO_DECLARE_EXCEPTION(Net_API, InterfaceNotFoundException, NetException)
POCO_DECLARE_EXCEPTION(Net_API, NoMessageException, NetException)
POCO_DECLARE_EXCEPTION(Net_API, MessageException, NetException)
POCO_DECLARE_EXCEPTION(Net_API, MultipartException, MessageException)
POCO_DECLARE_EXCEPTION(Net_API, HTTPException, NetException)
POCO_DECLARE_EXCEPTION(Net_API, NotAuthenticatedException, HTTPException)
POCO_DECLARE_EXCEPTION(Net_API, UnsupportedRedirectException, HTTPException)
POCO_DECLARE_EXCEPTION(Net_API, FTPException, NetException)
POCO_DECLARE_EXCEPTION(Net_API, SMTPException, NetException)
POCO_DECLARE_EXCEPTION(Net_API, POP3Exception, NetException)
POCO_DECLARE_EXCEPTION(Net_API, ICMPException, NetException)
POCO_DECLARE_EXCEPTION(Net_API, HTMLFormException, NetException)
POCO_DECLARE_EXCEPTION(Net_API, WebSocketException, NetException)


}
} // namespace Poco::Net


#endif // Net_NetException_INCLUDED
