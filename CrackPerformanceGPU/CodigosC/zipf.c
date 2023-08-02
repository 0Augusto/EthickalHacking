/*
A ordem de complexidade do código fornecido pode ser analisada em relação às duas principais funções: `generalizedHarmonic()` e `generateZipfSequence()`.

A função `generalizedHarmonic()` contém um loop `for` que executa `n` iterações. Portanto, sua complexidade é O(n).

A função `generateZipfSequence()` também possui um loop `for` que executa `length` iterações. Dentro desse loop, há um loop `while` que, em média, é executado várias vezes proporcionalmente à probabilidade cumulativa. A quantidade exata de iterações do loop `while` pode variar dependendo dos valores gerados aleatoriamente e da distribuição Zipf. No entanto, a complexidade média esperada desse loop `while` é O(1), pois o número de iterações geralmente não cresce linearmente com o tamanho dos dados. Portanto, podemos considerar a complexidade de `generateZipfSequence()` como O(length).

Dado que a função `generateZipfSequence()` é chamada no programa principal com `sequenceLength` igual a 10, a ordem de complexidade total do código é O(length) = O(10) = O(1). Isso significa que a complexidade do código não aumenta à medida que o tamanho da sequência a ser gerada aumenta.

O algoritmo Zipf é uma distribuição estatística que modela a frequência de ocorrência de elementos em um conjunto de dados. Essa distribuição é amplamente utilizada em diversas áreas, como análise de tráfego de rede, estudos de palavras-chave em motores de busca, distribuição de popularidade em redes sociais, entre outros.

A distribuição Zipf é caracterizada por ter uma cauda longa, ou seja, alguns elementos são extremamente frequentes, enquanto a maioria dos elementos é raramente observada. Isso pode ser descrito pela função de densidade de probabilidade Zipf:

P(k) = (1 / k^s) / H(n, s)

Onde:

P(k) é a probabilidade de ocorrência de um elemento k;
s é o parâmetro da distribuição Zipf, conhecido como o parâmetro de forma (shape parameter). Ele controla a forma da distribuição e geralmente é um valor maior que 0;
n é o número total de elementos no conjunto de dados;
H(n, s) é a função harmônica generalizada, que é a soma dos termos da sequência harmônica generalizada definida por H(n, s) = 1^(-s) + 2^(-s) + 3^(-s) + ... + n^(-s).
Quando o parâmetro s é pequeno (próximo de 0), a distribuição Zipf se assemelha a uma distribuição uniforme, onde todos os elementos têm aproximadamente a mesma frequência. À medida que s aumenta, a distribuição Zipf se torna mais inclinada, com alguns elementos se tornando muito mais frequentes do que outros.

A seguir, vamos explicar como gerar uma distribuição Zipf utilizando o algoritmo da inversa da função acumulada (Inverse CDF method). Esse método é útil para gerar amostras de uma distribuição Zipf com base em uma distribuição uniforme aleatória entre 0 e 1.

Passos para gerar uma amostra de uma distribuição Zipf:

Calcule a constante H(n, s), que é a soma dos termos da sequência harmônica generalizada.
Gere um número aleatório U entre 0 e 1 (distribuição uniforme).
Calcule o índice k usando a fórmula: k = int(n * (U^(1/s)))
O elemento k é uma amostra da distribuição Zipf.

Observe que quanto maior o valor de s, mais "inclinada" será a distribuição Zipf, ou seja, haverá mais elementos raros e poucos elementos muito comuns. Por outro lado, valores menores de s tornam a distribuição mais uniforme, com frequências mais próximas entre os elementos.
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 100 // Número total de elementos
#define S 1.0 // Parâmetro de inclinação da distribuição Zipf

// Função para calcular a função harmônica generalizada H(N, s)
double generalizedHarmonic(int n, double s) {
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        sum += 1.0 / pow(i, s);
    }
    return sum;
}

// Função para gerar uma sequência de elementos com base na distribuição Zipf
void generateZipfSequence(int *sequence, int length) {
    double harmonic = generalizedHarmonic(N, S); // Calcula a função harmônica geral com base em N e S
    for (int i = 0; i < length; i++) {
        double randomValue = (double)rand() / RAND_MAX; // Gera um valor aleatório entre 0 e 1
        double cumulativeProbability = 0.0;
        int element = 1;
        while (cumulativeProbability < randomValue) {
            cumulativeProbability += (1.0 / (pow(element, S) * harmonic)); // Calcula a probabilidade cumulativa para o elemento atual
            element++;
        }
        sequence[i] = element - 1; // Armazena o elemento gerado na sequência (subtraindo 1 para ajustar à base 0)
    }
}

int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios com base no tempo atual

    int sequenceLength = 10; // Comprimento da sequência a ser gerada
    int sequence[sequenceLength];

    generateZipfSequence(sequence, sequenceLength); // Gera a sequência de elementos com base na distribuição Zipf

    printf("Sequência gerada: ");
    for (int i = 0; i < sequenceLength; i++) {
        printf("%d ", sequence[i]); // Imprime os elementos da sequência gerada
    }
    printf("\n");

    return 0;
}

