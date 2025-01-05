#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "simple_fft2.h"
#define MAX 200

double complex polar(double mag, double angle){
  return (mag * cos(angle) + I * mag * sin(angle));
}

int ilog2(int N)    /*function to calculate the ilog2(.) of int numbers*/
{
  int k = N, i = 0;
  while(k) {
    k >>= 1;
    i++;
  }
  return i - 1;
}

int reverse(int N, int n)    //calculating revers number
{
  int j, p = 0;
  for(j = 1; j <= ilog2(N); j++) {
    if(n & (1 << (ilog2(N) - j)))
      p |= 1 << (j - 1);
  }
  return p;
}

void ordina(double complex* f1, int N) //using the reverse order in the array
{
  double complex f2[MAX];
  for(int i = 0; i < N; i++)
    f2[i] = f1[reverse(N, i)];
  for(int j = 0; j < N; j++)
    f1[j] = f2[j];
}

void transform(double complex* f, int N) //
{
  ordina(f, N);    //first: reverse order
  double complex *W;
  W = (double complex *)malloc(N / 2 * sizeof(double complex));
  W[1] = polar(1.0, -2.0 * M_PI / N);
  W[0] = 1;
  for(int i = 2; i < N / 2; i++)
    W[i] = pow(W[1], i);
  int n = 1;
  int a = N / 2;
  for(int j = 0; j < ilog2(N); j++) {
    for(int i = 0; i < N; i++) {
      if(!(i & n)) {
        double complex temp = f[i];
        double complex Temp = W[(i * a) % (n * a)] * f[i + n];
        f[i] = temp + Temp;
        f[i + n] = temp - Temp;
      }
    }
    n *= 2;
    a = a / 2;
  }
  free(W);
}

void simple_fft2(double complex* f, int N, double d)
{
  transform(f, N);
  for(int i = 0; i < N; i++)
    f[i] *= d; //multiplying by step
}
