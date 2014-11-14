﻿//
// Platform_VMS.h
//
// $Id: //poco/1.4/Foundation/include/Poco/Platform_VMS.h#1 $
//
// Library: Foundation
// Package: Core
// Module:  Platform
//
// Platform and architecture identification macros
// and platform-specific definitions for OpenVMS.
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


#ifndef Foundation_Platform_VMS_INCLUDED
#define Foundation_Platform_VMS_INCLUDED


// Define the POCO_DESCRIPTOR_STRING and POCO_DESCRIPTOR_LITERAL
// macros which we use instead of $DESCRIPTOR and $DESCRIPTOR64.
// Our macros work with both 32bit and 64bit pointer sizes.
#if __INITIAL_POINTER_SIZE != 64
#define POCO_DESCRIPTOR_STRING(name, string) \
		struct dsc$descriptor_s name =	\
		{								\
			string.size(),				\
			DSC$K_DTYPE_T,				\
			DSC$K_CLASS_S,				\
			(char*) string.data()		\
		}
#define POCO_DESCRIPTOR_LITERAL(name, string) \
		struct dsc$descriptor_s name =	\
		{								\
			sizeof(string) - 1,			\
			DSC$K_DTYPE_T,				\
			DSC$K_CLASS_S,				\
			(char*) string				\
		}
#else
#define POCO_DESCRIPTOR_STRING(name, string) \
		struct dsc64$descriptor_s name =\
		{								\
			1,							\
			DSC64$K_DTYPE_T,			\
			DSC64$K_CLASS_S,			\
			-1,							\
			string.size(),				\
			(char*) string.data()		\
		}
#define POCO_DESCRIPTOR_LITERAL(name, string) \
		struct dsc64$descriptor_s name =\
		{								\
			1,							\
			DSC64$K_DTYPE_T,			\
			DSC64$K_CLASS_S,			\
			-1,							\
			sizeof(string) - 1,			\
			(char*) string				\
		}
#endif


// No <sys/select.h> header file
#define POCO_NO_SYS_SELECT_H


#endif // Foundation_Platform_VMS_INCLUDED
