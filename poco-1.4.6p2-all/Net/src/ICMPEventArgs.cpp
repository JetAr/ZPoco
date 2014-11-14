﻿//
// ICMPEventArgs.cpp
//
// $Id: //poco/1.4/Net/src/ICMPEventArgs.cpp#1 $
//
// Library: Net
// Package: ICMP
// Module:  ICMPEventArgs
//
// Implementation of ICMPEventArgs
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


#include "Poco/Net/ICMPEventArgs.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/DNS.h"
#include "Poco/Exception.h"
#include "Poco/Net/NetException.h"
#include <numeric>


using Poco::IOException;
using Poco::InvalidArgumentException;


namespace Poco
{
namespace Net
{


ICMPEventArgs::ICMPEventArgs(const SocketAddress& address, int repetitions, int dataSize, int ttl):
    _address(address),
    _sent(0),
    _dataSize(dataSize),
    _ttl(ttl),
    _rtt(repetitions, 0),
    _errors(repetitions)
{
}


ICMPEventArgs::~ICMPEventArgs()
{
}


std::string ICMPEventArgs::hostName() const
{
    try
    {
        return DNS::resolve(_address.host().toString()).name();
    }
    catch (HostNotFoundException&)
    {
    }
    catch (NoAddressFoundException&)
    {
    }
    catch (DNSException&)
    {
    }
    catch (IOException&)
    {
    }
    return _address.host().toString();
}


std::string ICMPEventArgs::hostAddress() const
{
    return _address.host().toString();
}


void ICMPEventArgs::setRepetitions(int repetitions)
{
    _rtt.clear();
    _rtt.resize(repetitions, 0);
    _errors.assign(repetitions, "");
}


ICMPEventArgs& ICMPEventArgs::operator ++ ()
{
    ++_sent;
    return *this;
}


ICMPEventArgs ICMPEventArgs::operator ++ (int)
{
    ICMPEventArgs prev(*this);
    operator ++ ();
    return prev;
}


int ICMPEventArgs::received() const
{
    int received = 0;

    for (int i = 0; i < _rtt.size(); ++i)
    {
        if (_rtt[i]) ++received;
    }
    return received;
}


void ICMPEventArgs::setError(int index, const std::string& text)
{
    if (index >= _errors.size())
        throw InvalidArgumentException("Supplied index exceeds vector capacity.");

    _errors[index] = text;
}


const std::string& ICMPEventArgs::error(int index) const
{
    if (0 == _errors.size())
        throw InvalidArgumentException("Supplied index exceeds vector capacity.");

    if (-1 == index) index = _sent - 1;

    return _errors[index];
}


void ICMPEventArgs::setReplyTime(int index, int time)
{
    if (index >= _rtt.size())
        throw InvalidArgumentException("Supplied index exceeds array capacity.");
    if (0 == time) time = 1;
    _rtt[index] = time;
}


int ICMPEventArgs::replyTime(int index) const
{
    if (0 == _rtt.size())
        throw InvalidArgumentException("Supplied index exceeds array capacity.");

    if (-1 == index) index = _sent - 1;

    return _rtt[index];
}


int ICMPEventArgs::avgRTT() const
{
    if (0 == _rtt.size()) return 0;

    return (int) (std::accumulate(_rtt.begin(), _rtt.end(), 0) / _rtt.size());
}


float ICMPEventArgs::percent() const
{
    if (0 == _rtt.size()) return 0;

    return ((float) received() / (float) _rtt.size()) * (float) 100.0;
}


}
} // namespace Poco::Net
