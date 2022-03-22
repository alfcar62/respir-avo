/***************************************************************************
RespirAVO/UnisciMisure
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


#include <csvlib.h>
#include <stdarg.h>
#include <stdio.h>

#define NEW_LINE '\n'
#define END      0
#define COMMA    ','


static bool __AcceptableValue__(char __val)
{
    return __val != NEW_LINE &&
           __val != END;
}

static bool __IsSeparator__(char __val)
{
    return __val == COMMA;
}


void csvGetEntries(FILE *__csvf, ...)
{
    // Variable Argument initialization
    va_list _arguments;
    va_start(_arguments, __csvf);

    char  _buffer[MAX_STR_LEN];     // Line buffer
    char *_buffptr = _buffer;       // Base pointer of the buffer

    // Reads characters from the file until the EOL is reached
    while (__AcceptableValue__(*_buffptr = fgetc(__csvf)))
    {
        if (__IsSeparator__(*_buffptr)) // If a separator is reached
        {
            // Takes the pointer to the destination variable and a format string
            void *_var = va_arg(_arguments, void*);
            char *_fmt = va_arg(_arguments, char*);

            // Adds the NULL terminator to the buffer for security
            *_buffptr = END;

            // Reads from the buffer using the format string
            // If _var points to NULL, the operation is aborted as the column is disabled
            if (_var != NULL) sscanf(_buffer, _fmt, _var);

            // Resets the buffptr to the buffer's base
            _buffptr = _buffer;
            continue;
        }
        
        // Increments the buffer pointer
        _buffptr++;
    }

    va_end(_arguments);
}
