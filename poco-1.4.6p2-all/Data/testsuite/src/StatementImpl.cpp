﻿//
// StatementImpl.cpp
//
// $Id: //poco/1.4/Data/testsuite/src/StatementImpl.cpp#1 $
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
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


#include "StatementImpl.h"


namespace Poco
{
namespace Data
{
namespace Test
{


StatementImpl::StatementImpl()
{
}


StatementImpl::~StatementImpl()
{
}


void StatementImpl::compileImpl()
{
    // prepare binding
    _ptrBinder    = new Binder;
    _ptrExtractor = new Extractor;
    _ptrPrepare   = new Preparation();
}


bool StatementImpl::canBind() const
{
    return false;
}


void StatementImpl::bindImpl()
{
    // bind
    typedef Poco::Data::AbstractBindingVec Bindings;
    Bindings& binds = bindings();
    if (binds.empty())
        return;

    Bindings::iterator it    = binds.begin();
    Bindings::iterator itEnd = binds.end();
    std::size_t pos = 0;
    for (; it != itEnd && (*it)->canBind(); ++it)
    {
        (*it)->bind(pos);
        pos += (*it)->numOfColumnsHandled();
    }
}


bool StatementImpl::hasNext()
{
    return false;
}


void StatementImpl::next()
{
    Poco::Data::AbstractExtractionVec::iterator it    = extractions().begin();
    Poco::Data::AbstractExtractionVec::iterator itEnd = extractions().end();
    std::size_t pos = 0;
    for (; it != itEnd; ++it)
    {
        (*it)->extract(pos);
        pos += (*it)->numOfColumnsHandled();
    }
}


}
}
} // namespace Poco::Data::Test
