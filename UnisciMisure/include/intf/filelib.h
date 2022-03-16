#ifndef FILELIB_H
#define FILELIB_H

    #include <stdio.h>
    #include <stdint.h>

    // Codici di errore e successo
    #define FILE_ERROR NULL     // NullPointer, utilizzata per verificare se si è verificato un errore nell'apertura di una file stream
    #define FILE_OK 0           // Valore di successo restituito da funzioni/procedure


    typedef struct FileDefinition {
        FILE*    FilePionter;
        uint64_t FileSize;
    } file_t;


    file_t fileOpenRead(const char* __path);
    file_t fileOpenWrite(const char* __path);

#endif // FILELIB_H
