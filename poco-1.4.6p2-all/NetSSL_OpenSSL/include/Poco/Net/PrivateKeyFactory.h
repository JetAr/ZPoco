﻿//
// PrivateKeyFactory.h
//
// $Id: //poco/1.4/NetSSL_OpenSSL/include/Poco/Net/PrivateKeyFactory.h#1 $
//
// Library: NetSSL_OpenSSL
// Package: SSLCore
// Module:  PrivateKeyFactory
//
// Definition of the PrivateKeyFactory class.
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


#ifndef NetSSL_PrivateKeyFactory_INCLUDED
#define NetSSL_PrivateKeyFactory_INCLUDED


#include "Poco/Net/NetSSL.h"


namespace Poco
{
namespace Net
{


class PrivateKeyPassphraseHandler;


class NetSSL_API PrivateKeyFactory
    /// A PrivateKeyFactory is responsible for creating PrivateKeyPassphraseHandlers.
    ///
    /// You don't need to access this class directly. Use the macro
    ///     POCO_REGISTER_KEYFACTORY(namespace, PrivateKeyPassphraseHandlerName)
    /// instead (see the documentation of PrivateKeyPassphraseHandler for an example).
{
public:
    PrivateKeyFactory();
    /// Creates the PrivateKeyFactory.

    virtual ~PrivateKeyFactory();
    /// Destroys the PrivateKeyFactory.

    virtual PrivateKeyPassphraseHandler* create(bool onServer) const = 0;
    /// Creates a new PrivateKeyPassphraseHandler
};


class NetSSL_API PrivateKeyFactoryRegistrar
    /// Registrar class which automatically registers PrivateKeyFactories at the PrivateKeyFactoryMgr.
    ///
    /// You don't need to access this class directly. Use the macro
    ///     POCO_REGISTER_KEYFACTORY(namespace, PrivateKeyPassphraseHandlerName)
    /// instead (see the documentation of PrivateKeyPassphraseHandler for an example).

{
public:
    PrivateKeyFactoryRegistrar(const std::string& name, PrivateKeyFactory* pFactory);
    /// Registers the PrivateKeyFactory with the given name at the factory manager.

    virtual ~PrivateKeyFactoryRegistrar();
    /// Destroys the PrivateKeyFactoryRegistrar.
};


template<typename T>
class PrivateKeyFactoryImpl: public Poco::Net::PrivateKeyFactory
{
public:
    PrivateKeyFactoryImpl()
    {
    }

    ~PrivateKeyFactoryImpl()
    {
    }

    PrivateKeyPassphraseHandler* create(bool server) const
    {
        return new T(server);
    }
};


}
} // namespace Poco::Net


// DEPRECATED: register the factory directly at the FactoryMgr:
// Poco::Net::SSLManager::instance().privateKeyFactoryMgr().setFactory(name, new Poco::Net::PrivateKeyFactoryImpl<MyKeyHandler>());
#define POCO_REGISTER_KEYFACTORY(API, PKCLS)	\
	static Poco::Net::PrivateKeyFactoryRegistrar aRegistrar(std::string(#PKCLS), new Poco::Net::PrivateKeyFactoryImpl<PKCLS>());


#endif // NetSSL_PrivateKeyFactory_INCLUDED
