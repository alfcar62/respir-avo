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
#include <stdint.h>
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
#define CLEAR() system(CLEARSTR)

// Scelte
#define NO2 1               // Costante che rappresenta la scelta di NO2
#define VOC 2               // Costante che rappresenta la scelta di VOC
#define PM10 3              // Costante che rappresenta la scelta di PM10
#define PM25 4              // Costante che rappresenta la scelta di PM2.5

// massimi limiti consentiti
#define MAX_NO2 10          // ppb
#define MAX_VOC 10          // ppb
#define MAX_PM10 10         // umg/m3
#define MAX_PM25 10         // umg/m3

#define MAX_SPREAD 60


/**************************************************
DESCRIZIONE:
Visualizza un menu' che permette di sceglere la
misura che si vuole unire.

PARAMETRI:
    - Puntatore ad `int` "scelta"
    - Stringa "nome_mis"
**************************************************/
void menu(int *scelta, char nome_mis[])
{
    bool err = true;

    do
    {
        err = false;

        // Stampa opzioni
        println("Selezionare misura:");
        println("1: NO2");
        println("2: VOC");
        println("3: PM10");
        println("4: PM2.5");
        
        // Input utente
        printf("scelta: ");
        scanf("%d", scelta);
        
        // Valutazione inpu
        switch (*scelta)
        {
            case NO2  : strcpy(nome_mis, "NO2 (ppb)");    break;
            case VOC  : strcpy(nome_mis, "VOC (ppb)");    break;
            case PM10 : strcpy(nome_mis, "PM10 (ug/m3)"); break;
            case PM25 : strcpy(nome_mis, "PM25 (ug/m3)"); break;

            default:
                err = true;
                println("ERRORE: Opzione %d non valida.", *scelta);
                break;
        }
    } while (err);
}

int main(int argc, char const *argv[])
{
    // Inizializza gestore segnali
    sigSetup();

    // Si assicura che siano passati tutti i tre argomenti richiesti
    massert(4 == argc, -1, "Numero insufficiente di argomenti: Richiesti 3, dati %d", argc - 1);

    // Nomi file
    const char *fp_name = argv[1],   // Nome file posizioni
               *fm_name = argv[2],   // Nome file misure
               *fo_name = argv[3];   // Nome file di output

    // Timestamp
    unsigned long int p_time, // Timestamp posizioni
                      m_time; // Timestamp misure
    
    // Posizioni
    float p_lat,    // Latitudine
          p_lon;    // Longitudine
    
    // Array misure
    float misure[5];

    // Misure
    float *no2  = &misure[NO2],      // Diossido d'azzoto
          *voc  = &misure[VOC],      // Composti organici volatili
          *pm10 = &misure[PM10],     // PM10
          *pm25 = &misure[PM25];     // PM2.5
    
    // Scelta
    int   misura;
    char  mis_name[10];

    // Chiede misura da mettere nel file di output
    menu(&misura, mis_name);

    println("Inizio elaborazione");
    println("........................");

    // Apre file stream verso i file richiesti
    FILE *fp = fileOpenRead(fp_name);  // pointer al file delle posizioni
    FILE *fm = fileOpenRead(fm_name);  // pointer al file delle misure
    FILE *fo = fileOpenWrite(fo_name); // pointer al file di output
    
    // Ignora prima riga dei file di input
    csvIgnoreLine(fp);
    csvIgnoreLine(fm);

    // Stampa intestazione tabella CSV
    csvPutHeader(fo, 4, "timestamp", "latitudine", "longitudine", mis_name);

    // Determina se è possibile continaure o meno
    bool continuare = true;

    // Fino a quando non si raggiunge la fine di uno dei due file di input
    while (continuare)
    {
        // Se p_time è minore di m_time, si legge dal file posizioni, altrimenti dal file misure
        continuare = (p_time < m_time) ? FILE_OK == leggi_pos(fp, &p_time, &p_lat, &p_lon) :
                                         FILE_OK == leggi_mis(fm, &m_time, no2, voc, pm10, pm25);
        
        // Differenza tra i timestamp di misure e posizioni
        int diff = abs((int)p_time - (int)m_time);
        
        // Se la differenza tra i due timestamp è nella forbice accettabile
        if (diff < MAX_SPREAD)
            massert(
                scrivi_out(fo, p_time, p_lat, p_lon, misure[misura]),           // Condizione
                -4, "Errore nella scrittura del file di output '%s'", fo_name   // Se la condizione non è verificata
            );
    }

    // Chiusura delle stream su tutti i file
    fclose(fp);
    fclose(fm);
    fclose(fo);
}
