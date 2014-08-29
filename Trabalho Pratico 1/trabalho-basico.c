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
		argumentos->posicaoFinal,
		resultado
	);
	sum += resultado;
	pthread_exit(NULL);
}

int main(int argc, char *argv) {
        srand(time(NULL));

	int* A;
	int* B;
	A = geraVetor();
	B = geraVetor();

	pthread_t thread[NTHREADS];

	if (NTHREADS > VECSIZE) {
		printf("Erro: Número de threads é maior que o tamanho do vetor\n");
		return 1;
	} else if (VECSIZE%NTHREADS > 0) {
		printf("Erro: Impossível decidir balanceamento de número de elementos a ser processado por cada thread\n");
		return 1;
	}
	int i, j, k, c;
	printf("A = ");
	for(i = 0; i<VECSIZE; i++){
		if(i+1 == VECSIZE){
			printf("%d\n", A[i]);
		} else {
			printf("%d, ", A[i]);
		}
	}
	printf("B = ");
        for(i = 0; i<VECSIZE; i++){
                if(i+1 == VECSIZE){
                        printf("%d\n", B[i]);
                } else {
                        printf("%d, ", B[i]);
                }
        } 
	j = VECSIZE/NTHREADS;
	k = 0;
	argumentosThread argumentos[NTHREADS];
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
	for(i = 0; i < NTHREADS; i++){
		pthread_join(thread[i], NULL);
	}
	printf("Produto escalar = %d\n", sum);
	pthread_exit(NULL);
}
