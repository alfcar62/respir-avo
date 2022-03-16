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
