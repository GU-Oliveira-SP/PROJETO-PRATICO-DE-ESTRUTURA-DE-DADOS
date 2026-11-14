# Sistema de Biblioteca em C (BST + Fila Dinamica)

Projeto academico desenvolvido para a disciplina de **Estrutura de Dados** do curso de **Analise e Desenvolvimento de Sistemas (ADS — 3o Semestre)** da **FATEC Ribeirao Preto**.

- **Tema C — Biblioteca**: Gerenciamento de um catalogo de livros em memoria utilizando uma **Arvore Binaria de Busca (BST)** integrada a uma estrutura linear dinamica do tipo **Fila (Queue / FIFO)** independente para controle de IDs.

---

## Funcionalidades do Sistema

O programa opera via terminal por meio de um menu interativo com validacao rigorosa de entradas:

- `1` — **Cadastrar registro na BST**: Insere um novo livro (`id`, `titulo`, `autor`, `ano`), bloqueando IDs duplicados ou fora do intervalo valido (`1` a `INT_MAX`) e anos fora de `1` a `2026`.
- `2` — **Buscar registro na BST por ID**: Localiza e exibe os dados completos do livro pelo `id`.
- `3` — **Remover registro da BST por ID**: Exclui o livro tratando os 3 casos classicos de remocao (no folha, 1 filho e 2 filhos com sucessor em-ordem).
- `4` — **Listar registros da BST em-ordem**: Percorre a arvore em-ordem (`esquerda -> raiz -> direita`) exibindo os livros em ordem crescente de ID.
- `5` — **Inserir ID na estrutura linear (Fila)**: Executa `enqueue` no final da fila. A fila e independente da BST, permitindo IDs inexistentes na arvore e IDs repetidos.
- `6` — **Remover da estrutura linear (Fila)**: Executa `dequeue` removendo o elemento do inicio da fila (regra FIFO).
- `7` — **Exibir estrutura linear (Fila)**: Percorre e imprime todos os IDs da fila do inicio ao fim.
- `0` — **Encerrar**: Libera toda a memoria alocada dinamicamente pela BST (`bst_liberar`) e pela Fila (`fila_liberar`) antes de finalizar.

---

## Estrutura de Diretorios

```text
Projeto/
├── .gitignore
├── README.md
├── docs/
│   └── relatorio_base.md        # Texto-base do relatorio tecnico
├── include/
│   ├── bst.h                    # Structs Livro, NoBST e prototipos da BST
│   └── estrutura.h              # Structs NoFila, Fila e prototipos da Fila
├── src/
│   ├── main.c                   # Menu interativo e validacoes seguras de entrada
│   ├── bst.c                    # Implementacao das operacoes da BST
│   └── estrutura.c              # Implementacao das operacoes da Fila dinamica
└── tests/
    ├── roteiro_testes.md        # Roteiro detalhado e tabela de resultados (CT01 a CT11)
    └── entradas_teste.txt       # Sequencia de entradas para testes automatizados
```

---

## Ambiente e Compilacao

O projeto foi escrito em **C (padrao C11)** utilizando apenas bibliotecas padrao (`stdio.h`, `stdlib.h`, `string.h`, `ctype.h`, `limits.h`, `errno.h`).

### Compilacao no Linux / macOS

Abra o terminal na pasta raiz do projeto e execute:

```bash
gcc -Wall -Wextra -Iinclude src/main.c src/bst.c src/estrutura.c -o biblioteca
```

*(Opcionalmente, com flag explicita do padrao C11: `gcc -std=c11 -Wall -Wextra -Iinclude src/main.c src/bst.c src/estrutura.c -o biblioteca`)*

Para executar o programa interativamente:

```bash
./biblioteca
```

### Compilacao no Windows (MinGW / GCC)

Certifique-se de ter o **MinGW-w64** (ou MSYS2/GCC) instalado e adicionado ao `PATH` do sistema. No Prompt de Comando (CMD) ou PowerShell, na raiz do projeto, execute:

```bash
gcc -Wall -Wextra -Iinclude src/main.c src/bst.c src/estrutura.c -o biblioteca.exe
```

Para executar o programa interativamente:

- **CMD:** `biblioteca.exe`
- **PowerShell:** `.\biblioteca.exe`

### Compilacao no Windows (Visual Studio / MSVC Developer Command Prompt)

Caso utilize o compilador do **Visual Studio (Build Tools / MSVC)**, abra o **Developer Command Prompt for VS** (ou **x64 Native Tools Command Prompt for VS**) na raiz do projeto e execute:

```cmd
cl /std:c11 /W4 /D_CRT_SECURE_NO_WARNINGS /I include src\main.c src\bst.c src\estrutura.c /Fe:biblioteca.exe
```

Para executar o programa interativamente:

- **CMD:** `biblioteca.exe`
- **PowerShell:** `.\biblioteca.exe`

---

## Como Reproduzir os Testes e Verificar Memoria

### 1. Testes Manuais
Consulte a tabela completa de casos de teste (`CT01` a `CT11`) no arquivo [tests/roteiro_testes.md](tests/roteiro_testes.md). Nele estao descritos os passos para testar entradas invalidas, insercoes, remocoes dos 3 casos da BST e operacoes FIFO da Fila.

### 2. Teste Automatizado por Redirecionamento de Entrada
O arquivo [tests/entradas_teste.txt](tests/entradas_teste.txt) contem uma sequencia completa e deterministica de entradas simulando todos os cenarios de validacao, operacoes na BST, operacoes na Fila e encerramento (`0`).

- **Linux / macOS / Git Bash:**
  ```bash
  ./biblioteca < tests/entradas_teste.txt
  ```

- **Windows (CMD):**
  ```cmd
  biblioteca.exe < tests\entradas_teste.txt
  ```

- **Windows (PowerShell):**
  ```powershell
  Get-Content tests\entradas_teste.txt | .\biblioteca.exe
  ```

### 3. Verificacao de Vazamento de Memoria (Memory Leak)
No encerramento do programa (`0 - Encerrar`), todas as alocacoes sao liberadas pelas funcoes `bst_liberar()` (percurso em pos-ordem) e `fila_liberar()` (percurso sequencial com `free`). Para comprovar ausencia de vazamentos de memoria em ambiente Linux com **Valgrind**, execute:

```bash
gcc -g -std=c11 -Wall -Wextra -Iinclude src/main.c src/bst.c src/estrutura.c -o biblioteca
valgrind --leak-check=full --show-leak-kinds=all ./biblioteca < tests/entradas_teste.txt
```

O resultado esperado no relatorio final do Valgrind e `All heap blocks were freed -- no leaks are possible` (`0 bytes in 0 blocks` em `definitely lost` e `indirectly lost`).
