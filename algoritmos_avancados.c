#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ------------------------------------------------------------
// Estrutura da sala (nó da árvore do mapa da mansão)
// ------------------------------------------------------------
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// ------------------------------------------------------------
// Estrutura da árvore BST de pistas
// ------------------------------------------------------------
typedef struct NoPista {
    char pista[100];
    struct NoPista *esquerda;
    struct NoPista *direita;
} NoPista;

// ------------------------------------------------------------
// Função que cria uma sala dinamicamente com nome e pista
// ------------------------------------------------------------
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória para sala!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// ------------------------------------------------------------
// Função que cria um nó de pista na BST
// ------------------------------------------------------------
NoPista* criarNoPista(const char *pista) {
    NoPista *novo = (NoPista*) malloc(sizeof(NoPista));
    if (novo == NULL) {
        printf("Erro ao alocar memória para pista!\n");
        exit(1);
    }
    strcpy(novo->pista, pista);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// ------------------------------------------------------------
// Inserir pista na árvore BST (ordenada alfabeticamente)
// ------------------------------------------------------------
NoPista* inserirPista(NoPista *raiz, const char *pista) {
    if (raiz == NULL)
        return criarNoPista(pista);

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    // Se for igual, não insere novamente

    return raiz;
}

// ------------------------------------------------------------
// Exibe todas as pistas em ordem alfabética (in-order traversal)
// ------------------------------------------------------------
void exibirPistas(NoPista *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// ------------------------------------------------------------
// Explorar as salas e coletar pistas
// ------------------------------------------------------------
void explorarSalasComPistas(Sala *atual, NoPista **arvorePistas) {
    if (atual == NULL) return;

    printf("\nVocê está agora em: %s\n", atual->nome);

    // Se a sala tiver uma pista, adiciona na BST
    if (strlen(atual->pista) > 0) {
        printf("Pista encontrada: \"%s\"\n", atual->pista);
        *arvorePistas = inserirPista(*arvorePistas, atual->pista);
    } else {
        printf("Nenhuma pista encontrada aqui.\n");
    }

    // Se não há mais caminhos, finaliza
    if (atual->esquerda == NULL && atual->direita == NULL) {
        printf("\nFim do caminho! Você chegou ao último cômodo.\n");
        return;
    }

    char escolha;
    printf("\nEscolha o próximo caminho: (e) esquerda, (d) direita, (s) sair\n> ");
    scanf(" %c", &escolha);

    switch (escolha) {
        case 'e':
            if (atual->esquerda != NULL)
                explorarSalasComPistas(atual->esquerda, arvorePistas);
            else
                printf("Não há caminho à esquerda!\n");
            break;

        case 'd':
            if (atual->direita != NULL)
                explorarSalasComPistas(atual->direita, arvorePistas);
            else
                printf("Não há caminho à direita!\n");
            break;

        case 's':
            printf("\nExploração encerrada.\n");
            return;

        default:
            printf("Opção inválida! Tente novamente.\n");
            explorarSalasComPistas(atual, arvorePistas);
            break;
    }
}

// ------------------------------------------------------------
// Função principal
// ------------------------------------------------------------
int main() {
    // Criação da árvore (mapa da mansão)
    Sala *hall = criarSala("Hall de Entrada", "Pegadas misteriosas no tapete");
    Sala *salaEstar = criarSala("Sala de Estar", "Uma carta rasgada sobre a mesa");
    Sala *biblioteca = criarSala("Biblioteca", "Um livro fora de lugar na estante");
    Sala *cozinha = criarSala("Cozinha", "Uma faca com marcas estranhas");
    Sala *jardim = criarSala("Jardim", "");
    Sala *porao = criarSala("Porão", "Caixa de ferramentas aberta");
    Sala *quarto = criarSala("Quarto Misterioso", "Um retrato antigo quebrado");

    // Montagem do mapa da mansão
    hall->esquerda = salaEstar;
    hall->direita = biblioteca;
    salaEstar->esquerda = cozinha;
    salaEstar->direita = jardim;
    biblioteca->esquerda = porao;
    biblioteca->direita = quarto;

    // Árvore de pistas inicialmente vazia
    NoPista *arvorePistas = NULL;

    printf("=== DETECTIVE QUEST - NIVEL AVENTUREIRO ===\n");
    printf("Bem-vindo, detetive! Explore a mansão e colete pistas.\n");

    explorarSalasComPistas(hall, &arvorePistas);

    printf("\n===== FIM DA EXPLORAÇÃO =====\n");
    printf("Pistas coletadas em ordem alfabética:\n");
    exibirPistas(arvorePistas);

    // Liberação da memória (simplificada)
    free(hall);
    free(salaEstar);
    free(biblioteca);
    free(cozinha);
    free(jardim);
    free(porao);
    free(quarto);
    // (Não liberamos a árvore de pistas neste exemplo para simplificar)

    printf("\nObrigado por jogar Detective Quest!\n");
    return 0;
}
