#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "queue.h"

#define CADEIRAS 4
#define TOTAL_FUNCIONARIOS 2
#define TOTAL_CLIENTES 10

queue_t fila;
sem_t lock_espera[TOTAL_CLIENTES], lock_chamada, lock_fila;

int length_fila() {
	sem_wait(&lock_fila);
	int resultado = length(&fila);
	sem_post(&lock_fila);
	return resultado;
}

void enqueue_fila(int id_cliente) {
	sem_wait(&lock_fila);
	enqueue(&fila, id_cliente);
	sem_post(&lock_fila);
}

int dequeue_fila() {
	sem_wait(&lock_fila);
	int id_cliente = dequeue(&fila);
	sem_post(&lock_fila);
	return id_cliente;
}

void *cliente(void *clientes) {
	int id_cliente = *((int *)clientes);
	while(1) {
		int tamanho_fila = length_fila();
		if (tamanho_fila < CADEIRAS) {
			enqueue_fila(id_cliente);
			printf("Cliente %d: chegou (%d/%d lugares ocupados)\n", id_cliente, length_fila(), CADEIRAS);
			sem_post(&lock_chamada);
			sem_wait(&lock_espera[id_cliente]);
		} else {
			printf("Cliente %d: cartório lotado, saindo para dar uma volta (%d/%d lugares ocupados)\n", id_cliente, length_fila(), CADEIRAS);
		}
		sleep(10);
	}
}

void *funcionario(void *func) {
	int id_cliente;
	int id_funcionario = *((int *)func);
	while(1) {
		sem_wait(&lock_chamada);
		id_cliente = dequeue_fila(); // Remover o primeiro da fila
		printf ("Funcionário %d: atendendo cliente %d (%d/%d lugares ocupados)\n", id_funcionario, id_cliente, length_fila(), CADEIRAS);
		sleep(5 + rand() % 6);
		sem_post(&lock_espera[id_cliente]);
		printf("Funcionário %d: terminou de atender o cliente %d (%d/%d lugares ocupados)\n", id_funcionario, id_cliente, length_fila(), CADEIRAS);
	}
}

int main(int argc, char **argv) {
	pthread_t clientes[TOTAL_CLIENTES];
	pthread_t funcionarios[TOTAL_FUNCIONARIOS];
	int id;

	sem_init(&lock_fila, 0, 1);
	sem_init(&lock_chamada, 0, 1);

	for (id = 0; id < TOTAL_CLIENTES; id++) {
		sem_init(&lock_espera[id], 0, 0);
	}

	init_queue(&fila);
	for (id = 0; id < TOTAL_CLIENTES; id++) {
		int *argumento_id = malloc(sizeof(*argumento_id));
		if (argumento_id == NULL) {
			printf("Nao foi possivel alocar memoria para argumento_id\n");
			return 1;
		}
		*argumento_id = id;
		pthread_create(&clientes[id], NULL, cliente, (void *) argumento_id);
	}
	for (id = 0; id < TOTAL_FUNCIONARIOS; id++) {
		int *argumentos_id = malloc(sizeof(*argumentos_id));
		if (argumentos_id == NULL) {
			printf("Nao foi possivel alocar memoria para argumentos_id\n");
			return 1;
		}
		*argumentos_id = id;
		pthread_create(&funcionarios[id], NULL, funcionario, (void *) argumentos_id);
	}
	for (id = 0; id < TOTAL_CLIENTES; id++) {
		pthread_join(clientes[id], NULL);
	}
	for (id = 0; id < TOTAL_FUNCIONARIOS; id++) {
		pthread_join(funcionarios[id], NULL);
	}
	sem_destroy(&lock_fila);
	for (id = 0; id < TOTAL_CLIENTES; id++) {
		sem_destroy(&lock_espera[id]);
	}

	pthread_exit(NULL);
	pthread_exit(NULL);
}
