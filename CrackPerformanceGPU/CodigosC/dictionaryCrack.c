/*
 Aqui estão as principais alterações feitas no código:

 Solicitação do nome do arquivo: O programa agora solicita ao usuário que insira o nome do arquivo que contém as senhas a serem verificadas.
 Leitura do arquivo: O programa abre o arquivo de texto contendo as senhas a serem verificadas. As senhas são lidas linha por linha usando a função fgets.
 Remoção do caractere de nova linha: Antes de testar cada senha, é removido o caractere de nova linha (\n) do final da string lida.
 Verificação de senhas: O programa verifica cada senha lida do arquivo, dividindo o trabalho em várias threads, como no exemplo anterior. Cada linha do arquivo é tratada como uma senha diferente.
 Exibição da senha encontrada: Se uma senha correta for encontrada, o programa exibe a mensagem "Senha encontrada" e sai do loop de verificação.
 Lembre-se de que este é apenas um exemplo básico de como verificar senhas a partir de um arquivo de texto. A estrutura do arquivo e a lógica de verificação podem variar dependendo das suas necessidades específicas. Além disso, para testar senhas WiFi reais, você precisaria substituir a string correctPassword pela senha WiFi correta e implementar uma lógica de verificação apropriada para autenticar em uma rede WiFi real.
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
    FILE *file;
    char filename[100];
    char line[100];

    printf("Digite o nome do arquivo com as senhas a serem verificadas: ");
    scanf("%s", filename);

    // Abrir o arquivo
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Não foi possível abrir o arquivo %s.\n", filename);
        return 1;
    }

    // Ler senhas do arquivo e testar
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0; // Remover o caractere de nova linha (\n) do final

        const char *password = line;
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

        if (passwordFound) {
            printf("Senha encontrada: %s\n", password);
            break;
        }
    }

    fclose(file);

    if (!passwordFound) {
        printf("Nenhuma senha encontrada.\n");
    }

    return 0;
}

