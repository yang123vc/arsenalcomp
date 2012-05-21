/*
 * The Arsenal Library
 * Copyright (c) 2009-2012 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.It is provided "as is" without express 
 * or implied warranty.
 *
 */




#include "common.h"



#if defined(OS_FAMILY_UNIX)

#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <utime.h>


#endif





		
#if defined(OS_FAMILY_WINDOWS)

		#include "filesystem_win.h"

#elif defined(OS_FAMILY_UNIX)

		#include "filesystem_posix.h"
#else
		#error "Unknown OS!"
#endif

AR_NAMESPACE_BEGIN

AR_NAMESPACE_END

