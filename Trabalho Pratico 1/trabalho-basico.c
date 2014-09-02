#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define VECSIZE 16
#define NTHREADS 4

int sum;
typedef struct {
	int idThread;
	int posicaoInicial;
	int posicaoFinal;
	int *A;
	int *B;
} argumentosThread;
pthread_mutex_t mutex; // Variável global mutex

int* geraVetor() {
	int a;
	int* resultado = (int*) malloc(sizeof(int)*VECSIZE);
	for (a=0; a < VECSIZE; a++) {
		resultado[a] = rand()%10;
	}
	return resultado;
}

void *calculaProdutoEscalar(void *argumento){
	argumentosThread *argumentos = (argumentosThread*) argumento;
	int resultado = 0;
	int c,j;
	j = VECSIZE/NTHREADS;
	for (c = 0; c < j; c++) {
		resultado += argumentos->A[c] * argumentos->B[c];
	}
	printf(
		"Thread %d processou de %d até %d e: Produto escalar parcial: %d\n",
		argumentos->idThread,
		argumentos->posicaoInicial,
		argumentos->posicaoFinal-1,
		resultado
	);
	pthread_mutex_lock(&mutex); //início da região crítica
	sum += resultado;
	pthread_mutex_unlock(&mutex); //Final da região crítica
	pthread_exit(NULL);
}

void imprimirVetor(char vetor, int *valores) {
	int i;
	printf("%c = ", vetor);
	for(i = 0; i<VECSIZE; i++){
		if(i+1 == VECSIZE){
			printf("%d\n", valores[i]);
		} else {
			printf("%d, ", valores[i]);
		}
	}
}

void esperarThreads(pthread_t *threads) {
	int i;
	for(i =0; i<NTHREADS; i++) {
		pthread_join(threads[i], NULL);
	}
}

void imprimeResultado() {
	printf("Produto Escalar = %d\n", sum);
}

void prepararThreads(int *A, int *B, pthread_t *thread) {
	int i, j, k, c;
	j = VECSIZE/NTHREADS;
	k = 0;
	argumentosThread *argumentos = (argumentosThread*)malloc(sizeof(argumentosThread)*NTHREADS);
	pthread_mutex_init(&mutex, NULL); // criação da mutex
	for (i = 0; i < NTHREADS; i++) {
		argumentos[i].idThread = i; // Numero de thread
		argumentos[i].posicaoInicial = k; // Index inicial
		argumentos[i].A = (int*) malloc(sizeof(int)*j);
		argumentos[i].B = (int*) malloc(sizeof(int)*j);
		for(c = 0; c < j; c++){
			argumentos[i].A[c] = A[k];
			argumentos[i].B[c] = B[k];
			k++;
		}
		argumentos[i].posicaoFinal = k; // Index final
		pthread_create(&thread[i], NULL, calculaProdutoEscalar, (void *) &argumentos[i]);
	}
	pthread_mutex_destroy(&mutex); //destuição da mutex
}

int main(int argc, char *argv) {
        srand(time(NULL));
	int* A;
	int* B;
	A = geraVetor();
	B = geraVetor();

	if (NTHREADS > VECSIZE) {
		printf("Erro: Número de threads é maior que o tamanho do vetor\n");
		return 1;
	} else if (VECSIZE%NTHREADS > 0) {
		printf("Erro: Impossível decidir balanceamento de número de elementos a ser processado por cada thread\n");
		return 1;
	}
	pthread_t thread[NTHREADS];

	imprimirVetor('A', A);
	imprimirVetor('B', B);
	prepararThreads(A, B, thread);
	esperarThreads(thread);
	imprimeResultado();
	pthread_exit(NULL);
}
