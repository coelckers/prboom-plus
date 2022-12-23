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

#ifndef M_IO_INCLUDED
#define M_IO_INCLUDED

#include <stdio.h>
#include <sys/stat.h>

FILE *M_fopen(const char *filename, const char *mode);
int M_remove(const char *path);
int M_stat(const char *path, struct stat *buf);
int M_open(const char *filename, int oflag);
int M_access(const char *path, int mode);
char *M_getcwd(char *buffer, int len);
int M_mkdir(const char *dir);
char *M_getenv(const char *name);

char *M_ConvertSysNativeMBToUtf8(const char *str);
char *M_ConvertUtf8ToSysNativeMB(const char *str);

#endif // M_IO_INCLUDED
