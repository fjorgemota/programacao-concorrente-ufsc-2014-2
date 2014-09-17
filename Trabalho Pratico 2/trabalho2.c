#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "queue.h"

#define CADEIRAS 4
#define TOTAL_FUNCIONARIOS 2
#define TOTAL_CLIENTES 10

queue_t fila;
sem_t lock_espera_atendimento[TOTAL_CLIENTES], lock_ha_clientes, ha_funcionarios, lock_fila, lock_cliente, lock_funcionario;

int length_fila(){
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
		sem_wait(&lock_cliente); // Permite apenas um cliente nessa secao
		int tamanho_fila = length_fila();
		if (tamanho_fila < CADEIRAS) {
			enqueue_fila(id_cliente); // Bota o cliente na fila
			printf("Cliente %d: chegou (%d/%d lugares ocupados)\n", id_cliente, length_fila(), CADEIRAS);
			sem_post(&lock_cliente); // Libera o lock do cliente
			sem_wait(&ha_funcionarios); // Aguarda haver um funcionario disponivel
			sem_post(&lock_ha_clientes); // Informa os funcionarios de que ha clientes a serem atendidos
			sem_wait(&lock_espera_atendimento[id_cliente]); // Espera o cliente ser atendido
		} else {
			printf("Cliente %d: cartório lotado, saindo para dar uma volta (%d/%d lugares ocupados)\n", id_cliente, tamanho_fila, CADEIRAS);
			sem_post(&lock_cliente); // Libera o lock do cliente
		}
		sleep(10);
	}
	pthread_exit(NULL);
}

void *funcionario(void *func) {
	int id_cliente;
	int id_funcionario = *((int *)func);
	while(1) {
		sem_wait(&lock_ha_clientes); // Espera pelo menos um cliente se registrar
		sem_wait(&lock_funcionario); // Protege o procedimento do dequeue+impressao inicial
		id_cliente = dequeue_fila(); // Remove o primeiro cliente da fila
		printf ("Funcionário %d: atendendo cliente %d (%d/%d lugares ocupados)\n", id_funcionario, id_cliente, length_fila(), CADEIRAS);
		sem_post(&lock_funcionario); // Libera o lock do dequeue+impressao no funcionario
		sleep(5 + rand() % 6); // Aguarda um periodo randomico entre 5 e 10 segundos
		printf("Funcionário %d: terminou de atender o cliente %d (%d/%d lugares ocupados)\n", id_funcionario, id_cliente,  length(&fila), CADEIRAS);
		sem_post(&ha_funcionarios); // Informa o cliente de que um funcionario esta livre para o processamento
		sem_post(&lock_espera_atendimento[id_cliente]); // Informa o cliente em especifico de que o atendimento dele ja acabou
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
	sem_init(&lock_fila, 0, 1); // Protege a fila, que claramente não é atômica
	sem_init(&lock_cliente, 0, 1); // Protege uma determinada secao critica no cliente
	sem_init(&lock_funcionario, 0, 1); // Protege uma determinada secao critica no funcionario
	sem_init(&lock_ha_clientes, 0, 0); // Inicializa em zero pois inicialmente nao ha nenhum cliente
	sem_init(&ha_funcionarios, 0, TOTAL_FUNCIONARIOS); // Semaforo contador que permite apenas (TOTAL_FUNCIONARIOS) threads acessarem um segmento de codigo
	for (contador = 0; contador < TOTAL_CLIENTES; contador++) {
		sem_init(&lock_espera_atendimento[contador], 0, 0); // Inicializa em zero pois, a principio, 
		// o semaforo deve esperar o funcionario confirmar que o cliente foi atendido
	}
}

void destruirSemaforos() {
	int contador;
	sem_destroy(&lock_fila);
	sem_destroy(&lock_cliente);
	sem_destroy(&lock_funcionario);
	sem_destroy(&lock_ha_clientes);
	sem_destroy(&ha_funcionarios);
	for (contador = 0; contador < TOTAL_CLIENTES; contador++) {
		sem_destroy(&lock_espera_atendimento[contador]);
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
