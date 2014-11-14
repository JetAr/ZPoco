﻿//
// DataTypes.cpp
//
// $Id: //poco/1.4/Data/ODBC/src/DataTypes.cpp#1 $
//
// Library: Data/ODBC
// Package: ODBC
// Module:  DataTypes
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


#include "Poco/Data/ODBC/DataTypes.h"
#include "Poco/Format.h"
#include "Poco/Exception.h"


namespace Poco
{
namespace Data
{
namespace ODBC
{


DataTypes::DataTypes()
{
    _cDataTypes.insert(ValueType(SQL_CHAR, SQL_C_CHAR));
    _cDataTypes.insert(ValueType(SQL_VARCHAR, SQL_C_CHAR));
    _cDataTypes.insert(ValueType(SQL_LONGVARCHAR, SQL_C_CHAR));
    _cDataTypes.insert(ValueType(SQL_DECIMAL, SQL_C_DOUBLE));
    _cDataTypes.insert(ValueType(SQL_NUMERIC, SQL_C_DOUBLE));
    _cDataTypes.insert(ValueType(SQL_BIT, SQL_C_BIT));
    _cDataTypes.insert(ValueType(SQL_TINYINT, SQL_C_STINYINT));
    _cDataTypes.insert(ValueType(SQL_SMALLINT, SQL_C_SSHORT));
    _cDataTypes.insert(ValueType(SQL_INTEGER, SQL_C_SLONG));
    _cDataTypes.insert(ValueType(SQL_BIGINT, SQL_C_SBIGINT));
    _cDataTypes.insert(ValueType(SQL_REAL, SQL_C_FLOAT));
    _cDataTypes.insert(ValueType(SQL_FLOAT, SQL_C_DOUBLE));
    _cDataTypes.insert(ValueType(SQL_DOUBLE, SQL_C_DOUBLE));
    _cDataTypes.insert(ValueType(SQL_BINARY, SQL_C_BINARY));
    _cDataTypes.insert(ValueType(SQL_VARBINARY, SQL_C_BINARY));
    _cDataTypes.insert(ValueType(SQL_LONGVARBINARY, SQL_C_BINARY));
    _cDataTypes.insert(ValueType(SQL_TYPE_DATE, SQL_C_TYPE_DATE));
    _cDataTypes.insert(ValueType(SQL_TYPE_TIME, SQL_C_TYPE_TIME));
    _cDataTypes.insert(ValueType(SQL_TYPE_TIMESTAMP, SQL_C_TYPE_TIMESTAMP));

    _sqlDataTypes.insert(ValueType(SQL_C_CHAR, SQL_LONGVARCHAR));
    _sqlDataTypes.insert(ValueType(SQL_C_BIT, SQL_BIT));
    _sqlDataTypes.insert(ValueType(SQL_C_TINYINT, SQL_TINYINT));
    _sqlDataTypes.insert(ValueType(SQL_C_STINYINT, SQL_TINYINT));
    _sqlDataTypes.insert(ValueType(SQL_C_UTINYINT, SQL_TINYINT));
    _sqlDataTypes.insert(ValueType(SQL_C_SHORT, SQL_SMALLINT));
    _sqlDataTypes.insert(ValueType(SQL_C_SSHORT, SQL_SMALLINT));
    _sqlDataTypes.insert(ValueType(SQL_C_USHORT, SQL_SMALLINT));
    _sqlDataTypes.insert(ValueType(SQL_C_LONG, SQL_INTEGER));
    _sqlDataTypes.insert(ValueType(SQL_C_SLONG, SQL_INTEGER));
    _sqlDataTypes.insert(ValueType(SQL_C_ULONG, SQL_INTEGER));
    _sqlDataTypes.insert(ValueType(SQL_C_SBIGINT, SQL_BIGINT));
    _sqlDataTypes.insert(ValueType(SQL_C_UBIGINT, SQL_BIGINT));
    _sqlDataTypes.insert(ValueType(SQL_C_FLOAT, SQL_REAL));
    _sqlDataTypes.insert(ValueType(SQL_C_DOUBLE, SQL_DOUBLE));
    _sqlDataTypes.insert(ValueType(SQL_C_BINARY, SQL_LONGVARBINARY));
    _sqlDataTypes.insert(ValueType(SQL_C_TYPE_DATE, SQL_TYPE_DATE));
    _sqlDataTypes.insert(ValueType(SQL_C_TYPE_TIME, SQL_TYPE_TIME));
    _sqlDataTypes.insert(ValueType(SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP));
}


DataTypes::~DataTypes()
{
}


int DataTypes::cDataType(int sqlDataType) const
{
    DataTypeMap::const_iterator it = _cDataTypes.find(sqlDataType);

    if (_cDataTypes.end() == it)
        throw NotFoundException(format("C data type not found for SQL data type: %d", sqlDataType));

    return it->second;
}


int DataTypes::sqlDataType(int cDataType) const
{
    DataTypeMap::const_iterator it = _sqlDataTypes.find(cDataType);

    if (_sqlDataTypes.end() == it)
        throw NotFoundException(format("SQL data type not found for C data type: %d", cDataType));

    return it->second;
}


}
}
} // namespace Poco::Data::ODBC
