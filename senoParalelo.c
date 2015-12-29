#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "timer.h"
#include <math.h>
#include <float.h>

double a, b, erroMaximo, resultadoIntegral,resultadoParcial,tamanhoParticao;
double erroIteracaoAtual = DBL_MAX;

int nthreads;
int nParticoesIteracao = 1;
int threadsExecutadasNaIteracao = 0;
int flagFinaliza = 1;

pthread_mutex_t mutex;
pthread_cond_t cond,cond_controle;

typedef struct t{
    int pid;
    double a;
    double b;
    int particaoInicial;
    int particaoFinal;
    int particaoExtraInicial;
    int particaoExtraFinal;
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
    // Calcula quantas particoes a Thread vai pegar nessa iteração.
    int quantidadeParticoesPorThread = (int) floor(nParticoesIteracao/nthreads);

    // Se a existerem mais threads do que partições do intervalo de integração, ela não faz nada nessa iteração
    if(d->pid > nParticoesIteracao){
        d->particaoInicial = -1;
        d->particaoFinal = -1;
    }else if(d->pid == 1){ 
        // Se for a primeira thread, é dado a responsabilidade a parte, pois ela faz o começo das partição e um possível resto que possa existir
        // Ex. 128 partições, 5 Threads
        // Cada Thread vai fazer 25 partições, exceto a primeira, que vai fazer as 25, mais a 3 do final que sobram (126,127,128)
        d->particaoInicial = 1; 
        d->particaoFinal = quantidadeParticoesPorThread;        
        // Verifica se tem extra , se tiver atribui.
        int quantidadeParticaoExtras = nParticoesIteracao % nthreads;
        if(quantidadeParticaoExtras > 0){
            d->particaoExtraInicial = (nParticoesIteracao - quantidadeParticaoExtras) + 1;
            d->particaoExtraFinal = nParticoesIteracao;  
        }    
    }else{
        // Para todas as outras threads, aloca-se normalmente;
        d->particaoInicial = ((d->pid - 1) * quantidadeParticoesPorThread) + 1;
        d->particaoFinal = d->pid * quantidadeParticoesPorThread;
    }
    return;
}


// Calcula a inicio do intervalo de integraçao de uma partição
double calcula_inicio_intervalo(int particao){
    double inicio = 0;

    inicio = a + ((particao - 1) * tamanhoParticao);

    return inicio;
}

// Calcula o fim do intervalo de integraçao de uma partição
double calcula_fim_intervalo(int particao){
    double fim = 0;

    fim = a + (particao * tamanhoParticao);

    return fim;
}


// Thread de controle das outras threads
void * thread_controle(void * arg){
    printf("Criou a thread de [controle] \n");

    while(erroIteracaoAtual >  erroMaximo ){
        // Calcula o tamanho da partição no iteração atual
        tamanhoParticao = ((a-b)/nParticoesIteracao);

        // Libera as outras threads para calcularem a integral na iteração atual
        printf("Libera as threads para calcula_integral\n");
        pthread_cond_broadcast(&cond);        

        // Aguarda elas sinalizarem que terminaram de calcular
        printf("Thread de [controle] aguardando as threasd de integral sinalizarem\n");
        pthread_cond_wait(&cond_controle,&mutex);

        // Atualiza o valor do erro com base no valor da nova integral
        erroIteracaoAtual = resultadoIntegral - resultadoParcial;

        // Atualiza o valor da integral com o novo valor.
        resultadoIntegral = resultadoParcial;

        // Duplica o número de partições para a próxima iteração.
        nParticoesIteracao = 2 * nParticoesIteracao;                
    }


    free(arg);
    pthread_exit(NULL);
}


// Threads que calculam a integral
void * threads_integral(void * arg){
    threadInfo* dados = (threadInfo*) arg;

    printf("Criou a thread de [integral] [%d]\n", dados->pid);
    int i;
    double integralLocal = 0;
    double localA;
    double localB;

    

    while(flagFinaliza){
        
        // Aguarda a thread de controle sinalizar que devem começar outra iteração.
        printf("Thread %d se travou esperando liberação da thread de controle\n", dados->pid);
        pthread_cond_wait(&cond,&mutex);
        integralLocal = 0; 
        
        printf("Thread [%d] entrou no loop while\n", dados->pid );

        //Nesta função as partições que a thread tem que calcular são atualizadas.
        separa_responsabilidade(dados);

        // Verifica se a thread tem que calcular a integral nesta iteração
        if(dados->particaoInicial < 0){
            // Calcula o valor da integral só para o intervalo responsável da thread nesta iteração
            for(i = dados->particaoInicial ; i <= dados->particaoFinal; i++){ 
                localA = calcula_inicio_intervalo(i);
                localB = calcula_fim_intervalo(i);
                integralLocal += calcula_integral(localA,localB);
            }

            // Caso a thead seja responsável por calcular a parte extra
            if(dados->particaoExtraInicial > 0 ){
                for(i = dados->particaoExtraInicial ; i <= dados->particaoExtraFinal; i++){ 
                    localA = calcula_inicio_intervalo(i);
                    localB = calcula_fim_intervalo(i);
                    integralLocal += calcula_integral(localA,localB);
                }            
            }
        }
        pthread_mutex_lock(&mutex);    
        //Atualiza o resultado parcial com a parte que foi calculada por essa thread
        resultadoParcial += integralLocal;  

        // Atualiza o contador de threads da iteração atual
        threadsExecutadasNaIteracao++;
        pthread_mutex_unlock(&mutex);

        if(threadsExecutadasNaIteracao == nParticoesIteracao){
            pthread_cond_signal(&cond_controle);
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


    // Alocando argumentos para threads que calculam a integral
    for(i=1; i<=nthreads; i++){
        dados = (threadInfo*) malloc(sizeof(threadInfo));
        if(dados == NULL){
            printf("--ERRO: malloc() em argumentos threads\n"); exit(-1);
        }
        dados->pid = i;
        dados->a = a;
        dados->b = b;
        dados->particaoExtraInicial = -1;
        dados->particaoExtraFinal = -1;

        // Criando threads
        if(pthread_create(&threads[i], NULL, threads_integral, (void *) dados)){
            printf("--ERRO: pthread_create()\n"); exit(-1);
        }
 
    }

    //Cria a primeira thread separadamente, pois é a thread de controle
    pid = malloc(sizeof(int));
        if(pid == NULL){
            printf("--ERRO: malloc() em thread controle\n"); exit(-1);
    }
    *pid = 0;
    if(pthread_create(&threads[0], NULL, thread_controle, (void *) pid)){
        printf("--ERRO: pthread_create() na thread de controle\n"); exit(-1);
    }

    

    // Espera as threads terminarem
    for (i=0; i<nthreads + 1; i++) {
        if (pthread_join(threads[i], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1);
        }
    }

    // Saída do programa
    printf("O valor da integral de seno de %f até %f é: %f \n", a, b, resultadoIntegral);

    return 0;
}