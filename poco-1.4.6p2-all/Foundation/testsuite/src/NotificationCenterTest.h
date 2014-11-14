﻿//
// NotificationCenterTest.h
//
// $Id: //poco/1.4/Foundation/testsuite/src/NotificationCenterTest.h#1 $
//
// Definition of the NotificationCenterTest class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
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


#ifndef NotificationCenterTest_INCLUDED
#define NotificationCenterTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"
#include "Poco/Notification.h"
#include "Poco/AutoPtr.h"
#include <set>


class TestNotification;


class NotificationCenterTest: public CppUnit::TestCase
{
public:
    NotificationCenterTest(const std::string& name);
    ~NotificationCenterTest();

    void test1();
    void test2();
    void test3();
    void test4();
    void test5();
    void testAuto();
    void testDefaultCenter();

    void setUp();
    void tearDown();

    static CppUnit::Test* suite();

protected:
    void handle1(Poco::Notification* pNf);
    void handle2(Poco::Notification* pNf);
    void handle3(Poco::Notification* pNf);
    void handleTest(TestNotification* pNf);
    void handleAuto(const Poco::AutoPtr<Poco::Notification>& pNf);

private:
    std::set<std::string> _set;
};


#endif // NotificationCenterTest_INCLUDED
