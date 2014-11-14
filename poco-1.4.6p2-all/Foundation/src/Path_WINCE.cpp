﻿//
// Path_WIN32U.cpp
//
// $Id: //poco/1.4/Foundation/src/Path_WINCE.cpp#1 $
//
// Library: Foundation
// Package: Filesystem
// Module:  Path
//
// Copyright (c) 2006-2010, Applied Informatics Software Engineering GmbH.
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


#include "Poco/Path_WINCE.h"
#include "Poco/Environment_WINCE.h"
#include "Poco/UnicodeConverter.h"
#include "Poco/Buffer.h"
#include "Poco/Environment.h"
#include "Poco/Exception.h"
#include "Poco/UnWindows.h"


namespace Poco
{


std::string PathImpl::currentImpl()
{
    return("\\");
}


std::string PathImpl::homeImpl()
{
    return("\\");
}


std::string PathImpl::tempImpl()
{
    Buffer<wchar_t> buffer(MAX_PATH_LEN);
    DWORD n = GetTempPathW(static_cast<DWORD>(buffer.size()), buffer.begin());
    if (n > 0)
    {
        std::string result;
        UnicodeConverter::toUTF8(buffer.begin(), result);
        if (result[n - 1] != '\\')
            result.append("\\");
        return result;
    }
    throw SystemException("Cannot get current directory");
}


std::string PathImpl::nullImpl()
{
    return "NUL:";
}


std::string PathImpl::expandImpl(const std::string& path)
{
    std::string result;
    std::string::const_iterator it  = path.begin();
    std::string::const_iterator end = path.end();
    while (it != end)
    {
        if (*it == '%')
        {
            ++it;
            if (it != end && *it == '%')
            {
                result += '%';
            }
            else
            {
                std::string var;
                while (it != end && *it != '%') var += *it++;
                if (it != end) ++it;
                result += Environment::get(var, "");
            }
        }
        else result += *it++;
    }
    return result;
}


void PathImpl::listRootsImpl(std::vector<std::string>& roots)
{
    roots.clear();
    roots.push_back("\\");

    WIN32_FIND_DATAW fd;
    HANDLE hFind = FindFirstFileW(L"\\*.*", &fd);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
                    (fd.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY))
            {
                std::wstring name(fd.cFileName);
                name += L"\\Vol:";
                HANDLE h = CreateFileW(name.c_str(), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                if (h != INVALID_HANDLE_VALUE)
                {
                    // its a device volume
                    CloseHandle(h);
                    std::string name;
                    UnicodeConverter::toUTF8(fd.cFileName, name);
                    std::string root = "\\" + name;
                    roots.push_back(root);
                }
            }
        }
        while (FindNextFileW(hFind, &fd));
        FindClose(hFind);
    }
}


} // namespace Poco
