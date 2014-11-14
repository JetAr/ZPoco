﻿//
// StatementExecutor.cpp
//
// $Id: //poco/1.4/Data/MySQL/src/StatementExecutor.cpp#1 $
//
// Library: Data/MySQL
// Package: MySQL
// Module:  StatementExecutor
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
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


#include <mysql.h>
#include "Poco/Data/MySQL/StatementExecutor.h"
#include <sstream>


namespace Poco
{
namespace Data
{
namespace MySQL
{


StatementExecutor::StatementExecutor(MYSQL* mysql)
{
    h = mysql_stmt_init(mysql);

    if (!h)
    {
        throw StatementException("mysql_stmt_init error");
    }

    _state = STMT_INITED;
}


StatementExecutor::~StatementExecutor()
{
    mysql_stmt_close(h);
}


int StatementExecutor::state() const
{
    return _state;
}


void StatementExecutor::prepare(const std::string& query)
{
    if (_state >= STMT_COMPILED)
    {
        throw StatementException("Satement is already compiled");
    }

    // compile
    int res = mysql_stmt_prepare(h, query.c_str(), static_cast<unsigned int>(query.length()));

    if (res != 0)
    {
        throw StatementException("mysql_stmt_prepare error", h, query);
    }

    _query = query;
    _state = STMT_COMPILED;
}


void StatementExecutor::bindParams(MYSQL_BIND* params, size_t count)
{
    if (_state < STMT_COMPILED)
    {
        throw StatementException("Satement is not compiled yet");
    }

    if (count != mysql_stmt_param_count(h))
    {
        throw StatementException("wrong bind parameters count", 0, _query);
    }

    if (count == 0)
    {
        return;
    }

    int res = mysql_stmt_bind_param(h, params);

    if (res != 0)
    {
        throw StatementException("mysql_stmt_bind_param() error ", h, _query);
    }
}


void StatementExecutor::bindResult(MYSQL_BIND* result)
{
    if (_state < STMT_COMPILED)
    {
        throw StatementException("Satement is not compiled yet");
    }

    int res = mysql_stmt_bind_result(h, result);

    if (res != 0)
    {
        throw StatementException("mysql_stmt_bind_result error ", h, _query);
    }
}


void StatementExecutor::execute()
{
    if (_state < STMT_COMPILED)
    {
        throw StatementException("Satement is not compiled yet");
    }

    int res = mysql_stmt_execute(h);

    if (res != 0)
    {
        throw StatementException("mysql_stmt_execute error", h, _query);
    }

    _state = STMT_EXECUTED;
}


bool StatementExecutor::fetch()
{
    if (_state < STMT_EXECUTED)
    {
        throw StatementException("Satement is not executed yet");
    }

    int res = mysql_stmt_fetch(h);

    if ((res != 0) && (res != MYSQL_NO_DATA))
    {
        throw StatementException("mysql_stmt_fetch error", h, _query);
    }

    return (res == 0);
}


bool StatementExecutor::fetchColumn(size_t n, MYSQL_BIND *bind)
{
    if (_state < STMT_EXECUTED)
    {
        throw StatementException("Satement is not executed yet");
    }

    int res = mysql_stmt_fetch_column(h, bind, static_cast<unsigned int>(n), 0);

    if ((res != 0) && (res != MYSQL_NO_DATA))
    {
        std::ostringstream msg;
        msg << "mysql_stmt_fetch_column(" << n << ") error";
        throw StatementException(msg.str(), h, _query);
    }

    return (res == 0);
}


}
}
} // namespace Poco::Data::MySQL
