/*
Observe que, neste exemplo, dividimos a senha a ser testada em várias partes (chunks) e atribuímos cada chunk a uma thread diferente. Cada thread verifica se a senha fornecida é igual à senha correta. Se alguma thread encontrar a senha correta, todas as outras threads são encerradas e o programa imprime a senha encontrada. Isso ajuda a reduzir o tempo de verificação em cenários de senhas mais longas e complexas.

Lembre-se de que este é apenas um exemplo básico de como aproveitar multithreading e multicore para verificar senhas de forma mais eficiente. Dependendo do tamanho da lista de senhas a serem testadas e da complexidade da verificação real, pode ser necessário ajustar o código para obter o melhor desempenho. Além disso, sempre considere questões de segurança ao trabalhar com senhas e certifique-se de seguir as práticas recomendadas para armazenamento seguro e proteção contra ataques.
Para melhorar a eficiência da verificação de senha e aproveitar a capacidade de processamento multithreading e multicore, podemos dividir o trabalho em várias threads para testar diferentes senhas simultaneamente. Para fazer isso, podemos usar a biblioteca pthread em C, que permite trabalhar com threads.
*/

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define NUM_THREADS 4

const char *correctPassword = "mySecretPassword";
int passwordFound = 0;

// Estrutura para armazenar os argumentos da função da thread
struct ThreadArgs {
    const char *password;
    int start;
    int end;
};

// Função de verificação de senha executada pela thread
void *checkPassword(void *args) {
    struct ThreadArgs *threadArgs = (struct ThreadArgs *)args;
    const char *password = threadArgs->password;
    int start = threadArgs->start;
    int end = threadArgs->end;

    for (int i = start; i < end && !passwordFound; i++) {
        if (strcmp(password, correctPassword) == 0) {
            printf("Senha encontrada: %s\n", password);
            passwordFound = 1;
        }
    }

    pthread_exit(NULL);
}

int main() {
    const char *password = "password_to_test"; // Substitua por sua lista de senhas a serem testadas
    int passwordLength = strlen(password);
    int chunkSize = passwordLength / NUM_THREADS;

    pthread_t threads[NUM_THREADS];
    struct ThreadArgs threadArgs[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        threadArgs[i].password = password;
        threadArgs[i].start = i * chunkSize;
        threadArgs[i].end = (i == NUM_THREADS - 1) ? passwordLength : (i + 1) * chunkSize;

        pthread_create(&threads[i], NULL, checkPassword, (void *)&threadArgs[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    if (!passwordFound) {
        printf("Senha não encontrada.\n");
    }

    return 0;
}

