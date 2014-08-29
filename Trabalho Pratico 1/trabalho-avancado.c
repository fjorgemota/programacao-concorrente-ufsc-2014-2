#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define VECSIZE 16
#define NTHREADS 4

int sum;

int[] geraVetor() {
	int a;
	int resultado[VECSIZE];
	srand(time(NULL));
	for (a=0; a < VECSIZE; a++) {
		resultado[a] = rand()%10;
	}
	return resultado;
}

int calculaDivisaoMaxima(){
	int resto = VECSIZE % NTHREADS;
        int divisao = VECSIZE/NTHREADS;
        int divisaoMaxima;
        if(resto == 0){
                divisaoMaxima = divisao;
        } else {
                divisaoMaxima = divisao + resto;
        }
	return divisaoMaxima;
}

int[][] divideParaThreads(int[] vetor){
	int divisaoMaxima = calculaDivisaoMaxima();
	int resultado[NTHREADS][divisaoMaxima];
	int i, j, m, k;
	k = 0;
	for(i = 0; i<NTHREADS; i++){
		if(i == 0){
			m = divisaoMaxima;
		} else {
			m = VECSIZE/NTHREADS;
		}
		for(j = 0;  j < m; j++){
			resultado[i][j] = vetor[k];
			k++;
		}
	}
	return resultado;
}

int main(int argc, char *argv) {
	int[] A = geraVetor();
	int[] B = geraVetor();

	pthread_t thread[NTHREADS];

}
