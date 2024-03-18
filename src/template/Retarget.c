/******************************************************************************/
/* RETARGET.C: 'Retarget' layer for target-dependent low level functions      */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005 Keil Software. All rights reserved.                     */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#include "stdio.h"
#include "ctype.h"
#include "stdlib.h"

#if defined(__MICROLIB)
void __aeabi_assert(const char *chCond, const char *chLine, int wErrCode) 
{
    (void)chCond;
    (void)chLine;
    (void)wErrCode;
    
    while(1) {
    }
}
#else

#if (__ARMCC_VERSION >= 6010050)
__asm(".global __use_no_semihosting\n\t");
__asm(".global __ARM_use_no_argv\n\t");
#else
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};

#endif

typedef int FILEHANDLE;
FILEHANDLE _sys_open(const char *name,int openmode)
{
 return 0;
}
int _sys_close(FILEHANDLE fh)
{
    return 0;
}
int _sys_write(FILEHANDLE fh, const unsigned char *buf, unsigned len, int mode)
{
    return 0;
}
int _sys_read(FILEHANDLE fh, unsigned char*buf, unsigned len, int mode)
{
    return 0;
}

int _sys_istty(FILEHANDLE fh)
{
    return 0;
}
int _sys_seek(FILEHANDLE fh, long pos)
{
    return 0;
}

int _sys_ensure(FILEHANDLE fh)
{
    return 0;
}

long _sys_flen(FILEHANDLE fh)
{
    return 0;
}
void _sys_exit(int status)
{
    //while(1);
}
int _sys_tmpnam(char *name, int fileno, unsigned maxlength)
{
    return 0;
}

void _ttywrch(int ch)
{
}
int remove(const char *filename)
{
    return 0;
}
char *_sys_command_string(char *cmd, int len)
{
 return NULL;
}

void __aeabi_assert(const char *chCond, const char *chLine, int wErrCode) 
{
    (void)chCond;
    (void)chLine;
    (void)wErrCode;
    
    while(1) {
    }
}

#endif

