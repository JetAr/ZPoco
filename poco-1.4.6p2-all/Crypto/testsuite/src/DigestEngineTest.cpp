﻿//
// DigestEngineTest.cpp
//
// $Id: //poco/1.4/Crypto/testsuite/src/DigestEngineTest.cpp#1 $
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
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


#include "DigestEngineTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Crypto/DigestEngine.h"


using Poco::Crypto::DigestEngine;


DigestEngineTest::DigestEngineTest(const std::string& name): CppUnit::TestCase(name)
{
}


DigestEngineTest::~DigestEngineTest()
{
}


void DigestEngineTest::testMD5()
{
    DigestEngine engine("MD5");

    // test vectors from RFC 1321

    engine.update("");
    assert (DigestEngine::digestToHex(engine.digest()) == "d41d8cd98f00b204e9800998ecf8427e");

    engine.update("a");
    assert (DigestEngine::digestToHex(engine.digest()) == "0cc175b9c0f1b6a831c399e269772661");

    engine.update("abc");
    assert (DigestEngine::digestToHex(engine.digest()) == "900150983cd24fb0d6963f7d28e17f72");

    engine.update("message digest");
    assert (DigestEngine::digestToHex(engine.digest()) == "f96b697d7cb7938d525a2f31aaf161d0");

    engine.update("abcdefghijklmnopqrstuvwxyz");
    assert (DigestEngine::digestToHex(engine.digest()) == "c3fcd3d76192e4007dfb496cca67e13b");

    engine.update("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    engine.update("abcdefghijklmnopqrstuvwxyz0123456789");
    assert (DigestEngine::digestToHex(engine.digest()) == "d174ab98d277d9f5a5611c2c9f419d9f");

    engine.update("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
    assert (DigestEngine::digestToHex(engine.digest()) == "57edf4a22be3c955ac49da2e2107b67a");
}


void DigestEngineTest::setUp()
{
}


void DigestEngineTest::tearDown()
{
}


CppUnit::Test* DigestEngineTest::suite()
{
    CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("DigestEngineTest");

    CppUnit_addTest(pSuite, DigestEngineTest, testMD5);

    return pSuite;
}
