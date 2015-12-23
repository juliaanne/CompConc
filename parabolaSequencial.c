#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "timer.h"
#include <math.h>

// Valor definido de -1 a 1
double valorReal = 1.5708;

// Função parabola
double calcula_funcao(double x){
    return sqrt(1-(x*x));
}

// Calculo da integral
double calcula_integral(double a, double b, double erroMaximo, int nparticoes){
    int i;
    double erroParcial, integral=0;
    double intervalo = ((b-a)/nparticoes);
    double limiteInferior = a;

    // Calculando o valor da integral
    for(i=0; i<nparticoes; i++){
        // Calculo do valor de cada npartição
        integral += intervalo * calcula_funcao((limiteInferior + (limiteInferior+intervalo))/2);
        limiteInferior+=intervalo;
    }

    //printf("Integral : %f \n ",integral);
    //sleep(1);
    erroParcial = fabs(valorReal-integral);

    // Verificando se o erro desta iteração é aceitável
    // Cálculo recursivo caso não seja aceitável
    if(erroParcial>erroMaximo){
        integral = calcula_integral(a, b, erroMaximo, nparticoes*2);
    }

    return integral;
}


int main(int argc, char *argv[]){
    double a, b, erroMaximo,valor;

    //Validando os argumentos da entrada.
    if(argc<4){
        printf("Use %s <limite inferior> <limite superior> <erro máximo> \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    //Recebendo os argumentos da entrada.
    a = atof(argv[1]);
    b = atof(argv[2]);
    erroMaximo = atof(argv[3]);

    // Chamando método da quadratura
    valor = calcula_integral(a, b, erroMaximo, 1);

    // Saída do programa
    printf("O valor da integral de seno de %f até %f é: %f \n", a, b, valor);

    return 0;
}