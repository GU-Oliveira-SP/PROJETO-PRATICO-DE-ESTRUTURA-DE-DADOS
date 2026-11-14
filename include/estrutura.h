#ifndef ESTRUTURA_H
#define ESTRUTURA_H

/*No da Fila dinamica.*/
typedef struct NoFila {
    int id;
    struct NoFila *proximo;
} NoFila;

/*Struct FIFO.*/
typedef struct {
    NoFila *inicio;
    NoFila *fim;
} Fila;

void fila_inicializar(Fila *f);

/* Insere um ID no final da fila (enqueue).*/
int fila_enqueue(Fila *f, int id);

/* Remove o ID do inicio da fila (dequeue). Guarda o valor removido em *id_removido.*/
int fila_dequeue(Fila *f, int *id_removido);

void fila_exibir(Fila *f);

void fila_liberar(Fila *f);

#endif 
