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


#ifndef __ARSENAL_COMMON_PATH_POSIX_H__
#define __ARSENAL_COMMON_PATH_POSIX_H__

AR_NAMESPACE_BEGIN


#if(0)
arStatus_t      AR_path_is_hidden(const wchar_t *path)
{

}

arStatus_t      AR_path_is_dev(const wchar_t *path)
{

}

arStatus_t      AR_path_is_dir(const wchar_t *path)
{

}

arStatus_t      AR_path_is_link(const wchar_t *path);
arStatus_t      AR_path_is_file(const wchar_t *path);


arStatus_t      AR_path_is_executable(const wchar_t *path);
arStatus_t      AR_path_is_writeable(const wchar_t *path);
arStatus_t      AR_path_is_readable(const wchar_t *path);

arStatus_t      AR_path_is_existed(const wchar_t *path);

arStatus_t      AR_path_set_executable(const wchar_t *path, ar_bool_t flag);
arStatus_t      AR_path_set_writeable(const wchar_t *path, ar_bool_t flag);
arStatus_t      AR_path_rename(const wchar_t *src_path, const wchar_t *dest_path);
arStatus_t      AR_path_remove(const wchar_t *path);
arStatus_t      AR_path_create_file(const wchar_t *path);
arStatus_t      AR_path_create_dir(const wchar_t *path);

arStatus_t      AR_path_create_path(const wchar_t *path);
arStatus_t      AR_path_remove_path(const wchar_t *path);


arStatus_t      AR_path_copyfile(const wchar_t *src, const wchar_t *dest, ar_bool_t truncated);
arStatus_t      AR_path_copylink(const wchar_t *src, const wchar_t *dest, ar_bool_t truncated);

arStatus_t      AR_path_get_size(const wchar_t *path, ar_uint_64_t *ps);
arStatus_t      AR_path_set_size(const wchar_t *path, ar_uint_64_t size);


arStatus_t      AR_path_copydir(const wchar_t *src, const wchar_t *dest);


#endif




AR_NAMESPACE_END

#endif
