#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "timer.h"
#include <math.h>


// Valor para comparação ao erro
double integralAnterior = 0;

// Função exponencial
double calcula_funcao(double x){
    return sqrt(1+(pow(x,4)));
}

// Calculo da integral
double calcula_integral(double a, double b, double erroMaximo, int nparticoes){
    int i;
    double erroParcial, integral=0;
    double intervalo = (b-a);
    double limiteInferior = a;

    integral = intervalo * calcula_funcao((limiteInferior + (limiteInferior + intervalo)) / 2);
    erroParcial = fabs(integralAnterior - integral);

    // Descomentar linhas abaixo para ver passo a passo
    //printf("Erro Parcial: %f\n", erroParcial );
    //printf("Integral Anterior: %f\n", integralAnterior);
    //printf("Integral Atual: %f\n\n", integral );

    while(erroParcial>erroMaximo) {
        // Atualizando valores
        integralAnterior = integral;
        integral=0;
        limiteInferior = a;
        nparticoes = nparticoes*2;

        // Calculando tamanho do intervalo
        intervalo = ((b-a)/nparticoes);

        // Calculando o valor da integral
        for (i = 0; i < nparticoes; i++) {
            // Calculo do valor de cada npartição
            integral += intervalo * calcula_funcao((limiteInferior + (limiteInferior + intervalo)) / 2);
            limiteInferior += intervalo;
        }

        // Calculo do erro
        erroParcial = fabs(integralAnterior - integral);

        // Descomentar linhas abaixo para ver passo a passo
        //printf("Erro Parcial: %f\n", erroParcial );
        //printf("Integral Anterior: %f\n", integralAnterior);
        //printf("Integral Atual: %f\n\n", integral );
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