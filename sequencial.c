#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "timer.h"
#include <math.h>

int funcao;

// Valor para comparação ao erro
double integralAnterior = 0;

// Função linear
double calcula_funcao1(double x){
    return 1+x;
}
// Função parabola
double calcula_funcao2(double x){
    return sqrt(1-(x*x));
}
// Função exponencial
double calcula_funcao3(double x){
    return sqrt(1+(pow(x,4)));
}
// Função seno
double calcula_funcao4(double x){
    return sin(x*x);
}
// Função elefante
double calcula_funcao5(double x){
    return cos(pow(M_E,-x)) * ((0.005 *(pow(x,3))) +1);
}

// Calculo da integral
double calcula_integral(double a, double b, double erroMaximo, int nparticoes){
    int i;
    double erroParcial, integral=0;
    double intervalo = ((b-a)/nparticoes);
    double limiteInferior = a;

    switch(funcao){
        case 1:{
            // Calculando o valor da integral
            for(i=0; i<nparticoes; i++){
                // Calculo do valor de cada npartição
                integral += intervalo * calcula_funcao1((limiteInferior + (limiteInferior+intervalo))/2);
                limiteInferior+=intervalo;
            }        
            break;
        }
        case 2:{
            // Calculando o valor da integral
            for(i=0; i<nparticoes; i++){
                // Calculo do valor de cada npartição
                integral += intervalo * calcula_funcao2((limiteInferior + (limiteInferior+intervalo))/2);
                limiteInferior+=intervalo;
            }        
            break;
        }
        case 3:{
            // Calculando o valor da integral
            for(i=0; i<nparticoes; i++){
                // Calculo do valor de cada npartição
                integral += intervalo * calcula_funcao3((limiteInferior + (limiteInferior+intervalo))/2);
                limiteInferior+=intervalo;
            }        
            break;
        }
        case 4:{
            // Calculando o valor da integral
            for(i=0; i<nparticoes; i++){
                // Calculo do valor de cada npartição
                integral += intervalo * calcula_funcao4((limiteInferior + (limiteInferior+intervalo))/2);
                limiteInferior+=intervalo;
            }        
            break;
        }
        case 5:{
            // Calculando o valor da integral
            for(i=0; i<nparticoes; i++){
                // Calculo do valor de cada npartição
                integral += intervalo * calcula_funcao5((limiteInferior + (limiteInferior+intervalo))/2);
                limiteInferior+=intervalo;
            }        
            break;
        }

    }

    // Calculo do erro
    erroParcial = fabs(integralAnterior-integral);

    // Verificando se o erro desta iteração é aceitável
    // Cálculo recursivo caso não seja aceitável
    if(erroParcial>erroMaximo){
        integralAnterior = integral;
        integral = calcula_integral(a, b, erroMaximo, nparticoes*2);
    }

    return integral;
}


int main(int argc, char *argv[]){
    double inicio, fim, tempoInicializacao, tempoExecucao;
    double a, b, erroMaximo,valor;

    //---------------------------------------------------------------- Parte 1: Inicialização
    GET_TIME(inicio);

    //Validando os argumentos da entrada.
    if(argc<4){
        printf("Use %s <limite inferior> <limite superior> <erro máximo> \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    //Recebendo os argumentos da entrada.
    a = atof(argv[1]);
    b = atof(argv[2]);
    erroMaximo = atof(argv[3]);
    funcao = atoi(argv[4]);


    GET_TIME(fim);
    tempoInicializacao = fim - inicio;


    //---------------------------------------------------------------- Parte 1: Execucão
    GET_TIME(inicio);

    // Chamando método da quadratura
    valor = calcula_integral(a, b, erroMaximo, 1);

    GET_TIME(fim);
    tempoExecucao = fim - inicio;

    printf("%.8lf", tempoExecucao+tempoInicializacao);


    return 0;
}
