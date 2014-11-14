﻿//
// KeyValueArgs.h
//
// $Id: //poco/1.4/Foundation/include/Poco/KeyValueArgs.h#1 $
//
// Library: Foundation
// Package: Cache
// Module:  KeyValueArgs
//
// Definition of the KeyValueArgs class.
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


#ifndef Foundation_KeyValueArgs_INCLUDED
#define Foundation_KeyValueArgs_INCLUDED


#include "Poco/Foundation.h"


namespace Poco
{


template <class TKey, class TValue>
class KeyValueArgs
    /// Simply event arguments class to transfer a key and a value via an event call.
    /// Note that key and value are *NOT* copied, only references to them are stored.
{
public:
    KeyValueArgs(const TKey& aKey, const TValue& aVal):
        _key(aKey),
        _value(aVal)
    {
    }

    KeyValueArgs(const KeyValueArgs& args):
        _key(args._key),
        _value(args._value)
    {
    }

    ~KeyValueArgs()
    {
    }

    const TKey& key() const
    /// Returns a reference to the key,
    {
        return _key;
    }

    const TValue& value() const
    /// Returns a Reference to the value.
    {
        return _value;
    }

protected:
    const TKey&   _key;
    const TValue& _value;

private:
    KeyValueArgs& operator = (const KeyValueArgs& args);
};


} // namespace Poco


#endif // Foundation_KeyValueArgs_INCLUDED
