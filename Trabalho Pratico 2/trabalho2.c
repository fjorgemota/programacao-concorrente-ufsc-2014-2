#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "queue.h"

#define CADEIRAS 4
#define TOTAL_FUNCIONARIOS 2
#define TOTAL_CLIENTES 10

queue_t fila;
sem_t semaforo_funcionario, semaforo_cliente, lock_cliente, lock_funcionario;

void *cliente(void *clientes) {
	int id_cliente = *((int *)clientes);
	// printf("id cliente: %d\n", id_cliente);
	while(1) {
		if (length(&fila) < CADEIRAS) {
			sem_wait(&semaforo_cliente);
			sem_wait(&lock_cliente);
			enqueue(&fila, id_cliente);
			printf("Cliente %d: chegou (%d/%d lugares ocupados)\n", id_cliente, length(&fila), CADEIRAS);
			sem_post(&lock_cliente);
			sem_post(&semaforo_cliente);
		} else {
			printf("Cliente %d: cartório lotado, saindo para dar uma volta (%d/%d lugares ocupados)\n", id_cliente, length(&fila), CADEIRAS);
		}
		sleep(10);
	}
}

void *funcionario(void *func) {
	int id_cliente;
	int id_funcionario = *((int *)func);
	while(1) { // semáforo
		sem_wait(&semaforo_funcionario);
		sem_wait(&lock_funcionario);
		id_cliente = dequeue(&fila);
		printf ("Funcionário %d: atendendo cliente %d (%d/%d lugares ocupados)\n", id_funcionario, id_cliente, length(&fila), CADEIRAS);
		sem_post(&semaforo_funcionario);
		sem_post(&lock_funcionario);
		sleep(5 + rand() % 6);
		printf("Funcionário %d: terminou de atender o cliente %d (%d/%d lugares ocupados)\n", id_funcionario, id_cliente, length(&fila), CADEIRAS);
	}
}

int main(int argc, char **argv) {
	pthread_t clientes[TOTAL_CLIENTES];
	pthread_t funcionarios[TOTAL_FUNCIONARIOS];
	int id;

	sem_init(&semaforo_cliente, 0, 4);
	sem_init(&semaforo_funcionario, 0, 4);
	sem_init(&lock_funcionario, 0, 1);
	sem_init(&lock_cliente, 0, 1);

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
		int *argumento_id = malloc(sizeof(*argumento_id));
		if (argumento_id == NULL) {
			printf("Nao foi possivel alocar memoria para argumento_id\n");
			return 1;
		}
		*argumento_id = id;
		pthread_create(&funcionarios[id], NULL, funcionario, (void *) argumento_id);
	}
	for (id = 0; id < TOTAL_CLIENTES; id++) {
		pthread_join(clientes[id], NULL);
	}
	for (id = 0; id < TOTAL_FUNCIONARIOS; id++) {
		pthread_join(funcionarios[id], NULL);
	}
	sem_destroy(&semaforo_cliente);

	pthread_exit(NULL);
	pthread_exit(NULL);
}
