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
	while(1) {
		if (length(&fila) < CADEIRAS) {
			sem_wait(&semaforo_cliente); // Semaforo que indica chegada dos clientes, caso cheio o proximo sera antendido apos
										// um funcionario chamar, quando abre vagas
			sem_wait(&lock_cliente); // mutex, o primeiro que chegar pega a vaga na cadeira
			enqueue(&fila, id_cliente); // Inserir na fila um funcionario
			printf("Cliente %d: chegou (%d/%d lugares ocupados)\n", id_cliente, length(&fila), CADEIRAS);
			sem_post(&lock_cliente); // mutex, o que entrou libera a vaga para o proximo que chegar
			sem_post(&semaforo_funcionario); // Semaforo indicando a chamada de um cliente
		} else {
			printf("Cliente %d: cartório lotado, saindo para dar uma volta (%d/%d lugares ocupados)\n", id_cliente, length(&fila), CADEIRAS);
		}
		sleep(10);
	}
}

void *funcionario(void *func) {
	int id_cliente;
	int id_funcionario = *((int *)func);
	while(1) {
		sem_wait(&semaforo_funcionario); // Semaforo que indica os funcionarios que atenderao os clientes
		sem_wait(&lock_funcionario); // mutex, apenas um funcionario chama um cliente por vez
		id_cliente = dequeue(&fila); // Remover o primeiro da fila
		printf ("Funcionário %d: atendendo cliente %d (%d/%d lugares ocupados)\n", id_funcionario, id_cliente, length(&fila), CADEIRAS);
		sem_post(&lock_funcionario); // mutex, um funcionario esta atendendo um cliente
		sem_post(&semaforo_cliente); // Semaforo indicando que o cliente ja foi atendido
		sleep(5 + rand() % 6);
		printf("Funcionário %d: terminou de atender o cliente %d (%d/%d lugares ocupados)\n", id_funcionario, id_cliente, length(&fila), CADEIRAS);
	}
}

int main(int argc, char **argv) {
	pthread_t clientes[TOTAL_CLIENTES];
	pthread_t funcionarios[TOTAL_FUNCIONARIOS];
	int id;

	sem_init(&semaforo_cliente, 0, 4);
	sem_init(&semaforo_funcionario, 0, 2);
	sem_init(&lock_cliente, 0, 1);
	sem_init(&lock_funcionario, 0, 1);

	init_queue(&fila);
	// printf("fila: %d\n", length(&fila));
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
	sem_destroy(&semaforo_cliente);
	sem_destroy(&semaforo_funcionario);
	sem_destroy(&lock_cliente);
	sem_destroy(&lock_funcionario);

	pthread_exit(NULL);
	pthread_exit(NULL);
}
