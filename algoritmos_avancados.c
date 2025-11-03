#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10

// ------------------------------------------------------------
// Estrutura da sala (árvore binária - mapa da mansão)
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
// Estrutura da tabela hash (pista -> suspeito)
// ------------------------------------------------------------
typedef struct {
    char pista[100];
    char suspeito[50];
} ParHash;

typedef struct {
    ParHash tabela[TAM_HASH];
} Hash;

// ------------------------------------------------------------
// Função utilitária de hash simples (soma de caracteres mod tamanho)
// ------------------------------------------------------------
int hashFunc(const char *chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++)
        soma += chave[i];
    return soma % TAM_HASH;
}

// ------------------------------------------------------------
// Função: criarSala()
// Cria dinamicamente uma sala com nome e pista
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
// Função: inserirPista()
// Insere uma pista na árvore BST em ordem alfabética
// ------------------------------------------------------------
NoPista* inserirPista(NoPista *raiz, const char *pista) {
    if (raiz == NULL) {
        NoPista *novo = (NoPista*) malloc(sizeof(NoPista));
        strcpy(novo->pista, pista);
        novo->esquerda = novo->direita = NULL;
        return novo;
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    return raiz;
}

// ------------------------------------------------------------
// Função: inserirNaHash()
// Adiciona uma associação pista -> suspeito na tabela hash
// ------------------------------------------------------------
void inserirNaHash(Hash *h, const char *pista, const char *suspeito) {
    int pos = hashFunc(pista);
    while (strlen(h->tabela[pos].pista) != 0) {
        pos = (pos + 1) % TAM_HASH; // tratamento de colisão simples
    }
    strcpy(h->tabela[pos].pista, pista);
    strcpy(h->tabela[pos].suspeito, suspeito);
}

// ------------------------------------------------------------
// Função: encontrarSuspeito()
// Retorna o suspeito associado à pista
// ------------------------------------------------------------
const char* encontrarSuspeito(Hash *h, const char *pista) {
    int pos = hashFunc(pista);
    for (int i = 0; i < TAM_HASH; i++) {
        int idx = (pos + i) % TAM_HASH;
        if (strlen(h->tabela[idx].pista) == 0)
            break;
        if (strcmp(h->tabela[idx].pista, pista) == 0)
            return h->tabela[idx].suspeito;
    }
    return "Desconhecido";
}

// ------------------------------------------------------------
// Função: exibirPistas()
// Exibe todas as pistas em ordem alfabética (in-order traversal)
// ------------------------------------------------------------
void exibirPistas(NoPista *raiz, Hash *h) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda, h);
        printf("- %s (Suspeito: %s)\n", raiz->pista, encontrarSuspeito(h, raiz->pista));
        exibirPistas(raiz->direita, h);
    }
}

// ------------------------------------------------------------
// Função: explorarSalas()
// Permite a navegação pela árvore e coleta das pistas
// ------------------------------------------------------------
void explorarSalas(Sala *atual, NoPista **arvorePistas, Hash *hash) {
    if (atual == NULL) return;

    printf("\nVocê está em: %s\n", atual->nome);
    if (strlen(atual->pista) > 0) {
        printf("Pista encontrada: \"%s\"\n", atual->pista);
        *arvorePistas = inserirPista(*arvorePistas, atual->pista);
    } else {
        printf("Nenhuma pista aqui.\n");
    }

    char escolha;
    printf("\nEscolha o próximo caminho: (e) esquerda, (d) direita, (s) sair\n> ");
    scanf(" %c", &escolha);

    switch (escolha) {
        case 'e':
            if (atual->esquerda != NULL)
                explorarSalas(atual->esquerda, arvorePistas, hash);
            else
                printf("Não há caminho à esquerda!\n");
            break;

        case 'd':
            if (atual->direita != NULL)
                explorarSalas(atual->direita, arvorePistas, hash);
            else
                printf("Não há caminho à direita!\n");
            break;

        case 's':
            printf("\nVocê decidiu encerrar a exploração.\n");
            return;

        default:
            printf("Opção inválida! Tente novamente.\n");
            explorarSalas(atual, arvorePistas, hash);
    }
}

// ------------------------------------------------------------
// Função: verificarSuspeitoFinal()
// Verifica se o suspeito acusado tem 2 ou mais pistas associadas
// ------------------------------------------------------------
void verificarSuspeitoFinal(NoPista *raiz, Hash *h, const char *acusado, int *contador) {
    if (raiz != NULL) {
        verificarSuspeitoFinal(raiz->esquerda, h, acusado, contador);
        const char *suspeito = encontrarSuspeito(h, raiz->pista);
        if (strcmp(suspeito, acusado) == 0)
            (*contador)++;
        verificarSuspeitoFinal(raiz->direita, h, acusado, contador);
    }
}

// ------------------------------------------------------------
// Função principal
// ------------------------------------------------------------
int main() {
    // Cria o mapa da mansão (árvore binária fixa)
    Sala *hall = criarSala("Hall de Entrada", "Pegadas enlameadas no tapete");
    Sala *salaEstar = criarSala("Sala de Estar", "Taça de vinho quebrada");
    Sala *biblioteca = criarSala("Biblioteca", "Livro de venenos aberto");
    Sala *cozinha = criarSala("Cozinha", "Faca ausente do suporte");
    Sala *jardim = criarSala("Jardim", "Luvas jogadas perto da fonte");
    Sala *porao = criarSala("Porão", "Caixa de ferramentas aberta");
    Sala *quarto = criarSala("Quarto Principal", "Joia desaparecida da penteadeira");

    // Ligações (árvore binária fixa)
    hall->esquerda = salaEstar;
    hall->direita = biblioteca;
    salaEstar->esquerda = cozinha;
    salaEstar->direita = jardim;
    biblioteca->esquerda = porao;
    biblioteca->direita = quarto;

    // Cria tabela hash e associa pistas a suspeitos
    Hash hash = {0};
    inserirNaHash(&hash, "Pegadas enlameadas no tapete", "Jardineiro");
    inserirNaHash(&hash, "Taça de vinho quebrada", "Governanta");
    inserirNaHash(&hash, "Livro de venenos aberto", "Mordomo");
    inserirNaHash(&hash, "Faca ausente do suporte", "Cozinheira");
    inserirNaHash(&hash, "Luvas jogadas perto da fonte", "Jardineiro");
    inserirNaHash(&hash, "Caixa de ferramentas aberta", "Mecânico");
    inserirNaHash(&hash, "Joia desaparecida da penteadeira", "Madame");

    // Árvore de pistas coletadas
    NoPista *arvorePistas = NULL;

    printf("=== DETECTIVE QUEST - NIVEL MESTRE ===\n");
    printf("Bem-vindo, detetive! Explore a mansão e relacione as pistas.\n");

    explorarSalas(hall, &arvorePistas, &hash);

    printf("\n===== FIM DA EXPLORAÇÃO =====\n");
    printf("Pistas coletadas:\n");
    exibirPistas(arvorePistas, &hash);

    char acusado[50];
    printf("\nQuem você acredita ser o culpado?\n> ");
    scanf(" %[^\n]", acusado);

    int contador = 0;
    verificarSuspeitoFinal(arvorePistas, &hash, acusado, &contador);

    printf("\n===== RESULTADO =====\n");
    if (contador >= 2)
        printf("Você acertou! %s é o culpado, com %d evidências.\n", acusado, contador);
    else
        printf("Acusação incorreta! %s não tem pistas suficientes contra si.\n", acusado);

    printf("\nObrigado por jogar Detective Quest!\n");
    return 0;
}
