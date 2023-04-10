#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Lienarni kongruentni generator pseudonahodnych cisel
// vrati dalsi iteraci rekrentniho vypoctu LCG
unsigned long int moje_rand(void)
{
    static unsigned long int s = 1;
// TODO: implementujte vlastni generator pseudonahodnych cisel

    unsigned long int a = 279470273;
    unsigned long int c = 4294967;
    unsigned long int random = a*s % c;
    s = random;

    return random;

}

    /*Funkce pro vypocet histogramu.
    vstupy:
            value ... pole s daty
            samples ... pocet dat
    vystupy:
            bin ... pole s prvky histogramu
            num_bins ... pocet prvku histogramu
    pozn.: pole value i bin museji byt predem alokovana*/



void histo (double *value, int samples, int *bin, int num_bins )
{
// TODO: implementujte algoritmus vypoctu histogramu
    int i;
    for (i = 0; i < num_bins; i++) {
        bin[i] = 0;
    }

    double max = value[0];
    double min = value[0];
    for (i = 0; i < samples; i++) {
        if (value[i] < min) min = value[i];
        if (value[i] > max) max = value[i];
    }
    int index;
    for (i = 0; i < samples; i++) {
        index = (num_bins / (max - min)) * (value[i] - min);
        bin[index]++;
    }


}

double gaussian(void) {
    int magic_num = 32768;
    double random1 = moje_rand() % magic_num;
    double random2 = moje_rand() % magic_num;
    random1 /= magic_num;
    random2 /= magic_num;
    double gauss = sqrt(-2.0*log(random1)) * cos(2.0*M_PI*random2);
    //printf("gauss %f\n", gauss);
    return gauss;
}

int main()
{

 double *values; int N=10000;
 int *histogram; int pocet_slotu=10;
 int i;

 values = (double*)malloc(N*sizeof(double)); // + kontrola
 histogram = malloc(pocet_slotu*sizeof(int)); // + kontrola

 for (i=0; i<N; i++)
 {
   //values[i]= moje_rand() % 32768;
   values[i] = gaussian();
 }


 histo (values, N, histogram, pocet_slotu);

 for (i=0; i<pocet_slotu; i++)
    printf("%5i : %5i\n", i, histogram[i] );


 free(histogram);
 free(values);

 return 0;

}

