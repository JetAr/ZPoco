﻿//
// ManifestTest.cpp
//
// $Id: //poco/1.4/Foundation/testsuite/src/ManifestTest.cpp#1 $
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


#include "ManifestTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Manifest.h"
#include "Poco/MetaObject.h"
#include <set>


using Poco::Manifest;
using Poco::MetaObject;


class MfTestBase
{
};


class MfTestObject: public MfTestBase
{
};


ManifestTest::ManifestTest(const std::string& name): CppUnit::TestCase(name)
{
}


ManifestTest::~ManifestTest()
{
}


void ManifestTest::testManifest()
{
    Manifest<MfTestBase> manifest;
    assert (manifest.empty());
    assert (manifest.size() == 0);
    assert (manifest.insert(new MetaObject<MfTestObject, MfTestBase>("MfTestObject1")));
    assert (!manifest.empty());
    assert (manifest.size() == 1);
    assert (manifest.insert(new MetaObject<MfTestObject, MfTestBase>("MfTestObject2")));
    MetaObject<MfTestObject, MfTestBase>* pMeta = new MetaObject<MfTestObject, MfTestBase>("MfTestObject2");
    assert (!manifest.insert(pMeta));
    delete pMeta;
    assert (!manifest.empty());
    assert (manifest.size() == 2);
    assert (manifest.insert(new MetaObject<MfTestObject, MfTestBase>("MfTestObject3")));
    assert (manifest.size() == 3);

    assert (manifest.find("MfTestObject1") != manifest.end());
    assert (manifest.find("MfTestObject2") != manifest.end());
    assert (manifest.find("MfTestObject3") != manifest.end());
    assert (manifest.find("MfTestObject4") == manifest.end());

    std::set<std::string> classes;

    Manifest<MfTestBase>::Iterator it = manifest.begin();
    assert (it != manifest.end());
    classes.insert(it->name());
    ++it;
    assert (it != manifest.end());
    classes.insert(it->name());
    ++it;
    assert (it != manifest.end());
    classes.insert(it->name());
    it++;
    assert (it == manifest.end());

    assert (classes.find("MfTestObject1") != classes.end());
    assert (classes.find("MfTestObject2") != classes.end());
    assert (classes.find("MfTestObject3") != classes.end());

    manifest.clear();
    assert (manifest.empty());
    assert (manifest.size() == 0);
    assert (manifest.insert(new MetaObject<MfTestObject, MfTestBase>("MfTestObject4")));
    assert (!manifest.empty());
    assert (manifest.size() == 1);
    it = manifest.begin();
    assert (it != manifest.end());
    assert (std::string(it->name()) == "MfTestObject4");
    ++it;
    assert (it == manifest.end());
}


void ManifestTest::setUp()
{
}


void ManifestTest::tearDown()
{
}


CppUnit::Test* ManifestTest::suite()
{
    CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ManifestTest");

    CppUnit_addTest(pSuite, ManifestTest, testManifest);

    return pSuite;
}
