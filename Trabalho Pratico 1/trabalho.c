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
	int contador,total;
	total = argumentos->posicaoFinal-argumentos->posicaoInicial;
	for (contador = 0; contador < total; contador++) {
		resultado += argumentos->A[contador] * argumentos->B[contador];
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

void esperarThreads(int nThreads, pthread_t *threads) {
	int i;
	for(i =0; i<nThreads; i++) {
		pthread_join(threads[i], NULL);
	}
}

void imprimeResultado() {
	printf("Produto Escalar = %d\n", sum);
}

void prepararThreads(int nThreads, int *A, int *B, pthread_t *thread) {
	int contador, tamanhoVetorBase, tamanhoVetor, contadorVetor, contadorVetorArgumento, resto;
	tamanhoVetorBase = VECSIZE/nThreads;
	resto = VECSIZE%nThreads;
	contadorVetor = 0;
	argumentosThread *argumentos = (argumentosThread*)malloc(sizeof(argumentosThread)*NTHREADS);
	pthread_mutex_init(&mutex, NULL); // criação da mutex
	for (contador = 0; contador < nThreads; contador++) {
		argumentos[contador].idThread = contador; // Numero de thread
		argumentos[contador].posicaoInicial = contadorVetor; // Index inicial
		tamanhoVetor = tamanhoVetorBase;
		if(resto > 0) {
			tamanhoVetor = tamanhoVetor + 1;
			resto--;
		}
		argumentos[contador].A = (int*) malloc(sizeof(int)*tamanhoVetor);
		argumentos[contador].B = (int*) malloc(sizeof(int)*tamanhoVetor);
		for(contadorVetorArgumento = 0; contadorVetorArgumento < tamanhoVetor; contadorVetorArgumento++){
			argumentos[contador].A[contadorVetorArgumento] = A[contadorVetor];
			argumentos[contador].B[contadorVetorArgumento] = B[contadorVetor];
			contadorVetor++;
		}
		argumentos[contador].posicaoFinal = contadorVetor; // Index final
		pthread_create(&thread[contador], NULL, calculaProdutoEscalar, (void *) &argumentos[contador]);
	}
	pthread_mutex_destroy(&mutex); //destruição da mutex
}

int calculaThreads(){
	int divisao = VECSIZE/NTHREADS;
	int resto = VECSIZE%NTHREADS;
	if(divisao == 0) { 
		return resto;
	} else {
		return NTHREADS;
	}
}

int main(int argc, char *argv) {
        srand(time(NULL));
	int* A;
	int* B;
	A = geraVetor();
	B = geraVetor();

	int nThreads = calculaThreads();
	pthread_t *thread = (pthread_t*) malloc(sizeof(pthread_t)*nThreads);

	imprimirVetor('A', A);
	imprimirVetor('B', B);
	prepararThreads(nThreads, A, B, thread);
	esperarThreads(nThreads, thread);
	imprimeResultado();
	pthread_exit(NULL);
}
