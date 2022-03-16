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


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>


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
WINDOWS: mingw32-make run
POSIX:  make run
******************************************************************************/


// Massimi file
#define MAX_LINES   1000    // Massimo numero di linee
#define MAX_STR_LEN 200     // Dimensione massima di una stringa

// Codici di errore e successo
#define FILE_ERROR NULL     // NullPointer, utilizzata per verificare se si è verificato un errore nell'apertura di una file stream
#define ERR -1              // Valore di errore restituito dalle funzioni/procedure
#define OK 0                // Valore di successo restituito da funzioni/procedure

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


/*****************************************************************************************************************************************
RET TYPE    NAME            ARGUMENTS
/*****************************************************************************************************************************************/
int         leggi_pos       (FILE *fp, unsigned long int *time, float *lat, float *lon);
int         leggi_mis       (FILE *fp, unsigned long int *time, float *no2, float *voc, float *pm10, float *pm25);
int         scrivi_out      (FILE *fp, unsigned long int time, double lat, double lon, int mis, float no2, float voc, float pm10, float pm25);
int         check_limite    (int tipo_mis, float misura, char superato[], int *perc_sup);
void        menu            (char p[], char m[], char o[], int *scelta, char nome_mis[]);
float       get_media       (int arr_mis[], int i);

/*************************
TYPE    NAME        VALUE
*************************/
int     cnt_pos  =  0;
int     cnt_mis  =  0;


int main()
{
    int arr_mis[100];
    int i;
    char fp_name[20], fm_name[20], fo_name[20];
    int ret;
    unsigned long int p_time, m_time; // timestamp
    float p_lat, p_lon;               // coordinate
    float voc, no2, pm25, pm10;       // misurazioni
    float mis_medio;
    char str[MAX_STR_LEN];
    long int difftime;
    int misura;
    char mis_name[10];

    FILE *fp; // pointer al file delle posizioni
    FILE *fm; // pointer al file delle misure
    FILE *fo; // pointer al file di output

    menu(fp_name, fm_name, fo_name, &misura, mis_name);

    printf("Inizio elaborazione\n");
    printf("\n........................\n");

    fp = fopen(fp_name, "r"); // apertura in modalit� lettura
    if (fp == FILE_ERROR)
    {
        printf("\nErrore in apertura file:-%s- non  esistente: \n", fp_name);
        return ERR;
    }

    fm = fopen(fm_name, "r"); // apertura in modalit� lettura
    if (fm == FILE_ERROR)
    {
        printf("\nErrore in apertura file:-%s- non  esistente: \n", fm_name);
        return ERR;
    }

    fo = fopen(fo_name, "w+"); // apertura in modalit� lettura/scrittura
    if (fo == FILE_ERROR)
    {
        printf("\nErrore in apertura file:-%s- non  esistente: \n", fo_name);
        return ERR;
    }

    if (fgets(str, MAX_STR_LEN, fp) == NULL) // skip prima riga (header)
    {
        printf("\nErrore in fgets() skip file posizioni\n");
        return ERR;
    }

    if (fgets(str, MAX_STR_LEN, fm) == NULL) // skip prima riga (header)
    {
        printf("\nErrore in fgets() skip  file misure()\n");
        return ERR;
    }

    // Prima riga intestazione
    fprintf(fo, "timestamp,latitude,longitude,%s,superato limite, percentuale sup", mis_name);

    ret = leggi_pos(fp, &p_time, &p_lat, &p_lon);
    if (ret != OK)
    {
        printf("\nErrore in leggi_pos() prima riga file posizioni)\n");
        goto FINE;
    }

    ret = leggi_mis(fm, &m_time, &no2, &voc, &pm10, &pm25);
    if (ret != OK)
    {
        printf("\nErrore in leggi_mis() prima riga file misure\n");
        goto FINE;
    }

    do
    {
        difftime = abs((int)p_time - (int)m_time);
        
        // mMno di 1 minuto di differenza
        if (difftime < 60)
        {
            ret = scrivi_out(fo, p_time, p_lat, p_lon, misura, no2, voc, pm10, pm25);
            if (ret != OK)
            {
                printf("\nErrore in scrivi_out()\n");
                goto FINE;
            }
        }

        if (p_time < m_time)
        {
            ret = leggi_pos(fp, &p_time, &p_lat, &p_lon);
            if (ret != OK)
            {
                printf("\nErrore in leggi_pos() prima riga file posizioni)\n");
                goto FINE;
            }
        }
        else
        {
            ret = leggi_mis(fm, &m_time, &no2, &voc, &pm10, &pm25);
            if (ret == EOF)
            {
                //        printf("\nEOF mis\n");
                goto FINE;
            }
        }
    } while (true); // do

FINE:
    fclose(fp);
    fclose(fm);
    fclose(fo);
    printf("\nCreato file %s OK\n", fo_name);
} // main

/********************************************************
DESCRIZIONE:
Legge dal file posizioni.csv
formato del file: timestamp, date, latitude, longitude
********************************************************/
int leggi_pos(FILE *fp, unsigned long int *time, float *lat, float *lon)
{
    char str1[MAX_STR_LEN];
    char str2[MAX_STR_LEN];
    int len;
    int i1;      // indice array str1
    int i2;      // indice array str2
    int pos = 0;
    int ret;

    // printf("\nentro in leggi_pos()\n");
    if (fgets(str1, MAX_STR_LEN, fp) == NULL) // lettura di una riga del file
        return EOF;

    len = strlen(str1);

    // printf("\nletto str1 = '%s' (len=%d)\n",str1,len);
    for (i1 = 0, i2 = 0; i1 < len; i1++, i2++)
    {
        if (str1[i1] == ',' || str1[i1] == '\n' || str1[i1] == '\0')
        {
            //       printf("\nprima di str2[i2] i2 = %d",i2);
            str2[i2] = '\0';

            pos++;
            switch (pos)
            {
                case 1: // timestamp
                    sscanf(str2, "%ld", time);
                    break;

                case 2: // date
                    break;

                case 3: // lat
                    sscanf(str2, "%f", lat);
                    break;

                case 4: // lon
                    sscanf(str2, "%f", lon);
                    break;
            }
            i2 = (-1);
        }
        else if (str1[i1] != '\n')
            str2[i2] = str1[i1];
        if (str1[i1] == '\n' || str1[i1] == '\0')
            str2[i2] = '\0';
    }

    cnt_pos++;
    return OK;
}

/**********************************************
DESCRIZIONE:
leggi_mis() Legge una riga dal file misure.csv
formato del file:
   timestamp data  NO2 VOC pm 10 pm 2.5
   + altro che non interessa

eturn OK
      ERR
**********************************************/
int leggi_mis(FILE *fp, unsigned long int *time, float *no2, float *voc, float *pm10, float *pm25)
{
    char str1[MAX_STR_LEN];
    char str2[MAX_STR_LEN];
    int len;
    int i1; // indice array str1
    int i2; // indice array str2
    int pos = 0;
    int ret;

    if (fgets(str1, MAX_STR_LEN, fp) == NULL) // lettura di una riga del file
        return EOF;

    len = strlen(str1);
    // printf("\nfile misure  str1 = %s\n",str1);
    for (i1 = 0, i2 = 0; i1 < len; i1++, i2++)
    {
        if (str1[i1] == ',' || str1[i1] == '\n' || str1[i1] == '\0')
        {
            str2[i2] = '\0';
            //      printf ("sottostringa letta n(str2): '%s'\n", str2);
            pos++;
            switch (pos)
            {
            case 1: // timestamp
                sscanf(str2, "%ld", time);
                break;

            case 2: // data e ora
                    // skip
                break;

            case 3: // NO2
                sscanf(str2, "%f", no2);
                break;

            case 4: // VOC
                sscanf(str2, "%f", voc);
                break;

            case 5: // PM10
                sscanf(str2, "%f", pm10);
                break;

            case 6: // PM25
                sscanf(str2, "%f", pm25);
                break;
            }
            i2 = (-1);
        }
        else if (str1[i1] != '\n')
            str2[i2] = str1[i1];
        if (str1[i1] == '\n' || str1[i1] == '\0')
        {
            str2[i2] = '\0';
            //          printf ("-----fine riga--------\n");
        }
    }
    cnt_mis++;
    // printf("\ncnt_mis = %d, letto mis time=%ld,no2=%f,voc=%f,pm10=%f,pm25=%f\n",
    //            cnt_mis, *time, *no2, *voc, *pm10, *pm25);
    return OK;
}

// scrive una riga sul file di output
int scrivi_out(FILE *fp, unsigned long int time, double lat, double lon, int tipo_mis, float no2, float voc, float pm10, float pm25)
{
    float misura;
    char superato_limite[10] = "NO";
    int ret;
    int perc_sup;

    switch (tipo_mis)
    {
    case NO2:
        misura = no2;
        break;
    case VOC:
        misura = voc;
        break;
    case PM10:
        misura = pm10;
        break;
    case PM25:
        misura = pm25;
        break;
    default:
        printf("\nmisura non prevista\n");
        return ERR;
        break;
    }
    ret = check_limite(tipo_mis, misura, superato_limite, &perc_sup);
    if (ret == ERR)
        return ERR;

    fprintf(fp, "\n%ld,%f,%f,%f,%s %d", time, lat, lon, misura, superato_limite, perc_sup);
    return OK;
}

float get_media(int arr_mis[], int i)
{
    return OK;
}

int check_limite(int tipo_mis, float misura, char superato[], int *perc_sup)
{
    strcpy(superato, "NO");
    
    switch (tipo_mis)
    {
        case NO2:
            if (misura > MAX_NO2)
            {
                strcpy(superato, "SI");
                *perc_sup = roundf((100 * (misura - MAX_NO2) / MAX_NO2));
                //  	       printf("\nMAX=%d,misura = %f,perc sup=%d",MAX_NO2,misura,*perc_sup);
            }
            break;
        case VOC:
            if (misura > MAX_VOC)
            {
                strcpy(superato, "SI");
                *perc_sup = roundf((100 * (misura - MAX_VOC) / MAX_VOC));
            }
            break;
        case PM10:
            if (misura > MAX_PM10)
            {
                strcpy(superato, "SI");
                *perc_sup = roundf((100 * (misura - MAX_PM10) / MAX_PM10));
            }
            break;
        case PM25:
            if (misura > MAX_PM25)
            {
                strcpy(superato, "SI");
                *perc_sup = roundf((100 * (misura - MAX_PM25) / MAX_PM25));
            }
            break;
        default:
            printf("\nmisura non prevista\n");
            return ERR;
            break;
    }

    return OK;
}

/**************************************************
menu: vidualizza il menu delle scelte utente
@params: char p[]: nome del file delle posizioni
      char m[]: nome del file delle misure
      char o[]: nome del file di output
      int *scelta : scelta dell'utente
      char nome_mis[]: misura da rilevare
**************************************************/
void menu(char p[], char m[], char o[], int *scelta, char nome_mis[])
{
    int err = true;
    printf("\nnome del file delle posizioni:");
    scanf("%s", p);

    printf("\nnome del file delle misure:");
    scanf("%s", m);

    printf("\nnome del file di output:");
    scanf("%s", o);

    do
    {
        err = false;
        printf("\nquale misura vuoi elaborare:");
        printf("\n1: NO2:");
        printf("\n2: VOC:");
        printf("\n3: PM10:");
        printf("\n4: PM2.5:");
        printf("\nscelta: ");
        scanf("%d", scelta);
        switch (*scelta)
        {
        case NO2:
            strcpy(nome_mis, "NO2 (ppb)");
            break;
        case VOC:
            strcpy(nome_mis, "VOC (ppb)");
            break;
        case PM10:
            strcpy(nome_mis, "PM10 (ug/m3)");
            break;
        case PM25:
            strcpy(nome_mis, "PM25 (ug/m3)");
            break;

        default:
            err = true;
            printf("\ripetere\n");
            break;
        }
    } while (err);
}
