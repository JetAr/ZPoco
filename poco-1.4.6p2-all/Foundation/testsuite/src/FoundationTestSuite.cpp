﻿//
// FoundationTestSuite.cpp
//
// $Id: //poco/1.4/Foundation/testsuite/src/FoundationTestSuite.cpp#3 $
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


#include "FoundationTestSuite.h"
#include "CoreTestSuite.h"
#include "DateTimeTestSuite.h"
#include "StreamsTestSuite.h"
#include "CryptTestSuite.h"
#include "NotificationsTestSuite.h"
#include "ThreadingTestSuite.h"
#include "SharedLibraryTestSuite.h"
#include "LoggingTestSuite.h"
#include "FilesystemTestSuite.h"
#include "UUIDTestSuite.h"
#include "TextTestSuite.h"
#include "URITestSuite.h"
#if !defined(POCO_VXWORKS)
#include "ProcessesTestSuite.h"
#endif
#include "TaskTestSuite.h"
#include "EventTestSuite.h"
#include "CacheTestSuite.h"
#include "HashingTestSuite.h"


CppUnit::Test* FoundationTestSuite::suite()
{
    CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("FoundationTestSuite");

    pSuite->addTest(CoreTestSuite::suite());
    pSuite->addTest(DateTimeTestSuite::suite());
    pSuite->addTest(StreamsTestSuite::suite());
    pSuite->addTest(CryptTestSuite::suite());
    pSuite->addTest(NotificationsTestSuite::suite());
    pSuite->addTest(ThreadingTestSuite::suite());
    pSuite->addTest(SharedLibraryTestSuite::suite());
    pSuite->addTest(LoggingTestSuite::suite());
    pSuite->addTest(FilesystemTestSuite::suite());
    pSuite->addTest(UUIDTestSuite::suite());
    pSuite->addTest(TextTestSuite::suite());
    pSuite->addTest(URITestSuite::suite());
#if !defined(POCO_VXWORKS)
    pSuite->addTest(ProcessesTestSuite::suite());
#endif
    pSuite->addTest(TaskTestSuite::suite());
    pSuite->addTest(EventTestSuite::suite());
    pSuite->addTest(CacheTestSuite::suite());
    pSuite->addTest(HashingTestSuite::suite());

    return pSuite;
}
