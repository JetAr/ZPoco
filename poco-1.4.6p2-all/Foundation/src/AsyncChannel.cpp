﻿//
// AsyncChannel.cpp
//
// $Id: //poco/1.4/Foundation/src/AsyncChannel.cpp#2 $
//
// Library: Foundation
// Package: Logging
// Module:  AsyncChannel
//
// Copyright (c) 2004-2007, Applied Informatics Software Engineering GmbH.
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


#include "Poco/AsyncChannel.h"
#include "Poco/Notification.h"
#include "Poco/Message.h"
#include "Poco/Formatter.h"
#include "Poco/AutoPtr.h"
#include "Poco/LoggingRegistry.h"
#include "Poco/Exception.h"


namespace Poco
{


class MessageNotification: public Notification
{
public:
    MessageNotification(const Message& msg):
        _msg(msg)
    {
    }

    ~MessageNotification()
    {
    }

    const Message& message() const
    {
        return _msg;
    }

private:
    Message _msg;
};


AsyncChannel::AsyncChannel(Channel* pChannel, Thread::Priority prio):
    _pChannel(pChannel),
    _thread("AsyncChannel")
{
    if (_pChannel) _pChannel->duplicate();
    _thread.setPriority(prio);
}


AsyncChannel::~AsyncChannel()
{
    close();
    if (_pChannel) _pChannel->release();
}


void AsyncChannel::setChannel(Channel* pChannel)
{
    FastMutex::ScopedLock lock(_channelMutex);

    if (_pChannel) _pChannel->release();
    _pChannel = pChannel;
    if (_pChannel) _pChannel->duplicate();
}


Channel* AsyncChannel::getChannel() const
{
    return _pChannel;
}


void AsyncChannel::open()
{
    FastMutex::ScopedLock lock(_threadMutex);

    if (!_thread.isRunning())
        _thread.start(*this);
}


void AsyncChannel::close()
{
    if (_thread.isRunning())
    {
        while (!_queue.empty()) Thread::sleep(100);

        do
        {
            _queue.wakeUpAll();
        }
        while (!_thread.tryJoin(100));
    }
}


void AsyncChannel::log(const Message& msg)
{
    open();

    _queue.enqueueNotification(new MessageNotification(msg));
}


void AsyncChannel::setProperty(const std::string& name, const std::string& value)
{
    if (name == "channel")
        setChannel(LoggingRegistry::defaultRegistry().channelForName(value));
    else if (name == "priority")
        setPriority(value);
    else
        Channel::setProperty(name, value);
}


void AsyncChannel::run()
{
    AutoPtr<Notification> nf = _queue.waitDequeueNotification();
    while (nf)
    {
        MessageNotification* pNf = dynamic_cast<MessageNotification*>(nf.get());
        {
            FastMutex::ScopedLock lock(_channelMutex);

            if (pNf && _pChannel) _pChannel->log(pNf->message());
        }
        nf = _queue.waitDequeueNotification();
    }
}


void AsyncChannel::setPriority(const std::string& value)
{
    Thread::Priority prio = Thread::PRIO_NORMAL;

    if (value == "lowest")
        prio = Thread::PRIO_LOWEST;
    else if (value == "low")
        prio = Thread::PRIO_LOW;
    else if (value == "normal")
        prio = Thread::PRIO_NORMAL;
    else if (value == "high")
        prio = Thread::PRIO_HIGH;
    else if (value == "highest")
        prio = Thread::PRIO_HIGHEST;
    else
        throw InvalidArgumentException("thread priority", value);

    _thread.setPriority(prio);
}


} // namespace Poco
