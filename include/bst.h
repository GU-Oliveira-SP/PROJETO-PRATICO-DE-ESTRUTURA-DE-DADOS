#ifndef BST_H
#define BST_H

/* Struct dos dados de um livro do catalogo.*/
typedef struct {
    int id;          
    char titulo[80];  
    char autor[80];   
    int ano;          
} Livro;

/*No da Arvore Binaria de Busca (BST).*/
typedef struct NoBST {
    Livro dado;
    struct NoBST *esquerda;
    struct NoBST *direita;
} NoBST;

NoBST* bst_inserir(NoBST *raiz, Livro novo_livro);

NoBST* bst_buscar(NoBST *raiz, int id);

NoBST* bst_remover(NoBST *raiz, int id);

void bst_listar_em_ordem(NoBST *raiz);

void bst_liberar(NoBST *raiz);

#endif 
