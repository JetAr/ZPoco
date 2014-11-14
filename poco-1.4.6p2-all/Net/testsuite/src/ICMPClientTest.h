﻿//
// ICMPClientTest.h
//
// $Id: //poco/1.4/Net/testsuite/src/ICMPClientTest.h#1 $
//
// Definition of the ICMPClientTest class.
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


#ifndef ICMPClientTest_INCLUDED
#define ICMPClientTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"
#include "Poco/Net/ICMPClient.h"
#include "Poco/Net/ICMPEventArgs.h"


class ICMPClientTest: public CppUnit::TestCase
{
public:
    ICMPClientTest(const std::string& name);
    ~ICMPClientTest();

    void testPing();

    void setUp();
    void tearDown();

    static CppUnit::Test* suite();

    void onBegin(const void* pSender, Poco::Net::ICMPEventArgs& args);
    void onReply(const void* pSender, Poco::Net::ICMPEventArgs& args);
    void onError(const void* pSender, Poco::Net::ICMPEventArgs& args);
    void onEnd(const void* pSender, Poco::Net::ICMPEventArgs& args);

private:
    Poco::Net::ICMPClient _icmpClient;
};


#endif // ICMPClientTest_INCLUDED
