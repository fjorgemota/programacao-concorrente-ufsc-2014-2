#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int somaDivisores(int numero) {
    int divisores, contagem;
    divisores = 1 + numero;
    // Estava esperando usar schedule(guided), porque, as primeiras
    // threads iriam realizar mais contas, porém, "menores" (depende de
    // qual foi o intervalo definido) e as últimas, com verificações maiores
    // realizaria menos operações (gastando menos tempo para calcular).
    // O que eu tenho dúvida é como seria o tamanho chunk (bloco).
    for (contagem = 2; contagem < numero; contagem++) {
        if (numero % contagem == 0) {
            divisores += contagem;
        }
    }
    return divisores;
}

int main(int argc, char **argv) {
    if (argc < 4) {
        printf("./versao-paralela <min> <max> <nthreads>\n");
        return 1;
    }
    int n1, n2, minimo, maximo, nthreads, intervalo;
    double *cache;
    minimo = atoi(argv[1]);
    maximo = atoi(argv[2]);
    intervalo = maximo-minimo;
    nthreads = atoi(argv[3]);
    if (nthreads < 1) {
        printf("nthreads deve ser maior ou igual a 1\n");
        return 1;
    }
    omp_set_num_threads(nthreads);
    cache = (double*) malloc(sizeof(double)*intervalo);
    if (cache == NULL) {
        #pragma omp parallel
        #pragma omp for schedule(dynamic, nthreads) private(n1, n2)
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
    } else {
        #pragma omp parallel
        #pragma omp for schedule(dynamic, nthreads) private(n1)
        for(n1 = 0; n1 <= intervalo; n1++) {
            cache[n1] = (double) somaDivisores(n1 + minimo);
        }
        #pragma omp parallel
        #pragma omp for schedule(dynamic, nthreads) private(n1, n2)
        for(n1 = 0; n1 <= intervalo; n1++) {
            double resposta1 = (cache[n1]/(n1+minimo));
            for(n2 = n1 + 1; n2 <= intervalo; n2++) {
                double resposta2 = (cache[n2]/(n2+minimo));
                if (resposta1 == resposta2) {
                    printf("%d e %d são mutuamente amigos\n", n1 + minimo, n2 + minimo);
                }
            }   
        }
        free(cache);
    }
    
    return 0;
}