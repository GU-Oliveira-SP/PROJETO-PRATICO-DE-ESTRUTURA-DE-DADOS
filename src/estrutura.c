#include <stdio.h>
#include <stdlib.h>
#include "../include/estrutura.h"

/*
 * Inicializa a fila definindo inicio e fim como NULL (fila vazia).
 */
void fila_inicializar(Fila *f) {
    f->inicio = NULL;
    f->fim = NULL;
}

/*
 * 1. Enqueue (Inserir ID no final da fila).
 * A fila e independente da BST: aceita qualquer ID positivo e permite repetidos.
 * Retorna 1 se inseriu com sucesso ou 0 se falhou na alocacao.
 */
int fila_enqueue(Fila *f, int id) {
    NoFila *novo = (NoFila*)malloc(sizeof(NoFila));
    if (novo == NULL) {
        printf("Erro: falha ao alocar memoria para o no da fila!\n");
        return 0;
    }

    novo->id = id;
    novo->proximo = NULL;

    /* Se a fila estiver vazia, o novo no passa a ser tanto o inicio quanto o fim */
    if (f->inicio == NULL) {
        f->inicio = novo;
        f->fim = novo;
    } else {
        /* Caso contrario, encadeia apos o ultimo no e atualiza o ponteiro fim */
        f->fim->proximo = novo;
        f->fim = novo;
    }

    return 1;
}

/*
 * 2. Dequeue (Remover o ID do inicio da fila - regra FIFO).
 * Retorna 1 se removeu (salvando o ID em *id_removido) ou 0 se a fila estiver vazia.
 */
int fila_dequeue(Fila *f, int *id_removido) {
    if (f->inicio == NULL) {
        return 0; /* Fila vazia */
    }

    NoFila *removido = f->inicio;
    *id_removido = removido->id;

    /* Avanca o inicio para o proximo no da fila */
    f->inicio = removido->proximo;

    /* Se o inicio ficou NULL, a fila esvaziou; precisamos ajustar fim = NULL tambem */
    if (f->inicio == NULL) {
        f->fim = NULL;
    }

    free(removido);
    return 1;
}

/*
 * 3. Exibir todos os IDs da fila (do inicio ate o fim).
 */
void fila_exibir(Fila *f) {
    if (f->inicio == NULL) {
        printf("A fila esta vazia! Nenhum ID para exibir.\n");
        return;
    }

    NoFila *atual = f->inicio;
    printf("Fila (Inicio -> Fim): ");
    while (atual != NULL) {
        printf("[%d]", atual->id);
        if (atual->proximo != NULL) {
            printf(" -> ");
        }
        atual = atual->proximo;
    }
    printf("\n");
}

/*
 * 4. Liberar toda a memoria alocada pelos nos da fila ao encerrar o programa.
 */
void fila_liberar(Fila *f) {
    NoFila *atual = f->inicio;
    while (atual != NULL) {
        NoFila *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }

    f->inicio = NULL;
    f->fim = NULL;
}
