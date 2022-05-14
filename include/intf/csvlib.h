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


#ifndef CSVLIB_H
#define CSVLIB_H

    #include <filelib.h>
    #include <stdarg.h>
    #include <stdbool.h>

    #define MAX_STR_LEN 200     // Maximum string length
    #define IGNORE      NULL    // Inogres a column


    int       vcsvGetEntries    (FILE *__csvf, va_list __args);
    int       csvGetEntries     (FILE *__csvf, ...);
    int       csvIgnoreLine     (FILE *__csvf);
    void      csvPutHeader      (FILE *__csvf, int __cols, ...);

#endif // CSVLIB_H
