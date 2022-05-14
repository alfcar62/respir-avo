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


#ifndef WRAPPERS_H
#define WRAPPERS_H

    #include <stdio.h>

    // Scelte
    #define NO2 1               // Costante che rappresenta la scelta di NO2
    #define VOC 2               // Costante che rappresenta la scelta di VOC
    #define PM10 3              // Costante che rappresenta la scelta di PM10
    #define PM25 4              // Costante che rappresenta la scelta di PM2.5
    #define ALL 5


    typedef struct posizione {
        unsigned long int   timestamp;
                      float lat;
                      float lon;
    } pos_t;

    typedef struct misura {
        unsigned long int   timestamp;
                      float no2;
                      float voc;
                      float pm10;
                      float pm25;
    } mis_t;


    int         leggi_pos       (FILE *__file, pos_t *__pos);
    int         leggi_mis       (FILE *__file, mis_t *__mis);
    int         scrivi_out      (FILE *__file, pos_t __pos, mis_t __mis, int __opt);

#endif // WRAPPERS_H
