﻿//
// Range.h
//
// $Id: //poco/1.4/Data/include/Poco/Data/Range.h#1 $
//
// Library: Data
// Package: DataCore
// Module:  Range
//
// Definition of the Range class.
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


#ifndef Data_Range_INCLUDED
#define Data_Range_INCLUDED


#include "Poco/Data/Data.h"
#include "Poco/Data/Limit.h"


namespace Poco
{
namespace Data
{


class Data_API Range
    /// Range stores information how many rows a query should return.
{
public:
    Range(Poco::UInt32 lowValue, Poco::UInt32 upValue, bool hardLimit);
    /// Creates the Range. lowValue must be smaller equal than upValue

    ~Range();
    /// Destroys the Limit.

    const Limit& lower() const;
    /// Returns the lower limit

    const Limit& upper() const;
    /// Returns the upper limit

private:
    Limit _lower;
    Limit _upper;
};


//
// inlines
//
inline const Limit& Range::lower() const
{
    return _lower;
}


inline const Limit& Range::upper() const
{
    return _upper;
}


template <typename T> Limit limit(T lim, bool hard = false)
/// Creates an upperLimit
{
    return Limit(static_cast<Poco::UInt32>(lim), hard, false);
}


template <typename T> Limit upperLimit(T lim, bool hard = false)
{
    return limit(lim, hard);
}


template <typename T> Limit lowerLimit(T lim)
{
    return Limit(static_cast<Poco::UInt32>(lim), true, true);
}


template <typename T> Range range(T low, T upp, bool hard = false)
{
    return Range(static_cast<Poco::UInt32>(low), static_cast<Poco::UInt32>(upp), hard);
}


}
} // namespace Poco::Data


#endif // Data_Range_INCLUDED
