﻿//
// DatagramSocketTest.cpp
//
// $Id: //poco/1.4/Net/testsuite/src/DatagramSocketTest.cpp#1 $
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
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


#include "DatagramSocketTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "UDPEchoServer.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"
#include "Poco/Timespan.h"
#include "Poco/Stopwatch.h"


using Poco::Net::Socket;
using Poco::Net::DatagramSocket;
using Poco::Net::SocketAddress;
using Poco::Net::IPAddress;
using Poco::Timespan;
using Poco::Stopwatch;
using Poco::TimeoutException;
using Poco::InvalidArgumentException;
using Poco::IOException;


DatagramSocketTest::DatagramSocketTest(const std::string& name): CppUnit::TestCase(name)
{
}


DatagramSocketTest::~DatagramSocketTest()
{
}


void DatagramSocketTest::testEcho()
{
    UDPEchoServer echoServer;
    DatagramSocket ss;
    ss.connect(SocketAddress("localhost", echoServer.port()));
    int n = ss.sendBytes("hello", 5);
    assert (n == 5);
    char buffer[256];
    n = ss.receiveBytes(buffer, sizeof(buffer));
    assert (n == 5);
    assert (std::string(buffer, n) == "hello");
    ss.close();
}


void DatagramSocketTest::testSendToReceiveFrom()
{
    UDPEchoServer echoServer(SocketAddress("localhost", 0));
    DatagramSocket ss;
    int n = ss.sendTo("hello", 5, SocketAddress("localhost", echoServer.port()));
    assert (n == 5);
    char buffer[256];
    SocketAddress sa;
    n = ss.receiveFrom(buffer, sizeof(buffer), sa);
    assert (sa.host() == echoServer.address().host());
    assert (sa.port() == echoServer.port());
    assert (n == 5);
    assert (std::string(buffer, n) == "hello");
    ss.close();
}


void DatagramSocketTest::testBroadcast()
{
    UDPEchoServer echoServer;
    DatagramSocket ss(IPAddress::IPv4);
    SocketAddress sa("255.255.255.255", echoServer.port());
    try
    {
        int n = ss.sendTo("hello", 5, sa);
        // not all socket implementations fail if broadcast option is not set
        // fail ("broadcast option not set - must throw");
    }
    catch (IOException&)
    {
    }
    ss.setBroadcast(true);
    int n = ss.sendTo("hello", 5, sa);
    assert (n == 5);
    char buffer[256];
    n = ss.receiveBytes(buffer, sizeof(buffer));
    assert (n == 5);
    assert (std::string(buffer, n) == "hello");
    ss.close();
}


void DatagramSocketTest::setUp()
{
}


void DatagramSocketTest::tearDown()
{
}


CppUnit::Test* DatagramSocketTest::suite()
{
    CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("DatagramSocketTest");

    CppUnit_addTest(pSuite, DatagramSocketTest, testEcho);
    CppUnit_addTest(pSuite, DatagramSocketTest, testSendToReceiveFrom);
    CppUnit_addTest(pSuite, DatagramSocketTest, testBroadcast);

    return pSuite;
}
