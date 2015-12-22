#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
//#include "timer.h"
#include <math.h>

double valorReal = 0.804776; // de 0 a 2

double calcula_funcao(double x){
  return sin(x*x);
}

double calcula_integral(double a, double b, double erroMaximo, int nparticoes){
  int i;
  double integral=0;
  double intervalo= ((b-a)/nparticoes);
  double aDavez = a;
  for(i=0; i<nparticoes; i++){
    integral += intervalo * calcula_funcao((aDavez + (aDavez+intervalo))/2);
    aDavez+=intervalo;
  }
  printf("Integral : %f \n ",integral);
  sleep(1);
  double erro = fabs(valorReal-integral);

  if(erro >erroMaximo){
    integral = calcula_integral(a, b, erroMaximo, nparticoes*2);
  }
        
  return integral;
}
  

int main(int argc, char *argv[]){
  double a, b, erroMaximo,valor;
  
  //Validando os argumentos da entrada.
  if(argc<4){
    printf("Use %s <limite inferior> <limite superior> <erro máximo>", argv[0]);
    exit(EXIT_FAILURE);
  }
  
  //Recebendo os argumentos da entrada.
  a = atof(argv[1]);
  b = atof(argv[2]);
  erroMaximo = atof(argv[3]);

  valor = calcula_integral(a, b, erroMaximo, 1);
  
  printf("Integral %f \n ", valor); 
  return 0;
}