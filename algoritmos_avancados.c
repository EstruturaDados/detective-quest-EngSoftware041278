#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ------------------------------------------------------------
// Estrutura da sala (nó da árvore)
// ------------------------------------------------------------
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// ------------------------------------------------------------
// Função que cria uma sala dinamicamente
// ------------------------------------------------------------
Sala* criarSala(const char *nome) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// ------------------------------------------------------------
// Função recursiva para explorar as salas da mansão
// ------------------------------------------------------------
void explorarSalas(Sala *atual) {
    if (atual == NULL) return;

    printf("\nVocê está agora em: %s\n", atual->nome);

    // Verifica se chegou a um nó-folha (sem saídas)
    if (atual->esquerda == NULL && atual->direita == NULL) {
        printf("Fim do caminho! Você chegou ao último cômodo.\n");
        return;
    }

    char escolha;
    printf("Escolha o próximo caminho: (e) esquerda, (d) direita, (s) sair\n> ");
    scanf(" %c", &escolha);

    switch (escolha) {
        case 'e':
            if (atual->esquerda != NULL)
                explorarSalas(atual->esquerda);
            else
                printf("Não há caminho à esquerda!\n");
            break;

        case 'd':
            if (atual->direita != NULL)
                explorarSalas(atual->direita);
            else
                printf("Não há caminho à direita!\n");
            break;

        case 's':
            printf("Exploração encerrada. Voltando ao Hall...\n");
            return;

        default:
            printf("Opção inválida! Tente novamente.\n");
            explorarSalas(atual);
            break;
    }
}

// ------------------------------------------------------------
// Função principal - monta o mapa fixo da mansão
// ------------------------------------------------------------
int main() {
    // Criação das salas
    Sala *hall = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *cozinha = criarSala("Cozinha");
    Sala *jardim = criarSala("Jardim");
    Sala *porao = criarSala("Porão");
    Sala *quarto = criarSala("Quarto Misterioso");

    // Montagem manual da árvore (mapa da mansão)
    hall->esquerda = salaEstar;
    hall->direita = biblioteca;

    salaEstar->esquerda = cozinha;
    salaEstar->direita = jardim;

    biblioteca->esquerda = porao;
    biblioteca->direita = quarto;

    // Início da exploração
    printf("=== DETECTIVE QUEST ===\n");
    printf("Bem-vindo à mansão Enigma!\n");
    printf("Você começa no Hall de Entrada.\n");

    explorarSalas(hall);

    // Libera memória (simples, sem recursão profunda)
    free(hall);
    free(salaEstar);
    free(biblioteca);
    free(cozinha);
    free(jardim);
    free(porao);
    free(quarto);

    printf("\nObrigado por jogar Detective Quest!\n");
    return 0;
}
