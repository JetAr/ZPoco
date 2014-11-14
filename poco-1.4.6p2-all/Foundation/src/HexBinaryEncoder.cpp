﻿//
// HexBinaryEncoder.cpp
//
// $Id: //poco/1.4/Foundation/src/HexBinaryEncoder.cpp#2 $
//
// Library: Foundation
// Package: Streams
// Module:  HexBinary
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


#include "Poco/HexBinaryEncoder.h"


namespace Poco
{


HexBinaryEncoderBuf::HexBinaryEncoderBuf(std::ostream& ostr):
    _pos(0),
    _lineLength(72),
    _uppercase(0),
    _buf(*ostr.rdbuf())
{
}


HexBinaryEncoderBuf::~HexBinaryEncoderBuf()
{
    try
    {
        close();
    }
    catch (...)
    {
    }
}


void HexBinaryEncoderBuf::setLineLength(int lineLength)
{
    _lineLength = lineLength;
}


int HexBinaryEncoderBuf::getLineLength() const
{
    return _lineLength;
}


void HexBinaryEncoderBuf::setUppercase(bool flag)
{
    _uppercase = flag ? 16 : 0;
}


int HexBinaryEncoderBuf::writeToDevice(char c)
{
    static const int eof = std::char_traits<char>::eof();
    static const char digits[] = "0123456789abcdef0123456789ABCDEF";

    if (_buf.sputc(digits[_uppercase + ((c >> 4) & 0xF)]) == eof) return eof;
    ++_pos;
    if (_buf.sputc(digits[_uppercase + (c & 0xF)]) == eof) return eof;
    if (++_pos >= _lineLength && _lineLength > 0)
    {
        if (_buf.sputc('\n') == eof) return eof;
        _pos = 0;
    }
    return charToInt(c);
}


int HexBinaryEncoderBuf::close()
{
    sync();
    return _buf.pubsync();
}


HexBinaryEncoderIOS::HexBinaryEncoderIOS(std::ostream& ostr): _buf(ostr)
{
    poco_ios_init(&_buf);
}


HexBinaryEncoderIOS::~HexBinaryEncoderIOS()
{
}


int HexBinaryEncoderIOS::close()
{
    return _buf.close();
}


HexBinaryEncoderBuf* HexBinaryEncoderIOS::rdbuf()
{
    return &_buf;
}


HexBinaryEncoder::HexBinaryEncoder(std::ostream& ostr): HexBinaryEncoderIOS(ostr), std::ostream(&_buf)
{
}


HexBinaryEncoder::~HexBinaryEncoder()
{
}


} // namespace Poco
