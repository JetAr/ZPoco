﻿//
// SignalHandler.cpp
//
// $Id: //poco/1.4/Foundation/src/SignalHandler.cpp#2 $
//
// Library: Foundation
// Package: Threading
// Module:  SignalHandler
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


#include "Poco/SignalHandler.h"


#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_VXWORKS)


#include "Poco/Thread.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Exception.h"
#include <cstdlib>
#include <signal.h>


namespace Poco
{


SignalHandler::JumpBufferVec SignalHandler::_jumpBufferVec;


SignalHandler::SignalHandler()
{
    JumpBufferVec& jbv = jumpBufferVec();
    JumpBuffer buf;
    jbv.push_back(buf);
}


SignalHandler::~SignalHandler()
{
    jumpBufferVec().pop_back();
}


sigjmp_buf& SignalHandler::jumpBuffer()
{
    return jumpBufferVec().back().buf;
}


void SignalHandler::throwSignalException(int sig)
{
    switch (sig)
    {
    case SIGILL:
        throw SignalException("Illegal instruction");
    case SIGBUS:
        throw SignalException("Bus error");
    case SIGSEGV:
        throw SignalException("Segmentation violation");
    case SIGSYS:
        throw SignalException("Invalid system call");
    default:
        throw SignalException(NumberFormatter::formatHex(sig));
    }
}


void SignalHandler::install()
{
#ifndef POCO_NO_SIGNAL_HANDLER
    struct sigaction sa;
    sa.sa_handler = handleSignal;
    sa.sa_flags   = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGILL,  &sa, 0);
    sigaction(SIGBUS,  &sa, 0);
    sigaction(SIGSEGV, &sa, 0);
    sigaction(SIGSYS,  &sa, 0);
#endif
}


void SignalHandler::handleSignal(int sig)
{
    JumpBufferVec& jb = jumpBufferVec();
    if (!jb.empty())
        siglongjmp(jb.back().buf, sig);

    // Abort if no jump buffer registered
    std::abort();
}


SignalHandler::JumpBufferVec& SignalHandler::jumpBufferVec()
{
    ThreadImpl* pThread = ThreadImpl::currentImpl();
    if (pThread)
        return pThread->_jumpBufferVec;
    else
        return _jumpBufferVec;
}


} // namespace Poco


#endif // POCO_OS_FAMILY_UNIX
