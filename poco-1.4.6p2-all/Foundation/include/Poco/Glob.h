﻿//
// Glob.h
//
// $Id: //poco/1.4/Foundation/include/Poco/Glob.h#1 $
//
// Library: Foundation
// Package: Filesystem
// Module:  Glob
//
// Definition of the Glob class.
//
// Copyright (c) 2004-2009, Applied Informatics Software Engineering GmbH.
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


#ifndef Foundation_Glob_INCLUDED
#define Foundation_Glob_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/TextIterator.h"
#include <set>


namespace Poco
{


class Path;


class Foundation_API Glob
    /// This class implements glob-style pattern matching
    /// as known from Unix shells.
    ///
    /// In the pattern string, '*' matches any sequence of characters,
    /// '?' matches any single character, [SET] matches any single character
    /// in the specified set, [!SET] matches any character not in the
    /// specified set.
    ///
    /// A set is composed of characters or ranges; a range looks like
    /// character hyphen character (as in 0-9 or A-Z).
    /// [0-9a-zA-Z_] is the set of characters allowed in C identifiers.
    /// Any other character in the pattern must be matched exactly.
    ///
    /// To suppress the special syntactic significance of any of '[]*?!-\',
    /// and match the character exactly, precede it with a backslash.
    ///
    /// All strings are assumed to be UTF-8 encoded.
{
public:
    enum Options
    /// Flags that modify the matching behavior.
    {
        GLOB_DEFAULT         = 0x00, /// default behavior
        GLOB_DOT_SPECIAL     = 0x01, /// '*' and '?' do not match '.' at beginning of subject
        GLOB_FOLLOW_SYMLINKS = 0x02, /// follow symbolic links
        GLOB_CASELESS        = 0x04, /// ignore case when comparing characters
        GLOB_DIRS_ONLY       = 0x80  /// only glob for directories (for internal use only)
    };

    Glob(const std::string& pattern, int options = 0);
    /// Creates the Glob, using the given pattern. The pattern
    /// must not be an empty string.
    ///
    /// If the GLOB_DOT_SPECIAL option is specified, '*' and '?' do
    /// not match '.' at the beginning of a matched subject. This is useful for
    /// making dot-files invisible in good old Unix-style.

    ~Glob();
    /// Destroys the Glob.

    bool match(const std::string& subject);
    /// Matches the given subject against the glob pattern.
    /// Returns true if the subject matches the pattern, false
    /// otherwise.

    static void glob(const std::string& pathPattern, std::set<std::string>& files, int options = 0);
    /// Creates a set of files that match the given pathPattern.
    ///
    /// The path may be give in either Unix, Windows or VMS syntax and
    /// is automatically expanded by calling Path::expand().
    ///
    /// The pattern may contain wildcard expressions even in intermediate
    /// directory names (e.g. /usr/include/*/*.h).
    ///
    /// Note that, for obvious reasons, escaping characters in a pattern
    /// with a backslash does not work in Windows-style paths.
    ///
    /// Directories that for whatever reason cannot be traversed are
    /// ignored.

    static void glob(const char* pathPattern, std::set<std::string>& files, int options = 0);
    /// Creates a set of files that match the given pathPattern.
    ///
    /// The path may be give in either Unix, Windows or VMS syntax and
    /// is automatically expanded by calling Path::expand().
    ///
    /// The pattern may contain wildcard expressions even in intermediate
    /// directory names (e.g. /usr/include/*/*.h).
    ///
    /// Note that, for obvious reasons, escaping characters in a pattern
    /// with a backslash does not work in Windows-style paths.
    ///
    /// Directories that for whatever reason cannot be traversed are
    /// ignored.

    static void glob(const Path& pathPattern, std::set<std::string>& files, int options = 0);
    /// Creates a set of files that match the given pathPattern.
    ///
    /// The pattern may contain wildcard expressions even in intermediate
    /// directory names (e.g. /usr/include/*/*.h).
    ///
    /// Note that, for obvious reasons, escaping characters in a pattern
    /// with a backslash does not work in Windows-style paths.
    ///
    /// Directories that for whatever reason cannot be traversed are
    /// ignored.

protected:
    bool match(TextIterator& itp, const TextIterator& endp, TextIterator& its, const TextIterator& ends);
    bool matchAfterAsterisk(TextIterator itp, const TextIterator& endp, TextIterator its, const TextIterator& ends);
    bool matchSet(TextIterator& itp, const TextIterator& endp, int c);
    static void collect(const Path& pathPattern, const Path& base, const Path& current, const std::string& pattern, std::set<std::string>& files, int options);
    static bool isDirectory(const Path& path, bool followSymlink);

private:
    std::string _pattern;
    int         _options;

    Glob();
    Glob(const Glob&);
    Glob& operator = (const Glob&);
};


} // namespace Poco


#endif // Foundation_Glob_INCLUDED
