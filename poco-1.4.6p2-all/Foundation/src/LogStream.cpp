﻿//
// LogStream.cpp
//
// $Id: //poco/1.4/Foundation/src/LogStream.cpp#1 $
//
// Library: Foundation
// Package: Logging
// Module:  LogStream
//
// Copyright (c) 2006-2007, Applied Informatics Software Engineering GmbH.
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


#include "Poco/LogStream.h"


namespace Poco
{


//
// LogStreamBuf
//


LogStreamBuf::LogStreamBuf(Logger& logger, Message::Priority priority):
    _logger(logger),
    _priority(priority)
{
}


LogStreamBuf::~LogStreamBuf()
{
}


void LogStreamBuf::setPriority(Message::Priority priority)
{
    _priority = priority;
}


int LogStreamBuf::writeToDevice(char c)
{
    if (c == '\n' || c == '\r')
    {
        Message msg(_logger.name(), _message, _priority);
        _message.clear();
        _logger.log(msg);
    }
    else _message += c;
    return c;
}


//
// LogIOS
//


LogIOS::LogIOS(Logger& logger, Message::Priority priority):
    _buf(logger, priority)
{
    poco_ios_init(&_buf);
}


LogIOS::~LogIOS()
{
}


LogStreamBuf* LogIOS::rdbuf()
{
    return &_buf;
}


//
// LogStream
//


LogStream::LogStream(Logger& logger, Message::Priority priority):
    LogIOS(logger, priority),
    std::ostream(&_buf)
{
}


LogStream::LogStream(const std::string& loggerName, Message::Priority priority):
    LogIOS(Logger::get(loggerName), priority),
    std::ostream(&_buf)
{
}


LogStream::~LogStream()
{
}


LogStream& LogStream::fatal()
{
    return priority(Message::PRIO_FATAL);
}


LogStream& LogStream::fatal(const std::string& message)
{
    _buf.logger().fatal(message);
    return priority(Message::PRIO_FATAL);
}


LogStream& LogStream::critical()
{
    return priority(Message::PRIO_CRITICAL);
}


LogStream& LogStream::critical(const std::string& message)
{
    _buf.logger().critical(message);
    return priority(Message::PRIO_CRITICAL);
}


LogStream& LogStream::error()
{
    return priority(Message::PRIO_ERROR);
}


LogStream& LogStream::error(const std::string& message)
{
    _buf.logger().error(message);
    return priority(Message::PRIO_ERROR);
}


LogStream& LogStream::warning()
{
    return priority(Message::PRIO_WARNING);
}


LogStream& LogStream::warning(const std::string& message)
{
    _buf.logger().warning(message);
    return priority(Message::PRIO_WARNING);
}


LogStream& LogStream::notice()
{
    return priority(Message::PRIO_NOTICE);
}


LogStream& LogStream::notice(const std::string& message)
{
    _buf.logger().notice(message);
    return priority(Message::PRIO_NOTICE);
}


LogStream& LogStream::information()
{
    return priority(Message::PRIO_INFORMATION);
}


LogStream& LogStream::information(const std::string& message)
{
    _buf.logger().information(message);
    return priority(Message::PRIO_INFORMATION);
}


LogStream& LogStream::debug()
{
    return priority(Message::PRIO_DEBUG);
}


LogStream& LogStream::debug(const std::string& message)
{
    _buf.logger().debug(message);
    return priority(Message::PRIO_DEBUG);
}


LogStream& LogStream::trace()
{
    return priority(Message::PRIO_TRACE);
}


LogStream& LogStream::trace(const std::string& message)
{
    _buf.logger().trace(message);
    return priority(Message::PRIO_TRACE);
}


LogStream& LogStream::priority(Message::Priority priority)
{
    _buf.setPriority(priority);
    return *this;
}


} // namespace Poco
