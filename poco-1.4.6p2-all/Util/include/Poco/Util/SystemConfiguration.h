﻿//
// SystemConfiguration.h
//
// $Id: //poco/1.4/Util/include/Poco/Util/SystemConfiguration.h#2 $
//
// Library: Util
// Package: Configuration
// Module:  SystemConfiguration
//
// Definition of the SystemConfiguration class.
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


#ifndef Util_SystemConfiguration_INCLUDED
#define Util_SystemConfiguration_INCLUDED


#include "Poco/Util/Util.h"
#include "Poco/Util/AbstractConfiguration.h"


namespace Poco
{
namespace Util
{


class Util_API SystemConfiguration: public AbstractConfiguration
    /// This class implements a Configuration interface to
    /// various system properties and environment variables.
    ///
    /// The following properties are supported:
    ///   - system.osName: the operating system name
    ///   - system.osVersion: the operating system version
    ///   - system.osArchitecture: the operating system architecture
    ///   - system.nodeName: the node (or host) name
    ///   - system.nodeId: system ID, based on the Ethernet address (format "xxxxxxxxxxxx")
    ///     of the first Ethernet adapter found on the system.
    ///   - system.currentDir: the current working directory
    ///   - system.homeDir: the user's home directory
    ///   - system.tempDir: the system's temporary directory
    ///   - system.dateTime: the current UTC date and time, formatted in ISO 8601 format.
    ///   - system.pid: the current process ID.
    ///   - system.env.<NAME>: the environment variable with the given <NAME>.
    ///
    /// An attempt to set a system variable will result in an
    /// InvalidAccessException being thrown.
    ///
    /// Enumerating environment variables is not supported.
    /// An attempt to call keys("system.env") will return an empty range.
    ///
    /// Removing key is not supported. An attempt to remove a key results
    /// in a NotImplementedException being thrown.
{
public:
    SystemConfiguration();
    /// Creates the SystemConfiguration.

protected:
    bool getRaw(const std::string& key, std::string& value) const;
    void setRaw(const std::string& key, const std::string& value);
    void enumerate(const std::string& key, Keys& range) const;
    void removeRaw(const std::string& key);
    ~SystemConfiguration();

private:
    static bool getEnv(const std::string& name, std::string& value);

    static const std::string OSNAME;
    static const std::string OSVERSION;
    static const std::string OSARCHITECTURE;
    static const std::string NODENAME;
    static const std::string NODEID;
    static const std::string CURRENTDIR;
    static const std::string HOMEDIR;
    static const std::string TEMPDIR;
    static const std::string DATETIME;
#if !defined(POCO_VXWORKS)
    static const std::string PID;
#endif
    static const std::string ENV;
};


}
} // namespace Poco::Util


#endif // Util_SystemConfiguration_INCLUDED
