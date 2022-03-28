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


#include "wrappers.h"
#include <csvlib.h>
#include <stdio.h>


/***************************************************************************
DESCRIZIONE:
Legge i valori timestamp, latitudine e longitudine dal file delle posizioni.

PARAMETRI:
    - Puntatore a FILE CSV
    - Puntatore a `unsigned long int` "time"
    - Puntatore a `float` "lat"
    - Puntatore a `float` "lon"

RETURN:
    - FILE_OK (0) se l'operazione è stata conclusa con successo
    - Un valore positivo se è stato raggiunto EOF
***************************************************************************/
int leggi_pos(FILE *file, unsigned long int *time, float *lat, float *lon)
{
    return csvGetEntries(file, time, "%lu", IGNORE, NULL, lat, "%f", lon, "%f");
}

/***************************************************************************
DESCRIZIONE:
Legge i valori timestamp, NO2, VOC, PM10 e PM2.5 dal file delle misure.

PARAMETRI:
    - puntatore a FILE CSV
    - Puntatore a `unsigned long int` "time"
    - Puntatore a `float` "no2"
    - Puntatore a `float` "voc"
    - Puntatore a `float` "pm10"
    - Puntatore a `float` "pm25"

RETURN:
    - FILE_OK (0) se l'operazione è stata conclusa con successo
    - Un valore positivo se è stato raggiunto EOF
***************************************************************************/
int leggi_mis(FILE *file, unsigned long int *time, float *no2, float *voc, float *pm10, float *pm25)
{
    return csvGetEntries(file, time, "%lu", IGNORE, NULL, no2, "%f", voc, "%f", pm10, "%f", pm25, "%f", IGNORE, NULL, IGNORE, NULL, IGNORE, NULL, IGNORE, NULL);
}

/******************************************************************************
DESCRIZIONE:
Scrive i valori timestamp, latitudine, longitudine e misura sul file di output.

PARAMETRI:
    - Puntatore a FILE CSV
    - unsigned long int "time"
    - float "lat"
    - float "lon"
    - float "mis"

RETURN:
    - Un valore positivo se l'operazione è stata conclusa con successo
    - Altrimenti 0
******************************************************************************/
int scrivi_out(FILE *file, unsigned long int time, float lat, float lon, float mis)
{
    // NOTA: Codice temporaneo, manca ancora implementazione per csvPutEntries() in CSVLib
    return fprintf(file, "%lu,%f,%f,%f\n", time, lat, lon, mis);
}
