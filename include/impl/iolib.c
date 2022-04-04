/***************************************************************************
RespirAVO/unicsv
Copyright 2022 3C A. Avogadro & RespirAVO Contributors

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
***************************************************************************/


#include <stdarg.h>
#include <stdio.h>
#include <iolib.h>


/************************************************
DESCRIPTION:
va_list interface for println

Arguments are passed in the same order as printf,
but in a va_list.

Returns the result of the vprintf call
************************************************/
int vprintln(const char *__fmt, va_list __args)
{
    int _ret = vprintf(__fmt, __args);
    printf("\n");

    return _ret;
}

/************************************************
DESCRIPTION:
Prints a formatted string and then goes to a new line.

Arguments are passed in the same order as printf

Returns the result of the vprintln call
************************************************/
int println(const char *__fmt, ...)
{
    va_list _arguments;
    va_start(_arguments, __fmt);

    int _ret = vprintln(__fmt, _arguments);

    va_end(_arguments);
    return _ret;
}
