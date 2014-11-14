﻿//
// PipeStream.h
//
// $Id: //poco/1.4/Foundation/include/Poco/PipeStream.h#1 $
//
// Library: Foundation
// Package: Processes
// Module:  PipeStream
//
// Definition of the PipeStream class.
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


#ifndef Foundation_PipeStream_INCLUDED
#define Foundation_PipeStream_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Pipe.h"
#include "Poco/BufferedStreamBuf.h"
#include <istream>
#include <ostream>


namespace Poco
{


class Foundation_API PipeStreamBuf: public BufferedStreamBuf
    /// This is the streambuf class used for reading from and writing to a Pipe.
{
public:
    typedef BufferedStreamBuf::openmode openmode;

    PipeStreamBuf(const Pipe& pipe, openmode mode);
    /// Creates a PipeStreamBuf with the given Pipe.

    ~PipeStreamBuf();
    /// Destroys the PipeStreamBuf.

    void close();
    /// Closes the pipe.

protected:
    int readFromDevice(char* buffer, std::streamsize length);
    int writeToDevice(const char* buffer, std::streamsize length);

private:
    enum
    {
        STREAM_BUFFER_SIZE = 1024
    };

    Pipe _pipe;
};


class Foundation_API PipeIOS: public virtual std::ios
    /// The base class for PipeInputStream and
    /// PipeOutputStream.
    ///
    /// This class is needed to ensure the correct initialization
    /// order of the stream buffer and base classes.
{
public:
    PipeIOS(const Pipe& pipe, openmode mode);
    /// Creates the PipeIOS with the given Pipe.

    ~PipeIOS();
    /// Destroys the PipeIOS.
    ///
    /// Flushes the buffer, but does not close the pipe.

    PipeStreamBuf* rdbuf();
    /// Returns a pointer to the internal PipeStreamBuf.

    void close();
    /// Flushes the stream and closes the pipe.

protected:
    PipeStreamBuf _buf;
};


class Foundation_API PipeOutputStream: public PipeIOS, public std::ostream
    /// An output stream for writing to a Pipe.
{
public:
    PipeOutputStream(const Pipe& pipe);
    /// Creates the PipeOutputStream with the given Pipe.

    ~PipeOutputStream();
    /// Destroys the PipeOutputStream.
    ///
    /// Flushes the buffer, but does not close the pipe.
};


class Foundation_API PipeInputStream: public PipeIOS, public std::istream
    /// An input stream for reading from a Pipe.
    ///
    /// Using formatted input from a PipeInputStream
    /// is not recommended, due to the read-ahead behavior of
    /// istream with formatted reads.
{
public:
    PipeInputStream(const Pipe& pipe);
    /// Creates the PipeInputStream with the given Pipe.

    ~PipeInputStream();
    /// Destroys the PipeInputStream.
};


} // namespace Poco


#endif // Foundation_PipeStream_INCLUDED
