﻿//
// Validator.h
//
// $Id: //poco/1.4/Util/include/Poco/Util/Validator.h#1 $
//
// Library: Util
// Package: Options
// Module:  Validator
//
// Definition of the Validator class.
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


#ifndef Util_Validator_INCLUDED
#define Util_Validator_INCLUDED


#include "Poco/Util/Util.h"
#include "Poco/RefCountedObject.h"


namespace Poco
{
namespace Util
{


class Option;


class Util_API Validator: public Poco::RefCountedObject
    /// Validator specifies the interface for option validators.
    ///
    /// Option validators provide a simple way for the automatic
    /// validation of command line argument values.
{
public:
    virtual void validate(const Option& option, const std::string& value) = 0;
    /// Validates the value for the given option.
    /// Does nothing if the value is valid.
    ///
    /// Throws an InvalidOptionException otherwise.

protected:
    Validator();
    /// Creates the Validator.

    virtual ~Validator();
    /// Destroys the Validator.
};


}
} // namespace Poco::Util


#endif // Util_Validator_INCLUDED
