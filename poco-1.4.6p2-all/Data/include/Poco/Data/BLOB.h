﻿//
// BLOB.h
//
// $Id: //poco/1.4/Data/include/Poco/Data/BLOB.h#1 $
//
// Library: Data
// Package: DataCore
// Module:  BLOB
//
// Definition of the BLOB class.
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


#ifndef Data_BLOB_INCLUDED
#define Data_BLOB_INCLUDED


#include "Poco/Data/Data.h"
#include "Poco/SharedPtr.h"
#include "Poco/DynamicAnyHolder.h"
#include "Poco/Exception.h"
#include <vector>


namespace Poco
{
namespace Data
{


class Data_API BLOB
    /// Representation of a Binary Large OBject.
    ///
    /// A BLOB can hold arbitrary binary data.
    /// The maximum size depends on the underlying database.
    ///
    /// The BLOBInputStream and BLOBOutputStream classes provide
    /// a convenient way to access the data in a BLOB.
{
public:
    typedef std::vector<char>::const_iterator Iterator;

    BLOB();
    /// Creates an empty BLOB.

    BLOB(const std::vector<char>& content);
    /// Creates the BLOB, content is deep-copied.

    BLOB(const char* const pContent, std::size_t size);
    /// Creates the BLOB by deep-copying pContent.

    BLOB(const std::string& content);
    /// Creates a BLOB from a string.

    BLOB(const BLOB& other);
    /// Creates a BLOB by copying another one.

    ~BLOB();
    /// Destroys the BLOB.

    BLOB& operator = (const BLOB& other);
    /// Assignment operator.

    bool operator == (const BLOB& other) const;
    /// Compares for equality BLOB by value.

    bool operator != (const BLOB& other) const;
    /// Compares for inequality BLOB by value.

    void swap(BLOB& other);
    /// Swaps the BLOB with another one.

    const std::vector<char>& content() const;
    /// Returns the content.

    const char* rawContent() const;
    /// Returns the raw content.
    ///
    /// If the BLOB is empty, returns NULL.

    void assignRaw(const char* pChar, std::size_t count);
    /// Assigns raw content to internal storage.

    void appendRaw(const char* pChar, std::size_t count);
    /// Assigns raw content to internal storage.

    void clear(bool doCompact = false);
    /// Clears the content of the blob.
    /// If doCompact is true, trims the excess capacity.

    void compact();
    /// Trims the internal storage excess capacity.

    Iterator begin() const;
    Iterator end() const;

    std::size_t size() const;
    /// Returns the size of the BLOB in bytes.

private:
    Poco::SharedPtr<std::vector<char> > _pContent;

    friend class BLOBStreamBuf;
};


//
// inlines
//
inline const std::vector<char>& BLOB::content() const
{
    return *_pContent;
}


inline const char* BLOB::rawContent() const
{
    if (_pContent->empty())
        return 0;
    else
        return &(*_pContent)[0];
}


inline std::size_t BLOB::size() const
{
    return _pContent->size();
}


inline bool BLOB::operator == (const BLOB& other) const
{
    return *_pContent == *other._pContent;
}


inline bool BLOB::operator != (const BLOB& other) const
{
    return *_pContent != *other._pContent;
}


inline BLOB::Iterator BLOB::begin() const
{
    return _pContent->begin();
}


inline BLOB::Iterator BLOB::end() const
{
    return _pContent->end();
}


inline void BLOB::assignRaw(const char* pChar, std::size_t count)
{
    poco_assert_dbg (pChar);
    BLOB tmp(pChar, count);
    swap(tmp);
}


inline void BLOB::appendRaw(const char* pChar, std::size_t count)
{
    poco_assert_dbg (pChar);
    _pContent->insert(_pContent->end(), pChar, pChar+count);
}


inline void BLOB::swap(BLOB& other)
{
    using std::swap;
    swap(_pContent, other._pContent);
}


inline void BLOB::clear(bool doCompact)
{
    _pContent->clear();
    if (doCompact) compact();
}


inline void BLOB::compact()
{
    std::vector<char>(*_pContent).swap(*_pContent);
}


}
} // namespace Poco::Data


//
// DynamicAnyHolderImpl<BLOB>
//


namespace Poco
{


template <>
class DynamicAnyHolderImpl<Data::BLOB>: public DynamicAnyHolder
{
public:
    DynamicAnyHolderImpl(const Data::BLOB& val): _val(val)
    {
    }

    ~DynamicAnyHolderImpl()
    {
    }

    const std::type_info& type() const
    {
        return typeid(Data::BLOB);
    }

    void convert(Int8&) const
    {
        throw Poco::BadCastException();
    }

    void convert(Int16&) const
    {
        throw Poco::BadCastException();
    }

    void convert(Int32&) const
    {
        throw Poco::BadCastException();
    }

    void convert(Int64&) const
    {
        throw Poco::BadCastException();
    }

    void convert(UInt8&) const
    {
        throw Poco::BadCastException();
    }

    void convert(UInt16&) const
    {
        throw Poco::BadCastException();
    }

    void convert(UInt32&) const
    {
        throw Poco::BadCastException();
    }

    void convert(UInt64&) const
    {
        throw Poco::BadCastException();
    }

    void convert(bool&) const
    {
        throw Poco::BadCastException();
    }

    void convert(float&) const
    {
        throw Poco::BadCastException();
    }

    void convert(double&) const
    {
        throw Poco::BadCastException();
    }

    void convert(char&) const
    {
        throw Poco::BadCastException();
    }

    void convert(std::string& val) const
    {
        val.assign(_val.begin(), _val.end());
    }

    void convert(Poco::DateTime&) const
    {
        throw Poco::BadCastException();
    }

    void convert(Poco::LocalDateTime&) const
    {
        throw Poco::BadCastException();
    }

    void convert(Poco::Timestamp&) const
    {
        throw Poco::BadCastException();
    }

    DynamicAnyHolder* clone() const
    {
        return new DynamicAnyHolderImpl(_val);
    }

    bool isArray() const
    {
        return false;
    }

    bool isInteger() const
    {
        return false;
    }

    bool isSigned() const
    {
        return false;
    }

    bool isNumeric() const
    {
        return false;
    }

    bool isString() const
    {
        return false;
    }

private:
    Data::BLOB _val;
};


} // namespace Poco


#endif // Data_BLOB_INCLUDED
