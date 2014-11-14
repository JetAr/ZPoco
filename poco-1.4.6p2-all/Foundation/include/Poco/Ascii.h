﻿//
// Ascii.h
//
// $Id: //poco/1.4/Foundation/include/Poco/Ascii.h#1 $
//
// Library: Foundation
// Package: Core
// Module:  Ascii
//
// Definition of the Ascii class.
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


#ifndef Foundation_Ascii_INCLUDED
#define Foundation_Ascii_INCLUDED


#include "Poco/Foundation.h"


namespace Poco
{


class Foundation_API Ascii
    /// This class contains enumerations and static
    /// utility functions for dealing with ASCII characters
    /// and their properties.
    ///
    /// The classification functions will also work if
    /// non-ASCII character codes are passed to them,
    /// but classification will only check for
    /// ASCII characters.
    ///
    /// This allows the classification methods to be used
    /// on the single bytes of a UTF-8 string, without
    /// causing assertions or inconsistent results (depending
    /// upon the current locale) on bytes outside the ASCII range,
    /// as may be produced by Ascii::isSpace(), etc.
{
public:
    //z 字符属性
    enum CharacterProperties
    /// ASCII character properties.
    {
        ACP_CONTROL  = 0x0001,
        ACP_SPACE    = 0x0002,
        ACP_PUNCT    = 0x0004,
        ACP_DIGIT    = 0x0008,
        ACP_HEXDIGIT = 0x0010,
        ACP_ALPHA    = 0x0020,
        ACP_LOWER    = 0x0040,
        ACP_UPPER    = 0x0080,
        ACP_GRAPH    = 0x0100,
        ACP_PRINT    = 0x0200
    };

    static int properties(int ch);
    /// Return the ASCII character properties for the
    /// character with the given ASCII value.
    ///
    /// If the character is outside the ASCII range
    /// (0 .. 127), 0 is returned.

    static bool hasSomeProperties(int ch, int properties);
    /// Returns true if the given character is
    /// within the ASCII range and has at least one of
    /// the given properties.

    static bool hasProperties(int ch, int properties);
    /// Returns true if the given character is
    /// within the ASCII range and has all of
    /// the given properties.

    static bool isAscii(int ch);
    /// Returns true iff the given character code is within
    /// the ASCII range (0 .. 127).

    static bool isSpace(int ch);
    /// Returns true iff the given character is a whitespace.

    static bool isDigit(int ch);
    /// Returns true iff the given character is a digit.

    static bool isHexDigit(int ch);
    /// Returns true iff the given character is a hexadecimal digit.

    static bool isPunct(int ch);
    /// Returns true iff the given character is a punctuation character.

    static bool isAlpha(int ch);
    /// Returns true iff the given character is an alphabetic character.

    static bool isAlphaNumeric(int ch);
    /// Returns true iff the given character is an alphabetic character.

    static bool isLower(int ch);
    /// Returns true iff the given character is a lowercase alphabetic
    /// character.

    static bool isUpper(int ch);
    /// Returns true iff the given character is an uppercase alphabetic
    /// character.

    static int toLower(int ch);
    /// If the given character is an uppercase character,
    /// return its lowercase counterpart, otherwise return
    /// the character.

    static int toUpper(int ch);
    /// If the given character is a lowercase character,
    /// return its uppercase counterpart, otherwise return
    /// the character.

private:
    //z 这里声明为 static const
    static const int CHARACTER_PROPERTIES[128];
};


//
// inlines
//
inline int Ascii::properties(int ch)
{
    if (isAscii(ch))
        return CHARACTER_PROPERTIES[ch];
    else
        return 0;
}

//z 判断是否是ascII值
inline bool Ascii::isAscii(int ch)
{
    return (static_cast<UInt32>(ch) & 0xFFFFFF80) == 0;
}


inline bool Ascii::hasProperties(int ch, int props)
{
    return (properties(ch) & props) == props;
}


inline bool Ascii::hasSomeProperties(int ch, int props)
{
    return (properties(ch) & props) != 0;
}


inline bool Ascii::isSpace(int ch)
{
    return hasProperties(ch, ACP_SPACE);
}


inline bool Ascii::isDigit(int ch)
{
    return hasProperties(ch, ACP_DIGIT);
}


inline bool Ascii::isHexDigit(int ch)
{
    return hasProperties(ch, ACP_HEXDIGIT);
}


inline bool Ascii::isPunct(int ch)
{
    return hasProperties(ch, ACP_PUNCT);
}


inline bool Ascii::isAlpha(int ch)
{
    return hasProperties(ch, ACP_ALPHA);
}


inline bool Ascii::isAlphaNumeric(int ch)
{
    return hasSomeProperties(ch, ACP_ALPHA | ACP_DIGIT);
}


inline bool Ascii::isLower(int ch)
{
    return hasProperties(ch, ACP_LOWER);
}

//z 都是采用的直接查表的方式来解决的
inline bool Ascii::isUpper(int ch)
{
    return hasProperties(ch, ACP_UPPER);
}


inline int Ascii::toLower(int ch)
{
    if (isUpper(ch))
        return ch + 32;
    else
        return ch;
}


inline int Ascii::toUpper(int ch)
{
    if (isLower(ch))
        return ch - 32;
    else
        return ch;
}


} // namespace Poco


#endif // Foundation_Ascii_INCLUDED
