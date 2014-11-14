﻿//
// WinCEDriver.cpp
//
// $Id: //poco/1.4/NetSSL_OpenSSL/testsuite/src/WinCEDriver.cpp#1 $
//
// Console-based test driver for Windows CE.
//
// Copyright (c) 2004-2010, Applied Informatics Software Engineering GmbH.
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


#include "CppUnit/TestRunner.h"
#include "NetSSLTestSuite.h"
#include "Poco/Util/Application.h"
#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/Net/HTTPSStreamFactory.h"
#include <cstdlib>


class NetSSLApp: public Poco::Util::Application
{
public:
    NetSSLApp()
    {
        Poco::Net::initializeSSL();
        Poco::Net::HTTPStreamFactory::registerFactory();
        Poco::Net::HTTPSStreamFactory::registerFactory();
    }

    ~NetSSLApp()
    {
        Poco::Net::uninitializeSSL();
    }

    int main(const std::vector<std::string>& args)
    {
        CppUnit::TestRunner runner;
        runner.addTest("NetSSLTestSuite", NetSSLTestSuite::suite());
        return runner.run(_targs) ? 0 : 1;
    }

    void setup(const std::vector<std::string>& args)
    {
        char* argv[] =
        {
            const_cast<char*>(args[0].c_str())
        };

        init(1, argv);
        for (std::size_t i = 0; i < args.size(); ++i)
            _targs.push_back(args[i]);
    }

protected:
    void initialize(Poco::Util::Application& self)
    {
        loadConfiguration(); // load default configuration files, if present
        Poco::Util::Application::initialize(self);
    }

private:
    std::vector<std::string> _targs;
};


int _tmain(int argc, wchar_t* argv[])
{
    std::vector<std::string> args;
    for (int i = 0; i < argc; ++i)
    {
        char buffer[1024];
        std::wcstombs(buffer, argv[i], sizeof(buffer));
        args.push_back(std::string(buffer));
    }

    NetSSLApp app;
    try
    {
        app.setup(args);
        return app.run();
    }
    catch (Poco::Exception& exc)
    {
        std::cout << exc.displayText() << std::endl;
        return 1;
    }
}
