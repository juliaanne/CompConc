#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <float.h>
//#include "timer.h"

pthread_mutex_t mutex,mutexIntegral;
pthread_cond_t cond_barreira,condPodeConsumir;

double a, b, erroMaximo, resultadoIntegral, integralIteracao, erroIteracaoAtual = DBL_MAX, tamanhoParticao;
int tamanhoVetorTarefas = 100, qtdTarefas = 0, nthreads, nparticoes, nParticoesParaCalcular = 2, nParticoesPorThread, threadsExecutaram = 0, nparticoesIteracao = 1, out;

typedef struct 
{
    double a;
    double b;
    double area;
} tarefa ;

tarefa * vetorTarefa;


double calcula_funcao(double x){
    return 1+x;
}

void insere_tarefa(tarefa t){
    pthread_mutex_lock(&mutex);
    if(tamanhoVetorTarefas == qtdTarefas){
        tamanhoVetorTarefas = tamanhoVetorTarefas * 2;
        realloc(vetorTarefas, sizeof(tarefa) * tamanhoVetorTarefas)
    }
    vetorTarefas[qtdTarefas] = t;
    qtdTarefas++;
    pthread_cond_broadcast(&condPodeConsumir);
    pthread_mutex_unlock(&mutex);
}

tarefa * retira_tarefa(){
    tarefa *t;
    t = (tarefa *) malloc(sizeof(tarefa));
    pthread_mutex_lock(&mutex);
    if(qtdTarefas>0){
        t = &vetorTarefas[qtdTarefas];
        qtdTarefas--;
    } else {
        return NULL;
    }
    pthread_mutex_unlock(&mutex);
    return t;
}

tarefa cria_tarefa(double a, double b, double area){
    tarefa t1;
    double intervalo = b-a;

    t1.a = a;
    t1.b = (intervalo/2) + a;
    t1.area = area;

    return t1;
}

void init_tarefa(double a, double b, double area){
    double intervalo = b-a;
    tarefa t1, t2;
    double umQuartoDoIntervalo = intervalo/4;
    double tresQuartosDoIntervalo = 3*(intervalo/4);

    areaEsqueda = intervalo/2 * calcula_funcao( a + umQuartoDoIntervalo);
    areaDireita = intervalo/2 * calcula_funcao(a + tresQuartosDoIntervalo);

    t1.a = a;
    t1.b = (intervalo/2) + a;
    t1.area = areaEsqueda;

    t2.a = (intervalo/2) + a;
    t2.b = b;
    t2.area = areaDireita;

    insere_tarefa(t1);
    insere_tarefa(t2);
}

// Calculo da integral
double calcula_integral(double localA, double localB){
    double integral=0;
    double base = localB - localA;
    double altura = calcula_funcao((base/2) + localA);

    integral = base * altura;
    return integral;
}

// // Função auxiliar de debug
// void imprime_vetor_double(double * vetor, int tamanho){
//     int i;
//     printf("Vetor: ");
//     for (i = 0; i < tamanho; i++)
//     {
//         printf("[%f]",vetor[i] );
//     }
//     printf("\n");
// }
// // Função auxiliar de debug
// void imprime_vetor_int(int * vetor, int tamanho){
//     int i;
//     printf("Vetor: ");
//     for (i = 0; i < tamanho; i++)
//     {
//         printf("[%d]",vetor[i] );
//     }
//     printf("\n");
// }

// // Calcula a inicio do intervalo de integraçao de uma partição
// double calcula_inicio_intervalo(int particao){
//     double inicio = 0;

//     inicio = a + (particao * tamanhoParticao);

//     return inicio;
// }

// // Calcula o fim do intervalo de integraçao de uma partição
// double calcula_fim_intervalo(int particao){
//     double fim = 0;

//     fim = a + ((particao + 1) * tamanhoParticao);

//     return fim;
// }

// // Função barreira para aguardar todas as threads terminarem e a última fazer os ajustes para a próxima iteração
// void barreira(int pid) {
//     int i;
//     pthread_mutex_lock(&mutex);
//     threadsExecutaram++;
//     if(threadsExecutaram < nthreads){
//         printf("Thread %d se travou esperando as outras acabarem de calcular integral\n", pid);
//         pthread_cond_wait(&cond_barreira, &mutex);
//     }else{
//         // Zera os valores da última iteração
//         threadsExecutaram=0;
//         integralIteracao = 0;

//         // Calcula o valor da integral atual percorrendo o vetor de integrais parciais
//         for (i = 0; i < nthreads; i++)
//         {
//             integralIteracao += vetorIntegral[i];
//         }


//         imprime_vetor_double(vetorIntegral, nthreads);
//         printf("---------- Integral Iteracao: %f \n", integralIteracao);

//         // Calcula o erro na iteração atual
//         erroIteracaoAtual = fabs(resultadoIntegral - integralIteracao);

//         // Atualiza a integral final com o valor da interação atual.
//         resultadoIntegral = integralIteracao;

//         // Dobra o número de partições para a próxima iteração
//         nparticoes = 2 * nparticoes;

//         // Atualiza o valor do tamanho de cada intervalo de integração
//         tamanhoParticao = (b - a)/nparticoes;

//         // Atualiza o vetor de partições com os intervalos finais de cada partição
//         preenche_vetor_particoes();

//         printf("---------- Erro Iteracao: %f \n", erroIteracaoAtual );
//         printf("Thread %d liberou todas as outras threads\n", pid);

//             pthread_cond_broadcast(&cond_barreira);
//     }
//     pthread_mutex_unlock(&mutex);
// }

void * threads_integral (void* arg){
    // Descobre o pid da thread
    int* p = (int *) arg;
    int pid = * p;
    tarefa* t;

    int i;
    double erroIteracaoAtual = 0, integralLocal, integralEsquerda = 0, integralDireita = 0, intervalo;

    printf("-- Thread %d criada!\n", pid);

    // Enquanto o erro da iteração for maior que o erro máximo, calcula a integral mais uma vez
    pthread_mutex_lock(&mutex);
    while(qtdTarefas > 0){
        pthread_mutex_unlock(&mutex);
        t1 = retira_tarefa();
        
        while(erroIteracaoAtual < erroMaximo){
            integralEsquerda = 0;
            integralDireita = 0;
            integralLocal = 0;
                
            if (t1 != NULL){
                intervalo = (t1->b - t1->a);
                integralEsquerda = calcula_integral(t1->a, intervalo + t1->a);
                integralDireita = calcula_integral(intervalo + t1->a , t1->b);
                integralLocal = integralDireita + integralEsquerda;
                erroIteracaoAtual = t1->area - integralLocal;
                if(erroIteracaoAtual < erroMaximo){
                    pthread_mutex_lock(&mutexIntegral);
                    resultadoIntegral += integralLocal;
                    pthread_mutex_unlock(&mutexIntegral);
                }else{
                    t1 = cria_tarefa(t1->a, intervalo + t1->a, integralEsquerda);
                    t2 = cria_tarefa(intervalo + t1->a , t1->b, integralDireita);
                    insere_tarefa(t2);
                }
            } //ToDO: TRATAR
        }
    }

    pthread_exit(NULL);
}


// Verifica se o número de argumentos da entrada está correto.
void valida_entrada(int argc, char *argv[]){
    if(argc<5){
        printf("Use %s <limite inferior> <limite superior> <erro máximo> <#threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}

// Verifica se o número de threads está correto.
void valida_threads(int nthreads){
    if(nthreads <1 || nthreads>8){
        printf("Numero de threads deve ser entre 1 e 8\n");
        exit(EXIT_FAILURE);
    }
}


int main(int argc, char *argv[]){
    //double inicio, fim, tempoInicializacao, tempoExecucao;
    pthread_t* threads;
    int i;
    int* pid;
    double integralInicial;

    //---------------------------------------------------------------- Parte 1: Inicialização
    //GET_TIME(inicio);

    // Validando a entrada
    valida_entrada(argc,argv);

    // Recebendo os argumentos da entrada.
    a = atof(argv[1]);
    b = atof(argv[2]);
    erroMaximo = atof(argv[3]);
    nthreads = atoi(argv[4]);

    // Validando numero de threads
    valida_threads(nthreads);

    // Alocando espaço para o vetor de threads
    threads = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if(threads==NULL) {
        printf("--ERRO: malloc() em vetor de threads\n"); exit(-1);
    }

    // Alocando espaço para o vetor de tarefas
    vetorTarefas = (tarefa *) malloc(sizeof(tarefa) * tamanhoVetorTarefas);
    if(vetorTarefas == NULL){
        printf("--ERRO: malloc() em vetor de integrais\n");
    }

    // Calcula o primeiro valor da integral
    integralInicial = calcula_integral(a,b);

    // Preenche o vetor de tarefas pela primeira vez
    init_tarefas(a, b, integralInicial);

    //GET_TIME(fim);
    //tempoInicializacao = fim - inicio;


    //---------------------------------------------------------------- Parte 2: Execucão
    //GET_TIME(inicio);

    // Criando as threads
    for (i= 0; i < nthreads; i++) {
        pid = malloc(sizeof(int));
        if(pid == NULL){
            printf("--ERRO: malloc() em alocação threads\n"); exit(-1);
        }

        //Coloca o valor de i em pid;
        *pid = i;

        // Criando threads
        if(pthread_create(&threads[i], NULL, threads_integral, (void *) pid)){
            printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }

    // Espera as threads terminarem
    for (i=0; i< nthreads ; i++) {
        if (pthread_join(threads[i], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1);
        }
    }

    //GET_TIME(fim);
    //tempoExecucao = fim - inicio;

    //printf("-- FUNÇÃO ELEFANTE PARALELA --\n");
    //printf("Número de threads é: %d\n", nthreads);
    printf("Resultado integral de elefante é: %.8lf\n\n", resultadoIntegral);
    //printf("Tempo de inicialização é: %.8lf\n", tempoInicializacao);
    //printf("Tempo de execução é: %.8lf\n", tempoExecucao);
    //printf("Tempo total é: %.8lf\n", tempoExecucao+tempoInicializacao);
    //printf("\n\n");

    return 0;
}
