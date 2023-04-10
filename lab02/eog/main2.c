
#include <string.h>
#include "grx/include/grx20.h"
#include "grx/include/grxkeys.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "tools.h"


int main()
{
  double  mx, mn;
  int NSAMP, i;

  NSAMP=spocitej_delku("eog.txt");
  printf("\n%i\n", NSAMP);
  double *data = alokuj_vektor(NSAMP);
  nacti_vektor(data, NSAMP, "eog.txt");

  double *cor = alokuj_vektor(128);
  FILE *ofi = fopen ("eogcor.txt", "r"); if(ofi==NULL) {printf("err open file\n"); exit(-1); }
  int j;
  for (j=0; j<128; j++) fscanf(ofi, "%lf",&cor[j]);
  fclose (ofi);
  double *data_out = alokuj_vektor(NSAMP);
  korelace ( data, data_out, NSAMP, cor, 128);

  GrSetMode(  GR_width_height_graphics, 1500, 400 );
  GrClearScreen( GrAllocColor(0,0,0) );
  minmax(data, NSAMP, &mn, &mx);
  zobraz_data (data, 0, NSAMP-1, mn, mx, GrAllocColor(0,255,0));
  minmax(data_out, NSAMP, &mn, &mx);
  zobraz_data (data_out, 11/2, NSAMP-1-11/2, mn, mx, GrAllocColor(0,0,250));

  printf("\nstisknente lib. klavesu v okne s grafikou ...\n");
  GrKeyRead();

  return 0;
}

