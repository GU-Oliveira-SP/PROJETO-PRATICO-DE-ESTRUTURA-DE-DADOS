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
 * Compara o IDs para decidir se vai para a subarvore da esquerda (menor) ou da direita (maior).
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
 * 2. Busca por ID na BST (recursiva).*/
NoBST* bst_buscar(NoBST *raiz, int id) {
    
    if (raiz == NULL || raiz->dado.id == id) {
        return raiz;
    }

    /* Se o ID procurado for menor, busca na esquerda */
    if (id < raiz->dado.id) {
        return bst_buscar(raiz->esquerda, id);
    }

    /* busca na direita */
    return bst_buscar(raiz->direita, id);
}

/*
 * Funcao auxiliar que encontra o no com o menor ID em uma subarvore.*/
static NoBST* encontrarMinimo(NoBST *no) {
    NoBST *atual = no;
    while (atual != NULL && atual->esquerda != NULL) {
        atual = atual->esquerda;
    }
    return atual;
}

/*
 * 3. Remocao por ID na BST (recursiva).
 * Retorna a nova raiz da subarvore apos a remocao.
 */
NoBST* bst_remover(NoBST *raiz, int id) {
    
    if (raiz == NULL) {
        return NULL;
    }

    /* Procura o no a ser removido na subarvore esquerda ou direita */
    if (id < raiz->dado.id) {
        raiz->esquerda = bst_remover(raiz->esquerda, id);
    } else if (id > raiz->dado.id) {
        raiz->direita = bst_remover(raiz->direita, id);
    } else {
        
        /* Caso 1: No folha (0 filhos) */
        if (raiz->esquerda == NULL && raiz->direita == NULL) {
            free(raiz);
            return NULL;
        }
        /* Caso 2a: No com apenas 1 filho (apenas na direita) */
        else if (raiz->esquerda == NULL) {
            NoBST *filho = raiz->direita;
            free(raiz);
            return filho;
        }
        /* Caso 2b: No com apenas 1 filho (apenas na esquerda) */
        else if (raiz->direita == NULL) {
            NoBST *filho = raiz->esquerda;
            free(raiz);
            return filho;
        }
        /* Caso 3: No com 2 filhos */
        else {
            /* Encontra o sucessor em-ordem (menor no da subarvore direita) */
            NoBST *sucessor = encontrarMinimo(raiz->direita);

            raiz->dado = sucessor->dado;

            raiz->direita = bst_remover(raiz->direita, sucessor->dado.id);
        }
    }

    return raiz;
}

/*4. Listagem Em-Ordem (Esquerda -> Raiz -> Direita).*/
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

/*5. Liberacao de memoria da BST em Pos-Ordem (Esquerda -> Direita -> Raiz).*/
void bst_liberar(NoBST *raiz) {
    if (raiz != NULL) {
        bst_liberar(raiz->esquerda);
        bst_liberar(raiz->direita);
        free(raiz);
    }
}
