﻿//
// TextBufferIterator.cpp
//
// $Id: //poco/1.4/Foundation/src/TextBufferIterator.cpp#1 $
//
// Library: Foundation
// Package: Text
// Module:  TextBufferIterator
//
// Copyright (c) 2010, Applied Informatics Software Engineering GmbH.
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


#include "Poco/TextBufferIterator.h"
#include "Poco/TextEncoding.h"
#include <algorithm>
#include <cstring>


namespace Poco
{


TextBufferIterator::TextBufferIterator():
    _pEncoding(0),
    _it(0),
    _end(0)
{
}


TextBufferIterator::TextBufferIterator(const char* begin, const TextEncoding& encoding):
    _pEncoding(&encoding),
    _it(begin),
    _end(begin + std::strlen(begin))
{
}


TextBufferIterator::TextBufferIterator(const char* begin, std::size_t size, const TextEncoding& encoding):
    _pEncoding(&encoding),
    _it(begin),
    _end(begin + size)
{
}


TextBufferIterator::TextBufferIterator(const char* begin, const char* end, const TextEncoding& encoding):
    _pEncoding(&encoding),
    _it(begin),
    _end(end)
{
}


TextBufferIterator::TextBufferIterator(const char* end):
    _pEncoding(0),
    _it(end),
    _end(end)
{
}


TextBufferIterator::~TextBufferIterator()
{
}


TextBufferIterator::TextBufferIterator(const TextBufferIterator& it):
    _pEncoding(it._pEncoding),
    _it(it._it),
    _end(it._end)
{
}


TextBufferIterator& TextBufferIterator::operator = (const TextBufferIterator& it)
{
    if (&it != this)
    {
        _pEncoding = it._pEncoding;
        _it        = it._it;
        _end       = it._end;
    }
    return *this;
}


void TextBufferIterator::swap(TextBufferIterator& it)
{
    std::swap(_pEncoding, it._pEncoding);
    std::swap(_it, it._it);
    std::swap(_end, it._end);
}


int TextBufferIterator::operator * () const
{
    poco_check_ptr (_pEncoding);
    poco_assert (_it != _end);
    const char* it = _it;

    unsigned char buffer[TextEncoding::MAX_SEQUENCE_LENGTH];
    unsigned char* p = buffer;

    if (it != _end)
        *p++ = *it++;
    else
        *p++ = 0;

    int read = 1;
    int n = _pEncoding->queryConvert(buffer, 1);

    while (-1 > n && (_end - it) >= -n - read)
    {
        while (read < -n && it != _end)
        {
            *p++ = *it++;
            read++;
        }
        n = _pEncoding->queryConvert(buffer, read);
    }

    if (-1 > n)
    {
        return -1;
    }
    else
    {
        return n;
    }
}


TextBufferIterator& TextBufferIterator::operator ++ ()
{
    poco_check_ptr (_pEncoding);
    poco_assert (_it != _end);

    unsigned char buffer[TextEncoding::MAX_SEQUENCE_LENGTH];
    unsigned char* p = buffer;

    if (_it != _end)
        *p++ = *_it++;
    else
        *p++ = 0;

    int read = 1;
    int n = _pEncoding->sequenceLength(buffer, 1);

    while (-1 > n && (_end - _it) >= -n - read)
    {
        while (read < -n && _it != _end)
        {
            *p++ = *_it++;
            read++;
        }
        n = _pEncoding->sequenceLength(buffer, read);
    }
    while (read < n && _it != _end)
    {
        _it++;
        read++;
    }

    return *this;
}


TextBufferIterator TextBufferIterator::operator ++ (int)
{
    TextBufferIterator prev(*this);
    operator ++ ();
    return prev;
}


} // namespace Poco
