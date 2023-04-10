#include <string.h>
#include "grx/include/grx20.h"
#include "grx/include/grxkeys.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

GrTextOption grt;
//extern GrColor *egacolors;
#define BLACK        egacolors[0]
#define BLUE         egacolors[1]
#define GREEN        egacolors[2]
#define CYAN         egacolors[3]
#define RED          egacolors[4]
#define MAGENTA      egacolors[5]
#define BROWN        egacolors[6]
#define LIGHTGRAY    egacolors[7]
#define DARKGRAY     egacolors[8]
#define LIGHTBLUE    egacolors[9]
#define LIGHTGREEN   egacolors[10]
#define LIGHTCYAN    egacolors[11]
#define LIGHTRED     egacolors[12]
#define LIGHTMAGENTA egacolors[13]
#define YELLOW       egacolors[14]
#define WHITE        egacolors[15]

GrColor *egacolors;


void inicializuj_grafiku()
{
  GrSetMode(  GR_width_height_graphics, 1500, 400 );
  grt.txo_font = &GrDefaultFont;
  grt.txo_fgcolor.v = GrBlack();
  grt.txo_bgcolor.v = GrWhite();
  grt.txo_direct = GR_TEXT_RIGHT;
  grt.txo_xalign = GR_ALIGN_LEFT;
  grt.txo_yalign = GR_ALIGN_TOP;
  grt.txo_chrtype = GR_BYTE_TEXT;
  //GrColor *egacolors;
  egacolors = GrAllocEgaColors();


}

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
  FILE *infile; long int c; long int r;

  infile=fopen(soubor, "r");
  if (infile==NULL) { printf("\nnepodarilo se otevrit soubor\n"); return(-2); }
  c=0; r=0;
  do {
          r=fscanf(infile,"%lf", &vec[c]);
          c++;

  } while ( ( r==1 ) && (c<pocet) );
  fclose(infile);
  c-=1;
  return 0;

}


void zobraz_data (double *data, long int zacatek, long int konec, double miny, double maxy, GrColor *barva)
{

  double x,y, y0, x0, k, l;
  long int i, step;
  k = (double)GrMaxY()/(double)(maxy-miny) ;
  l = (double)GrMaxX()/(double)(konec-zacatek) ;
  x0= (0) *l  ; y0= data[zacatek]*k - miny*k;
  //step= (konec-zacatek)/GrMaxX();
  step=1;
  //GrCircle( x0,y0, 5, GrAllocColor(0,255,0) );
  for(i=zacatek+1; i<=konec; i+=step)
  {

     x= (i) * l - l*zacatek;
     y= data[i] * k - miny*k;
     GrLine( x0,y0, x, y, barva[i] );
     x0=x; y0=y;
     // GrPlot ( x, y, GrAllocColor(0,255,0) );
     //GrCircle( x,y, 2, GrAllocColor(0,255,0) );
  }

}

void zobraz_popis_os(long int zacatek, long int konec)
{ char szacatek[20], skonec[20];
     sprintf(szacatek,"%15li",zacatek); sprintf(skonec,"%15li",konec);
     GrDrawString( szacatek, strlen( szacatek ), 10, GrMaxY()-20, &grt );
     GrDrawString( skonec, strlen( skonec ), GrMaxX()-130, GrMaxY()-20, &grt );
}

double *deriv(double* sig, long int delka) {
    double *data_dif;
    data_dif = alokuj_vektor(delka); // alokace pameti pro zderivovany signal

    for (int i=0; i<delka; i++) data_dif[i]=0;

    for (int i=5; i<delka; i++) {
        if (i < 5) {
            data_dif[i] = pow((sig[i] - sig[i-1]), 2);
        }
        data_dif[i] = pow((sig[i] - sig[i-5]) / 5, 2); // vynulovani vysledku
        // sem vypocet 1. diference signalu ( 2 nebo 3 bodova formule)

    };

    return data_dif;
}

int zpracuj_signal (double* sig, long int sig_delka, GrColor *priznak)
{
 int i;
 double *dif;
 dif = deriv(sig, sig_delka);

    for (i = 0; i < sig_delka; i++) {
        if (sig[i] > 40.0) {
            priznak[i] = RED;
        } else if (sig[i] >= 39.0) {
            priznak[i] = LIGHTRED;
        } else if (sig[i] >= 38.0) {
            priznak[i] = YELLOW;
        } else {
            priznak[i] = GREEN;
        }

        if ((dif[i] > 0.02) | (dif[i] < -0.02)) {
            priznak[i] = MAGENTA;
        }
    }

    free(dif);
}


int main()
{
  double *data, mx, mn;
  long int NSAMP, i;
  long int  xpos, okno;
  char kl;

  inicializuj_grafiku();

  NSAMP=spocitej_delku("orig-lapvatd-2.txt");
  printf("\n%li\n", NSAMP);

  data = alokuj_vektor(NSAMP);

  nacti_vektor(data, NSAMP, "orig-lapvatd-2.txt");

  GrColor *priznak;
  priznak=(GrColor*)malloc(sizeof(GrColor)*NSAMP);
  if(priznak==NULL) { printf("\nnepodarilo se alokovat pamet\n"); exit(-2); }

  zpracuj_signal(data, NSAMP, priznak);

  mx=data[0]; mn=data[0];
  for(i=1; i<NSAMP; i++) {
    if (data[i]>mx) mx=data[i];
    if (data[i]<mn) mn=data[i];
  }

  xpos=0;
  okno=15000;
  do {
   GrClearScreen( GrAllocColor(55,55,55) );

   zobraz_data (data, xpos, xpos+okno-1, mn, mx, priznak);
   zobraz_popis_os(xpos, xpos+okno-1);


   system("cls");
   printf("\n[f]-dopredu, [b]-dozadu, [q]-konec");
   printf("\n[d]-zmena delky okna");
   printf("\n\n");
   printf("\npocatek okna xpos = %li", xpos);
   printf("\ndelka okna = %li", okno);
   printf("\ndelka souboru = %li", NSAMP);
   printf("\nstisknente lib. klavesu v okne s grafikou ...\n");

    kl = GrKeyRead();
    switch(kl)
    {
      case 'f': xpos=xpos+okno; if((xpos+okno)>NSAMP) xpos=NSAMP-okno; break;
      case 'b': xpos=xpos-okno; if((xpos-okno)<0) xpos=0; break;

      case 'd': printf("\n\nzadejte novou delku okna d=");
                 scanf("%li", &okno);
                 if((xpos+okno)>NSAMP) xpos=NSAMP-okno;
                 if(xpos<0) xpos=0;
                break;
    }


  } while ( kl != 'q' );

  return 0;
}

