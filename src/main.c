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

/******************************************************************************
@data: Marzo 2022
@auhor: Classe 3CI as 2021/22

@scopo:
legge i file misure.csv e posizioni.csv
e produce in output il file out.csv mettendo insieme i dati dei due file,
controllando le vicinanze del timestamp e prendendo le misure
che sono distanti temporalmente  meno di 1 minuto  tra i due file

1) formato del file misure.csv
   - timestamp
   - data
   - NO2 (ppb)
   - VOC (ppb)
   - pm 10 (ug/m3)
   - pm 2.5 (ug/m3)
   + altro che non interessa

2) formato del file posizioni.csv
   - timestamp
   - data
   - latitude
   - longitude

3) formato del file out.csv
   - timestamp
   - latitude
   - longitude
   - NO2 (ppb)
   - VOC (ppb)
   - pm 10 (ug/m3)
   - pm 2.5 (ug/m3)

@compilazione:
WINDOWS: mingw32-make
POSIX:  make

@esecuzione:
WINDOWS: mingw32-make run fp=posizioni.csv fm=misure.csv fo=out.csv
POSIX:  make run fp=posizioni.csv fm=misure.csv fo=out.csv
******************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <filelib.h>
#include <csvlib.h>
#include <assertlib.h>
#include <siglib.h>
#include <iolib.h>
#include "wrappers.h"


// Crea costante con comando di clear
#ifdef _WIN32
    #define CLEARSTR "cls"
#else
    #define CLEARSTR "clear"
#endif

// Macro per screen clear
#define CLEAR() (void)system(CLEARSTR)

// Scelte
#define NO2 1               // Costante che rappresenta la scelta di NO2
#define VOC 2               // Costante che rappresenta la scelta di VOC
#define PM10 3              // Costante che rappresenta la scelta di PM10
#define PM25 4              // Costante che rappresenta la scelta di PM2.5

#define MAX_SPREAD 60


void disegna_logo()
{
    CLEAR();

    println("              _                ");
    println("             (_)               ");
    println("  _   _ _ __  _  ___ _____   __");
    println(" | | | | '_ \\| |/ __/ __\\ \\ / /");
    println(" | |_| | | | | | (__\\__ \\\\ V / ");
    println("  \\__,_|_| |_|_|\\___|___/ \\_/  \n\n");
}

/**************************************************
DESCRIZIONE:
Visualizza un menu' che permette di sceglere la
misura che si vuole unire.

PARAMETRI:
    - Puntatore ad `int` "__scelta"
    - Stringa "__nome_mis"
**************************************************/
void menu(int *__scelta, char __nome_mis[])
{
    while (true)
    {
        disegna_logo();

        // Stampa opzioni
        println("Selezionare misura");
        println("1. NO2");
        println("2. VOC");
        println("3. PM10");
        println("4. PM2.5");
        
        // Input utente
        printf("\nscelta: ");
        (void)scanf("%d", __scelta);  // Cast a void utilizzato per annullare valore ritornato
        
        // Valutazione inpu
        switch (*__scelta)
        {
            case NO2  : strcpy(__nome_mis, "NO2 (ppb)");    return;
            case VOC  : strcpy(__nome_mis, "VOC (ppb)");    return;
            case PM10 : strcpy(__nome_mis, "PM10 (ug/m3)"); return;
            case PM25 : strcpy(__nome_mis, "PM25 (ug/m3)"); return;
        }
    }
}

int main(int argc, char **argv)
{
    // Inizializza gestore segnali
    sigSetup();

    // Timestamp
    unsigned long int _p_time = 0, // Timestamp posizioni
                      _m_time = 0; // Timestamp misure
    
    // Posizioni
    float _p_lat,    // Latitudine
          _p_lon;    // Longitudine
    
    // Array misure
    float _misure[5];

    // Misure
    float *_no2  = &_misure[NO2],      // Diossido d'azzoto
          *_voc  = &_misure[VOC],      // Composti organici volatili
          *_pm10 = &_misure[PM10],     // PM10
          *_pm25 = &_misure[PM25];     // PM2.5
    
    // Nomi file
    char _fp_name[20],   // Nome file posizioni
         _fm_name[20],   // Nome file misure
         _fo_name[20];   // Nome file di output

    // Controllo argomenti commandline - Codice da pulire
    disegna_logo();
    massert(argc <= 4, -4, "Troppi argomenti: richiesti massimo 3, forniti %d.", argc);
    if (argc < 2) { printf("Inserire nome file posizioni: "); scanf("%s", _fp_name);                                            }
    if (argc < 3) { printf("Inserire nome file misure: ");    scanf("%s", _fm_name);  if (argc == 2) strcpy(_fp_name, argv[1]); }
    if (argc < 4) { printf("Inserire nome file di output: "); scanf("%s", _fo_name);  if (argc == 3) strcpy(_fm_name, argv[2]); }

    // Scelta
    int   _misura;
    char  _mis_name[10];

    // Chiede misura da mettere nel file di output
    menu(&_misura, _mis_name);
    disegna_logo();

    // Apre file stream verso i file richiesti
    println("Apertura file...");
    FILE *_fp = fileOpenRead(_fp_name);  // pointer al file delle posizioni
    FILE *_fm = fileOpenRead(_fm_name);  // pointer al file delle misure
    FILE *_fo = fileOpenWrite(_fo_name); // pointer al file di output

    // Ignora prima riga dei file di input
    println("Ignorando intestazioni CSV...");
    csvIgnoreLine(_fp);
    csvIgnoreLine(_fm);

    // Stampa intestazione tabella CSV
    println("Inserendo intestazione in file di output...");
    csvPutHeader(_fo, 4, "timestamp", "latitudine", "longitudine", _mis_name);

    printf("\nProgresso:\n[");

    // Determina se è possibile continaure o meno
    volatile bool _continuare = true;
             int  _count      = 0;

    // Fino a quando non si raggiunge la fine di uno dei due file di input
    while (_continuare)
    {
        // Se p_time è minore di m_time, si legge dal file posizioni, altrimenti dal file misure
        _continuare = (_p_time < _m_time) ? FILE_OK == leggi_pos(_fp, &_p_time, &_p_lat, &_p_lon) :
                                            FILE_OK == leggi_mis(_fm, &_m_time, _no2, _voc, _pm10, _pm25);
        
        // Differenza tra i timestamp di misure e posizioni
        int _diff = abs((int)_p_time - (int)_m_time);

        // Se la differenza tra i due timestamp è nella forbice accettabile
        if (_diff < MAX_SPREAD)
            massert(
                scrivi_out(_fo, _p_time, _p_lat, _p_lon, _misure[_misura]),           // Condizione
                -4, "Errore nella scrittura del file di output '%s'", _fo_name        // Se la condizione non è verificata
            );

        if (_count++ % 100 == 0)
            printf("#");
    }

    // Chiusura delle stream su tutti i file
    fclose(_fp);
    fclose(_fm);
    fclose(_fo);

    // Stampa due righe vuote alla fine
    println("\t100%\n");
}
