﻿//
// ODBCOracleTest.cpp
//
// $Id: //poco/1.4/Data/ODBC/testsuite/src/ODBCOracleTest.cpp#1 $
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


#include "ODBCOracleTest.h"
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


#define ORACLE_ODBC_DRIVER "Oracle in XE"
#define ORACLE_DSN "PocoDataOracleTest"
#define ORACLE_SERVER "localhost"
#define ORACLE_PORT "1521"
#define ORACLE_SID "XE"
#define ORACLE_UID "scott"
#define ORACLE_PWD "tiger"


using namespace Poco::Data;
using Poco::Data::ODBC::Utility;
using Poco::Data::ODBC::ConnectionException;
using Poco::Data::ODBC::StatementException;
using Poco::Data::ODBC::StatementDiagnostics;
using Poco::format;
using Poco::NotFoundException;


const bool ODBCOracleTest::bindValues[8] = {true, true, true, false, false, true, false, false};
Poco::SharedPtr<Poco::Data::Session> ODBCOracleTest::_pSession = 0;
Poco::SharedPtr<SQLExecutor> ODBCOracleTest::_pExecutor = 0;
std::string ODBCOracleTest::_dsn = ORACLE_DSN;
std::string ODBCOracleTest::_dbConnString;
Poco::Data::ODBC::Utility::DriverMap ODBCOracleTest::_drivers;
Poco::Data::ODBC::Utility::DSNMap ODBCOracleTest::_dataSources;


ODBCOracleTest::ODBCOracleTest(const std::string& name):
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
            if (_pSession && _pSession->isConnected())
                std::cout << "*** Connected to " << _dbConnString << std::endl;
        }
        catch (ConnectionException& ex)
        {
            std::cout << "!!! WARNING: Connection failed. Oracle tests will fail !!!" << std::endl;
            std::cout << ex.toString() << std::endl;
        }

        if (!_pExecutor) _pExecutor = new SQLExecutor("Oracle SQL Executor", _pSession);
    }
    else if (!_pSession && !beenHere)
        std::cout << "!!! WARNING: No driver or DSN found. Oracle tests will fail !!!" << std::endl;

    beenHere = true;
}


ODBCOracleTest::~ODBCOracleTest()
{
    ODBC::Connector::unregisterConnector();
}


void ODBCOracleTest::testBareboneODBC()
{
    if (!_pSession) fail ("Test not available.");

    std::string tableCreateString = "CREATE TABLE Test "
                                    "(First VARCHAR(30),"
                                    "Second VARCHAR(30),"
                                    "Third BLOB,"
                                    "Fourth INTEGER,"
                                    "Fifth NUMBER,"
                                    "Sixth TIMESTAMP)";

    _pExecutor->bareboneODBCTest(_dbConnString, tableCreateString, SQLExecutor::PB_IMMEDIATE, SQLExecutor::DE_MANUAL);
    _pExecutor->bareboneODBCTest(_dbConnString, tableCreateString, SQLExecutor::PB_IMMEDIATE, SQLExecutor::DE_BOUND);
    _pExecutor->bareboneODBCTest(_dbConnString, tableCreateString, SQLExecutor::PB_AT_EXEC, SQLExecutor::DE_MANUAL);
    _pExecutor->bareboneODBCTest(_dbConnString, tableCreateString, SQLExecutor::PB_AT_EXEC, SQLExecutor::DE_BOUND);
}


void ODBCOracleTest::testSimpleAccess()
{
    if (!_pSession) fail ("Test not available.");

    std::string tableName("Person");
    int count = 0;

    recreatePersonTable();

    *_pSession << "SELECT count(*) FROM sys.all_all_tables WHERE table_name = upper(?)", into(count), use(tableName), now;
    assert (1 == count);

    for (int i = 0; i < 8;)
    {
        recreatePersonTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->simpleAccess();
        i += 2;
    }
}


void ODBCOracleTest::testComplexType()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreatePersonTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->complexType();
        i += 2;
    }
}


void ODBCOracleTest::testComplexTypeTuple()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreatePersonTupleTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->complexTypeTuple();
        i += 2;
    }
}


void ODBCOracleTest::testSimpleAccessVector()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreatePersonTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->simpleAccessVector();
        i += 2;
    }
}


void ODBCOracleTest::testComplexTypeVector()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreatePersonTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->complexTypeVector();
        i += 2;
    }
}


void ODBCOracleTest::testInsertVector()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreateStringsTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->insertVector();
        i += 2;
    }
}


void ODBCOracleTest::testInsertEmptyVector()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreateStringsTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->insertEmptyVector();
        i += 2;
    }
}


void ODBCOracleTest::testInsertSingleBulk()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreateIntsTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->insertSingleBulk();
        i += 2;
    }
}


void ODBCOracleTest::testInsertSingleBulkVec()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreateIntsTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->insertSingleBulkVec();
        i += 2;
    }
}


void ODBCOracleTest::testLimit()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreateIntsTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->limits();
        i += 2;
    }
}


void ODBCOracleTest::testLimitZero()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreateIntsTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->limitZero();
        i += 2;
    }
}


void ODBCOracleTest::testLimitOnce()
{
    if (!_pSession) fail ("Test not available.");

    recreateIntsTable();
    _pExecutor->limitOnce();

}


void ODBCOracleTest::testLimitPrepare()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreateIntsTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->limitPrepare();
        i += 2;
    }
}



void ODBCOracleTest::testPrepare()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreateIntsTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->prepare();
        i += 2;
    }
}


void ODBCOracleTest::testSetSimple()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreatePersonTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->setSimple();
        i += 2;
    }
}


void ODBCOracleTest::testSetComplex()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreatePersonTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->setComplex();
        i += 2;
    }
}


void ODBCOracleTest::testSetComplexUnique()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreatePersonTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->setComplexUnique();
        i += 2;
    }
}

void ODBCOracleTest::testMultiSetSimple()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreatePersonTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->multiSetSimple();
        i += 2;
    }
}


void ODBCOracleTest::testMultiSetComplex()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreatePersonTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->multiSetComplex();
        i += 2;
    }
}


void ODBCOracleTest::testMapComplex()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreatePersonTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->mapComplex();
        i += 2;
    }
}


void ODBCOracleTest::testMapComplexUnique()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreatePersonTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->mapComplexUnique();
        i += 2;
    }
}


void ODBCOracleTest::testMultiMapComplex()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreatePersonTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->multiMapComplex();
        i += 2;
    }
}


void ODBCOracleTest::testSelectIntoSingle()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreatePersonTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->selectIntoSingle();
        i += 2;
    }
}


void ODBCOracleTest::testSelectIntoSingleStep()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreatePersonTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->selectIntoSingleStep();
        i += 2;
    }
}


void ODBCOracleTest::testSelectIntoSingleFail()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreatePersonTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->selectIntoSingleFail();
        i += 2;
    }
}


void ODBCOracleTest::testLowerLimitOk()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreatePersonTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->lowerLimitOk();
        i += 2;
    }
}


void ODBCOracleTest::testSingleSelect()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreatePersonTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->singleSelect();
        i += 2;
    }
}


void ODBCOracleTest::testLowerLimitFail()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreatePersonTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->lowerLimitFail();
        i += 2;
    }
}


void ODBCOracleTest::testCombinedLimits()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreatePersonTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->combinedLimits();
        i += 2;
    }
}



void ODBCOracleTest::testRange()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreatePersonTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->ranges();
        i += 2;
    }
}


void ODBCOracleTest::testCombinedIllegalLimits()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreatePersonTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->combinedIllegalLimits();
        i += 2;
    }
}



void ODBCOracleTest::testIllegalRange()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreatePersonTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->illegalRange();
        i += 2;
    }
}


void ODBCOracleTest::testEmptyDB()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreatePersonTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->emptyDB();
        i += 2;
    }
}


void ODBCOracleTest::testBLOB()
{
    if (!_pSession) fail ("Test not available.");

    const std::size_t maxFldSize = 1000000;
    _pSession->setProperty("maxFieldSize", Poco::Any(maxFldSize-1));
    recreatePersonBLOBTable();

    try
    {
        _pExecutor->blob(maxFldSize);
        fail ("must fail");
    }
    catch (DataException&)
    {
        _pSession->setProperty("maxFieldSize", Poco::Any(maxFldSize));
    }

    for (int i = 0; i < 8;)
    {
        recreatePersonBLOBTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->blob(maxFldSize);
        i += 2;
    }

    recreatePersonBLOBTable();
    try
    {
        _pExecutor->blob(maxFldSize+1);
        fail ("must fail");
    }
    catch (DataException&) { }
}


void ODBCOracleTest::testBLOBStmt()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreatePersonBLOBTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->blobStmt();
        i += 2;
    }
}


void ODBCOracleTest::testBool()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreateBoolsTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->bools();
        i += 2;
    }
}


void ODBCOracleTest::testFloat()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreateFloatsTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->floats();
        i += 2;
    }
}


void ODBCOracleTest::testDouble()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreateFloatsTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->doubles();
        i += 2;
    }
}


void ODBCOracleTest::testTuple()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreateTuplesTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->tuples();
        i += 2;
    }
}


void ODBCOracleTest::testTupleVector()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreateTuplesTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->tupleVector();
        i += 2;
    }
}


void ODBCOracleTest::testInternalExtraction()
{
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
        recreateVectorsTable();
        _pSession->setFeature("autoBind", bindValues[i]);
        _pSession->setFeature("autoExtract", bindValues[i+1]);
        _pExecutor->internalExtraction();
        i += 2;
    }
}


void ODBCOracleTest::dropTable(const std::string& tableName)
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
            if (942 == it->_nativeError)//ORA-00942 (table does not exist)
            {
                ignoreError = true;
                break;
            }
        }

        if (!ignoreError)
        {
            std::cout << ex.displayText() << std::endl;
            throw;
        }
    }
}


void ODBCOracleTest::recreatePersonTable()
{
    dropTable("Person");
    try
    {
        *_pSession << "CREATE TABLE Person (LastName VARCHAR2(30), FirstName VARCHAR2(30), Address VARCHAR2(30), Age INTEGER)", now;
    }
    catch(ConnectionException& ce)
    {
        std::cout << ce.toString() << std::endl;
        fail ("recreatePersonTable()");
    }
    catch(StatementException& se)
    {
        std::cout << se.toString() << std::endl;
        fail ("recreatePersonTable()");
    }
}


void ODBCOracleTest::recreatePersonTupleTable()
{
    dropTable("Person");
    try
    {
        *_pSession << "CREATE TABLE Person (LastName1 VARCHAR2(30), FirstName1 VARCHAR2(30), Address1 VARCHAR2(30), Age1 INTEGER,"
                   "LastName2 VARCHAR2(30), FirstName2 VARCHAR2(30), Address2 VARCHAR2(30), Age2 INTEGER)", now;
    }
    catch(ConnectionException& ce)
    {
        std::cout << ce.toString() << std::endl;
        fail ("recreatePersonTupleTable()");
    }
    catch(StatementException& se)
    {
        std::cout << se.toString() << std::endl;
        fail ("recreatePersonTupleTable()");
    }
}


void ODBCOracleTest::recreatePersonBLOBTable()
{
    dropTable("Person");
    try
    {
        *_pSession << "CREATE TABLE Person (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Image BLOB)", now;
    }
    catch(ConnectionException& ce)
    {
        std::cout << ce.toString() << std::endl;
        fail ("recreatePersonBLOBTable()");
    }
    catch(StatementException& se)
    {
        std::cout << se.toString() << std::endl;
        fail ("recreatePersonBLOBTable()");
    }
}


void ODBCOracleTest::recreateIntsTable()
{
    dropTable("Strings");
    try
    {
        *_pSession << "CREATE TABLE Strings (str INTEGER)", now;
    }
    catch(ConnectionException& ce)
    {
        std::cout << ce.toString() << std::endl;
        fail ("recreateIntsTable()");
    }
    catch(StatementException& se)
    {
        std::cout << se.toString() << std::endl;
        fail ("recreateIntsTable()");
    }
}


void ODBCOracleTest::recreateStringsTable()
{
    dropTable("Strings");
    try
    {
        *_pSession << "CREATE TABLE Strings (str VARCHAR(30))", now;
    }
    catch(ConnectionException& ce)
    {
        std::cout << ce.toString() << std::endl;
        fail ("recreateStringsTable()");
    }
    catch(StatementException& se)
    {
        std::cout << se.toString() << std::endl;
        fail ("recreateStringsTable()");
    }
}


void ODBCOracleTest::recreateBoolsTable()
{
    dropTable("Strings");
    try
    {
        *_pSession << "CREATE TABLE Strings (str INTEGER)", now;
    }
    catch(ConnectionException& ce)
    {
        std::cout << ce.toString() << std::endl;
        fail ("recreateBoolsTable()");
    }
    catch(StatementException& se)
    {
        std::cout << se.toString() << std::endl;
        fail ("recreateBoolsTable()");
    }
}


void ODBCOracleTest::recreateFloatsTable()
{
    dropTable("Strings");
    try
    {
        *_pSession << "CREATE TABLE Strings (str NUMBER)", now;
    }
    catch(ConnectionException& ce)
    {
        std::cout << ce.toString() << std::endl;
        fail ("recreateFloatsTable()");
    }
    catch(StatementException& se)
    {
        std::cout << se.toString() << std::endl;
        fail ("recreateFloatsTable()");
    }
}


void ODBCOracleTest::recreateTuplesTable()
{
    dropTable("Tuples");
    try
    {
        *_pSession << "CREATE TABLE Tuples "
                   "(int0 INTEGER, int1 INTEGER, int2 INTEGER, int3 INTEGER, int4 INTEGER, int5 INTEGER, int6 INTEGER, "
                   "int7 INTEGER, int8 INTEGER, int9 INTEGER, int10 INTEGER, int11 INTEGER, int12 INTEGER, int13 INTEGER,"
                   "int14 INTEGER, int15 INTEGER, int16 INTEGER, int17 INTEGER, int18 INTEGER, int19 INTEGER)", now;
    }
    catch(ConnectionException& ce)
    {
        std::cout << ce.toString() << std::endl;
        fail ("recreateTuplesTable()");
    }
    catch(StatementException& se)
    {
        std::cout << se.toString() << std::endl;
        fail ("recreateTuplesTable()");
    }
}


void ODBCOracleTest::recreateVectorsTable()
{
    dropTable("Vectors");
    try
    {
        *_pSession << "CREATE TABLE Vectors (int0 INTEGER, flt0 NUMBER, str0 VARCHAR(30))", now;
    }
    catch(ConnectionException& ce)
    {
        std::cout << ce.toString() << std::endl;
        fail ("recreateVectorsTable()");
    }
    catch(StatementException& se)
    {
        std::cout << se.toString() << std::endl;
        fail ("recreateVectorsTable()");
    }
}


void ODBCOracleTest::checkODBCSetup()
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
            if (((itDrv->first).find("Oracle") != std::string::npos) &&
                    ((itDrv->first).find("Microsoft") == std::string::npos))
            {
                std::cout << "Driver found: " << itDrv->first
                          << " (" << itDrv->second << ')' << std::endl;
                driverFound = true;
                break;
            }
        }

        if (!driverFound)
        {
            std::cout << "Oracle driver NOT found, tests will fail." << std::endl;
            return;
        }

        Utility::DSNMap::iterator itDSN = _dataSources.begin();
        for (; itDSN != _dataSources.end(); ++itDSN)
        {
            if (((itDSN->first).find(_dsn) != std::string::npos) &&
                    ((itDSN->second).find("Oracle") != std::string::npos))
            {
                std::cout << "DSN found: " << itDSN->first
                          << " (" << itDSN->second << ')' << std::endl;
                dsnFound = true;
                break;
            }
        }

        if (!dsnFound)
        {
            if (!_pSession && _dbConnString.empty())
            {
                std::cout << "Oracle DSN NOT found, will attempt to connect without it." << std::endl;
                _dbConnString = "DRIVER={" ORACLE_ODBC_DRIVER "};"
                                "DBQ=" ORACLE_SERVER ":" ORACLE_PORT "/" ORACLE_SID ";"
                                "UID=" ORACLE_UID ";"
                                "PWD=" ORACLE_PWD ";"
                                "TLO=O;" //?
                                "FBS=60000;" // fetch buffer size (bytes), default 60000
                                "FWC=F;" // force SQL_WCHAR support (T/F), default F
                                "CSR=F;" // close cursor (T/F), default F
                                "MDI=Me;" // metadata (SQL_ATTR_METADATA_ID) ID default (T/F), default T
                                "MTS=T;" //?
                                "DPM=F;" // disable SQLDescribeParam (T/F), default F
                                "NUM=NLS;" // numeric settings (NLS implies Globalization Support)
                                "BAM=IfAllSuccessful;" // batch autocommit, (IfAllSuccessful/UpToFirstFailure/AllSuccessful), default IfAllSuccessful
                                "BTD=F;" // bind timestamp as date (T/F), default F
                                "RST=T;" // resultsets (T/F), default T
                                "LOB=T;" // LOB writes (T/F), default T
                                "FDL=0;" // failover delay (default 10)
                                "FRC=0;" // failover retry count (default 10)
                                "QTO=T;" // query timout option (T/F), default T
                                "FEN=F;" // failover (T/F), default T
                                "XSM=Default;" // schema field (Default/Database/Owner), default Default
                                "EXC=F;" // EXEC syntax (T/F), default F
                                "APA=T;" // thread safety (T/F), default T
                                "DBA=W;"; // write access

                return;
            }
            else if (!_dbConnString.empty())
            {
                std::cout << "Oracle tests not available." << std::endl;
                return;
            }
        }
    }

    if (!_pSession)
        format(_dbConnString, "DSN=%s;Uid=%s;Pwd=%s;", _dsn,
               std::string(ORACLE_UID),
               std::string(ORACLE_PWD));
}


void ODBCOracleTest::setUp()
{
}


void ODBCOracleTest::tearDown()
{
}


CppUnit::Test* ODBCOracleTest::suite()
{
    CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ODBCOracleTest");

    CppUnit_addTest(pSuite, ODBCOracleTest, testBareboneODBC);
    CppUnit_addTest(pSuite, ODBCOracleTest, testSimpleAccess);
    CppUnit_addTest(pSuite, ODBCOracleTest, testComplexType);
    CppUnit_addTest(pSuite, ODBCOracleTest, testComplexTypeTuple);
    CppUnit_addTest(pSuite, ODBCOracleTest, testSimpleAccessVector);
    CppUnit_addTest(pSuite, ODBCOracleTest, testComplexTypeVector);
    CppUnit_addTest(pSuite, ODBCOracleTest, testInsertVector);
    CppUnit_addTest(pSuite, ODBCOracleTest, testInsertEmptyVector);
    CppUnit_addTest(pSuite, ODBCOracleTest, testInsertSingleBulk);
    CppUnit_addTest(pSuite, ODBCOracleTest, testInsertSingleBulkVec);
    CppUnit_addTest(pSuite, ODBCOracleTest, testLimit);
    CppUnit_addTest(pSuite, ODBCOracleTest, testLimitOnce);
    CppUnit_addTest(pSuite, ODBCOracleTest, testLimitPrepare);
    CppUnit_addTest(pSuite, ODBCOracleTest, testLimitZero);
    CppUnit_addTest(pSuite, ODBCOracleTest, testPrepare);
    CppUnit_addTest(pSuite, ODBCOracleTest, testSetSimple);
    CppUnit_addTest(pSuite, ODBCOracleTest, testSetComplex);
    CppUnit_addTest(pSuite, ODBCOracleTest, testSetComplexUnique);
    CppUnit_addTest(pSuite, ODBCOracleTest, testMultiSetSimple);
    CppUnit_addTest(pSuite, ODBCOracleTest, testMultiSetComplex);
    CppUnit_addTest(pSuite, ODBCOracleTest, testMapComplex);
    CppUnit_addTest(pSuite, ODBCOracleTest, testMapComplexUnique);
    CppUnit_addTest(pSuite, ODBCOracleTest, testMultiMapComplex);
    CppUnit_addTest(pSuite, ODBCOracleTest, testSelectIntoSingle);
    CppUnit_addTest(pSuite, ODBCOracleTest, testSelectIntoSingleStep);
    CppUnit_addTest(pSuite, ODBCOracleTest, testSelectIntoSingleFail);
    CppUnit_addTest(pSuite, ODBCOracleTest, testLowerLimitOk);
    CppUnit_addTest(pSuite, ODBCOracleTest, testLowerLimitFail);
    CppUnit_addTest(pSuite, ODBCOracleTest, testCombinedLimits);
    CppUnit_addTest(pSuite, ODBCOracleTest, testCombinedIllegalLimits);
    CppUnit_addTest(pSuite, ODBCOracleTest, testRange);
    CppUnit_addTest(pSuite, ODBCOracleTest, testIllegalRange);
    CppUnit_addTest(pSuite, ODBCOracleTest, testSingleSelect);
    CppUnit_addTest(pSuite, ODBCOracleTest, testEmptyDB);
    CppUnit_addTest(pSuite, ODBCOracleTest, testBLOB);
    CppUnit_addTest(pSuite, ODBCOracleTest, testBLOBStmt);
    CppUnit_addTest(pSuite, ODBCOracleTest, testBool);
    CppUnit_addTest(pSuite, ODBCOracleTest, testFloat);
    CppUnit_addTest(pSuite, ODBCOracleTest, testDouble);
    CppUnit_addTest(pSuite, ODBCOracleTest, testTuple);
    CppUnit_addTest(pSuite, ODBCOracleTest, testTupleVector);
    CppUnit_addTest(pSuite, ODBCOracleTest, testInternalExtraction);

    return pSuite;
}
