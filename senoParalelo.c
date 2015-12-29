#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "timer.h"
#include <math.h>
#include <float.h>

double a, b, erroMaximo, resultadoIntegral;
double erroDaIteracaoAtual = DBL_MAX;

int nthreads;
int nParticoesIteracao = 1;

pthread_mutex_t mutex;
pthread_cond_t cond;

typedef struct t{
    int pid;
    double a;
    double b;
    int particaoInicial;
    int particaoFinal;
    int particaoExtraInicial = -1;
    int particaoFinal = -1;
} threadInfo;

threadInfo* dados;

// Função seno
double calcula_funcao(double x){
    return sin(x*x);
}

// Calculo da integral
double calcula_integral(double localA, double localB){
    
    double integral=0;
    double base = localB - localA;
    double altura = calcula_funcao(base/2);

    integral = base * altura;
    return integral;
}


// Diz para a thread qual parte do intervalo de integração ela deve calcular
void separa_responsabilidade(threadInfo* d){
    int i = 0;

    // Calcula quantas particoes a Thread vai pegar nessa iteração.
    int quantidadeParticoesPorThread = (int) floor(nParticoesIteracao/nthreads);

    // Se a existerem mais threads do que partições do intervalo de integração, ela não faz nada nessa iteração
    if(d->pid > nParticoesIteracao){
        return -1;    
    }else if(d->pid == 1){ 
        // Se for a primeira thread, é dado a responsabilidade a parte, pois ela faz o começo das partição e um possível resto que possa existir
        // Ex. 128 partições, 5 Threads
        // Cada Thread vai fazer 25 partições, exceto a primeira, que vai fazer as 25, mais a 3 do final que sobram (126,127,128)
        d->particaoInicial = 1; 
        d->particaoFinal = qua1ntidadeParticoesPorThread;        
        // Verifica se tem extra , se tiver atribui.
        quantidadeParticaoExtras = nParticoesIteracao % nthreads;
        if(quantidadeParticaoExtras > 0){
            d->particaoExtraInicial = (nParticoesIteracao - quantidadeParticaoExtras) + 1;
            d->particaoExtraFinal = nParticoesIteracao;  
        }    
    }else{
        // Para todas as outras threads, aloca-se normalmente;
        d->particaoInicial = ((d->pid - 1) * qua1ntidadeParticoesPorThread) + 1;
        d->particaoFinal = d->pid * qua1ntidadeParticoesPorThread;
    }
}

void * thread_controle(void * arg){
    int * p = (int *) arg;
    int pid = * p;

    while(erroIteracaoAtual >  erroMaximo ){
        // Calcula o tamanho da partição no iteração atual
        tamanhoParticao = ((a-b)/nParticoesIteracao);

        // Libera as outras threads para calcularem a integral na iteração atual
        mutex_cond_broadcast(&cond);        

        // Aguarda elas sinalizarem que terminaram de calcular
        mutex_cond_wait(&cond);

        // Atualiza o valor do erro com base no valor da nova integral
        erroIteracaoAtual = resultadoIntegral - integralLocal;

        // Atualiza o valor da integral com o novo valor.
        resultadoIntegral = integralLocal;

        // Duplica o número de partições para a próxima iteração.
        nParticoesIteracao = 2 * nParticoesIteracao;                
    }

    free(arg);
    pthread_exit(NULL);
}



double calculo_inicio_intervalo(int particao){
    
}


// Threads que calculam a integral
void * threads_integral(void * arg){
    threadInfo* dados = (threadInfo*) arg;

    double integralLocal = 0;
    double tamanhoParticao;
    double localA;
    double localB;

    // Aguarda a thread de controle sinalizar que devem começar outra iteração.
    mutex_cond_wait(&cond); 

    while(erroIteracaoAtual >  erroMaximo ){
        
        printf("Thread [%d] entrou no loop while\n", pid );

        //Nesta função as partições que a thread tem que calcular são atualizadas.
        separa_responsabilidade(dados);

        // Calcula o valor da integral só para o intervalo responsável da thread nesta iteração
        for(i = intervaloInicio ; i <= intervaloFim; i++){ 
            localA = (tamanhoParticao * (intervaloInicio - 1)) + tamanhoParticao;
            localB = (tamanhoParticao * particaoId) + tamanhoParticao;               
            integralLocal += calcula_integral(localA,localB);
        }
        pthread_mutex_lock(&mutex);    
        resultadoIntegral += integralLocal;  
        quantasThreadsJaCalcularaNaIteracaoAtual++;
        pthread_mutex_unlock(&mutex);

        if(quantasThreadsJaCalcularaNaIteracaoAtual == nParticoesIteracao){
            erroDaIteracaoAtual = valorReal - resultadoIntegral;
            nParticoesIteracao = 2 * nParticoesIteracao;    
            if(erroDaIteracaoAtual > erroMaximo  ){
                resultadoIntegral = 0;
            }else{
                break;
            }
            pthread_cond_broadcast(&cond);
        }else{
            pthread_cond_wait(&cond,&mutex);
        }
        
    }

    free(arg);
    pthread_exit(NULL);
}


int main(int argc, char *argv[]){
    double a, b;
    pthread_t* threads;
    int i;
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

    // Calcula o primeiro valor da integral
    resultadoIntegral = calcula_integral(a,b);


    //Cria a primeira thread separadamente, pois é a thread de controle
    pid = malloc(sizeof(int));
        if(pid == NULL){
            printf("--ERRO: malloc() em thread controle\n"); exit(-1);
    }
    *pid = 0;
    if(pthread_create(&threads[0], NULL, threads_integral, (void *) pid)){
        printf("--ERRO: pthread_create() na thread de controle\n"); exit(-1);
    }

    // Alocando argumentos para threads restantes
    for(i=1; i<nthreads; i++){
        dados = (threadInfo*) malloc(sizeof(threadInfo));
        if(dados == NULL){
            printf("--ERRO: malloc() em argumentos threads\n"); exit(-1);
        }
        dados->pid = i;
        dados->a = a;
        dados->b = b;

        // Criando threads
        if(pthread_create(&threads[i], NULL, threads_integral, (void *) dados)){
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
    printf("O valor da integral de seno de %f até %f é: %f \n", a, b, resultadoIntegral);

    return 0;
}