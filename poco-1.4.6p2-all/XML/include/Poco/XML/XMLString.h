﻿//
// XMLString.h
//
// $Id: //poco/1.4/XML/include/Poco/XML/XMLString.h#1 $
//
// Library: XML
// Package: XML
// Module:  XMLString
//
// Definition of the XMLString class.
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


#ifndef XML_XMLString_INCLUDED
#define XML_XMLString_INCLUDED


#include "Poco/XML/XML.h"


namespace Poco
{
namespace XML
{


//
// The XML parser uses the string classes provided by the C++
// standard library (based on the basic_string<> template).
// In Unicode mode, a std::wstring is used, otherwise
// a std::string is used.
// To turn on Unicode mode, #define XML_UNICODE and
// XML_UNICODE_WCHAR_T when compiling the library.
//
// XML_UNICODE  XML_UNICODE_WCHAR_T  XMLChar    XMLString
// --------------------------------------------------------------
//     N                 N           char       std::string
//     N                 Y           wchar_t    std::wstring
//     Y                 Y           wchar_t    std::wstring
//     Y                 N           <not supported>
//
#if defined(XML_UNICODE_WCHAR_T)

// Unicode - use wchar_t
typedef wchar_t      XMLChar;
typedef std::wstring XMLString;

std::string fromXMLString(const XMLString& str);
/// Converts an XMLString into an UTF-8 encoded
/// string.

XMLString toXMLString(const std::string& str);
/// Converts an UTF-8 encoded string into an
/// XMLString

#define XML_LIT(lit) L##lit

#elif defined(XML_UNICODE)

// not supported - leave XMLString undefined

#else

// Characters are UTF-8 encoded
typedef char        XMLChar;
typedef std::string XMLString;

inline const std::string& fromXMLString(const XMLString& str)
{
    return str;
}

inline const XMLString& toXMLString(const std::string& str)
{
    return str;
}

#define XML_LIT(lit) lit

#endif


}
} // namespace Poco::XML


#endif // XML_XMLString_INCLUDED
