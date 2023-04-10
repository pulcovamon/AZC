#include <string.h>
#include "grx/include/grx20.h"
#include "grx/include/grxkeys.h"
#include <stdio.h>
#include <stdlib.h>
#define MAXHORIZ 148
#define MAXVERTI 302


GrColor paleta[256];


int vykresli_mri( int **m,int h, int v)
{ int i,j;

  GrContext *grc;

  if( (grc =  GrCreateContext( MAXVERTI*2,MAXHORIZ*2,NULL,NULL )) == NULL){
       printf("chyba\n"); system("pause"); exit(-1);
  }
  GrSetContext( grc );
  GrClearContext( GrWhite() );



  for(i=0; i< h; i+=1)
    {
        for(j=0; j< v; j+=1)
        {
            // sem misto vypisu textu doplnte kod pro vykresleni bitmapy na obrazovku
            // vyuzijte predmichane palety odstinu sede - paleta[0...254]
            //printf("prvek [%i][%i] = %i\n", i, j, m[i][j]);;

            if (m[i][j] > 250) {
                GrPlot(i, j, GrAllocColor(255, 0, 0));
            } else {
            GrPlot(i, j, GrAllocColor(m[i][j], m[i][j], m[i][j]));
            }
        }

    }

    for (i = 0; i < 3; i++) {
        GrBox(i, i, MAXVERTI-i, MAXHORIZ-i ,GrAllocColor(0, 0, 255));
    }

  GrSetContext(NULL);
  GrBitBlt(NULL,0,0,grc, 0,0,MAXVERTI-1,MAXHORIZ-1,GrIMAGE);
  GrDestroyContext(grc);

}

int main()
{

   GrSetMode(  GR_width_height_graphics, MAXVERTI, MAXHORIZ);
   GrSetRGBcolorMode();
   GrClearScreen( GrAllocColor(0,0,0) );

   int i,j;
   int prum, hodnota, pocet, min,max;
   FILE *soubor;
   char nazev[50];

   for(i=0; i<256; i++) paleta[i] = GrAllocColor(i,i,i);

   int **matice =(int **)malloc(sizeof(int *) * MAXVERTI) ;
   for(i=0; i<MAXVERTI; i++) matice[i]=(int *)malloc(sizeof(int) * MAXHORIZ );

   FILE *fin= fopen("panorama.txt", "r");
   if (fin==NULL) {printf("soubor nelze otevrit\n"); system("pause"); exit(-1); }

  // UKAZKA NACTENI MATICE (OBRAZKU)
   for(i=0; i< MAXVERTI; i++)
   {
        for(j=0; j< MAXHORIZ; j++)
        {
            fscanf(fin, "%i", &matice[i][j]);

        }

   }

   vykresli_mri(matice, MAXVERTI, MAXHORIZ) ;

   system("pause");
   return 0;

}
