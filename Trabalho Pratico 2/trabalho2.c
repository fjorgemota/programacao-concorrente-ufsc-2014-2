#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "queue.h"

#define CADEIRAS 4
#define TOTAL_FUNCIONARIOS 2
#define TOTAL_CLIENTES 10

queue_t fila;
sem_t lock_espera[TOTAL_CLIENTES], lock_sem_clientes, lock_fila;

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
			sem_post(&lock_sem_clientes);
			sem_wait(&lock_espera[id_cliente]);
		} else {
			printf("Cliente %d: cartório lotado, saindo para dar uma volta (%d/%d lugares ocupados)\n", id_cliente, length_fila(), CADEIRAS);
		}
		sleep(10);
	}
	pthread_exit(NULL);
}

void *funcionario(void *func) {
	int id_cliente;
	int id_funcionario = *((int *)func);
	while(1) {
		sem_wait(&lock_sem_clientes);
		id_cliente = dequeue_fila(); // Remover o primeiro da fila
		printf ("Funcionário %d: atendendo cliente %d (%d/%d lugares ocupados)\n", id_funcionario, id_cliente, length_fila(), CADEIRAS);
		sleep(5 + rand() % 6);
		sem_post(&lock_espera[id_cliente]);
		printf("Funcionário %d: terminou de atender o cliente %d (%d/%d lugares ocupados)\n", id_funcionario, id_cliente, length_fila(), CADEIRAS);
	}
	pthread_exit(NULL);
}

void criaThreadsClientes(pthread_t *clientes, int *argumentos) {
	int id;
	for (id = 0; id < TOTAL_CLIENTES; id++) {
		argumentos[id] = id;
		pthread_create(&clientes[id], NULL, cliente, (void *) &argumentos[id]);
	}
}

void criaThreadsFuncionarios(pthread_t *funcionarios, int *argumentos) {
	int id;
	for (id = 0; id < TOTAL_FUNCIONARIOS; id++) {
		argumentos[id] = id;
		pthread_create(&funcionarios[id], NULL, funcionario, (void *) &argumentos[id]);
	}
}

void esperarThreadsClientes(pthread_t *clientes) {
	int id;
	for (id = 0; id < TOTAL_CLIENTES; id++) {
		pthread_join(clientes[id], NULL);
	}
}

void esperarThreadsFuncionarios(pthread_t *funcionarios) {
	int id;
	for (id = 0; id < TOTAL_FUNCIONARIOS; id++) {
		pthread_join(funcionarios[id], NULL);
	}
}

void inicializarSemaforos() {
	int contador;
	sem_init(&lock_fila, 0, 1);
	sem_init(&lock_sem_clientes, 0, 1);
	for (contador = 0; contador < TOTAL_CLIENTES; contador++) {
		sem_init(&lock_espera[contador], 0, 0);
	}
}

void destruirSemaforos() {
	int contador;
	sem_destroy(&lock_fila);
	sem_destroy(&lock_sem_clientes);
	for (contador = 0; contador < TOTAL_CLIENTES; contador++) {
		sem_destroy(&lock_espera[contador]);
	}
}

int main(int argc, char **argv) {
	pthread_t clientes[TOTAL_CLIENTES];
	pthread_t funcionarios[TOTAL_FUNCIONARIOS];
	int argumentos_clientes[TOTAL_CLIENTES];
	int argumentos_funcionarios[TOTAL_FUNCIONARIOS];

	inicializarSemaforos();

	init_queue(&fila);
	criaThreadsClientes(clientes, argumentos_clientes);
	criaThreadsFuncionarios(funcionarios, argumentos_funcionarios);
	esperarThreadsClientes(clientes);
	esperarThreadsFuncionarios(funcionarios);

	destruirSemaforos();
}
