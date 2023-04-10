#include <string.h>
#include "grx/include/grx20.h"
#include "grx/include/grxkeys.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



int spocitej_delku(char *soubor)
{
  FILE *infile; long int c=0; int r=0; double h;
  infile=fopen(soubor, "r");
  if (infile==NULL) { printf("\nnepodarilo se otevrit soubor\n"); exit(-2); }

  do {
          r=fscanf(infile,"%lf", &h);
          c++;

  } while ( ( r==1 ) && (c<300000) );
  fclose(infile);
  return c-1;
}



double* alokuj_vektor (long int pocet)
{ double *o;
  o=(double*)malloc(sizeof(double)*pocet);
  if(o==NULL ) { printf("\nnepodarilo se alokovat pamet\n"); exit(-2); }
  return o;
}

int nacti_vektor (double* vec, long int pocet, char *soubor)
{
  FILE *infile; long int c; int r;

  infile=fopen(soubor, "r");
  if (infile==NULL) { printf("\nnepodarilo se otevrit soubor\n"); exit(-2); }
  c=0; r=0;
  do {
          r=fscanf(infile,"%lf", &vec[c]);
          c++;

  } while ( ( r==1 ) && (c<pocet) );
  fclose(infile);
  c-=1;

}


double stredni_hodnota (double * signal, int delka)
{
   int i; double s = 0.0;

   for (i=0; i<delka; i++) s += signal[i];

   return s / (double)delka ;

}



void konvoluce (double *vstupni_signal, double *vystupni_signal,
                    int delka_signalu, double *jadro, int delka_jadra)
{

  // zde implementujte algoritmus pro vypocet konvoluce
  // vstupy: pole vstuopni_signal (o delce delka_signalu),
  //         pole jadro (o delce delka_jadra)
  // vystup: pole vystupbni_signal (o delce delka_signalu)
  //

  // poznamka: zatim je vystupni signal vynulovan
  int shift = (delka_jadra - 1) / 2;
  for (int i=shift; i<delka_signalu; i++) {
    vystupni_signal[i] = 0.0;
    for (int j=0; j<delka_jadra; j++) {
        if ((i-j) > 0) {
            vystupni_signal[i-shift] += (vstupni_signal[i-j] * jadro[j]);
        } else {
            vystupni_signal[i-shift] += (vstupni_signal[0] * jadro[j]);
        }
    }
  }
}

void correlation (double *input, double *output, int len_sig, double *kernel, int len_kernel) {

    int shift = (len_sig - 1) / 2;
    for (int i=shift; i<len_sig; i++) {
        output[i] = 0.0;
        for (int j=len_kernel-1; j>=0; j--) {
            if ((i-j) > 0) {
                output[i-shift] += (input[i-j] * kernel[j]);
            } else {
                output[i-shift] += (input[0] * (kernel[j]);
            }
        }
  }

}



void minmax (double *vekt, long int delka, double *min, double *max)
{ long int i;
  *max=vekt[0]; *min=vekt[0];
  for(i=1; i<delka; i++) {
    if (vekt[i]>*max) *max=vekt[i];
    if (vekt[i]<*min) *min=vekt[i];
  }
}

void zobraz_data (double *data, int zacatek, int konec, double miny, double maxy, GrColor barva)
{
  double x,y, y0, x0, k, l;
  int i;
  k = (double)GrMaxY()/(double)(maxy-miny) ;
  l = (double)GrMaxX()/(double)(konec-zacatek) ;
  x0= (0) *l  ; y0= data[zacatek]*k - miny*k;
  //GrCircle( x0,y0, 5, GrAllocColor(0,255,0) );
  for(i=zacatek+1; i<=konec; i++)
  {

     x= (i) * l - l*zacatek;
     y= data[i] * k - miny*k;
     GrLine( x0,y0, x, y, barva );
     x0=x; y0=y;
     //GrPlot ( x, y, GrAllocColor(0,255,0) );
     //GrCircle( x,y, 2, GrAllocColor(0,255,0) );
  }

}
