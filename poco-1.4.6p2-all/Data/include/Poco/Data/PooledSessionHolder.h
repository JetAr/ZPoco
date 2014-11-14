﻿//
// PooledSessionHolder.h
//
// $Id: //poco/1.4/Data/include/Poco/Data/PooledSessionHolder.h#1 $
//
// Library: Data
// Package: SessionPooling
// Module:  PooledSessionHolder
//
// Definition of the PooledSessionHolder class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
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


#ifndef Data_PooledSessionHolder_INCLUDED
#define Data_PooledSessionHolder_INCLUDED


#include "Poco/Data/Data.h"
#include "Poco/Data/SessionImpl.h"
#include "Poco/AutoPtr.h"
#include "Poco/Timestamp.h"
#include "Poco/Mutex.h"


namespace Poco
{
namespace Data
{


class SessionPool;


class Data_API PooledSessionHolder: public Poco::RefCountedObject
    /// This class is used by SessionPool to manage SessionImpl objects.
{
public:
    PooledSessionHolder(SessionPool& owner, SessionImpl* pSessionImpl);
    /// Creates the PooledSessionHolder.

    ~PooledSessionHolder();
    /// Destroys the PooledSessionHolder.

    SessionImpl* session();
    /// Returns a pointer to the SessionImpl.

    SessionPool& owner();
    /// Returns a reference to the SessionHolder's owner.

    void access();
    /// Updates the last access timestamp.

    int idle() const;
    /// Returns the number of seconds the session has not been used.

private:
    SessionPool& _owner;
    Poco::AutoPtr<SessionImpl> _pImpl;
    Poco::Timestamp _lastUsed;
    mutable Poco::FastMutex _mutex;
};


//
// inlines
//
inline SessionImpl* PooledSessionHolder::session()
{
    return _pImpl;
}


inline SessionPool& PooledSessionHolder::owner()
{
    return _owner;
}


inline void PooledSessionHolder::access()
{
    Poco::FastMutex::ScopedLock lock(_mutex);

    _lastUsed.update();
}


inline int PooledSessionHolder::idle() const
{
    Poco::FastMutex::ScopedLock lock(_mutex);

    return (int) (_lastUsed.elapsed()/Poco::Timestamp::resolution());
}


}
} // namespace Poco::Data


#endif // Data_PooledSessionHolder_INCLUDED
