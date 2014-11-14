﻿//
// File_WIN32.h
//
// $Id: //poco/1.4/Foundation/include/Poco/File_WIN32.h#2 $
//
// Library: Foundation
// Package: Filesystem
// Module:  File
//
// Definition of the FileImpl class for WIN32.
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


#ifndef Foundation_File_WIN32_INCLUDED
#define Foundation_File_WIN32_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Timestamp.h"


namespace Poco
{


class Foundation_API FileImpl
{
protected:
    typedef UInt64 FileSizeImpl;

    FileImpl();
    FileImpl(const std::string& path);
    virtual ~FileImpl();
    void swapImpl(FileImpl& file);
    void setPathImpl(const std::string& path);
    const std::string& getPathImpl() const;
    bool existsImpl() const;
    bool canReadImpl() const;
    bool canWriteImpl() const;
    bool canExecuteImpl() const;
    bool isFileImpl() const;
    bool isDirectoryImpl() const;
    bool isLinkImpl() const;
    bool isDeviceImpl() const;
    bool isHiddenImpl() const;
    Timestamp createdImpl() const;
    Timestamp getLastModifiedImpl() const;
    void setLastModifiedImpl(const Timestamp& ts);
    FileSizeImpl getSizeImpl() const;
    void setSizeImpl(FileSizeImpl size);
    void setWriteableImpl(bool flag = true);
    void setExecutableImpl(bool flag = true);
    void copyToImpl(const std::string& path) const;
    void renameToImpl(const std::string& path);
    void removeImpl();
    bool createFileImpl();
    bool createDirectoryImpl();
    static void handleLastErrorImpl(const std::string& path);

private:
    std::string _path;

    friend class FileHandle;
    friend class DirectoryIteratorImpl;
    friend class WindowsDirectoryWatcherStrategy;
};


//
// inlines
//
inline const std::string& FileImpl::getPathImpl() const
{
    return _path;
}


} // namespace Poco


#endif // Foundation_File_WIN32_INCLUDED
