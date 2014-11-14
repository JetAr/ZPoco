﻿//
// PageReader.h
//
// $Id: //poco/1.4/PageCompiler/src/PageReader.h#1 $
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


#ifndef PageReader_INCLUDED
#define PageReader_INCLUDED


#include "Poco/Poco.h"
#include <istream>
#include <ostream>
#include <sstream>


class Page;


class PageReader
    /// This class implements the parser for reading page files
    /// containing JSP-style tags.
{
public:
    PageReader(Page& page, const std::string& path);
    /// Creates the PageReader, using the given Page.

    PageReader(const PageReader& parent, const std::string& path);
    /// Creates the PageReader, using the given PageReader as parent.

    ~PageReader();
    /// Destroys the PageReader.

    void parse(std::istream& pageStream);
    /// Parses a HTML file containing server page directives,
    /// converts the file into C++ code and adds the code
    /// to the reader's Page object. Also parses page
    /// attributes and include directives.

    void emitLineDirectives(bool flag = true);
    /// Enables writing of #line directives to generated code.

protected:
    enum ParsingState
    {
        STATE_MARKUP,
        STATE_IMPLDECL,
        STATE_HDRDECL,
        STATE_PREHANDLER,
        STATE_BLOCK,
        STATE_EXPR,
        STATE_COMMENT,
        STATE_ATTR
    };

    static const std::string MARKUP_BEGIN;
    static const std::string MARKUP_END;
    static const std::string EXPR_BEGIN;
    static const std::string EXPR_END;

    void include(const std::string& path);
    void parseAttributes();
    void nextToken(std::istream& istr, std::string& token);
    void handleAttribute(const std::string& name, const std::string& value);
    std::string where() const;

protected:
    void generateLineDirective(std::ostream& ostr);

private:
    PageReader();
    PageReader(const PageReader&);
    PageReader& operator = (const PageReader&);

    Page& _page;
    const PageReader* _pParent;
    std::string _path;
    std::string _attrs;
    int _line;
    bool _emitLineDirectives;
};


#endif // PageReader_INCLUDED
