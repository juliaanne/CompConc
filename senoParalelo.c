#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "timer.h"
#include <math.h>

int nthreads;
double a, b, erroMaximo, nparticoes=1, resultadoIntegral;

pthread_mutex_t mutex;

// Valor definido de 0 a 2
double valorReal = 0.804776;

// Função seno
double calcula_funcao(double x){
    return sin(x*x);
}

// Calculo da integral
double calcula_integral(double localA, double localB, double nLocalParticoes){
    int i;
    double integral=0;
    double intervalo = ((localB-localA)/nLocalParticoes);
    double limiteInferior = localA;

    // Calculando o valor da integral
    for(i=0; i<nparticoes; i++){
        // Calculo do valor de cada npartição
        integral += intervalo * calcula_funcao((limiteInferior + (limiteInferior+intervalo))/2);
        limiteInferior+=intervalo;
    }
    return integral;
}

// Controle das threads
void * threads_integral(void * args){
    int pid = * (int *) args;
    double erroParcial, integralParcial=0;
    double localA, localB, nLocalParticoes;


    while(1){
        localA = a



    }
    integralParcial = calcula_integral(localA, localB, nLocalParticoes);
    erroParcial = fabs(valorReal-integral);

    if(erroParcial>erroMaximo){
        // Calcular valores dos limites

        integralParcial = calcula_integral(localA, localB, nLocalParticoes);

        pthread_mutex_lock(&mutex);
        resultadoIntegral += integralParcial;
        pthread_mutex_unlock(&mutex);

        integralParcial = calcula_integral(a, b, erroMaximo, nparticoes*2);
    }






    free(args);
    pthread_exit(NULL);
}


int main(int argc, char *argv[]){
    double a, b, erroMaximo;
    pthread_t* threads;
    int t;
    int *pid;

    // Validando os argumentos da entrada.
    if(argc<5){
        printf("Use %s <limite inferior> <limite superior> <erro máximo> <#threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Recebendo os argumentos da entrada.
    a = atof(argv[1]);
    b = atof(argv[2]);
    erroMaximo = atof(argv[3]);
    nthreads = atoi(argv[4]);

    // Alocando espaço para o vetor de threads.
    threads = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if(threads==NULL) {
        printf("--ERRO: malloc() em alocação threads\n"); exit(-1);
    }

    // Alocando argumentos para threads
    for(t=0; t<nthreads; t++){
        pid = malloc(sizeof(int));
        if(arg == NULL){
            printf("--ERRO: malloc() em argumentos threads\n"); exit(-1);
        }

        *pid=t;

        // Criando threads
        if(pthread_create(&threads[t], NULL, threads_integral, (void *) pid)){
            printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }

    // Espera as threads terminarem
    for (t=0; t<nthreads; t++) {
        if (pthread_join(threads[t], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1);
        }
    }

    // Saída do programa
    printf("O valor da integral de seno de %f até %f é: %f \n", a, b, integralFinal);

    return 0;
}