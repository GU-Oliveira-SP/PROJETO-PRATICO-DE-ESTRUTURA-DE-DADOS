#include <stdio.h>
#include <stdlib.h>
#include "../include/bst.h"

/*
 * Funcao auxiliar interna para alocar e inicializar um novo no da BST.
 */
static NoBST* criar_no(Livro novo_livro) {
    NoBST *novo = (NoBST*)malloc(sizeof(NoBST));
    if (novo == NULL) {
        printf("Erro: falha ao alocar memoria para o novo livro!\n");
        return NULL;
    }

    novo->dado = novo_livro;
    novo->esquerda = NULL;
    novo->direita = NULL;

    return novo;
}

/*
 * 1. Insercao na BST (recursiva classica).
 * Compara o ID do novo livro com o ID do no atual para decidir se vai
 * para a subarvore da esquerda (menor) ou da direita (maior).
 */
NoBST* bst_inserir(NoBST *raiz, Livro novo_livro) {
    /* Caso base: chegou em uma posicao vazia, cria e retorna o novo no */
    if (raiz == NULL) {
        return criar_no(novo_livro);
    }

    /* Se o ID for menor, insere na subarvore da esquerda */
    if (novo_livro.id < raiz->dado.id) {
        raiz->esquerda = bst_inserir(raiz->esquerda, novo_livro);
    }
    /* Se o ID for maior, insere na subarvore da direita */
    else if (novo_livro.id > raiz->dado.id) {
        raiz->direita = bst_inserir(raiz->direita, novo_livro);
    }
    /* Se o ID for igual, rejeita a insercao (nao permite duplicados) */
    else {
        printf("Erro: ja existe um livro cadastrado com o ID %d!\n", novo_livro.id);
    }

    return raiz;
}

/*
 * 2. Busca por ID na BST (recursiva).
 * Retorna o ponteiro para o no encontrado ou NULL se nao existir.
 */
NoBST* bst_buscar(NoBST *raiz, int id) {
    /* Caso base: arvore vazia ou chegou ao fim sem encontrar, ou encontrou o ID */
    if (raiz == NULL || raiz->dado.id == id) {
        return raiz;
    }

    /* Se o ID procurado for menor, busca na esquerda */
    if (id < raiz->dado.id) {
        return bst_buscar(raiz->esquerda, id);
    }

    /* Caso contrario, busca na direita */
    return bst_buscar(raiz->direita, id);
}

/*
 * 3. Listagem Em-Ordem (Esquerda -> Raiz -> Direita).
 * Exibe os livros em ordem crescente de ID.
 */
void bst_listar_em_ordem(NoBST *raiz) {
    if (raiz != NULL) {
        bst_listar_em_ordem(raiz->esquerda);
        printf("ID: %d | Titulo: \"%s\" | Autor: %s | Ano: %d\n",
               raiz->dado.id,
               raiz->dado.titulo,
               raiz->dado.autor,
               raiz->dado.ano);
        bst_listar_em_ordem(raiz->direita);
    }
}

/*
 * 4. Liberacao de memoria da BST em Pos-Ordem (Esquerda -> Direita -> Raiz).
 * Garante que os filhos sejam liberados antes do no pai.
 */
void bst_liberar(NoBST *raiz) {
    if (raiz != NULL) {
        bst_liberar(raiz->esquerda);
        bst_liberar(raiz->direita);
        free(raiz);
    }
}
