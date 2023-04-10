
#include <string.h>
#include "grx/include/grx20.h"
#include "grx/include/grxkeys.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tools.h"

int main()
{
  double *data, *data_filt, mx, mn;
  int NSAMP, i;

  NSAMP=spocitej_delku("eog.txt");
  printf("\n%i\n", NSAMP);
  data = alokuj_vektor(NSAMP);
  nacti_vektor(data, NSAMP, "eog.txt");

  data_filt = alokuj_vektor(NSAMP);
  double kern[11] = { 0.045016, 0.075683, 0.104797, 0.128759, 0.144510, 0.0, 0.144510, 0.128759, 0.104797, 0.075683, 0.045016 };
  konvoluce ( data, data_filt, NSAMP, kern, 11);

  GrSetMode(  GR_width_height_graphics, 1500, 400 );
  GrClearScreen( GrAllocColor(0,0,0) );
  minmax(data, NSAMP, &mn, &mx);
  zobraz_data (data, 0, NSAMP-1, mn, mx, GrAllocColor(0,255,0));
  zobraz_data (data_filt, 11/2, NSAMP-1-11/2, mn, mx, GrAllocColor(0,0,250));

  printf("\nstisknente lib. klavesu v okne s grafikou ...\n");
  GrKeyRead();

  return 0;
}

