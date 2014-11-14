﻿//
// FIFOEvent.h
//
// $Id: //poco/1.4/Foundation/include/Poco/FIFOEvent.h#2 $
//
// Library: Foundation
// Package: Events
// Module:  FIFOEvent
//
// Implementation of the FIFOEvent template.
//
// Copyright (c) 2006-2011, Applied Informatics Software Engineering GmbH.
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


#ifndef Foundation_FIFOEvent_INCLUDED
#define Foundation_FIFOEvent_INCLUDED


#include "Poco/AbstractEvent.h"
#include "Poco/FIFOStrategy.h"
#include "Poco/AbstractDelegate.h"


namespace Poco
{


//@ deprecated
template <class TArgs, class TMutex = FastMutex>
class FIFOEvent: public AbstractEvent <
    TArgs,
    FIFOStrategy<TArgs, AbstractDelegate<TArgs> >,
    AbstractDelegate<TArgs>,
    TMutex
    >
    /// A FIFOEvent uses internally a FIFOStrategy which guarantees
    /// that delegates are invoked in the order they were added to
    /// the event.
    ///
    /// Note that as of release 1.4.2, this is the default behavior
    /// implemented by BasicEvent, so this class is provided
    /// for backwards compatibility only.
{
public:
    FIFOEvent()
    {
    }

    ~FIFOEvent()
    {
    }

private:
    FIFOEvent(const FIFOEvent& e);
    FIFOEvent& operator = (const FIFOEvent& e);
};


} // namespace Poco


#endif // Foundation_FIFOEvent_INCLUDED
