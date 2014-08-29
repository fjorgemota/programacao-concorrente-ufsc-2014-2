#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define VECSIZE 16
#define NTHREADS 4

int sum;
struct argumentosThread {
	int idThread;
	int posicaoInicial;
	int posicaoFinal;
	int *A;
	int *B;
};

int* geraVetor() {
	int a;
	int* resultado = (int*) malloc(sizeof(int)*VECSIZE);
	for (a=0; a < VECSIZE; a++) {
		resultado[a] = rand()%10;
	}
	return resultado;
}

void *calculaProdutoEscalar(void *argumento){
	struct argumentosThread matrizes = (struct argumentosThread) argumento;
	int resultado;
	int c,j;
	j = VECSIZE/NTHREADS;
	for (c = 0; c < j; c++) {
		resultado += matrizes[1][c] * matrizes[2][c];
	}
	printf("Thread %d processou de %d até %d e: Produto escalar parcial: %d", matrizes[0][0], matrizes[0][1], matrizes[0][2], resultado);
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
	j = VECSIZE/NTHREADS;
	if(j < 3){
		j = 3; // Para poder passar informacoes sobre os indices que a thread processou
	}
	k = 0;
	struct argumentosThread argumentos;
	for (i = 0; i < NTHREADS; i++) {
		argumentos.idThread = i; // Numero de thread
		argumentos.posicaoInicial = k; // Index inicial
		for(c = 0; c < j; c++){
			argumentos.A[c] = A[k];
			argumentos.B[c] = B[k];
			k++;
		}
		argumentos.posicaoFinal = k; // Index final
		pthread_create(&thread[i], NULL, calculaProdutoEscalar, (void *) argumentos);
	}
	pthread_exit(NULL);
}
