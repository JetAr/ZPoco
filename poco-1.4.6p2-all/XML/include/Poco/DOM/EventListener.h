﻿//
// EventListener.h
//
// $Id: //poco/1.4/XML/include/Poco/DOM/EventListener.h#1 $
//
// Library: XML
// Package: DOM
// Module:  DOMEvents
//
// Definition of the DOM EventListener interface.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
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


#ifndef DOM_EventListener_INCLUDED
#define DOM_EventListener_INCLUDED


#include "Poco/XML/XML.h"
#include "Poco/XML/XMLString.h"


namespace Poco
{
namespace XML
{


class Event;


class XML_API EventListener
    /// The EventListener interface is the primary method for handling events. Users
    /// implement the EventListener interface and register their listener on an
    /// EventTarget using the AddEventListener method. The users should also remove
    /// their EventListener from its EventTarget after they have completed using
    /// the listener.
    ///
    /// When a Node is copied using the cloneNode method the EventListeners attached
    /// to the source Node are not attached to the copied Node. If the user wishes
    /// the same EventListeners to be added to the newly created copy the user must
    /// add them manually.
{
public:
    virtual void handleEvent(Event* evt) = 0;
    /// This method is called whenever an event occurs of the
    /// type for which the EventListener interface was registered.

protected:
    virtual ~EventListener();
};


}
} // namespace Poco::XML


#endif // DOM_EventListener_INCLUDED
