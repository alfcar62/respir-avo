#include <filelib.h>
#include <csvlib.h>
#include <stdio.h>


int main()
{
    FILE *file = fileOpenRead("posizioni.csv");
    csvIgnoreLine(file);

    unsigned long int time = 0;
    float a = 0, b = 0;

    csvGetEntries(file, &time, "%lu", NULL, NULL, &a, "%f", &b, "%f");
    printf("\n%lu %f %f\n", time, a, b);
}
