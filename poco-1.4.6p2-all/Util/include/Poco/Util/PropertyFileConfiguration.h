﻿//
// PropertyFileConfiguration.h
//
// $Id: //poco/1.4/Util/include/Poco/Util/PropertyFileConfiguration.h#1 $
//
// Library: Util
// Package: Configuration
// Module:  PropertyFileConfiguration
//
// Definition of the PropertyFileConfiguration class.
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


#ifndef Util_PropertyFileConfiguration_INCLUDED
#define Util_PropertyFileConfiguration_INCLUDED


#include "Poco/Util/Util.h"
#include "Poco/Util/MapConfiguration.h"
#include <istream>
#include <ostream>


namespace Poco
{
namespace Util
{


class Util_API PropertyFileConfiguration: public MapConfiguration
    /// This implementation of a Configuration reads properties
    /// from a Java-style properties file.
    ///
    /// The file syntax is implemented as follows.
    ///   - a line starting with a hash '#' or exclamation mark '!' is treated as a comment and ignored
    ///   - every other line denotes a property assignment in the form
    ///     <key> = <value> or
    ///     <key> : <value>
    ///
    /// Keys and values may contain special characters represented by the following escape sequences:
    ///   - \t: tab (0x09)
    ///   - \n: line feed (0x0a)
    ///   - \r: carriage return (0x0d)
    ///   - \f: form feed (0x0c)
    ///
    /// For every other sequence that starts with a backslash, the backslash is removed.
    /// Therefore, the sequence \a would just yield an 'a'.
    ///
    /// A value can spread across multiple lines if the last character in a line (the character
    /// immediately before the carriage return or line feed character) is a single backslash.
    ///
    /// Property names are case sensitive. Leading and trailing whitespace is
    /// removed from both keys and values. A property name can neither contain
    /// a colon ':' nor an equal sign '=' character.
{
public:
    PropertyFileConfiguration();
    /// Creates an empty PropertyFileConfiguration.

    PropertyFileConfiguration(std::istream& istr);
    /// Creates an PropertyFileConfiguration and loads the configuration data
    /// from the given stream, which must be in properties file format.

    PropertyFileConfiguration(const std::string& path);
    /// Creates an PropertyFileConfiguration and loads the configuration data
    /// from the given file, which must be in properties file format.

    void load(std::istream& istr);
    /// Loads the configuration data from the given stream, which
    /// must be in properties file format.

    void load(const std::string& path);
    /// Loads the configuration data from the given file, which
    /// must be in properties file format.

    void save(std::ostream& ostr) const;
    /// Writes the configuration data to the given stream.
    ///
    /// The data is written as a sequence of statements in the form
    /// <key>: <value>
    /// separated by a newline character.

    void save(const std::string& path) const;
    /// Writes the configuration data to the given file.

protected:
    ~PropertyFileConfiguration();

private:
    void parseLine(std::istream& istr);
    static int readChar(std::istream& istr);
};


}
} // namespace Poco::Util


#endif // Util_PropertyFileConfiguration_INCLUDED
