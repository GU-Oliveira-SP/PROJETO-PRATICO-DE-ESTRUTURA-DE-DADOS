# Roteiro de Testes — Sistema de Biblioteca (BST + Fila Dinamica)

Este documento descreve os cenarios de teste reproduziveis do projeto academico **Tema C — Biblioteca**, validando as operacoes da Arvore Binaria de Busca (BST), da Fila dinamica (FIFO), as regras de entrada segura e a liberacao de memoria.

---

## 1. Como Compilar e Executar os Testes Automatizados

### Compilacao (GCC / MinGW)
```bash
gcc -std=c11 -Wall -Wextra -Iinclude src/*.c -o biblioteca
```

### Execucao via Redirecionamento de Entrada

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

- **Verificacao de Vazamento de Memoria (Linux com Valgrind):**
  ```bash
  valgrind --leak-check=full --show-leak-kinds=all ./biblioteca < tests/entradas_teste.txt
  ```

---

## 2. Topologia da Arvore BST nos Testes

Durante os testes (`CT04`), sao inseridos os IDs `50, 30, 70, 20, 40, 80`, formando uma arvore que contempla todos os tipos de nos (folha, 1 filho e 2 filhos):

```text
        [50]              <-- Raiz (2 filhos)
       /    \
    [30]    [70]          <-- [30] tem 2 filhos; [70] tem 1 filho (direita)
    /  \       \
 [20]  [40]    [80]       <-- [20], [40] e [80] sao nos folha (0 filhos)
```

Apos a sequencia de remocoes no `CT08` (`20` -> folha, `70` -> 1 filho, `50` -> 2 filhos substituido pelo sucessor em-ordem `80`), a arvore passa a ter a seguinte estrutura:

```text
        [80]              <-- Nova raiz (sucessor em-ordem do 50)
       /
    [30]
       \
       [40]
```

---

## 3. Tabela de Casos de Teste

| ID do Teste | Caso de Teste | Passos / Entradas | Resultado Esperado | Resultado Obtido | Status |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **CT01** | Operacoes em estruturas vazias | Executar opcoes `2` (buscar BST), `3` (remover BST), `4` (listar BST), `6` (dequeue Fila) e `7` (exibir Fila) antes de qualquer insercao. | O sistema deve informar que o catalogo e a fila estao vazios, sem travar ou encerrar. | Exibiu: `O catalogo esta vazio! Nenhum livro cadastrado.`, `O catalogo esta vazio! Nenhum livro para remover.`, `O catalogo esta vazio! Nenhum livro para listar.`, `A fila esta vazia! Nenhum ID para remover.` e `A fila esta vazia! Nenhum ID para exibir.` | Passou |
| **CT02** | Validacao de IDs invalidos | Na opcao `1`, testar os IDs: `0`, `-10`, `abc`, `12abc` e `999999999999` (acima de `INT_MAX`). | Rejeitar todos os valores invalidos e solicitar novo ID ate receber um inteiro positivo valido (`50`). | Exibiu `Valor fora do intervalo permitido (1 a 2147483647).` para `0`, `-10` e `999999999999`; exibiu `Entrada invalida! Digite apenas numeros inteiros (sem letras ou simbolos).` para `abc` e `12abc`. Aceitou `50`. | Passou |
| **CT03** | Validacao de texto (trim e limite de 79 chars) e ano | Ainda na opcao `1` (ID `50`): testar titulo vazio (`""`), apenas espacos (`"     "`), 80 letras `'A'`, depois `"  Algoritmos e Estruturas de Dados  "`. Autor: `"Thomas Cormen"`. Testar anos `0`, `-5`, `2027`, `"dois mil"`, e por fim `2009`. | Rejeitar campos vazios/so espacos e textos > 79 chars (limpando buffer); aplicar trim nas extremidades; rejeitar anos fora de `1..2026` ou nao numericos. | Exibiu `O campo nao pode ficar vazio ou conter apenas espacos!`, `Texto muito longo! O limite maximo e de 79 caracteres.`, aplicou trim salvando `"Algoritmos e Estruturas de Dados"`, rejeitou os anos invalidos e cadastrou o livro com ano `2009`. | Passou |
| **CT04** | Insercao de multiplos livros na BST | Cadastrar via opcao `1` os livros com IDs `30` (`Codigo Limpo`, 2008), `70` (`O Programador Pragmatico`, 1999), `20` (`Linguagem C`, 1988), `40` (`Padroes de Projeto`, 1994) e `80` (`Engenharia de Software`, 2011). | Todos os registros devem ser inseridos respeitando a propriedade da BST (`esquerda < raiz < direita`). | Todos os 5 registros adicionais retornaram `Livro cadastrado com sucesso!`. | Passou |
| **CT05** | Tentativa de insercao de ID duplicado | Na opcao `1`, informar o ID `30` (ja existente na BST). | Rejeitar o cadastro imediatamente antes de solicitar titulo, autor e ano, exibindo erro. | Exibiu `Erro: ja existe um livro cadastrado com o ID 30! Cadastro cancelado.` e retornou ao menu. | Passou |
| **CT06** | Listagem Em-Ordem da BST | Executar a opcao `4`. | Exibir os 6 livros ordenados de forma crescente pelo ID (`20, 30, 40, 50, 70, 80`) com o titulo de ID `50` ja sem espacos extras do trim. | Listou exatamente na ordem `20`, `30`, `40`, `50` (`"Algoritmos e Estruturas de Dados"`), `70`, `80`. | Passou |
| **CT07** | Busca por ID existente e inexistente | Executar opcao `2` buscando ID `40` e depois opcao `2` buscando ID `99`. | Para `40`, exibir todos os campos formatados (`ID`, `Titulo`, `Autor`, `Ano`). Para `99`, informar que nao foi encontrado. | Para `40` exibiu `Padroes de Projeto` / `Erich Gamma` / `1994`; para `99` exibiu `Livro com ID 99 nao foi encontrado no catalogo.` | Passou |
| **CT08** | Remocao na BST (inexistente + 3 casos) e nova listagem | Executar opcao `3` para: `99` (inexistente), `20` (Caso 1: no folha), `70` (Caso 2: 1 filho), `50` (Caso 3: 2 filhos, raiz), e depois opcao `4`. | Informar erro para `99`; remover `20`, `70` e `50` preservando a BST; listar `30, 40, 80` em ordem crescente. | Exibiu `Livro com ID 99 nao foi encontrado no catalogo.`, removeu `20`, `70` e `50` com sucesso e a listagem exibiu apenas `30`, `40`, `80`. | Passou |
| **CT09** | Fila Dinamica — Insercao (`enqueue`) e exibicao | Executar opcao `5` inserindo `30` (existe na BST), `999` (nao existe na BST), `30` (repetido) e `50` (ja removido da BST); depois executar opcao `7`. | Aceitar todos os IDs positivos independentemente da BST (inclusive repetidos) e exibir do inicio ao fim. | Todos inseridos com sucesso e a opcao `7` exibiu: `Fila (Inicio -> Fim): [30] -> [999] -> [30] -> [50]`. | Passou |
| **CT10** | Fila Dinamica — Remocao (`dequeue`) FIFO e fila esvaziada | Executar opcao `6` duas vezes, executar opcao `7`, executar opcao `6` tres vezes (a ultima ja com fila vazia). | Remover na ordem FIFO (`30`, depois `999`), exibir `[30] -> [50]`, remover `30` e `50`, e na terceira tentativa informar fila vazia. | Removeu `30`, depois `999`; opcao `7` exibiu `[30] -> [50]`; removeu `30`, depois `50`; e por fim exibiu `A fila esta vazia! Nenhum ID para remover.` | Passou |
| **CT11** | Encerramento com liberacao de memoria | Inserir ID `100` na fila (opcao `5`) para garantir existencia de nos alocados tanto na BST (`30, 40, 80`) quanto na Fila (`100`), e selecionar opcao `0`. | Executar `bst_liberar` e `fila_liberar`, exibir mensagem de despedida e encerrar com codigo `0`. | Exibiu `Liberando memoria da BST e da Fila e encerrando o sistema. Ate mais!` e finalizou com codigo de saida `0`. | Passou |
