﻿//
// ODBCDB2Test.cpp
//
// $Id: //poco/1.4/Data/ODBC/testsuite/src/ODBCDB2Test.cpp#1 $
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


#include "ODBCDB2Test.h"
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


#define DB2_ODBC_DRIVER "IBM DB2 ODBC DRIVER - DB2COPY1"
#define DB2_DSN "PocoDataDB2Test"
#define DB2_SERVER "localhost"
#define DB2_PORT "50000"
#define DB2_DB "POCOTEST"
#define DB2_UID "db2admin"
#define DB2_PWD "db2admin"


const bool ODBCDB2Test::bindValues[8] = {true, true, true, false, false, true, false, false};
Poco::SharedPtr<Poco::Data::Session> ODBCDB2Test::_pSession = 0;
Poco::SharedPtr<SQLExecutor> ODBCDB2Test::_pExecutor = 0;
std::string ODBCDB2Test::_dsn = "PocoDataDB2Test";
std::string ODBCDB2Test::_dbConnString;
Poco::Data::ODBC::Utility::DriverMap ODBCDB2Test::_drivers;
Poco::Data::ODBC::Utility::DSNMap ODBCDB2Test::_dataSources;


ODBCDB2Test::ODBCDB2Test(const std::string& name):
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
            std::cout << "!!! WARNING: Connection failed. DB2 tests will fail !!!" << std::endl;
            std::cout << ex.toString() << std::endl;
        }

        if (_pSession && _pSession->isConnected())
            std::cout << "*** Connected to " << _dsn << '(' << _dbConnString << ')' << std::endl;
        if (!_pExecutor) _pExecutor = new SQLExecutor("DB2 SQL Executor", _pSession);
    }
    else if (!_pSession && !beenHere)
        std::cout << "!!! WARNING: No driver or DSN found. DB2 tests will fail !!!" << std::endl;

    beenHere = true;
}


ODBCDB2Test::~ODBCDB2Test()
{
    ODBC::Connector::unregisterConnector();
}


void ODBCDB2Test::testBareboneODBC()
{
    if (!_pSession) fail ("Test not available.");

    std::string tableCreateString = "CREATE TABLE Test "
                                    "(First VARCHAR(30),"
                                    "Second VARCHAR(30),"
                                    "Third BLOB,"
                                    "Fourth INTEGER,"
                                    "Fifth FLOAT,"
                                    "Sixth TIMESTAMP)";

    _pExecutor->bareboneODBCTest(_dbConnString, tableCreateString, SQLExecutor::PB_IMMEDIATE, SQLExecutor::DE_MANUAL);
    _pExecutor->bareboneODBCTest(_dbConnString, tableCreateString, SQLExecutor::PB_IMMEDIATE, SQLExecutor::DE_BOUND);
    _pExecutor->bareboneODBCTest(_dbConnString, tableCreateString, SQLExecutor::PB_AT_EXEC, SQLExecutor::DE_MANUAL);
    _pExecutor->bareboneODBCTest(_dbConnString, tableCreateString, SQLExecutor::PB_AT_EXEC, SQLExecutor::DE_BOUND);
}


void ODBCDB2Test::testSimpleAccess()
{
    if (!_pSession) fail ("Test not available.");

    std::string tableName("Person");
    int count = 0;

    recreatePersonTable();

    *_pSession << "SELECT count(*) FROM sysibm.systables WHERE name = 'PERSON' AND creator = 'DB2ADMIN'",
               into(count),
               use(tableName),
               now;
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


void ODBCDB2Test::testComplexType()
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


void ODBCDB2Test::testSimpleAccessVector()
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


void ODBCDB2Test::testComplexTypeVector()
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


void ODBCDB2Test::testInsertVector()
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


void ODBCDB2Test::testInsertEmptyVector()
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


void ODBCDB2Test::testInsertSingleBulk()
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


void ODBCDB2Test::testInsertSingleBulkVec()
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


void ODBCDB2Test::testLimit()
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


void ODBCDB2Test::testLimitZero()
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


void ODBCDB2Test::testLimitOnce()
{
    if (!_pSession) fail ("Test not available.");

    recreateIntsTable();
    _pExecutor->limitOnce();

}


void ODBCDB2Test::testLimitPrepare()
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



void ODBCDB2Test::testPrepare()
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


void ODBCDB2Test::testSetSimple()
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


void ODBCDB2Test::testSetComplex()
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


void ODBCDB2Test::testSetComplexUnique()
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

void ODBCDB2Test::testMultiSetSimple()
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


void ODBCDB2Test::testMultiSetComplex()
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


void ODBCDB2Test::testMapComplex()
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


void ODBCDB2Test::testMapComplexUnique()
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


void ODBCDB2Test::testMultiMapComplex()
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


void ODBCDB2Test::testSelectIntoSingle()
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


void ODBCDB2Test::testSelectIntoSingleStep()
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


void ODBCDB2Test::testSelectIntoSingleFail()
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


void ODBCDB2Test::testLowerLimitOk()
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


void ODBCDB2Test::testSingleSelect()
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


void ODBCDB2Test::testLowerLimitFail()
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


void ODBCDB2Test::testCombinedLimits()
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



void ODBCDB2Test::testRange()
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


void ODBCDB2Test::testCombinedIllegalLimits()
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



void ODBCDB2Test::testIllegalRange()
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


void ODBCDB2Test::testEmptyDB()
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


void ODBCDB2Test::testBLOB()
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


void ODBCDB2Test::testBLOBStmt()
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


void ODBCDB2Test::testBool()
{
    fail ("not supported");
    /*
    if (!_pSession) fail ("Test not available.");

    for (int i = 0; i < 8;)
    {
    	recreateBoolsTable();
    	_pSession->setFeature("autoBind", bindValues[i]);
    	_pSession->setFeature("autoExtract", bindValues[i+1]);
    	_pExecutor->bools();
    	i += 2;
    }*/
}


void ODBCDB2Test::testFloat()
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


void ODBCDB2Test::testDouble()
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


void ODBCDB2Test::testTuple()
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


void ODBCDB2Test::testTupleVector()
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


void ODBCDB2Test::testInternalExtraction()
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


void ODBCDB2Test::dropTable(const std::string& tableName)
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
            if (-204 == it->_nativeError)//(table does not exist)
            {
                ignoreError = true;
                break;
            }
        }

        if (!ignoreError) throw;
    }
}


void ODBCDB2Test::recreatePersonTable()
{
    dropTable("Person");
    try
    {
        *_pSession << "CREATE TABLE Person (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Age INTEGER)", now;
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


void ODBCDB2Test::recreatePersonBLOBTable()
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


void ODBCDB2Test::recreateIntsTable()
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


void ODBCDB2Test::recreateStringsTable()
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


void ODBCDB2Test::recreateBoolsTable()
{
    dropTable("Strings");
    try
    {
        *_pSession << "CREATE TABLE Strings (str SMALLINT)", now;
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


void ODBCDB2Test::recreateFloatsTable()
{
    dropTable("Strings");
    try
    {
        *_pSession << "CREATE TABLE Strings (str FLOAT)", now;
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


void ODBCDB2Test::recreateTuplesTable()
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


void ODBCDB2Test::recreateVectorsTable()
{
    dropTable("Vectors");
    try
    {
        *_pSession << "CREATE TABLE Vectors (i0 INTEGER, flt0 FLOAT, str0 VARCHAR(30))", now;
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


void ODBCDB2Test::checkODBCSetup()
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
            if (((itDrv->first).find("IBM DB2") != std::string::npos))
            {
                std::cout << "Driver found: " << itDrv->first
                          << " (" << itDrv->second << ')' << std::endl;
                driverFound = true;
                break;
            }
        }

        if (!driverFound)
        {
            std::cout << "DB2 driver NOT found, tests will fail." << std::endl;
            return;
        }

        Utility::DSNMap::iterator itDSN = _dataSources.begin();
        for (; itDSN != _dataSources.end(); ++itDSN)
        {
            if (((itDSN->first).find(_dsn) != std::string::npos) &&
                    ((itDSN->second).find("IBM DB2") != std::string::npos))
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
                std::cout << "DB2 DSN NOT found, will attempt to connect without it." << std::endl;
                _dbConnString = "Driver=" DB2_ODBC_DRIVER ";"
                                "Database=" DB2_DB ";"
                                "Hostname=" DB2_SERVER ";"
                                "Port=" DB2_PORT ";"
                                "Protocol=TCPIP;"
                                "Uid=" DB2_UID ";"
                                "Pwd=" DB2_PWD ";";
            }
            else if (!_dbConnString.empty())
            {
                std::cout << "DB2 tests not available." << std::endl;
                return;
            }
        }
    }

    if (!_pSession)
        format(_dbConnString, "DSN=%s;Uid=db2admin;Pwd=db2admin;", _dsn);
}


void ODBCDB2Test::setUp()
{
}


void ODBCDB2Test::tearDown()
{
    dropTable("Person");
    dropTable("Strings");
}


CppUnit::Test* ODBCDB2Test::suite()
{
    CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ODBCDB2Test");

    CppUnit_addTest(pSuite, ODBCDB2Test, testBareboneODBC);
    CppUnit_addTest(pSuite, ODBCDB2Test, testSimpleAccess);
    CppUnit_addTest(pSuite, ODBCDB2Test, testComplexType);
    CppUnit_addTest(pSuite, ODBCDB2Test, testSimpleAccessVector);
    CppUnit_addTest(pSuite, ODBCDB2Test, testComplexTypeVector);
    CppUnit_addTest(pSuite, ODBCDB2Test, testInsertVector);
    CppUnit_addTest(pSuite, ODBCDB2Test, testInsertEmptyVector);
    CppUnit_addTest(pSuite, ODBCDB2Test, testInsertSingleBulk);
    CppUnit_addTest(pSuite, ODBCDB2Test, testInsertSingleBulkVec);
    CppUnit_addTest(pSuite, ODBCDB2Test, testLimit);
    CppUnit_addTest(pSuite, ODBCDB2Test, testLimitOnce);
    CppUnit_addTest(pSuite, ODBCDB2Test, testLimitPrepare);
    CppUnit_addTest(pSuite, ODBCDB2Test, testLimitZero);
    CppUnit_addTest(pSuite, ODBCDB2Test, testPrepare);
    CppUnit_addTest(pSuite, ODBCDB2Test, testSetSimple);
    CppUnit_addTest(pSuite, ODBCDB2Test, testSetComplex);
    CppUnit_addTest(pSuite, ODBCDB2Test, testSetComplexUnique);
    CppUnit_addTest(pSuite, ODBCDB2Test, testMultiSetSimple);
    CppUnit_addTest(pSuite, ODBCDB2Test, testMultiSetComplex);
    CppUnit_addTest(pSuite, ODBCDB2Test, testMapComplex);
    CppUnit_addTest(pSuite, ODBCDB2Test, testMapComplexUnique);
    CppUnit_addTest(pSuite, ODBCDB2Test, testMultiMapComplex);
    CppUnit_addTest(pSuite, ODBCDB2Test, testSelectIntoSingle);
    CppUnit_addTest(pSuite, ODBCDB2Test, testSelectIntoSingleStep);
    CppUnit_addTest(pSuite, ODBCDB2Test, testSelectIntoSingleFail);
    CppUnit_addTest(pSuite, ODBCDB2Test, testLowerLimitOk);
    CppUnit_addTest(pSuite, ODBCDB2Test, testLowerLimitFail);
    CppUnit_addTest(pSuite, ODBCDB2Test, testCombinedLimits);
    CppUnit_addTest(pSuite, ODBCDB2Test, testCombinedIllegalLimits);
    CppUnit_addTest(pSuite, ODBCDB2Test, testRange);
    CppUnit_addTest(pSuite, ODBCDB2Test, testIllegalRange);
    CppUnit_addTest(pSuite, ODBCDB2Test, testSingleSelect);
    CppUnit_addTest(pSuite, ODBCDB2Test, testEmptyDB);
    CppUnit_addTest(pSuite, ODBCDB2Test, testBLOB);
    CppUnit_addTest(pSuite, ODBCDB2Test, testBLOBStmt);
    CppUnit_addTest(pSuite, ODBCDB2Test, testBool);
    CppUnit_addTest(pSuite, ODBCDB2Test, testFloat);
    CppUnit_addTest(pSuite, ODBCDB2Test, testDouble);
    CppUnit_addTest(pSuite, ODBCDB2Test, testTuple);
    CppUnit_addTest(pSuite, ODBCDB2Test, testTupleVector);
    CppUnit_addTest(pSuite, ODBCDB2Test, testInternalExtraction);

    return pSuite;
}
