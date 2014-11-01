#include <stdio.h>
#include <stdlib.h>

int somaDivisores(int numero) {
    int divisores, contagem;
    divisores = 1 + numero;
    for (contagem = 2; contagem < numero; contagem++) {
        if (numero % contagem == 0) {
            divisores += contagem;
        }
    }
    return divisores;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("./versao-sequencial <min> <max>\n");
        return 1;
    }
    int n1, n2, minimo, maximo, intervalo;
    double *cache;
    minimo = atoi(argv[1]);
    maximo = atoi(argv[2]);
    intervalo = maximo-minimo;
    cache = (double*) malloc(sizeof(double)*intervalo);
    if (cache == NULL) {
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
        for(n1 = 0; n1 <= intervalo; n1++) {
            cache[n1] = (double) somaDivisores(n1 + minimo);
        }
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