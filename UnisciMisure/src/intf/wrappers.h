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


#ifndef WRAPPERS_H
#define WRAPPERS_H

    #include <stdio.h>


    int         leggi_pos       (FILE *__file, unsigned long int *__time, float *__lat, float *__lon);
    int         leggi_mis       (FILE *__file, unsigned long int *__time, float *__no2, float *__voc, float *__pm10, float *__pm25);
    int         scrivi_out      (FILE *__file, unsigned long int __time, float __lat, float __lon, float __mis);

#endif // WRAPPERS_H
