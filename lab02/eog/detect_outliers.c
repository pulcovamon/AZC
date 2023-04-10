#include <string.h>
#include "grx/include/grx20.h"
#include "grx/include/grxkeys.h"
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
    x0= (0) *l  ;
    y0= data[zacatek]*k - miny*k;
    //step= (konec-zacatek)/GrMaxX();
    step=1;
    //GrCircle( x0,y0, 5, GrAllocColor(0,255,0) );
    for(i=zacatek+1; i<=konec; i+=step)
    {

        x= (i) * l - l*zacatek;
        y= data[i] * k - miny*k;
        GrLine( x0,GrMaxY()-y0, x, GrMaxY()-y, barva );
        x0=x;
        y0=y;

    }

}

void zobraz_popis_os(long int zacatek, long int konec)
{
    char szacatek[20], skonec[20];
    sprintf(szacatek,"%15li",zacatek);
    sprintf(skonec,"%15li",konec);
    GrDrawString( szacatek, strlen( szacatek ), 10, GrMaxY()-20, &grt );
    GrDrawString( skonec, strlen( skonec ), GrMaxX()-130, GrMaxY()-20, &grt );
}


int main()
{
    double *data,  *data_dif, mx, mn;
    int NSAMP, i;


    NSAMP=spocitej_delku("eog.txt");
    printf("\n%i\n", NSAMP);

    data = alokuj_vektor(NSAMP);

    nacti_vektor(data, NSAMP, "eog.txt");

    inicializuj_grafiku();
    GrClearScreen( GrAllocColor(0,0,0) );

    zobraz_popis_os(0, NSAMP-1);
    minmax(data,NSAMP,&mn, &mx);
    zobraz_data (data, 0, NSAMP-1, mn, mx, GrAllocColor(0,255,0));

    data_dif = alokuj_vektor(NSAMP); // alokace pameti pro zderivovany signal
    data_dif[0] = data[0];

    for (i=0; i<NSAMP; i++) data_dif[i]=0;

    for (i=5; i<NSAMP; i++) {
        if (i < 5) {
            data_dif[i] = pow((data[i] - data[i-1]), 2);
        }
        data_dif[i] = pow((data[i] - data[i-5]) / 5, 2); // vynulovani vysledku
        // sem vypocet 1. diference signalu ( 2 nebo 3 bodova formule)

    };

    for (i=0; i<NSAMP-20; i++) {
        for (int j=0; j<20; j++) {
            data_dif[i] += data_dif[i+j];
        }
        data_dif[i] /= 5;
    }

    minmax(data_dif,NSAMP,&mn, &mx); // nastaveni rozsahu zobrayovane amplitudy vysledku
    zobraz_data (data_dif, 0, NSAMP-1, mn, mx, GrAllocColor(0,0,255));  // zobrazeni vysledku


    printf("\nstisknente lib. klavesu v okne s grafikou ...\n");
    GrKeyRead();

    return 0;
}




