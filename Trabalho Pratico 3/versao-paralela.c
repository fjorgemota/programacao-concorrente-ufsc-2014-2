#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/**!
 * Calcula a soma dos divisores de um determinado número
 * 
 * @param numero: Número para o qual a soma dos divisores
 *                deve ser calculado.
 */ 
unsigned int somaDivisores(unsigned int numero) {
    unsigned int divisores, contagem;
    divisores = 1 + numero;
    for (contagem = 2; contagem < numero; contagem++) {
        if (numero % contagem == 0) {
            divisores += contagem;
        }
    }
    return divisores;
}

/**!
 * Calcula os números mutuamente amigos entre um determinado
 * número mínimo e um determinado número máximo sem usar 
 * nenhuma cache (calculando a soma dos divisores durante a 
 * comparação)
 * 
 * @param minimo: Número mínimo para o qual o cache deve ser
 *                pre-calculado
 * @param maximo: Número máximo para o qual o cache deve ser
 *                pre-calculado
 * @param nThreads: Tamanho do chunk que deve ser distribuido
 *                para cada thread durante o preenchimento do
 *                cache.
 */ 
void calculaNumerosMutuamenteAmigosEntre(unsigned int minimo, unsigned int maximo, unsigned int nThreads) {
    unsigned int n1, n2;
    #pragma omp parallel
    #pragma omp for schedule(dynamic, nThreads) private(n1, n2)
    for(n1 = minimo; n1 <= maximo; n1++) {
        double soma1 = (double) somaDivisores(n1);
        double resposta1 = soma1/n1;
        for(n2 = n1 + 1; n2 <= maximo; n2++) {
            double soma2 = (double) somaDivisores(n2);
            double resposta2 = soma2/n2;
            if (resposta1 == resposta2) {
                printf("%d e %d são mutuamente amigos\n", n1, n2);
            }
        }   
    }
}

/**!
 * Calcula os números mutuamente amigos entre um determinado
 * número mínimo e um determinado número máximo usando a cache
 * criada pela função criaCacheEntre.
 * 
 * @param minimo: Número mínimo para o qual o cache deve ser
 *                pre-calculado
 * @param maximo: Número máximo para o qual o cache deve ser
 *                pre-calculado
 * @param nThreads: Tamanho do chunk que deve ser distribuido
 *                para cada thread durante o preenchimento do
 *                cache.
 * @param cache: Cache a ser usada como referência para o
 *                calculo dos números mutuamente amigos.
 */ 
void calculaNumerosMutuamenteAmigosUsandoCacheEntre(unsigned int minimo, unsigned int maximo, unsigned int nThreads, double *cache) {
    if (cache == NULL) {
        calculaNumerosMutuamenteAmigosEntre(minimo, maximo, nThreads);
        return;
    }
    unsigned int n1, n2, intervalo;
    intervalo = maximo - minimo;
    #pragma omp parallel
    #pragma omp for schedule(dynamic, nThreads) private(n1, n2)
    for(n1 = 0; n1 <= intervalo; n1++) {
        double resposta1 = (cache[n1]/(n1+minimo));
        for(n2 = n1 + 1; n2 <= intervalo; n2++) {
            double resposta2 = (cache[n2]/(n2+minimo));
            if (resposta1 == resposta2) {
                printf("%d e %d são mutuamente amigos\n", n1 + minimo, n2 + minimo);
            }
        }   
    }
}

/**!
 * Cria um vetor de cache e o preenche com valores da soma 
 * de divisores para todos os numeros entre um determinado
 * número mínimo e um determinado número máximo e retorna
 * o ponteiro para esse vetor.
 * 
 * @param minimo: Número mínimo para o qual o cache deve ser
 *                pre-calculado
 * @param maximo: Número máximo para o qual o cache deve ser
 *                pre-calculado
 * @param nThreads: Tamanho do chunk que deve ser distribuido
 *                para cada thread durante o preenchimento do
 *                cache.
 */ 
double* criaCacheEntre(unsigned int minimo, unsigned int maximo, unsigned int nThreads) {
    unsigned int intervalo, n1;
    double *cache;
    intervalo = maximo-minimo;
    cache = (double*) malloc((intervalo+1)*sizeof(double));
    #pragma omp parallel
    #pragma omp for schedule(dynamic, nThreads) private(n1)
    for(n1 = 0; n1 <= intervalo; n1++) {
        cache[n1] = (double) somaDivisores(n1 + minimo);
    }
    return cache;
}

int main(int argc, char **argv) {
    if (argc < 4) {
        printf("./versao-paralela <min> <max> <nthreads>\n");
        return 1;
    }
    unsigned int minimo, maximo;
    int nThreads;
    minimo = atoi(argv[1]);
    maximo = atoi(argv[2]);
    nThreads = atoi(argv[3]);
    if (nThreads < 1) {
        printf("nthreads deve ser maior ou igual a 1\n");
        return 1;
    }
    omp_set_num_threads(nThreads);
    double *cache = criaCacheEntre(minimo, maximo, nThreads);
    if (cache == NULL) {
        calculaNumerosMutuamenteAmigosEntre(minimo, maximo, nThreads);
    } else {
        calculaNumerosMutuamenteAmigosUsandoCacheEntre(minimo, maximo, nThreads, cache);
        free(cache);
    }
    
    return 0;
}