﻿//
// InvalidCertificateHandler.h
//
// $Id: //poco/1.4/NetSSL_OpenSSL/include/Poco/Net/InvalidCertificateHandler.h#1 $
//
// Library: NetSSL_OpenSSL
// Package: SSLCore
// Module:  InvalidCertificateHandler
//
// Definition of the InvalidCertificateHandler class.
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
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


#ifndef NetSSL_InvalidCertificateHandler_INCLUDED
#define NetSSL_InvalidCertificateHandler_INCLUDED


#include "Poco/Net/NetSSL.h"
#include "Poco/Net/VerificationErrorArgs.h"


namespace Poco
{
namespace Net
{


class NetSSL_API InvalidCertificateHandler
    /// A InvalidCertificateHandler is invoked whenever an error occurs verifying the certificate. It allows the user
    /// to inspect and accept/reject the certificate.
    /// One can install one's own InvalidCertificateHandler by implementing this interface. Note that
    /// in the implementation file of the subclass the following code must be present (assuming you use the namespace My_API
    /// and the name of your handler class is MyGuiHandler):
    ///
    ///    #include "Poco/Net/CertificateHandlerFactory.h"
    ///    ...
    ///    POCO_REGISTER_CHFACTORY(My_API, MyGuiHandler)
    ///
    /// One can either set the handler directly in the startup code of the main method of ones application by calling
    ///
    ///    SSLManager::instance().initialize(mypassphraseHandler, myguiHandler, mySSLContext)
    ///
    /// or in case one uses Poco::Util::Application one can rely on an XML configuration and put the following entry
    /// under the path openSSL.invalidCertificateHandler:
    ///
    ///    <invalidCertificateHandler>
    ///        <name>MyGuiHandler<name>
    ///        <options>
    ///            [...] // Put optional config params for the handler here
    ///        </options>
    ///    </invalidCertificateHandler>
    ///
    /// Note that the name of the InvalidCertificateHandler must be same as the one provided to the POCO_REGISTER_CHFACTORY macro.
{
public:
    InvalidCertificateHandler(bool handleErrorsOnServerSide);
    /// Creates the InvalidCertificateHandler.
    ///
    /// Set handleErrorsOnServerSide to true if the certificate handler is used on the server side.
    /// Automatically registers at one of the SSLManager::VerificationError events.

    virtual ~InvalidCertificateHandler();
    /// Destroys the InvalidCertificateHandler.

    virtual void onInvalidCertificate(const void* pSender, VerificationErrorArgs& errorCert) = 0;
    /// Receives the questionable certificate in parameter errorCert. If one wants to accept the
    /// certificate, call errorCert.setIgnoreError(true).

protected:
    bool _handleErrorsOnServerSide;
    /// Stores if the certificate handler gets invoked by the server (i.e. a client certificate is wrong)
    /// or the client (a server certificate is wrong)
};


}
} // namespace Poco::Net


#endif // NetSSL_InvalidCertificateHandler_INCLUDED
