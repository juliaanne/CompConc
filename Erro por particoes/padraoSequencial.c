#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
//#include "timer.h"
#include <math.h>
#include <float.h>

// Valor para comparação ao erro
double integralAnterior = DBL_MAX;

double erroMaximo;

// Função seno
double calcula_funcao(double x){
    //return 1+x;
    //return sin(x*x);
    return cos(pow(M_E,-x)) * ((0.005 *(pow(x,3))) +1);
}

// Calculo da integral
double calcula_integral(double a, double b, double integralAnterior){
    double erroParcial, integral=0;
    double intervalo = (b-a);
    double integralEsquerda, integralDireita;
    double umQuartoDoIntervalo = intervalo/4;
    double tresQuartosDoIntervalo = 3*(intervalo/4);

    integralEsquerda = intervalo/2 * calcula_funcao( a + umQuartoDoIntervalo);
    integralDireita = intervalo/2 * calcula_funcao(a + tresQuartosDoIntervalo);

    integral = integralDireita+integralEsquerda;

    // Calculo do erro
    erroParcial = fabs(integralAnterior-integral);

    printf("A: %f\n", a);
    printf("B: %f\n", b);
    printf("pmEsquerda: %f\n", a + umQuartoDoIntervalo);
    printf("pmDireita: %f\n", a + tresQuartosDoIntervalo);
    printf("integralEsquerda: %f\n", integralEsquerda);
    printf("integralDireita: %f\n", integralDireita);
    printf("erroParcial: %f\n\n\n", erroParcial);


    // Verificando se o erro desta iteração é aceitável
    // Cálculo recursivo caso não seja aceitável
    if(erroParcial>erroMaximo){
        integralEsquerda = calcula_integral(a, (intervalo/2)+a, integralEsquerda);
        integralDireita = calcula_integral((intervalo/2)+a, b, integralDireita);
    }

    return integralDireita+integralEsquerda;;
}
  

int main(int argc, char *argv[]){
    //double inicio, fim, tempoInicializacao, tempoExecucao;
    double a, b, valor;

    //---------------------------------------------------------------- Parte 1: Inicialização
    //GET_TIME(inicio);

    //Validando os argumentos da entrada.
    if(argc<4){
    printf("Use %s <limite inferior> <limite superior> <erro máximo> \n", argv[0]);
    exit(EXIT_FAILURE);
    }

    //Recebendo os argumentos da entrada.
    a = atof(argv[1]);
    b = atof(argv[2]);
    erroMaximo = atof(argv[3]);

    //GET_TIME(fim);
    //tempoInicializacao = fim - inicio;


    //---------------------------------------------------------------- Parte 1: Execucão
    //GET_TIME(inicio);

    // Chamando método da quadratura
    valor = calcula_integral(a, b, integralAnterior);

    //GET_TIME(fim);
    //tempoExecucao = fim - inicio;

    // Saída do programa
    //printf("-- FUNÇÃO SENO SEQUENCIAL --\n");
    printf("O valor da integral de seno de %.1lf até %.1lf é: %.8lf \n", a, b, valor);
    //printf("Tempo de inicialização é: %.8lf\n", tempoInicializacao);
    //printf("Tempo de execução é: %.8lf\n", tempoExecucao);
    //printf("Tempo total é: %.8lf\n", tempoExecucao+tempoInicializacao);
    //printf("\n\n");

    return 0;
}
