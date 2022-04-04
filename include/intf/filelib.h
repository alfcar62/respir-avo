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


#ifndef FILELIB_H
#define FILELIB_H

    #include <stdio.h>
    #include <stdint.h>

    // Codici di errore e successo
    #define FILE_ERROR NULL     // NullPointer, utilizzata per verificare se si è verificato un errore nell'apertura di una file stream
    #define FILE_OK    0        // Valore di successo restituito da funzioni/procedure


    FILE*   fileOpenRead    (const char* __path);
    FILE*   fileOpenWrite   (const char* __path);

#endif // FILELIB_H
