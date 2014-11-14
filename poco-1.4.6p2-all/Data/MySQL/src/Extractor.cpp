﻿//
// MySQLException.cpp
//
// $Id: //poco/1.4/Data/MySQL/src/Extractor.cpp#2 $
//
// Library: Data/MySQL
// Package: MySQL
// Module:  Extractor
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
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

#include "Poco/Data/MySQL/Extractor.h"


namespace Poco
{
namespace Data
{
namespace MySQL
{


Extractor::Extractor(StatementExecutor& st, ResultMetadata& md): _stmt(st), _metadata(md)
{
}


Extractor::~Extractor()
{
}


bool Extractor::extract(std::size_t pos, Poco::Int8& val)
{
    return realExtractFixed(pos, MYSQL_TYPE_TINY, &val);
}


bool Extractor::extract(std::size_t pos, Poco::UInt8& val)
{
    return realExtractFixed(pos, MYSQL_TYPE_TINY, &val);
}


bool Extractor::extract(std::size_t pos, Poco::Int16& val)
{
    return realExtractFixed(pos, MYSQL_TYPE_SHORT, &val);
}


bool Extractor::extract(std::size_t pos, Poco::UInt16& val)
{
    return realExtractFixed(pos, MYSQL_TYPE_SHORT, &val);
}


bool Extractor::extract(std::size_t pos, Poco::Int32& val)
{
    return realExtractFixed(pos, MYSQL_TYPE_LONG, &val);
}


bool Extractor::extract(std::size_t pos, Poco::UInt32& val)
{
    return realExtractFixed(pos, MYSQL_TYPE_LONG, &val);
}


bool Extractor::extract(std::size_t pos, Poco::Int64& val)
{
    return realExtractFixed(pos, MYSQL_TYPE_LONGLONG, &val);
}


bool Extractor::extract(std::size_t pos, Poco::UInt64& val)
{
    return realExtractFixed(pos, MYSQL_TYPE_LONGLONG, &val);
}


bool Extractor::extract(std::size_t pos, bool& val)
{
    return realExtractFixed(pos, MYSQL_TYPE_TINY, &val);
}


bool Extractor::extract(std::size_t pos, float& val)
{
    return realExtractFixed(pos, MYSQL_TYPE_FLOAT, &val);
}


bool Extractor::extract(std::size_t pos, double& val)
{
    return realExtractFixed(pos, MYSQL_TYPE_DOUBLE, &val);
}


bool Extractor::extract(std::size_t pos, char& val)
{
    return realExtractFixed(pos, MYSQL_TYPE_TINY, &val);
}


bool Extractor::extract(std::size_t pos, std::string& val)
{
    if (_metadata.columnsReturned() <= pos)
        throw MySQLException("Extractor: attempt to extract more paremeters, than query result contain");

    if (_metadata.isNull(static_cast<Poco::UInt32>(pos)))
        return false;

    if (_metadata.metaColumn(static_cast<Poco::UInt32>(pos)).type() != Poco::Data::MetaColumn::FDT_STRING)
        throw MySQLException("Extractor: not a string");

    val.assign(_metadata.rawData(pos), _metadata.length(pos));
    return true;
}


bool Extractor::extract(std::size_t pos, Poco::Data::BLOB& val)
{
    if (_metadata.columnsReturned() <= pos)
        throw MySQLException("Extractor: attempt to extract more paremeters, than query result contain");

    if (_metadata.isNull(static_cast<Poco::UInt32>(pos)))
        return false;

    if (_metadata.metaColumn(static_cast<Poco::UInt32>(pos)).type() != Poco::Data::MetaColumn::FDT_BLOB)
        throw MySQLException("Extractor: not a blob");

    unsigned long length= 0;
    MYSQL_BIND bind = { 0 };
    bind.buffer= 0;
    bind.buffer_length= 0;
    bind.length= &length;
    if ( ! _stmt.fetchColumn(pos, &bind) )
        return false;

    std::vector<char> data(length);
    bind.buffer = &data[0];
    bind.buffer_length= length;
    if ( ! _stmt.fetchColumn(pos, &bind) )
        return false;

    val.assignRaw(&data[0], length);
    return true;
}


bool Extractor::realExtractFixed(std::size_t pos, enum_field_types type, void* buffer, size_t length)
{
    MYSQL_BIND bind = {0};
    my_bool isNull = 0;

    bind.is_null       = &isNull;
    bind.buffer_type   = type;
    bind.buffer        = buffer;
    bind.buffer_length = static_cast<unsigned long>(length);

    if (!_stmt.fetchColumn(pos, &bind))
        return false;

    return isNull == 0;
}


}
}
} // namespace Poco::Data::MySQL
