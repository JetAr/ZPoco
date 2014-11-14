﻿//
// Base64Test.cpp
//
// $Id: //poco/1.4/Foundation/testsuite/src/Base64Test.cpp#1 $
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


#include "Base64Test.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Base64Decoder.h"
#include "Poco/Exception.h"
#include <sstream>


using Poco::Base64Encoder;
using Poco::Base64Decoder;
using Poco::DataFormatException;


Base64Test::Base64Test(const std::string& name): CppUnit::TestCase(name)
{
}


Base64Test::~Base64Test()
{
}


void Base64Test::testEncoder()
{
    {
        std::ostringstream str;
        Base64Encoder encoder(str);
        encoder << std::string("\00\01\02\03\04\05", 6);
        encoder.close();
        assert (str.str() == "AAECAwQF");
    }
    {
        std::ostringstream str;
        Base64Encoder encoder(str);
        encoder << std::string("\00\01\02\03", 4);
        encoder.close();
        assert (str.str() == "AAECAw==");
    }
    {
        std::ostringstream str;
        Base64Encoder encoder(str);
        encoder << "ABCDEF";
        encoder.close();
        assert (str.str() == "QUJDREVG");
    }
}


void Base64Test::testDecoder()
{
    {
        std::istringstream istr("AAECAwQF");
        Base64Decoder decoder(istr);
        assert (decoder.good() && decoder.get() == 0);
        assert (decoder.good() && decoder.get() == 1);
        assert (decoder.good() && decoder.get() == 2);
        assert (decoder.good() && decoder.get() == 3);
        assert (decoder.good() && decoder.get() == 4);
        assert (decoder.good() && decoder.get() == 5);
        assert (decoder.good() && decoder.get() == -1);
    }
    {
        std::istringstream istr("AAECAwQ=");
        Base64Decoder decoder(istr);
        assert (decoder.good() && decoder.get() == 0);
        assert (decoder.good() && decoder.get() == 1);
        assert (decoder.good() && decoder.get() == 2);
        assert (decoder.good() && decoder.get() == 3);
        assert (decoder.good() && decoder.get() == 4);
        assert (decoder.good() && decoder.get() == -1);
    }
    {
        std::istringstream istr("AAECAw==");
        Base64Decoder decoder(istr);
        assert (decoder.good() && decoder.get() == 0);
        assert (decoder.good() && decoder.get() == 1);
        assert (decoder.good() && decoder.get() == 2);
        assert (decoder.good() && decoder.get() == 3);
        assert (decoder.good() && decoder.get() == -1);
    }
    {
        std::istringstream istr("QUJDREVG");
        Base64Decoder decoder(istr);
        std::string s;
        decoder >> s;
        assert (s == "ABCDEF");
        assert (decoder.eof());
        assert (!decoder.fail());
    }
    {
        std::istringstream istr("QUJ\r\nDRE\r\nVG");
        Base64Decoder decoder(istr);
        std::string s;
        decoder >> s;
        assert (s == "ABCDEF");
        assert (decoder.eof());
        assert (!decoder.fail());
    }
    {
        std::istringstream istr("QUJD#REVG");
        Base64Decoder decoder(istr);
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


void Base64Test::testEncodeDecode()
{
    {
        std::stringstream str;
        Base64Encoder encoder(str);
        encoder << "The quick brown fox ";
        encoder << "jumped over the lazy dog.";
        encoder.close();
        Base64Decoder decoder(str);
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
        Base64Encoder encoder(str);
        encoder.write(src.data(), (std::streamsize) src.size());
        encoder.close();
        Base64Decoder decoder(str);
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


void Base64Test::setUp()
{
}


void Base64Test::tearDown()
{
}


CppUnit::Test* Base64Test::suite()
{
    CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("Base64Test");

    CppUnit_addTest(pSuite, Base64Test, testEncoder);
    CppUnit_addTest(pSuite, Base64Test, testDecoder);
    CppUnit_addTest(pSuite, Base64Test, testEncodeDecode);

    return pSuite;
}
