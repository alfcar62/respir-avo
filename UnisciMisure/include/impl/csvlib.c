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

#define NEW_LINE '\n'   // New line character
#define END        0    // String terminator
#define COMMA    ','    // CSV Separator


/************************************************
DESCRIPTION:
Static bool function - Used only inside this file

Returns true if the specified char is a
valid CSV Separator.
************************************************/
static bool __IsSeparator__(char __val)
{
    return __val == COMMA ||
           __val == NEW_LINE;
}

/***********************************************************
DESCRIPTION:
va_list interface for csvGetEntries

Arguments are passed as follows: 
    - CSV File
    - va_list containing arguments in the same order as
      csvGetEntries
***********************************************************/
int vcsvGetEntries(FILE *__csvf, va_list __args)
{
    char  _buffer[MAX_STR_LEN];     // Line buffer
    char *_buffptr = _buffer;       // Base pointer of the buffer

    // Until the EOF is reached
    while (!feof(__csvf))
    {
        // Reads the next character, saves it into a buffer and makes a copy in _curchr
        *_buffptr    = fgetc(__csvf);
        char _curchr = *_buffptr;

        if (__IsSeparator__(_curchr)) // If a separator is reached
        {
            // Takes the pointer to the destination variable and a format string
            void *_var = va_arg(__args, void*);
            char *_fmt = va_arg(__args, char*);

            // Adds the NULL terminator to the buffer for safety
            *_buffptr = END;

            // Reads from the buffer using the format string
            // If _var points to NULL, the operation is aborted as the column is disabled
            if (_var != NULL) sscanf(_buffer, _fmt, _var);

            // If the separator is a new line, all entries have been read and the loop breaks
            if (_curchr == NEW_LINE) break;

            // Resets the buffptr to the buffer's base
            _buffptr = _buffer;
            continue;
        }
        
        // Increments the buffer pointer
        _buffptr++;
    }

    return feof(__csvf);
}

/***********************************************************
DESCRIPTION:
Reads all columns from the current line in the CSV File.
It uses a va_list to get an unspecified number of arguments.

Arguments are passed as follows: 
    - CSV File
    - Pointer to destination variable for first column
    - Format string for first column
    - Pointer to destination variable for nth column
    - Format string for nth column

To ignore a column, just give IGNORE or NULL
as both variable pointer and format string. 
***********************************************************/
int csvGetEntries(FILE *__csvf, ...)
{
    // Variable Argument initialization
    va_list _arguments;
    va_start(_arguments, __csvf);

    int _ret = vcsvGetEntries(__csvf, _arguments);

    va_end(_arguments);
    return _ret;
}

/****************************************************
DESCRIPTION:
Reads an entire line and discards it.
This is used for CSV Headers which may not be needed.
****************************************************/
void csvIgnoreLine(FILE *__csvf)
{
    // Reads from the file stream until a new line is reached
    while (fgetc(__csvf) != NEW_LINE);
}
