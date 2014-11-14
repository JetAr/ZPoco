﻿//
// TimerTaskAdapter.h
//
// $Id: //poco/1.4/Util/include/Poco/Util/TimerTaskAdapter.h#1 $
//
// Library: Util
// Package: Timer
// Module:  TimerTaskAdapter
//
// Definition of the TimerTaskAdapter class template.
//
// Copyright (c) 2009, Applied Informatics Software Engineering GmbH.
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


#ifndef Util_TimerTaskAdapter_INCLUDED
#define Util_TimerTaskAdapter_INCLUDED


#include "Poco/Util/Util.h"
#include "Poco/Util/TimerTask.h"


namespace Poco
{
namespace Util
{


template <class C>
class TimerTaskAdapter: public TimerTask
    /// This class template simplifies the implementation
    /// of TimerTask objects by allowing a member function
    /// of an object to be called as task.
{
public:
    typedef void (C::*Callback)(TimerTask&);

    TimerTaskAdapter(C& object, Callback method): _pObject(&object), _method(method)
        /// Creates the TimerTaskAdapter, using the given
        /// object and its member function as task target.
        ///
        /// The member function must accept one argument,
        /// a reference to a TimerTask object.
    {
    }

    void run()
    {
        (_pObject->*_method)(*this);
    }

protected:
    ~TimerTaskAdapter()
    /// Destroys the TimerTaskAdapter.
    {
    }

private:
    TimerTaskAdapter();

    C*       _pObject;
    Callback _method;
};


}
} // namespace Poco::Util


#endif // Util_TimerTaskAdapter_INCLUDED
