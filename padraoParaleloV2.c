#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <float.h>
//#include "timer.h"

pthread_mutex_t mutex,mutexInsere,mutexIntegral,mutexTravadas,mutexQtdTarefas;
pthread_cond_t condPodeConsumir;

double a, b, erroMaximo, resultadoIntegral, integralIteracao;
int flagAcabou = 1 ,threadsTravadas = 0, tamanhoVetorTarefas = 100, qtdTarefas = 0, nthreads;

typedef struct 
{
    double a;
    double b;
    double area;
} tarefa ;

tarefa * vetorTarefas;


double calcula_funcao(double x){
    return 1+x;
}

void imprime_vetor_tarefa(){
    int i;
    printf("Vetor de tarefa:");
    for ( i = 0; i < qtdTarefas; ++i)
    {
        printf("[a=%f | b=%f | area=%f] ",vetorTarefas[i].a,vetorTarefas[i].b,vetorTarefas[i].area);
    }
    printf("\n");
}

void insere_tarefa(tarefa t){
    pthread_mutex_lock(&mutex);
    if(tamanhoVetorTarefas == qtdTarefas){
        tamanhoVetorTarefas = tamanhoVetorTarefas * 2;
        vetorTarefas = realloc(vetorTarefas, sizeof(tarefa) * tamanhoVetorTarefas);
    }
    vetorTarefas[qtdTarefas] = t;
    qtdTarefas++;
    pthread_cond_broadcast(&condPodeConsumir);
    pthread_mutex_unlock(&mutex);
}

tarefa retira_tarefa(){
    tarefa t;
    pthread_mutex_lock(&mutexInsere);
    t = vetorTarefas[qtdTarefas - 1];
    qtdTarefas--;
    pthread_mutex_unlock(&mutexInsere);
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

void inicia_tarefa(double a, double b, double area){
    double intervalo = b-a;
    tarefa t1, t2;
    double umQuartoDoIntervalo = intervalo/4;
    double tresQuartosDoIntervalo = 3*(intervalo/4);
    double areaEsquerda, areaDireita;

    areaEsquerda = intervalo/2 * calcula_funcao( a + umQuartoDoIntervalo);
    areaDireita = intervalo/2 * calcula_funcao(a + tresQuartosDoIntervalo);

    t1.a = a;
    t1.b = (intervalo/2) + a;
    t1.area = areaEsquerda;

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


int verifica_thread_travadas(){
    pthread_mutex_lock(&mutexTravadas);
    if(threadsTravadas == nthreads - 1){
        pthread_mutex_unlock(&mutexTravadas);
        return 1;
    }else{
        pthread_mutex_unlock(&mutexTravadas);
        return 0;
    }
    
}

int verifica_se_existem_tarefas(){
    pthread_mutex_lock(&mutexQtdTarefas);
    if(qtdTarefas > 0){
        pthread_mutex_unlock(&mutexQtdTarefas);
        return 1;
    }
    else{
        pthread_mutex_unlock(&mutexQtdTarefas);
        return 0;
    }
}

void * threads_integral (void* arg){
    // Descobre o pid da thread
    int* p = (int *) arg;
    int pid = * p;
    int i;
    tarefa t1, t2;

    double erroIteracaoAtual = DBL_MAX, integralLocal, integralEsquerda = 0, integralDireita = 0, intervalo;

    printf("-- Thread %d criada!\n", pid);

    // Enquanto o erro da iteração for maior que o erro máximo, calcula a integral mais uma vez
    while(flagAcabou){
        printf("Thread %d entrou no while \n", pid);
        printf("Thread %d lockou pra ler o n de tarefas \n", pid);
        if(verifica_se_existem_tarefas()){
            printf("Thread %d achou %d tarefas \n", pid ,qtdTarefas);    
            t1 = retira_tarefa();
            printf("Thread %d , T1 safado: %f \n", pid, t1.b );
            while(erroIteracaoAtual > erroMaximo){
                printf("---- Thread %d entrou pra calcular\n", pid);
                integralEsquerda = 0;
                integralDireita = 0;
                integralLocal = 0;
                
                intervalo = (t1.b - t1.a);
                printf("--- Thread %d intervalo da vez: [%f-%f] \n", pid,t1.a, t1.b );
                integralEsquerda = calcula_integral(t1.a, intervalo + t1.a);
                integralDireita = calcula_integral(intervalo + t1.a , t1.b);
                integralLocal = integralDireita + integralEsquerda;
                erroIteracaoAtual = t1.area - integralLocal;
                printf("---- Thread %d  integralLocal:%f erroIteracaoAtual: %f\n",pid,integralLocal,erroIteracaoAtual );
                if(erroIteracaoAtual < erroMaximo){
                    printf("Thread %d entrou em erroIteracaoAtual %f < erroMaximo\n", pid, erroIteracaoAtual);
                    pthread_mutex_lock(&mutexIntegral);
                    resultadoIntegral += integralLocal;
                    printf("Resultado integral é: %f\n", resultadoIntegral);
                    erroIteracaoAtual = DBL_MAX;
                    pthread_mutex_unlock(&mutexIntegral);
                    break;
                }else{
                    printf("Thread %d. Erro não é bom ainda %f . Divide MAIS SAPORRA\n", pid, erroIteracaoAtual);
                    t1 = cria_tarefa(t1.a, intervalo + t1.a, integralEsquerda);
                    t2 = cria_tarefa(intervalo + t1.a , t1.b, integralDireita);
                    insere_tarefa(t2);
                }
            }
        }else if(verifica_thread_travadas()){
            printf("Thread %d sou a última, seto flagAcabou\n", pid);
            flagAcabou = 0;
            pthread_cond_broadcast(&condPodeConsumir);
            printf("Thread %d Sinalizei meu bonde\n",pid);
            break;
        }else{
            printf("Thread %d não pegou nenhuma tarefa, vai se travar\n", pid );
            pthread_mutex_lock(&mutexTravadas);
            threadsTravadas++;
            printf("Thread %d , somos o bonde das threads travadas n:%d \n",pid,threadsTravadas );
            pthread_cond_wait(&condPodeConsumir, &mutexTravadas);
            printf("Thread %d fui sinalizada\n", pid);
        }
    }
    printf("Sai Thread:%d\n" ,pid);
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

    pthread_cond_init(&condPodeConsumir,NULL);

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
    inicia_tarefa(a, b, integralInicial);

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
