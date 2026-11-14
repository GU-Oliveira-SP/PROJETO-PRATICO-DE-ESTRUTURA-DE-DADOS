#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>

#include "../include/bst.h"
#include "../include/estrutura.h"

#define TAM_TEXTO 80
#define ANO_MIN 1
#define ANO_MAX 2026

/*Funcao para descartar o restante da linha no stdin*/
void limpar_buffer_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* Apenas consome os caracteres restantes ate o Enter */
    }
}

/*
 * Funcao auxiliar que remove espacos em branco do inicio e do fim de uma string.
 */
void remover_espacos_extremidades(char *str) {
    int inicio = 0;
    int fim = (int)strlen(str) - 1;
    int i;

    /* Encontra o primeiro caractere que nao e espaco */
    while (str[inicio] != '\0' && isspace((unsigned char)str[inicio])) {
        inicio++;
    }

    /* Encontra o ultimo caractere que nao e espaco */
    while (fim >= inicio && isspace((unsigned char)str[fim])) {
        fim--;
    }

    /* Desloca os caracteres validos para o comeco da string */
    for (i = 0; i <= (fim - inicio); i++) {
        str[i] = str[inicio + i];
    }
    str[i] = '\0';
}

/*
 * Funcao para ler um numero inteiro dentro de um intervalo [minimo, maximo].
 * Rejeita letras, simbolos, espacos misturados, strings vazias e overflow.
 */
int ler_inteiro_intervalo(const char *mensagem, int minimo, int maximo) {
    char buffer[64];
    char *fim_conversao;
    long valor_lido;

    while (1) {
        printf("%s", mensagem);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Erro na leitura da entrada. Tente novamente.\n");
            clearerr(stdin);
            continue;
        }

        /* Verifica se o '\n' veio na leitura. */
        if (strchr(buffer, '\n') == NULL) {
            limpar_buffer_stdin();
            printf("Entrada muito longa! Digite um numero valido.\n");
            continue;
        }

        /* Remove espacos do inicio e do fim (inclusive o '\n') */
        remover_espacos_extremidades(buffer);

        if (buffer[0] == '\0') {
            printf("Entrada vazia! Digite um numero inteiro entre %d e %d.\n", minimo, maximo);
            continue;
        }

        /* Converte a string para long usando strtol na base 10 */
        errno = 0;
        valor_lido = strtol(buffer, &fim_conversao, 10);

        /* Se fim_conversao nao aponta para '\0', havia letras ou simbolos */
        if (*fim_conversao != '\0') {
            printf("Entrada invalida! Digite apenas numeros inteiros (sem letras ou simbolos).\n");
            continue;
        }

        /* Verifica se houve overflow ou fora do intervalo */
        if (errno == ERANGE || valor_lido < minimo || valor_lido > maximo) {
            printf("Valor fora do intervalo permitido (%d a %d).\n", minimo, maximo);
            continue;
        }

        return (int)valor_lido;
    }
}

/*a) Leitura de ID: aceita apenas inteiros positivos (> 0) ate INT_MAX.*/
int ler_id(const char *mensagem) {
    return ler_inteiro_intervalo(mensagem, 1, INT_MAX);
}

/*b) Leitura de Ano: aceita apenas inteiros no intervalo de 1 a 2026.*/
int ler_ano(const char *mensagem) {
    return ler_inteiro_intervalo(mensagem, ANO_MIN, ANO_MAX);
}

void ler_texto(const char *mensagem, char *destino, int tamanho_max) {
    /*
     * diferenciar:
     * - Uma entrada de exatos 79 caracteres seguida de '\n' (valida!)
     * - Uma entrada de 80 ou mais caracteres sem '\n' (invalida!)
     */
    char buffer_temp[TAM_TEXTO + 1];

    while (1) {
        printf("%s", mensagem);

        if (fgets(buffer_temp, sizeof(buffer_temp), stdin) == NULL) {
            printf("Erro na leitura da entrada. Tente novamente.\n");
            clearerr(stdin);
            continue;
        }

        /* Se o '\n' nao foi encontrado no buffer_temp, o usuario digitou mais de 79 caracteres */
        if (strchr(buffer_temp, '\n') == NULL) {
            limpar_buffer_stdin();
            printf("Texto muito longo! O limite maximo e de %d caracteres.\n", tamanho_max - 1);
            continue;
        }

        /* Remove o '\n' e eventuais espacos no inicio e no fim */
        remover_espacos_extremidades(buffer_temp);

        if (buffer_temp[0] == '\0') {
            printf("O campo nao pode ficar vazio ou conter apenas espacos!\n");
            continue;
        }

        /* Copia para o array de destino */
        strncpy(destino, buffer_temp, (size_t)tamanho_max - 1);
        destino[tamanho_max - 1] = '\0';
        break;
    }
}

void exibir_menu(void) {
    printf("\n========================================\n");
    printf("        SISTEMA DE BIBLIOTECA           \n");
    printf("========================================\n");
    printf("1 - Cadastrar registro na BST\n");
    printf("2 - Buscar registro na BST por ID\n");
    printf("3 - Remover registro da BST por ID\n");
    printf("4 - Listar registros da BST em-ordem\n");
    printf("5 - Inserir ID na estrutura linear (Fila)\n");
    printf("6 - Remover da estrutura linear (Fila)\n");
    printf("7 - Exibir estrutura linear (Fila)\n");
    printf("0 - Encerrar\n");
    printf("========================================\n");
}

int main(void) {
    int opcao;

    NoBST *raiz_catalogo = NULL;
    Fila fila_espera;

    fila_inicializar(&fila_espera);

    do {
        exibir_menu();
        opcao = ler_inteiro_intervalo("Escolha uma opcao: ", 0, 7);

        switch (opcao) {
            case 1: {
                Livro novo_livro;
                printf("\n--- [1 - Cadastrar Livro na BST] ---\n");
                novo_livro.id = ler_id("Digite o ID do livro (> 0): ");

                /* Verifica se o ID ja existe antes de pedir o restante dos dados */
                if (bst_buscar(raiz_catalogo, novo_livro.id) != NULL) {
                    printf("Erro: ja existe um livro cadastrado com o ID %d! Cadastro cancelado.\n",
                           novo_livro.id);
                    break;
                }

                ler_texto("Digite o titulo do livro: ", novo_livro.titulo, TAM_TEXTO);
                ler_texto("Digite o autor do livro: ", novo_livro.autor, TAM_TEXTO);
                novo_livro.ano = ler_ano("Digite o ano de publicacao (1 a 2026): ");

                raiz_catalogo = bst_inserir(raiz_catalogo, novo_livro);
                printf("Livro cadastrado com sucesso!\n");
                break;
            }
            case 2: {
                int id_busca;
                NoBST *encontrado;

                printf("\n--- [2 - Buscar Livro por ID] ---\n");
                if (raiz_catalogo == NULL) {
                    printf("O catalogo esta vazio! Nenhum livro cadastrado.\n");
                    break;
                }

                id_busca = ler_id("Digite o ID do livro que deseja buscar: ");
                encontrado = bst_buscar(raiz_catalogo, id_busca);

                if (encontrado != NULL) {
                    printf("\nLivro encontrado:\n");
                    printf("  ID:     %d\n", encontrado->dado.id);
                    printf("  Titulo: %s\n", encontrado->dado.titulo);
                    printf("  Autor:  %s\n", encontrado->dado.autor);
                    printf("  Ano:    %d\n", encontrado->dado.ano);
                } else {
                    printf("Livro com ID %d nao foi encontrado no catalogo.\n", id_busca);
                }
                break;
            }
            case 3: {
                int id_remover;
                NoBST *alvo;

                printf("\n--- [3 - Remover Livro por ID] ---\n");
                if (raiz_catalogo == NULL) {
                    printf("O catalogo esta vazio! Nenhum livro para remover.\n");
                    break;
                }

                id_remover = ler_id("Digite o ID do livro que deseja remover: ");
                alvo = bst_buscar(raiz_catalogo, id_remover);

                if (alvo == NULL) {
                    printf("Livro com ID %d nao foi encontrado no catalogo.\n", id_remover);
                } else {
                    raiz_catalogo = bst_remover(raiz_catalogo, id_remover);
                    printf("Livro com ID %d removido com sucesso!\n", id_remover);
                }
                break;
            }
            case 4:
                printf("\n--- [4 - Catalogo de Livros (Em-Ordem)] ---\n");
                if (raiz_catalogo == NULL) {
                    printf("O catalogo esta vazio! Nenhum livro para listar.\n");
                } else {
                    bst_listar_em_ordem(raiz_catalogo);
                }
                break;
            case 5: {
                int id_fila;
                printf("\n--- [5 - Inserir ID na Estrutura Linear (Enqueue)] ---\n");
                id_fila = ler_id("Digite o ID inteiro positivo para inserir na fila (> 0): ");

                if (fila_enqueue(&fila_espera, id_fila)) {
                    printf("ID %d inserido no final da fila com sucesso!\n", id_fila);
                }
                break;
            }
            case 6: {
                int id_removido;
                printf("\n--- [6 - Remover da Estrutura Linear (Dequeue)] ---\n");

                if (fila_dequeue(&fila_espera, &id_removido)) {
                    printf("ID %d removido do inicio da fila com sucesso!\n", id_removido);
                } else {
                    printf("A fila esta vazia! Nenhum ID para remover.\n");
                }
                break;
            }
            case 7:
                printf("\n--- [7 - Exibir Estrutura Linear (Fila)] ---\n");
                fila_exibir(&fila_espera);
                break;
            case 0:
                printf("\nLiberando memoria da BST e da Fila e encerrando o sistema. Ate mais!\n");
                bst_liberar(raiz_catalogo);
                raiz_catalogo = NULL;
                fila_liberar(&fila_espera);
                break;
            default:
                printf("\nOpcao invalida!\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}
