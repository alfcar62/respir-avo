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
#include <filelib.h>
#include <stdio.h>

#define READ_MODE "r"       // Modalità di lettura
#define WRITE_MODE "w+"     // Modalità di scrittura


/**************************************************************
DESCRIPTION:
Static FILE* function - Used only inside this file

Returns a pointer to a file if possible.
It also checks that everything went smoothly and.
The assertion dumps the core if the file operation returns NULL
**************************************************************/
static FILE* __OpenFileStream__(const char* __path, const char* __mode)
{
    FILE* _file = fopen(__path, __mode);
    massert(_file != NULL, -2, "Impossibile aprire file '%s' in modalita' '%s'", __path, __mode);

    return _file;
}

/*************************
DESCRIPTION:
Opens a file in read mode.
*************************/
FILE* fileOpenRead(const char* __path)
{
    return __OpenFileStream__(__path, READ_MODE);
}

/*************************
DESCRIPTION:
Opens a file in write mode.
*************************/
FILE* fileOpenWrite(const char* __path)
{
    return __OpenFileStream__(__path, WRITE_MODE);
}
