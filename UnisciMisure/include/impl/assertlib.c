#include <assertlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


void massert(bool __condition, int __ecode, const char *__msg, ...)
{
    if (__condition) return;

    va_list _arguments;
    va_start(_arguments, __msg);

    vprintf(__msg, _arguments);
    printf("\n");

    va_end(_arguments);
    exit(__ecode);
}
