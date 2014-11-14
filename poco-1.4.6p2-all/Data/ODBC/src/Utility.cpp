﻿//
// Utility.cpp
//
// $Id: //poco/1.4/Data/ODBC/src/Utility.cpp#1 $
//
// Library: Data/ODBC
// Package: ODBC
// Module:  Utility
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


#include "Poco/Data/ODBC/Utility.h"
#include "Poco/Data/ODBC/Handle.h"
#include "Poco/Data/ODBC/ODBCException.h"
#include "Poco/NumberFormatter.h"


namespace Poco
{
namespace Data
{
namespace ODBC
{


const DataTypes Utility::_dataTypes;
const SQLSMALLINT Utility::boolDataType = (sizeof(bool) <= sizeof(char)) ? SQL_C_TINYINT :
        (sizeof(bool) == sizeof(short)) ? SQL_C_SHORT : SQL_C_LONG;


namespace
{
static const EnvironmentHandle envHandle;
}


Utility::DriverMap& Utility::drivers(Utility::DriverMap& driverMap)
{
    const int length = sizeof(POCO_SQLCHAR) * 512;

    POCO_SQLCHAR desc[length];
    std::memset(desc, 0, length);
    SQLSMALLINT len1 = length;
    POCO_SQLCHAR attr[length];
    std::memset(attr, 0, length);
    SQLSMALLINT len2 = length;
    RETCODE rc = 0;

    if (!Utility::isError(rc = SQLDrivers(envHandle,
                                          SQL_FETCH_FIRST,
                                          desc,
                                          length,
                                          &len1,
                                          attr,
                                          len2,
                                          &len2)))
    {
        do
        {
            driverMap.insert(DSNMap::value_type(std::string((char *) desc),
                                                std::string((char *) attr)));
            std::memset(desc, 0, length);
            std::memset(attr, 0, length);
            len2 = length;
        }
        while (!Utility::isError(rc = SQLDrivers(envHandle,
                                      SQL_FETCH_NEXT,
                                      desc,
                                      length,
                                      &len1,
                                      attr,
                                      len2,
                                      &len2)));
    }

    if (SQL_NO_DATA != rc)
        throw EnvironmentError(envHandle);

    return driverMap;
}


Utility::DSNMap& Utility::dataSources(Utility::DSNMap& dsnMap)
{
    const int length = sizeof(POCO_SQLCHAR) * 512;
    const int dsnLength = sizeof(POCO_SQLCHAR) * (SQL_MAX_DSN_LENGTH + 1);

    POCO_SQLCHAR dsn[dsnLength];
    std::memset(dsn, 0, dsnLength);
    SQLSMALLINT len1 = sizeof(POCO_SQLCHAR) * SQL_MAX_DSN_LENGTH;
    POCO_SQLCHAR desc[length];
    std::memset(desc, 0, length);
    SQLSMALLINT len2 = length;
    RETCODE rc = 0;

    while (!Utility::isError(rc = SQLDataSources(envHandle,
                                  SQL_FETCH_NEXT,
                                  dsn,
                                  SQL_MAX_DSN_LENGTH,
                                  &len1,
                                  desc,
                                  len2,
                                  &len2)))
    {
        dsnMap.insert(DSNMap::value_type(std::string((char *) dsn), std::string((char *) desc)));
        std::memset(dsn, 0, dsnLength);
        std::memset(desc, 0, length);
        len2 = length;
    }

    if (SQL_NO_DATA != rc)
        throw EnvironmentError(envHandle);

    return dsnMap;
}


}
}
} // namespace Poco::Data::ODBC
