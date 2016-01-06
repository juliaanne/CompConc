#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <float.h>

typedef struct t{
    double a;
    double b;
    double area;
    struct t *proximo;
} TAREFA;

pthread_mutex_t mutexInsereVetor, mutexRetiraVetor, mutexIntegral;
int a, b, nthreads, erroMaximo, numTarefas;
TAREFA *vetorTarefas;

double calcula_funcao(double x){
    return sin(x*x);
}

double calcula_integral(double localA, double localB){
    double integral=0;
    double intervalo = localB - localA;
    double altura = calcula_funcao((intervalo/2) + localA);

    integral = intervalo * altura;
    return integral;
}

void imprime_vetor_tarefa(){
    printf("VetorTarefas: \n");
    TAREFA *temporaria = vetorTarefas->proximo;
    while(temporaria->proximo != NULL){
    	printf("[ a=%f, b=%f, c=%f]\n", temporaria->a, temporaria->b, temporaria->area);
    	temporaria = temporaria->proximo;
    }
    printf("\n");
}

void insere_vetor_tarefa(double a, double b, double area){
	pthread_mutex_lock(&mutexInsereVetor);
	TAREFA *tarefa = malloc(sizeof(TAREFA));
	tarefa->a=a;
	tarefa->b=b;
	tarefa->area=area;
	tarefa->proximo=NULL;

	printf("Tarefa-a: %f\n", tarefa->a);
	printf("Tarefa-b: %f\n", tarefa->b);
	printf("Tarefa-area: %f\n", tarefa->area);

	if(numTarefas == 0){
		printf("Vetor Vazio\n");
		if(vetorTarefas->proximo == NULL) printf("É nulo\n");
		vetorTarefas->proximo=tarefa;
	} else {
		TAREFA *temporaria = vetorTarefas->proximo;
		while(temporaria->proximo != NULL)
			temporaria = temporaria->proximo;
		temporaria->proximo = tarefa;
	}
	numTarefas++;
    imprime_vetor_tarefa();
	pthread_mutex_unlock(&mutexInsereVetor);
}

TAREFA *retira_tarefa(){
    pthread_mutex_lock(&mutexRetiraVetor);
    if(vetorTarefas->proximo==NULL){
    	return NULL;
    } else {
	    TAREFA *temporaria = vetorTarefas->proximo;
	    vetorTarefas->proximo = temporaria->proximo;
	    numTarefas--;
	    return temporaria;
	}
	pthread_mutex_unlock(&mutexRetiraVetor);
}

void inicia_vetor_tarefa(double a, double b){
	double intervalo = b-a;
	double pontoMedio = (intervalo/2)+a;
    
    double umQuartoDoIntervalo = intervalo/4;
    double tresQuartosDoIntervalo = 3*(intervalo/4);
    double areaEsquerda, areaDireita;

    areaEsquerda = intervalo/2 * calcula_funcao(a + umQuartoDoIntervalo);
    areaDireita = intervalo/2 * calcula_funcao(a + tresQuartosDoIntervalo);

    //Debug
    printf("A: %f\n", a);
    printf("B: %f\n", b);
    printf("PM: %f\n", pontoMedio);
    printf("Area Esquerda: %f\n", areaEsquerda);
    printf("Area Direita: %f\n", areaDireita);

    insere_vetor_tarefa(a, pontoMedio, areaEsquerda);
    insere_vetor_tarefa(pontoMedio, b, areaDireita);
}

void * threads_integral (void* arg){
	int pid = * (int *) arg;
 
	printf("Thread id: %d\n", pid);
	// if(pid == 3){
	// 	imprime_vetor_tarefa();
	// 	printf("VetorTarefas[0].a %f\n", vetorTarefas[0].a);
	// 	printf("VetorTarefas[1].a %f\n", vetorTarefas[1].a);
	// }

    pthread_exit(NULL);
}

void valida_entrada(int argc, char *argv[]){
    if(argc<5){
        printf("Use %s <limite inferior> <limite superior> <erro máximo> <#threads>\n", argv[0]);
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
    double integralInicial;

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

    TAREFA *vetorTarefas = (TAREFA *) malloc(sizeof(TAREFA));
    if(vetorTarefas == NULL){
        printf("--ERRO: malloc() em vetor de integrais\n");
    } else { 
    	vetorTarefas->proximo = NULL;
    	numTarefas=0;
    }

    integralInicial = calcula_integral(a,b);
    printf("Integral Inicial: %f\n", integralInicial);
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

    //ToDo: Dar free
    return 0;
}
