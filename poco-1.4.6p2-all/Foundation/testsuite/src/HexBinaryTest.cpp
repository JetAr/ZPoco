﻿//
// HexBinaryTest.cpp
//
// $Id: //poco/1.4/Foundation/testsuite/src/HexBinaryTest.cpp#1 $
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


#include "HexBinaryTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/HexBinaryEncoder.h"
#include "Poco/HexBinaryDecoder.h"
#include "Poco/Exception.h"
#include <sstream>


using Poco::HexBinaryEncoder;
using Poco::HexBinaryDecoder;
using Poco::DataFormatException;


HexBinaryTest::HexBinaryTest(const std::string& name): CppUnit::TestCase(name)
{
}


HexBinaryTest::~HexBinaryTest()
{
}


void HexBinaryTest::testEncoder()
{
    {
        std::ostringstream str;
        HexBinaryEncoder encoder(str);
        encoder << std::string("\00\01\02\03\04\05", 6);
        encoder.close();
        assert (str.str() == "000102030405");
    }
    {
        std::ostringstream str;
        HexBinaryEncoder encoder(str);
        encoder << std::string("\00\01\02\03", 4);
        encoder.close();
        assert (str.str() == "00010203");
    }
    {
        std::ostringstream str;
        HexBinaryEncoder encoder(str);
        encoder << "ABCDEF";
        encoder << char(0xaa) << char(0xbb);
        encoder.close();
        assert (str.str() == "414243444546aabb");
    }
    {
        std::ostringstream str;
        HexBinaryEncoder encoder(str);
        encoder.rdbuf()->setUppercase();
        encoder << "ABCDEF";
        encoder << char(0xaa) << char(0xbb);
        encoder.close();
        assert (str.str() == "414243444546AABB");
    }
}


void HexBinaryTest::testDecoder()
{
    {
        std::istringstream istr("000102030405");
        HexBinaryDecoder decoder(istr);
        assert (decoder.good() && decoder.get() == 0);
        assert (decoder.good() && decoder.get() == 1);
        assert (decoder.good() && decoder.get() == 2);
        assert (decoder.good() && decoder.get() == 3);
        assert (decoder.good() && decoder.get() == 4);
        assert (decoder.good() && decoder.get() == 5);
        assert (decoder.good() && decoder.get() == -1);
    }
    {
        std::istringstream istr("0001020304");
        HexBinaryDecoder decoder(istr);
        assert (decoder.good() && decoder.get() == 0);
        assert (decoder.good() && decoder.get() == 1);
        assert (decoder.good() && decoder.get() == 2);
        assert (decoder.good() && decoder.get() == 3);
        assert (decoder.good() && decoder.get() == 4);
        assert (decoder.good() && decoder.get() == -1);
    }
    {
        std::istringstream istr("0a0bcdef");
        HexBinaryDecoder decoder(istr);
        assert (decoder.good() && decoder.get() == 0x0a);
        assert (decoder.good() && decoder.get() == 0x0b);
        assert (decoder.good() && decoder.get() == 0xcd);
        assert (decoder.good() && decoder.get() == 0xef);
        assert (decoder.good() && decoder.get() == -1);
    }
    {
        std::istringstream istr("0A0BCDEF");
        HexBinaryDecoder decoder(istr);
        assert (decoder.good() && decoder.get() == 0x0a);
        assert (decoder.good() && decoder.get() == 0x0b);
        assert (decoder.good() && decoder.get() == 0xcd);
        assert (decoder.good() && decoder.get() == 0xef);
        assert (decoder.good() && decoder.get() == -1);
    }
    {
        std::istringstream istr("00 01 02 03");
        HexBinaryDecoder decoder(istr);
        assert (decoder.good() && decoder.get() == 0);
        assert (decoder.good() && decoder.get() == 1);
        assert (decoder.good() && decoder.get() == 2);
        assert (decoder.good() && decoder.get() == 3);
        assert (decoder.good() && decoder.get() == -1);
    }
    {
        std::istringstream istr("414243444546");
        HexBinaryDecoder decoder(istr);
        std::string s;
        decoder >> s;
        assert (s == "ABCDEF");
        assert (decoder.eof());
        assert (!decoder.fail());
    }
    {
        std::istringstream istr("4041\r\n4243\r\n4445");
        HexBinaryDecoder decoder(istr);
        std::string s;
        decoder >> s;
        assert (s == "@ABCDE");
        assert (decoder.eof());
        assert (!decoder.fail());
    }
    {
        std::istringstream istr("AABB#CCDD");
        HexBinaryDecoder decoder(istr);
        std::string s;
        try
        {
            decoder >> s;
            assert (decoder.bad());
        }
        catch (DataFormatException&)
        {
        }
        assert (!decoder.eof());
    }
}


void HexBinaryTest::testEncodeDecode()
{
    {
        std::stringstream str;
        HexBinaryEncoder encoder(str);
        encoder << "The quick brown fox ";
        encoder << "jumped over the lazy dog.";
        encoder.close();
        HexBinaryDecoder decoder(str);
        std::string s;
        int c = decoder.get();
        while (c != -1)
        {
            s += char(c);
            c = decoder.get();
        }
        assert (s == "The quick brown fox jumped over the lazy dog.");
    }
    {
        std::string src;
        for (int i = 0; i < 255; ++i) src += char(i);
        std::stringstream str;
        HexBinaryEncoder encoder(str);
        encoder.write(src.data(), (std::streamsize) src.size());
        encoder.close();
        HexBinaryDecoder decoder(str);
        std::string s;
        int c = decoder.get();
        while (c != -1)
        {
            s += char(c);
            c = decoder.get();
        }
        assert (s == src);
    }
}


void HexBinaryTest::setUp()
{
}


void HexBinaryTest::tearDown()
{
}


CppUnit::Test* HexBinaryTest::suite()
{
    CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HexBinaryTest");

    CppUnit_addTest(pSuite, HexBinaryTest, testEncoder);
    CppUnit_addTest(pSuite, HexBinaryTest, testDecoder);
    CppUnit_addTest(pSuite, HexBinaryTest, testEncodeDecode);

    return pSuite;
}
