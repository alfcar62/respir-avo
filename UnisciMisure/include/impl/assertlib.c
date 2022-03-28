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


#include <assertlib.h>
#include <stdarg.h>
#include <stdlib.h>
#include <iolib.h>


/***************************************************************************
DESCRIPTION:
Custom assert procedure which exits the program if a condition is not met.
The procedure also prints an error message and exits with a given exit code.

Arguments are passed as follows:
    - Condition
    - Exit code
    - Error message (printf format string)
    - Error message format values
****************************************************************************/
void massert(bool __condition, int __ecode, const char *__msg, ...)
{
    // Returns if the condition is met
    if (__condition) return;

    // Creates variable argument list
    va_list _arguments;
    va_start(_arguments, __msg);

    // Prints error message
    vprintln(__msg, _arguments);

    // Exits the program
    va_end(_arguments);
    exit(__ecode);
}
