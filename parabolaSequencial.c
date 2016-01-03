#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "timer.h"
#include <math.h>

// Valor para comparação ao erro
double integralAnterior = 0;

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

    // Calculo do erro
    erroParcial = fabs(integralAnterior-integral);

    // Verificando se o erro desta iteração é aceitável
    // Cálculo recursivo caso não seja aceitável
    if(erroParcial>erroMaximo){
        integralAnterior =i ntegral;
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

    // Validando os limites de integração
    if(a < -1 || a > 1){
        printf("Limite inferior deve estar entre 1 e -1\n");
        exit(EXIT_FAILURE);
    }

    if(b < -1 || b > 1){
        printf("Limite superior deve estar entre 1 e -1\n");
        exit(EXIT_FAILURE);
    }

    GET_TIME(fim);
    tempoInicializacao = fim - inicio;


    //---------------------------------------------------------------- Parte 1: Execucão
    GET_TIME(inicio);

    // Chamando método da quadratura
    valor = calcula_integral(a, b, erroMaximo, 1);

    GET_TIME(fim);
    tempoExecucao = fim - inicio;

    // Saída do programa
    printf("O valor da integral de parabola de %f até %f é: %f \n", a, b, valor);
    printf("Tempo de inicialização é: %.8lf\n", tempoInicializacao);
    printf("Tempo de execução é: %.8lf\n", tempoExecucao);
    printf("Tempo total é: %.8lf\n", tempoExecucao+tempoInicializacao);

    return 0;
}