#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <float.h>

typedef struct {
    double a;
    double b;
    double area;
} TAREFA;

double a, b, erroMaximo, resultadoIntegral;
int nthreads, tamanhoVetorTarefas = 100, numTarefas, controladora = 0, threadsTravadas=0;
pthread_cond_t condQueroProcessar;
pthread_mutex_t mutexIntegral, mutexRetira, mutex;
TAREFA *vetorTarefas;


double calcula_funcao(double x){
    //return 1+x;
    //return cos(pow(M_E,-x)) * ((0.005 *(pow(x,3))) +1);
    return sqrt(1+(pow(x,4)));
    //return sin(x*x);
}

void imprime_vetor_tarefa(){
    int i;
    printf("VetorTarefas: \n");
    if(numTarefas==0){
        printf("VAZIO\n");
    } else {
        for (i = 0; i<numTarefas; ++i){
            printf("[ a=%f, b=%f | area=%f ] \n", vetorTarefas[i].a, vetorTarefas[i].b, vetorTarefas[i].area);
        }
        printf("\n");
    }
}

TAREFA * realocacao(){
    TAREFA * auxiliar;
    tamanhoVetorTarefas = tamanhoVetorTarefas * 2;
    auxiliar = (TAREFA *) realloc(vetorTarefas, sizeof(TAREFA) * tamanhoVetorTarefas);
    return auxiliar;
}

void insere_vetor_tarefa(TAREFA tarefa){
    pthread_mutex_lock(&mutex);
    if(tamanhoVetorTarefas == numTarefas){
        vetorTarefas = realocacao();
    }
    vetorTarefas[numTarefas] = tarefa;
    numTarefas++;
    //imprime_vetor_tarefa();
    pthread_mutex_unlock(&mutex);
}

TAREFA retira_vetor_tarefa(){
    TAREFA t;
    pthread_mutex_lock(&mutexRetira);
    t = vetorTarefas[numTarefas - 1];
    numTarefas--;
    pthread_mutex_unlock(&mutexRetira);
    return t;
}

void inicia_vetor_tarefa(double a, double b){
    TAREFA tEsquerda, tDireita;
    double intervalo = b-a;
    double pontoMedio = (intervalo/2)+a;
    
    double umQuartoDoIntervalo = intervalo/4;
    double tresQuartosDoIntervalo = 3*(intervalo/4);
    double areaEsquerda, areaDireita;

    areaEsquerda = intervalo/2 * calcula_funcao(a + umQuartoDoIntervalo);
    areaDireita = intervalo/2 * calcula_funcao(a + tresQuartosDoIntervalo);

    tEsquerda.a = a;
    tEsquerda.b = pontoMedio;
    tEsquerda.area = areaEsquerda;

    tDireita.a = pontoMedio;
    tDireita.b = b;
    tDireita.area = areaDireita;

    insere_vetor_tarefa(tEsquerda);
    insere_vetor_tarefa(tDireita);
}

void * threads_integral (void* arg){
    int pid = * (int *) arg;
    printf("Criada thread id: %d\n", pid);

    double intervalo, umQuartoDoIntervalo, tresQuartosDoIntervalo, areaDireita, areaEsquerda, areaTotal, erroIteracaoAtual, pontoMedio;

    while(1){
        TAREFA t;
        intervalo=0; umQuartoDoIntervalo=0; tresQuartosDoIntervalo=0; areaDireita=0; areaEsquerda=0; areaTotal=0; erroIteracaoAtual=0; pontoMedio=0;

        pthread_mutex_lock(&mutex);
        if(numTarefas ==0 && controladora){
            printf("Eu, thread %d, vou pegar uma tarefa das %d tarefas!\n", pid, numTarefas);
            threadsTravadas++;
            if(threadsTravadas == nthreads){
                printf("Eu, thread %d, sou a ultima thread e há %d tarefas. Vou sinalizar!\n", pid, numTarefas);
                controladora = 0;
                pthread_cond_broadcast(&condQueroProcessar);
                pthread_mutex_unlock(&mutex);
                pthread_exit(NULL);
            }
            printf("Eu, thread %d, vou me bloquear.", pid);
            pthread_cond_wait(&condQueroProcessar, &mutex);
        }

        if(controladora==0) break;

        //threadsTravadas--;
        pthread_mutex_unlock(&mutex);

        t = retira_vetor_tarefa();
        intervalo = t.b-t.a;
        //printf("Eu, thread %d, calculei intervalo %f\n", pid, intervalo);
        
        pontoMedio = (intervalo/2)+t.a;
        //printf("Eu, thread %d, calculei ponto médio: %f\n", pid, pontoMedio);
        
        umQuartoDoIntervalo = (intervalo/4) + t.a;
        //printf("Eu, thread %d, calculei umQuartoDoIntervalo: %f\n", pid, umQuartoDoIntervalo);
        
        tresQuartosDoIntervalo = 3*(intervalo/4) + t.a;
        //printf("Eu, thread %d, calculei tresQuartosDoIntervalo: %f\n", pid, tresQuartosDoIntervalo);
        
        areaEsquerda = intervalo/2 * calcula_funcao(umQuartoDoIntervalo);
        //printf("Eu, thread %d, calculei areaEsquerda: %f\n", pid, areaEsquerda);
        
        areaDireita = intervalo/2 * calcula_funcao(tresQuartosDoIntervalo);
        //printf("Eu, thread %d, calculei areaDireita: %f\n", pid, areaDireita);
        
        areaTotal = areaEsquerda + areaDireita;
        printf("Eu, thread %d, agora sei que areaTotal: %f\n", pid, areaTotal);
        
        //printf("Eu, thread %d, sei que a area do meu pai é: %f\n", pid, t.area);

        erroIteracaoAtual = fabs(t.area-areaTotal);
        printf("Eu, thread %d, calculei o erroIteracaoAtual: %f\n", pid, erroIteracaoAtual);


        if(erroIteracaoAtual>erroMaximo){
            TAREFA tEsquerda, tDireita;
            printf("Eu, a thread %d, sei que o erroIteracaoAtual %f  > erroMaximo %f. Vou dividir mais!\n", pid, erroIteracaoAtual, erroMaximo);
            
            tEsquerda.a = t.a;
            //printf("Eu, a thread %d, fiz a futura divisão: tEsquerda A é %f\n", pid, tEsquerda.a);
            
            tEsquerda.b = pontoMedio;
            //printf("Eu, a thread %d, fiz a futura divisão: tEsquerda B é %f\n", pid, tEsquerda.b);
            
            tEsquerda.area = areaEsquerda;
            //printf("Eu, a thread %d, fiz a futura divisão: tEsquerda area é %f\n", pid, tEsquerda.area);

            tDireita.a = pontoMedio;
            //printf("Eu, a thread %d, fiz a futura divisão: tDireita A é %f\n", pid, tDireita.a);
            
            tDireita.b = t.b;
            //printf("Eu, a thread %d, fiz a futura divisão: tEsquerda B é %f\n", pid, tDireita.b);
            
            tDireita.area = areaDireita;
            //printf("Eu, a thread %d, fiz a futura divisão: tEsquerda area é %f\n", pid, tDireita.area);

            printf("Eu, a thread %d, inseri tEsquerda e tDireita no vetorTarefas!\n", pid);
            insere_vetor_tarefa(tEsquerda);
            //pthread_cond_signal(&condQueroProcessar);
            insere_vetor_tarefa(tDireita);
            //pthread_cond_signal(&condQueroProcessar);
        } else {
            printf("Eu, a thread %d, sei que o erroIteracaoAtual %f está bom. Vou terminar.\n", pid, erroIteracaoAtual);
            
            pthread_mutex_lock(&mutexIntegral);
            
            resultadoIntegral += areaTotal;
            printf("Eu, a thread %d, antes de terminar, digo que resultadoIntegral é %f\n", pid, resultadoIntegral);
            
            pthread_mutex_unlock(&mutexIntegral);
        }


    }

    pthread_exit(NULL);
}



void valida_entrada(int argc, char *argv[]){
    if(argc<5){
        printf("Use %s <limite inferior> <limite superior> <erroIteracaoAtual máximo> <#threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}

void valida_threads(int nthreads){
    if(nthreads <1 || nthreads>8){
        printf("Numero de threads deve ser entre 1 e 8\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]){
    pthread_t *threads;
    int i;
    int *pid;

    valida_entrada(argc,argv);

    a = atof(argv[1]);
    b = atof(argv[2]);
    erroMaximo = atof(argv[3]);
    nthreads = atoi(argv[4]);

    valida_threads(nthreads);

    threads = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if(threads==NULL) {
        printf("--ERRO: malloc() em vetor de threads\n"); exit(-1);
    }

    vetorTarefas = (TAREFA *) malloc(sizeof(TAREFA) * tamanhoVetorTarefas);
    if(vetorTarefas == NULL){
        printf("--ERRO: malloc() em vetor de integrais\n");
    }

    inicia_vetor_tarefa(a, b);

    for (i= 0; i < nthreads; i++) {
        pid = malloc(sizeof(int));
        if(pid == NULL){
            printf("--ERRO: malloc() em alocação threads\n"); exit(-1);
        }
        *pid = i;
        if(pthread_create(&threads[i], NULL, threads_integral, (void *) pid)){
            printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }

    for (i=0; i< nthreads ; i++) {
        if (pthread_join(threads[i], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1);
        }
    }

    printf("**** O resultado final da integral é: %f\n", resultadoIntegral);

    free(vetorTarefas);    
    return 0;
}