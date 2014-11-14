﻿//
// SocketAddressTest.cpp
//
// $Id: //poco/1.4/Net/testsuite/src/SocketAddressTest.cpp#4 $
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


#include "SocketAddressTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"


using Poco::Net::SocketAddress;
using Poco::Net::IPAddress;
using Poco::Net::InvalidAddressException;
using Poco::Net::HostNotFoundException;
using Poco::Net::ServiceNotFoundException;
using Poco::Net::NoAddressFoundException;
using Poco::InvalidArgumentException;


SocketAddressTest::SocketAddressTest(const std::string& name): CppUnit::TestCase(name)
{
}


SocketAddressTest::~SocketAddressTest()
{
}

void SocketAddressTest::testSocketAddress()
{
    SocketAddress wild;
    assert (wild.host().isWildcard());
    assert (wild.port() == 0);

    SocketAddress sa1("192.168.1.100", 100);
    assert (sa1.host().toString() == "192.168.1.100");
    assert (sa1.port() == 100);

    SocketAddress sa2("192.168.1.100", "100");
    assert (sa2.host().toString() == "192.168.1.100");
    assert (sa2.port() == 100);

#if !defined(_WIN32_WCE)
    SocketAddress sa3("192.168.1.100", "ftp");
    assert (sa3.host().toString() == "192.168.1.100");
    assert (sa3.port() == 21);
#endif

    try
    {
        SocketAddress sa3("192.168.1.100", "f00bar");
        fail("bad service name - must throw");
    }
    catch (ServiceNotFoundException&)
    {
    }

    SocketAddress sa4("www.appinf.com", 80);
    assert (sa4.host().toString() == "50.57.108.29");
    assert (sa4.port() == 80);

    try
    {
        SocketAddress sa5("192.168.2.260", 80);
        fail("invalid address - must throw");
    }
    catch (HostNotFoundException&)
    {
    }
    catch (NoAddressFoundException&)
    {
    }

    try
    {
        SocketAddress sa6("192.168.2.120", "80000");
        fail("invalid port - must throw");
    }
    catch (ServiceNotFoundException&)
    {
    }

    SocketAddress sa7("192.168.2.120:88");
    assert (sa7.host().toString() == "192.168.2.120");
    assert (sa7.port() == 88);

    SocketAddress sa8("[192.168.2.120]:88");
    assert (sa8.host().toString() == "192.168.2.120");
    assert (sa8.port() == 88);

    try
    {
        SocketAddress sa9("[192.168.2.260]");
        fail("invalid address - must throw");
    }
    catch (InvalidArgumentException&)
    {
    }

    try
    {
        SocketAddress sa9("[192.168.2.260:88");
        fail("invalid address - must throw");
    }
    catch (InvalidArgumentException&)
    {
    }
}


void SocketAddressTest::testRelationals()
{
    SocketAddress sa1("192.168.1.100", 100);
    SocketAddress sa2("192.168.1.100:100");
    assert (sa1 == sa2);

    SocketAddress sa3("192.168.1.101", "99");
    assert (sa2 < sa3);

    SocketAddress sa4("192.168.1.101", "102");
    assert (sa3 < sa4);
}


void SocketAddressTest::testSocketAddress6()
{
#ifdef POCO_HAVE_IPv6
#endif
}


void SocketAddressTest::setUp()
{
}


void SocketAddressTest::tearDown()
{
}


CppUnit::Test* SocketAddressTest::suite()
{
    CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SocketAddressTest");

    CppUnit_addTest(pSuite, SocketAddressTest, testSocketAddress);
    CppUnit_addTest(pSuite, SocketAddressTest, testRelationals);
    CppUnit_addTest(pSuite, SocketAddressTest, testSocketAddress6);

    return pSuite;
}
