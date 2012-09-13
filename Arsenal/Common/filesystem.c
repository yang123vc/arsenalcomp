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




arStatus_t		AR_read_file(arFile_t *file, byte_t *data, size_t len, size_t *rn)
{
		size_t ret;
		arStatus_t status;
		AR_ASSERT(file != NULL && data != NULL && len > 0);

		status = AR_S_YES;
		if(rn)
		{
				*rn = 0;
		}

		ret = fread((void*)data, 1, (size_t)len, (FILE*)file);

		/*
		if(ret != len)
		{
                int err_code = errno;
				status = __map_last_error(err_code);
		}else
		{
				status = AR_S_YES;
		}
		*/

		if(rn)
		{
				*rn = ret;
		}

		return status;
}


arStatus_t		AR_write_file(arFile_t *file, const byte_t *data, size_t len, size_t *wn)
{
		
		size_t ret;
		arStatus_t status;
		AR_ASSERT(file != NULL && data != NULL && len > 0);

		status = AR_S_YES;

		ret = fwrite((const void*)data, 1, (size_t)len, (FILE*)file);

		if(ret != len)
		{
                int errcode = errno;
				status = __map_last_error(errcode);
		}else
		{
				status = AR_S_YES;
		}

		if(wn)
		{
				*wn = ret;
		}
		return status;
}



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

