#include <string.h>
#include "grx/include/grx20.h"
#include "grx/include/grxkeys.h"
#include <stdio.h>
#define MAXN 20

float minq1 (int N, double *X, double *Y, double *k, double *q)
{
  double x = 0.0, xx = 0.0, y = 0.0, xy = 0.0, rez = 0.0;
  int i;

  // zde implementujte linearni aproximaci metody nejmensich ctvercu
  for (i = 0; i < N; i++) {
    x += X[i];
    xx += X[i]*X[i];
    xy += X[i]*Y[i];
    y += Y[i];
  }

  *k = (N*xy - x*y) / (N*xx - x*x);
  *q = (xx*y - xy*x) / (N*xx - x*x);

  double sum = 0;
  for (i = 0; i < N; i++) {
    sum += pow((Y[i] - (*k*X[i] + *q)), 2);
  }

  rez = sqrt(sum/(N-1));

  return rez;
}



int main()
{
  char buf[100];
  GrTextOption grt;
  double X[MAXN], Y[MAXN], k, q, u ;
  int N, i;


  N=5;
  X[0]=20.5; Y[0]=765.0;
  X[1]=32.7; Y[1]=826.0;
  X[2]=51.0; Y[2]=873.0;
  X[3]=73.2; Y[3]=942.0;
  X[4]=95.7; Y[4]=1032.0;

  GrSetMode(  GR_width_height_graphics, 800, 600 );  //GrSetMode(  GR_biggest_graphics, 700, 500 );
  GrClearScreen( GrAllocColor(255, 255, 255) );

  for(i=0; i<N; i++)
    GrCircle(X[i]*7.0, 600.0-(Y[i]-600.0)*1.2, 5, GrAllocColor(0,0,255) );

  double presnost = minq1( N, X, Y, &k, &q);
  printf("rez. rozptyl: %lf", presnost);

  for(u=0; u<120; u++)
    GrCircle(u*7.0, 600.0-((k*u+q)-600.0)*1.2, 2, GrAllocColor(255,0,0) );


  grt.txo_font = &GrDefaultFont;
  grt.txo_fgcolor.v = GrAllocColor(2,2,200);
  grt.txo_bgcolor.v = GrWhite();
  grt.txo_direct = GR_TEXT_RIGHT;
  grt.txo_xalign = GR_ALIGN_CENTER;
  grt.txo_yalign = GR_ALIGN_CENTER;
  grt.txo_chrtype = GR_BYTE_TEXT;
  sprintf(buf,"Aproximace MINQ 1");
  GrDrawString( buf, strlen( buf ), GrMaxX()/2, 20 , &grt );
  sprintf(buf,"k= %4.4f   q= %4.4f", k, q );
  GrDrawString( buf, strlen( buf ), GrMaxX()/2+200, 550 , &grt );

  printf("\nstisknente lib. klavesu v okne s grafikou ...\n");
  GrKeyRead();
  //getch();

  return 0;
}

