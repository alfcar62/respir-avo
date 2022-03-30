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


#include "wrappers.h"
#include <csvlib.h>
#include <stdio.h>


/***************************************************************************
DESCRIZIONE:
Legge i valori timestamp, latitudine e longitudine dal file delle posizioni.

PARAMETRI:
    - Puntatore a FILE CSV
    - Puntatore a `unsigned long int` "__time"
    - Puntatore a `float` "__lat"
    - Puntatore a `float` "__lon"

RETURN:
    - FILE_OK (0) se l'operazione è stata conclusa con successo
    - Un valore positivo se è stato raggiunto EOF
***************************************************************************/
int leggi_pos(FILE *__file, unsigned long int *__time, float *__lat, float *__lon)
{
    return csvGetEntries(__file, __time, "%lu", IGNORE, NULL, __lat, "%f", __lon, "%f");
}

/***************************************************************************
DESCRIZIONE:
Legge i valori timestamp, NO2, VOC, PM10 e PM2.5 dal file delle misure.

PARAMETRI:
    - puntatore a FILE CSV
    - Puntatore a `unsigned long int` "__time"
    - Puntatore a `float` "__no2"
    - Puntatore a `float` "__voc"
    - Puntatore a `float` "__pm10"
    - Puntatore a `float` "__pm25"

RETURN:
    - FILE_OK (0) se l'operazione è stata conclusa con successo
    - Un valore positivo se è stato raggiunto EOF
***************************************************************************/
int leggi_mis(FILE *__file, unsigned long int *__time, float *__no2, float *__voc, float *__pm10, float *__pm25)
{
    return csvGetEntries(__file, __time, "%lu", IGNORE, NULL, __no2, "%f", __voc, "%f", __pm10, "%f", __pm25, "%f", IGNORE, NULL, IGNORE, NULL, IGNORE, NULL, IGNORE, NULL);
}

/******************************************************************************
DESCRIZIONE:
Scrive i valori timestamp, latitudine, longitudine e misura sul file di output.

PARAMETRI:
    - Puntatore a FILE CSV
    - unsigned long int "__time"
    - float "__lat"
    - float "__lon"
    - float "__mis"

RETURN:
    - Un valore positivo se l'operazione è stata conclusa con successo
    - Altrimenti 0
******************************************************************************/
int scrivi_out(FILE *__file, unsigned long int __time, float __lat, float __lon, float __mis)
{
    // NOTA: Codice temporaneo, manca ancora implementazione per csvPutEntries() in CSVLib
    return fprintf(__file, "%lu,%f,%f,%f\n", __time, __lat, __lon, __mis);
}
