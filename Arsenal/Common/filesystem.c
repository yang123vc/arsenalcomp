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
        #include "filesystem_path_win.h"

#elif defined(OS_FAMILY_UNIX)

		#include "filesystem_posix.h"
        #include "filesystem_path_posix.h"
#else
		#error "Unknown OS!"
#endif

AR_NAMESPACE_BEGIN






arStatus_t		AR_eof_file(arFile_t *file)
{
		int ret;
		AR_ASSERT(file != NULL);

		ret = feof((FILE*)file);

		if(ret != 0)
		{
				return AR_S_YES;
		}else
		{
				return AR_S_NO;
		}
}


arStatus_t		AR_error_file(arFile_t *file)
{
		int ret;
		AR_ASSERT(file != NULL);
		ret = ferror((FILE*)file);

		if(ret == 0)
		{
				return AR_S_NO;
		}else
		{
				return AR_S_YES;
		}
}




AR_NAMESPACE_END

