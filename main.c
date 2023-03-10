#include <string.h>
//#include <grx20.h>
//#include "grx20.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



/*
    Funkce dynamicky alokuje pamet pro vektor dat typu double.
    vstupni parametr:   "pocet" .... pocet prvku pole
    navratova hodnota:  ukazatel na alokovanou pamet
    chybovy stav: indikovan vypisem chybove hlasky
    na obrazovku a havarijnim ukoncenim programu.
*/
double *alokuj_vektor (unsigned long int pocet)
{
  double *p;

  p = (double*) malloc(sizeof(double) *pocet);
  if (p == NULL) {
    perror("malo pameti!");
    exit(-1);
  }

  return p ;
}


/*
  Otevre a projde soubor s nazvem "soubor" a vrati pocet prvku
  v nem obsazenych (v dohodnutem formatu - viz pruvodce blokem)
  V pripade ze se soubor nepodari otevrit,
  vypise se chybova hlaska a ukonci se program.

*/
unsigned long int zjisti_pocet_dat (char *soubor)
{

  unsigned long int cnt = 0;
  FILE *file;

  file = fopen(soubor, "r");
  if (file == NULL) {
    printf("\nnelze otevrit soubor ! koncim.\n");
    system("pause");
    exit(-2); 
  }

  for (char c = getc(file); c != EOF; c = getc(file)) {
    if (c == '\n') {
    cnt++;
    }
  }

  return cnt;
}

/*
   Nacte ze souboru s nazvem "soubor" vsechny [rvky v dohodnutem formatu
   do pole "vektor" s poctem prvku "delka" ,
   v pripade neuspechu vypise chybovou hlasku a havarijne ukonci program.
*/
void nacti_vektor (char *soubor, double *vektor, unsigned long int delka)
{
  FILE *file;

  file = fopen(soubor, "r");
  if (file == NULL) {
    printf("\nnelze otevrit soubor ! koncim.\n");
    system("pause");
    exit(-2); 
  }

  for (int i = 0; i <delka; i++) {
    fscanf(file, "%lf", &vektor[i]);
  }


}


/*
    Funkce vypise na obrazovku prvky pole "vektor", prvku je celkem "delka"

    vstupni parametry:      vektor .... alokovane pole
                            delka .... pocet prvku pole
    navratova hodnota:      neni
    chybovy stav: pri nulove nebo zaporne hodnote promenne "delka" se
    vypise chybobva hlaska a funkce se ukonci navratem.
*/
void tiskni_vektor (double *vektor, unsigned long int delka)
{

  for (int i = 0; i < delka; i++) {
    printf("%lf\n", vektor[i]);
  }

}


/* vrati aritmeticky prumer pole "vektor" s poctem prvku "delka" */
double prumer (double *vektor, unsigned long int delka)
{
  double mean;
  double sum = 0;
  int i;

  for (i = 0; i < delka; i++) {
    sum = sum + vektor[i];
  }

  mean = sum / delka;

  return mean;
}

/* vrati rozptyl pole "vektor" s poctem prvku "delka" */
double rozptyl (double *vektor, unsigned long int delka)
{

  double variance;
  double sum = 0;
  double mean = prumer(vektor, delka);

  for (int i = 0; i < delka; i++) {
    sum = sum + pow((mean - vektor[i]), 2);
  }

  variance = sum / delka;

  return 0;
}


/* vrati hodnotu nejvetsiho prvku pole "vektor" s poctem prvku "delka" */
double  maximum (double *vektor, unsigned long int delka)
{

  double max = 0;

  for (int i = 0; i < delka; i++) {
    if (vektor[i] > max) {
      max = vektor[i];
    }
  }

  return max;
}

/* vrati hodnotu nejmensiho pole "vektor" s poctem prvku "delka" */
double minimum (double *vektor, unsigned long int delka)
{

  double min = INFINITY;

  for (int i = 0; i < delka; i++) {
    if (vektor[i] < min) {
      min = vektor[i];
    }
  }

  return min;

}





int main()
{

  // Cilem je naucit se manipulovat s digitalizovanym 1D signalem v pameti pocitace,
  // nacitat ho z externiho souboru a provadet jednoduchou analyzu signalu v casove oblasti.
  // Doporuceny postup:
  // 1) zprovozntete si nacitani a pro kontrolu nasledny vypis dat pri staticke alokaci pameti (demo data EOG maji 885 vzorku)
  // 2) zobecnete reseni tak, aby byla pouzita dynamicka alokace pameti
  // 3) rozdelte program na podprogramy (fubnkce) s doporucenymi hlavickami a parametry
  // 4) overte funkcnost vysledneho reseni pomoci nize zakomentovaneho kodu
  // 5) z odladenych funkci vytvorte knihovnu (externi modul "nastroje.c" s hlavickovym souborem "nastroje.h"



// DEMO KOD SE STATICKOU ALOKACI PAMETI - NAKONEC SE NEPOUZIJE !
/*
  int i;
  FILE *fin;
  unsigned long int delka = 885;
  double data[885];

  fin=fopen("eog.txt","r");
  if (fin==NULL) { printf("\nnelze otevrit soubor ! koncim.\n"); system("pause"); exit(-2); }

  for(i=0; i<delka; i++) fscanf(fin, "%lf", &data[i]);

  for(i=0; i<delka; i++) printf("\n%lf", data[i]);

  return 0;
*/



  // tento kod bude pouzitelny po spravne implementaci vsech funkci:

  double *data;
  unsigned long int delka;

  delka = zjisti_pocet_dat ("eog.txt");

  printf("pocet vzorku v dane m soboru je: %lu\n", delka);

  data = alokuj_vektor (delka);
  nacti_vektor ( "eog.txt", data, delka);

  tiskni_vektor ( data, delka );

  printf("\nprumer casove rady je %lf\n", prumer(data, delka));
  printf("\nrozptyl casove rady je %lf\n", rozptyl(data, delka));
  printf("\nmax casove rady je %lf\n", maximum(data, delka));
  printf("\nmin casove rady je %lf\n", minimum(data, delka));

  free(data);


  // doplnkova uloha: zamyslete se nad tim, zda (pripadne jak)
  // by se funkce prumer, rozptyl, maximum a minimum daly sloucit do jedne funkce.

  return 0;


}
