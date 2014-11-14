﻿//
// ConsoleCertificateHandler.h
//
// $Id: //poco/1.4/NetSSL_OpenSSL/include/Poco/Net/ConsoleCertificateHandler.h#1 $
//
// Library: NetSSL_OpenSSL
// Package: SSLCore
// Module:  ConsoleCertificateHandler
//
// Definition of the ConsoleCertificateHandler class.
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


#ifndef NetSSL_ConsoleCertificateHandler_INCLUDED
#define NetSSL_ConsoleCertificateHandler_INCLUDED


#include "Poco/Net/NetSSL.h"
#include "Poco/Net/InvalidCertificateHandler.h"


namespace Poco
{
namespace Net
{


class NetSSL_API ConsoleCertificateHandler: public InvalidCertificateHandler
    /// A ConsoleCertificateHandler is invoked whenever an error occurs verifying the certificate.
    ///
    /// The certificate is printed to stdout and the user is asked via console if he wants to accept it.
{
public:
    ConsoleCertificateHandler(bool handleErrorsOnServerSide);
    /// Creates the ConsoleCertificateHandler.

    virtual ~ConsoleCertificateHandler();
    /// Destroys the ConsoleCertificateHandler.

    void onInvalidCertificate(const void* pSender, VerificationErrorArgs& errorCert);
    /// Prints the certificate to stdout and waits for user input on the console
    /// to decide if a certificate should be accepted/rejected.
};


}
} // namespace Poco::Net


#endif // NetSSL_ConsoleCertificateHandler_INCLUDED
