#include <string.h>
#include <grx20.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tools.h"
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

void zobraz_data (double *data, long int zacatek, long int konec, double miny, double maxy, GrColor barva)
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
     GrLine( x0,y0, x, y, barva );
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


int main()
{
  double *data, *data_filt, mx, mn;
  int NSAMP, i;


  NSAMP=spocitej_delku("eog.txt");
  printf("\n%i\n", NSAMP);

  data = alokuj_vektor(NSAMP);
  data_filt = alokuj_vektor(NSAMP);

  nacti_vektor(data, NSAMP, "eog.txt");

  mx=data[0]; mn=data[0];
  for(i=1; i<NSAMP; i++) {
    if (data[i]>mx) mx=data[i];
    if (data[i]<mn) mn=data[i];
  }

  GrSetMode(  GR_width_height_graphics, 1500, 400 );
  GrClearScreen( GrAllocColor(0,0,0) );

  double h[100] ;       // budouci konvolutorni jadro

  #define TWO_PI M_PI*2
  double fhi=25;        // horni mezni frekvence
  double fsamp=200;     // vzorkovaci frekvnece
  int N=21;             // delka filtru (rad je N-1)
  int m;                // pomocny index
  double omh=TWO_PI*(fhi/fsamp);  // normovana mezni frekvnce filtru
  double sum = 0;
  double sum_hamming = 0;
  double hamming = 0;

  for (i=0; i<N; i++)
                     {
                       m = i - ((N-1)/2.0);

                    if (i != 0) {
                      hamming = 0.54 - 0.45 * cos(2.0 * M_PI * (double)i / (double)N);
                    }

                    printf("hamming: %f\n", hamming);

                       // 1. zde implementovat navrhove vztahy pro FIR dolni propust
                    if (m != 0) {
                       h[i] = sin(m * omh) / (m * M_PI);
                    } else {
                       h[i] = omh * cos(m * omh) / M_PI;
                       }  // misto prirazeni nuly bude implementovany designovy vzorec
                    h[i] *= hamming;
                    sum += h[i];
                    sum_hamming += hamming;

                       printf("m=%i h[%i]=%lf\n", m, i, h[i]);
                     }
printf("sum hamming: %f", sum_hamming);
 for (i = 0; i < N; i++) {
    h[i] /= sum;
 }

  // 2. naspedne implementovat vynasobeni vybranou okenkovou funkci
  // . . .

  // 3. volitelne implementovat ad-hoc reseni konecnosti impulzni odezvy ( integral h[] se nerovna 0, proto je ve vysledku napr. offset )
  // 3.a  vypocte integralu h[], 3.b normovani h[] spoctenou hodnotou, aby nove h[] melo integral roven 1


  konvoluce ( data, data_filt, NSAMP, h, N); // vlastni filtrace = konvoluce signalu a jadra

  zobraz_data (data, 0, NSAMP-1, mn, mx, GrAllocColor(0,255,0)); // zobrazeni puvodniho signalu
  zobraz_data (data_filt, 0, NSAMP-1, mn, mx, GrAllocColor(255,0,0)); // zobrazeni vyfiltrovaneho signalu


  printf("\nstisknente lib. klavesu v okne s grafikou ...\n");
  GrKeyRead();

  return 0;
}





