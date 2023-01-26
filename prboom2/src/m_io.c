//  Copyright (C) 2022 Roman Fomin
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
// DESCRIPTION:
//      Compatibility wrappers from Chocolate Doom
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
  #include <io.h>
  #include <direct.h>
#else
  #include <sys/types.h>
  #include <unistd.h>
  #include <fcntl.h>
#endif

#include <sys/stat.h>

#include "doomtype.h"
#include "lprintf.h"

#ifdef _WIN32
static wchar_t *ConvertMultiByteToWide(const char *str, UINT code_page)
{
    wchar_t *wstr = NULL;
    int wlen = 0;

    wlen = MultiByteToWideChar(code_page, 0, str, -1, NULL, 0);

    if (!wlen)
    {
        errno = EINVAL;
        lprintf(LO_INFO, "Warning: Failed to convert path to wide encoding\n");
        return NULL;
    }

    wstr = malloc(sizeof(wchar_t) * wlen);

    if (!wstr)
    {
        lprintf(LO_INFO, "ConvertMultiByteToWide: Failed to allocate new string\n");
        return NULL;
    }

    if (MultiByteToWideChar(code_page, 0, str, -1, wstr, wlen) == 0)
    {
        errno = EINVAL;
        lprintf(LO_INFO, "Warning: Failed to convert path to wide encoding\n");
        free(wstr);
        return NULL;
    }

    return wstr;
}

static char *ConvertWideToMultiByte(const wchar_t *wstr, UINT code_page)
{
    char *str = NULL;
    int len = 0;

    len = WideCharToMultiByte(code_page, 0, wstr, -1, NULL, 0, NULL, NULL);

    if (!len)
    {
        errno = EINVAL;
        lprintf(LO_INFO, "Warning: Failed to convert path to multi byte encoding\n");
        return NULL;
    }

    str = malloc(sizeof(char) * len);

    if (!str)
    {
        lprintf(LO_INFO, "ConvertWideToMultiByte: Failed to allocate new string\n");
        return NULL;
    }

    if (WideCharToMultiByte(code_page, 0, wstr, -1, str, len, NULL, NULL) == 0)
    {
        errno = EINVAL;
        lprintf(LO_INFO, "Warning: Failed to convert path to multi byte encoding\n");
        free(str);
        return NULL;
    }

    return str;
}

static wchar_t *ConvertUtf8ToWide(const char *str)
{
    return ConvertMultiByteToWide(str, CP_UTF8);
}

static char *ConvertWideToUtf8(const wchar_t *wstr)
{
    return ConvertWideToMultiByte(wstr, CP_UTF8);
}

static wchar_t *ConvertSysNativeMBToWide(const char *str)
{
    return ConvertMultiByteToWide(str, CP_ACP);
}

static char *ConvertWideToSysNativeMB(const wchar_t *wstr)
{
    return ConvertWideToMultiByte(wstr, CP_ACP);
}
#endif

char *M_ConvertSysNativeMBToUtf8(const char *str)
{
#ifdef _WIN32
    char *ret = NULL;
    wchar_t *wstr = NULL;

    wstr = ConvertSysNativeMBToWide(str);

    if (!wstr)
    {
        return NULL;
    }

    ret = ConvertWideToUtf8(wstr);

    free(wstr);

    return ret;
#else
    return strdup(str);
#endif
}

char *M_ConvertUtf8ToSysNativeMB(const char *str)
{
#ifdef _WIN32
    char *ret = NULL;
    wchar_t *wstr = NULL;

    wstr = ConvertUtf8ToWide(str);

    if (!wstr)
    {
        return NULL;
    }

    ret = ConvertWideToSysNativeMB(wstr);

    free(wstr);

    return ret;
#else
    return strdup(str);
#endif
}

FILE* M_fopen(const char *filename, const char *mode)
{
#ifdef _WIN32
    FILE *file;
    wchar_t *wname = NULL;
    wchar_t *wmode = NULL;

    wname = ConvertUtf8ToWide(filename);

    if (!wname)
    {
        return NULL;
    }

    wmode = ConvertUtf8ToWide(mode);

    if (!wmode)
    {
        free(wname);
        return NULL;
    }

    file = _wfopen(wname, wmode);

    free(wname);
    free(wmode);

    return file;
#else
    return fopen(filename, mode);
#endif
}

int M_remove(const char *path)
{
#ifdef _WIN32
    wchar_t *wpath = NULL;
    int ret;

    wpath = ConvertUtf8ToWide(path);

    if (!wpath)
    {
        return 0;
    }

    ret = _wremove(wpath);

    free(wpath);

    return ret;
#else
    return remove(path);
#endif
}

int M_stat(const char *path, struct stat *buf)
{
#ifdef _WIN32
    wchar_t *wpath = NULL;
    struct _stat wbuf;
    int ret;

    wpath = ConvertUtf8ToWide(path);

    if (!wpath)
    {
        return -1;
    }

    ret = _wstat(wpath, &wbuf);

    // The _wstat() function expects a struct _stat* parameter that is
    // incompatible with struct stat*. We copy only the required compatible
    // field.
    buf->st_mode = wbuf.st_mode;
    buf->st_mtime = wbuf.st_mtime;

    free(wpath);

    return ret;
#else
    return stat(path, buf);
#endif
}

int M_open(const char *filename, int oflag)
{
#ifdef _WIN32
    wchar_t *wname = NULL;
    int ret;

    wname = ConvertUtf8ToWide(filename);

    if (!wname)
    {
        return 0;
    }

    ret = _wopen(wname, oflag);

    free(wname);

    return ret;
#else
    return open(filename, oflag);
#endif
}

int M_access(const char *path, int mode)
{
#ifdef _WIN32
    wchar_t *wpath = NULL;
    int ret;

    wpath = ConvertUtf8ToWide(path);

    if (!wpath)
    {
        return 0;
    }

    ret = _waccess(wpath, mode);

    free(wpath);

    return ret;
#else
    return access(path, mode);
#endif
}

char *M_getcwd(char *buffer, int len)
{
#ifdef _WIN32
    wchar_t *wret;
    char *ret;

    wret = _wgetcwd(NULL, 0);

    if (!wret)
    {
        return NULL;
    }

    ret = ConvertWideToUtf8(wret);

    free(wret);

    if (!ret)
    {
        return NULL;
    }

    if (buffer)
    {
        if (strlen(ret) >= len)
        {
            free(ret);
            return NULL;
        }

        strcpy(buffer, ret);
        free(ret);

        return buffer;
    }
    else
    {
        return ret;
    }
#else
    return getcwd(buffer, len);
#endif
}

int M_mkdir(const char *path)
{
#ifdef _WIN32
    wchar_t *wdir = NULL;
    int ret;

    wdir = ConvertUtf8ToWide(path);

    if (!wdir)
    {
        return -1;
    }

    ret = _wmkdir(wdir);

    free(wdir);

    return ret;
#else
    return mkdir(path, 0755);
#endif
}

#ifdef _WIN32
typedef struct {
    char *var;
    const char *name;
} env_var_t;

static env_var_t *env_vars;
static int num_vars;
#endif

char *M_getenv(const char *name)
{
#ifdef _WIN32
    int i;
    wchar_t *wenv = NULL, *wname = NULL;
    char *env;

    for (i = 0; i < num_vars; ++i)
    {
        if (!strcasecmp(name, env_vars[i].name))
           return env_vars[i].var;
    }

    wname = ConvertUtf8ToWide(name);

    if (!wname)
    {
        return NULL;
    }

    wenv = _wgetenv(wname);

    free(wname);

    if (wenv)
    {
        env = ConvertWideToUtf8(wenv);
    }
    else
    {
        env = NULL;
    }

    env_vars = realloc(env_vars, (num_vars + 1) * sizeof(*env_vars));
    env_vars[num_vars].var = env;
    env_vars[num_vars].name = strdup(name);
    ++num_vars;

    return env;
#else
    return getenv(name);
#endif
}
