﻿//
// BLOB.cpp
//
// $Id: //poco/1.4/Data/src/BLOB.cpp#1 $
//
// Library: Data
// Package: DataCore
// Module:  BLOB
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


#include "Poco/Data/BLOB.h"


namespace Poco
{
namespace Data
{


BLOB::BLOB():
    _pContent(new std::vector<char>())
{
}


BLOB::BLOB(const std::vector<char>& content):
    _pContent(new std::vector<char>(content))
{
}


BLOB::BLOB(const char* const pContent, std::size_t size):
    _pContent(new std::vector<char>(pContent, pContent + size))
{
}


BLOB::BLOB(const std::string& content):
    _pContent(new std::vector<char>(content.begin(), content.end()))
{
}


BLOB::BLOB(const BLOB& other):
    _pContent(other._pContent)
{
}


BLOB::~BLOB()
{
}


BLOB& BLOB::operator = (const BLOB& other)
{
    BLOB tmp(other);
    swap(tmp);
    return *this;
}


}
} // namespace Poco::Data
