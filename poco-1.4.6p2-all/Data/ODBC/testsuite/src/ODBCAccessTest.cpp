﻿//
// ODBCAccessTest.cpp
//
// $Id: //poco/1.4/Data/ODBC/testsuite/src/ODBCAccessTest.cpp#1 $
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


#include "ODBCAccessTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/String.h"
#include "Poco/Format.h"
#include "Poco/Exception.h"
#include "Poco/Data/Common.h"
#include "Poco/Data/BLOB.h"
#include "Poco/Data/StatementImpl.h"
#include "Poco/Data/ODBC/Connector.h"
#include "Poco/Data/ODBC/Utility.h"
#include "Poco/Data/ODBC/Diagnostics.h"
#include "Poco/Data/ODBC/ODBCException.h"
#include "Poco/Data/ODBC/ODBCStatementImpl.h"
#include <sqltypes.h>
#include <iostream>


using namespace Poco::Data;
using Poco::Data::ODBC::Utility;
using Poco::Data::ODBC::ConnectionException;
using Poco::Data::ODBC::StatementException;
using Poco::Data::ODBC::StatementDiagnostics;
using Poco::format;
using Poco::NotFoundException;


Poco::SharedPtr<Poco::Data::Session> ODBCAccessTest::_pSession = 0;
std::string ODBCAccessTest::_dsn = "PocoDataAccessTest";
std::string ODBCAccessTest::_dbConnString;
Poco::Data::ODBC::Utility::DriverMap ODBCAccessTest::_drivers;
Poco::Data::ODBC::Utility::DSNMap ODBCAccessTest::_dataSources;


ODBCAccessTest::ODBCAccessTest(const std::string& name):
    CppUnit::TestCase(name)
{
    static bool beenHere = false;

    ODBC::Connector::registerConnector();
    if (_drivers.empty() || _dataSources.empty())
    {
        Utility::drivers(_drivers);
        Utility::dataSources(_dataSources);
        checkODBCSetup();
    }
    if (!_pSession && !_dbConnString.empty() && !beenHere)
    {
        try
        {
            _pSession = new Session(SessionFactory::instance().create(ODBC::Connector::KEY, _dbConnString));
        }
        catch (ConnectionException& ex)
        {
            std::cout << "!!! WARNING: Connection failed. Access tests will fail !!!" << std::endl;
            std::cout << ex.toString() << std::endl;
        }

        //N.B. Access driver does not suport check for connection.
        if (_pSession) std::cout << "*** Connected to " << _dsn << '(' << _dbConnString << ')' << std::endl;
    }
    else if (!_pSession && !beenHere)
        std::cout << "!!! WARNING: No driver or DSN found. Access tests will fail !!!" << std::endl;

    beenHere = true;
}


ODBCAccessTest::~ODBCAccessTest()
{
    ODBC::Connector::unregisterConnector();
}


void ODBCAccessTest::testSimpleAccess()
{
    if (!_pSession) fail ("Test not available.");

    std::string lastName = "lastName";
    int age = 133132;
    int count = 0;
    std::string result;

    recreatePersonTable();

    count = 0;
    try
    {
        *_pSession << "INSERT INTO PERSON VALUES('lastName', 'firstName', 'Address', 133132)", now;
    }
    catch(ConnectionException& ce)
    {
        std::cout << ce.toString() << std::endl;
        fail ("testSimpleAccess()");
    }
    catch(StatementException& ex)
    {
        std::cout << ex.toString() << std::endl;
        fail ("testSimpleAccess()");
    }

    try
    {
        *_pSession << "SELECT COUNT(*) FROM PERSON", into(count), now;
    }
    catch(ConnectionException& ce)
    {
        std::cout << ce.toString() << std::endl;
        fail ("testSimpleAccess()");
    }
    catch(StatementException& ex)
    {
        std::cout << ex.toString() << std::endl;
        fail ("testSimpleAccess()");
    }
    assert (count == 1);

    try
    {
        *_pSession << "SELECT LastName FROM PERSON", into(result), now;
    }
    catch(ConnectionException& ce)
    {
        std::cout << ce.toString() << std::endl;
        fail ("testSimpleAccess()");
    }
    catch(StatementException& ex)
    {
        std::cout << ex.toString() << std::endl;
        fail ("testSimpleAccess()");
    }
    assert (lastName == result);

    try
    {
        *_pSession << "SELECT Age FROM PERSON", into(count), now;
    }
    catch(ConnectionException& ce)
    {
        std::cout << ce.toString() << std::endl;
        fail ("testSimpleAccess()");
    }
    catch(StatementException& ex)
    {
        std::cout << ex.toString() << std::endl;
        fail ("testSimpleAccess()");
    }
    assert (count == age);
}


void ODBCAccessTest::dropTable(const std::string& tableName)
{
    try
    {
        *_pSession << format("DROP TABLE %s", tableName), now;
    }
    catch (StatementException& ex)
    {
        bool ignoreError = false;
        const StatementDiagnostics::FieldVec& flds = ex.diagnostics().fields();
        StatementDiagnostics::Iterator it = flds.begin();
        for (; it != flds.end(); ++it)
        {
            if (-1305 == it->_nativeError)//MS Access -1305 (table does not exist)
            {
                ignoreError = true;
                break;
            }
        }

        if (!ignoreError) throw;
    }
}


void ODBCAccessTest::recreatePersonTable()
{
    dropTable("Person");
    *_pSession << "CREATE TABLE Person (LastName TEXT(30), FirstName TEXT(30), Address TEXT(30), Age INTEGER)", now;
}


void ODBCAccessTest::checkODBCSetup()
{
    static bool beenHere = false;

    if (!beenHere)
    {
        beenHere = true;

        bool driverFound = false;
        bool dsnFound = false;

        Utility::DriverMap::iterator itDrv = _drivers.begin();
        for (; itDrv != _drivers.end(); ++itDrv)
        {
            if (((itDrv->first).find("Microsoft Access Driver") != std::string::npos))
            {
                std::cout << "Driver found: " << itDrv->first
                          << " (" << itDrv->second << ')' << std::endl;
                driverFound = true;
                break;
            }
        }

        if (!driverFound)
        {
            std::cout << "Driver NOT found, will throw." << std::endl;
            throw NotFoundException("Microsoft Access ODBC driver.");
        }

        Utility::DSNMap::iterator itDSN = _dataSources.begin();
        for (; itDSN != _dataSources.end(); ++itDSN)
        {
            if (((itDSN->first).find(_dsn) != std::string::npos) &&
                    ((itDSN->second).find("Microsoft Access Driver") != std::string::npos))
            {
                std::cout << "DSN found: " << itDSN->first
                          << " (" << itDSN->second << ')' << std::endl;
                dsnFound = true;
                break;
            }
        }

        if (!dsnFound)
        {
            std::cout << "Access DSN NOT found, tests will fail." << std::endl;
            return;
        }
    }

    if (!_pSession)
        format(_dbConnString, "DSN=%s;Uid=Admin;Pwd=;", _dsn);
}


void ODBCAccessTest::setUp()
{
}


void ODBCAccessTest::tearDown()
{
    dropTable("Person");
}


CppUnit::Test* ODBCAccessTest::suite()
{
    CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ODBCAccessTest");

    CppUnit_addTest(pSuite, ODBCAccessTest, testSimpleAccess);

    return pSuite;
}
