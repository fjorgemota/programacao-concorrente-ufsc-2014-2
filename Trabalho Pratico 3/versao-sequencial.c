#include <stdio.h>
#include <stdlib.h>

/**!
 * Calcula e soma todos os divisores de um determinado numero
 *
 * @param numero: O numero que tera seus divisores verificados 
 *                e somados
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
 * Configura um vetor (cache) com a soma dos divisores dos 
 * numeros pertencentes ao intervalo de um numero minimo a
 * um maximo, retornando o ponteiro deste valor
 *
 * @param minimo: O numero minimo do intervalo a ser
 *                calculado para a cache
 * @param maximo: O numero maximo do intervalo a ser
 *                calculado para a cache
 */
double* criaCacheEntre(unsigned int minimo, unsigned int maximo) {
    unsigned int intervalo, n1;
    double* cache;
    intervalo = maximo - minimo;
    cache = (double*) malloc((intervalo+1)*sizeof(double));
    for(n1 = 0; n1 <= intervalo; n1++) {
            cache[n1] = (double) somaDivisores(n1 + minimo);
    }
    return cache;
}

/**!
 * Calcula quais os pares de numeros mutuamente amigos de um 
 * dado intervalo sem o uso do vetor cache criado na funçao 
 * configuraCache
 *
 * @param minimo: O numero minimo que compoe o extremo do intervalo
 * @param maximo: O numero maximo que compoe o extermo do intervalo
 */
void calculaNumerosMutuamenteAmigosEntre(unsigned int minimo, unsigned int maximo) {
    unsigned int n1, n2;
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
 * Calcula quais os pares de numeros mutuamente amigos de um
 * dado intervalo com o uso do vetor cache, criado na funçao
 * configuraCache
 *
 * @param minimo: O numero minimo que compoe o extremo do intervalo
 * @param maximo: O numero maximo que compoe o extremo do intervalo
 * @param cache: A cache com a soma dos numeros do intervalo
 *               composto pelos valores de minimo e maximo 
 */
void calculaNumerosMutuamenteAmigosUsandoCacheEntre(unsigned int minimo, unsigned int maximo, double* cache) {
    if (cache == NULL) {
        calculaNumerosMutuamenteAmigosEntre(minimo, maximo);
        return;
    }
    unsigned int n1, n2, intervalo;
    intervalo = maximo - minimo;
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

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("./versao-sequencial <min> <max>\n");
        return 1;
    }
    unsigned int n1, n2, minimo, maximo, intervalo;
    minimo = atoi(argv[1]);
    maximo = atoi(argv[2]);
    intervalo = maximo-minimo;
    double* cache = criaCacheEntre(minimo, maximo);
    if (cache == NULL) {
        calculaNumerosMutuamenteAmigosEntre(minimo, maximo);
    } else {
        calculaNumerosMutuamenteAmigosUsandoCacheEntre(minimo, maximo, cache);
        free(cache);
    }
    return 0;
}