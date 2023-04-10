#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NSAMP 27
int vysky[NSAMP];

int main()
{
  int i;
  FILE *fin;


  fin=fopen("vysky.txt","r");
  if (fin==NULL) { printf("\nnelze otevrit soubor ! koncim.\n"); system("pause"); exit(-2); }

  for(i=0; i<NSAMP; i++) fscanf(fin, "%i", &vysky[i]);
  fclose(fin);

  for(i=0; i<NSAMP; i++) printf("\n%i", vysky[i]);


  return 0;
}
