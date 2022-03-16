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


#include <stdio.h>
#include <filelib.h>
#include <assert.h>

#define READ_MODE "r"
#define WRITE_MODE "w+"


static FILE* __OpenFileStream__(const char* __path, const char* __mode)
{
    FILE* _file = fopen(__path, __mode);
    assert(_file != FILE_ERROR);

    return _file;
}

static uint64_t __GetFileSize__(FILE* __fileptr)
{
    fseek(__fileptr, 0, SEEK_END);
    uint64_t _fsize = ftell(__fileptr);
    fseek(__fileptr, 0, SEEK_SET);

    return _fsize;
}

static file_t __OpenFile__(const char* __path, const char* __mode)
{
    FILE* _file = __OpenFileStream__(__path, __mode);
    
    return (file_t) {
        .FilePionter = _file,
        .FileSize    = __GetFileSize__(_file)
    };
}

file_t fileOpenRead(const char* __path)
{
    return __OpenFile__(__path, READ_MODE);
}

file_t fileOpenWrite(const char* __path)
{
    return __OpenFile__(__path, WRITE_MODE);
}
