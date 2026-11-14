# Relatorio Tecnico — Sistema de Biblioteca em C (BST e Fila Dinamica)

**Instituicao:** FATEC Ribeirao Preto  
**Curso:** Tecnologia em Analise e Desenvolvimento de Sistemas (ADS — 3o Semestre)  
**Disciplina:** Estrutura de Dados  
**Tema:** Tema C — Biblioteca  

---

## 1. Tema e Modelagem dos Dados

O projeto implementa um sistema de gerenciamento de biblioteca via linha de comando na linguagem C (padrao C11), combinando duas estruturas de dados dinamicas: uma **Arvore Binaria de Busca (BST)** para o catalogo principal de livros e uma **Fila dinamica (FIFO)** independente para registro de IDs em ordem de chegada.

### 1.1 Estruturas Definidas

No cabecalho `include/bst.h`, o registro principal e o no da arvore foram modelados da seguinte forma:

- **`Livro`**: agrupa os dados de cada exemplar:
  - `int id`: identificador numerico unico e positivo (`1` ate `INT_MAX`).
  - `char titulo[80]`: armazena o titulo com limite de ate 79 caracteres uteis mais o terminador nulo `'\0'`.
  - `char autor[80]`: armazena o nome do autor com limite de ate 79 caracteres uteis mais `'\0'`.
  - `int ano`: ano de publicacao validado no intervalo de `1` a `2026`.
- **`NoBST`**: representa cada no da arvore, contendo `Livro dado` e os ponteiros `struct NoBST *esquerda` e `struct NoBST *direita`.

No cabecalho `include/estrutura.h`, a estrutura linear foi modelada de maneira modular e desacoplada:

- **`NoFila`**: armazena exclusivamente `int id` e o ponteiro `struct NoFila *proximo`.
- **`Fila`**: armazena os dois ponteiros de controle `NoFila *inicio` e `NoFila *fim`, permitindo insercao e remocao sem percorrer a estrutura.

### 1.2 Validacao Segura de Entrada

Para evitar falhas comuns do uso de `scanf` (como travamentos ao digitar letras em campos numericos ou estouro de buffer), todas as leituras em `src/main.c` utilizam `fgets` combinado com `strtol`:

- **`ler_inteiro_intervalo` (`ler_id` e `ler_ano`)**: verifica se o `'\n'` foi lido (caso contrario chama `limpar_buffer_stdin`), remove espacos das extremidades com `remover_espacos_extremidades`, rejeita entradas vazias, usa `strtol` na base 10 checando se `*fim_conversao == '\0'` (bloqueando misturas como `"12abc"` ou letras como `"abc"`) e verifica `errno == ERANGE` junto aos limites (`1` a `INT_MAX` para ID; `1` a `2026` para Ano).
- **`ler_texto`**: le em um buffer temporario de 81 posicoes (`TAM_TEXTO + 1`) para diferenciar com precisao textos de ate 79 caracteres de entradas maiores. Se exceder 79 caracteres, limpa o restante da linha no `stdin` e solicita nova entrada; apos o *trim*, tambem rejeita strings vazias ou formadas apenas por espacos.

---

## 2. Funcionamento da Arvore Binaria de Busca (BST)

A BST organiza os registros respeitando a propriedade fundamental de ordenacao pelo campo `id`: para qualquer no, todos os IDs na subarvore `esquerda` sao estritamente menores que o ID da raiz local, e todos os IDs na subarvore `direita` sao estritamente maiores.

- **Insercao (`bst_inserir`)**: antes da chamada na `main`, o sistema verifica com `bst_buscar` se o `id` ja existe; caso exista, cancela o cadastro imediatamente para evitar duplicidade. Quando o ID e inedito, `bst_inserir` desce recursivamente comparando `novo_livro.id` com `raiz->dado.id` ate encontrar um ponteiro `NULL`, momento em que a funcao auxiliar `criar_no` aloca um `NoBST` com `malloc` e inicializa seus ponteiros filhos como `NULL`.
- **Busca (`bst_buscar`)**: recebe a raiz e o `id` procurado. Se a raiz for `NULL` ou `raiz->dado.id == id`, retorna o ponteiro encontrado (ou `NULL`). Caso contrario, chama a si mesma para `esquerda` (se `id < raiz->dado.id`) ou para `direita` (se `id > raiz->dado.id`).
- **Listagem Em-Ordem (`bst_listar_em_ordem`)**: aplica o percurso recursivo classico **Esquerda -> Raiz -> Direita**. Como os menores valores ficam sempre a esquerda e os maiores a direita, esse percurso garante a impressao de todos os livros em ordem crescente de ID.
- **Remocao (`bst_remover`)**: apos validar na `main` que a arvore nao esta vazia e que o ID existe, a funcao desce recursivamente ate o no alvo e trata os tres casos classicos da BST:
  1. **Caso 1 — No folha (0 filhos):** quando `esquerda == NULL` e `direita == NULL`, o no e liberado com `free(raiz)` e a funcao retorna `NULL` para o ponteiro pai.
  2. **Caso 2 — No com 1 filho:** guarda-se o ponteiro do unico filho existente (direito ou esquerdo) em uma variavel auxiliar `filho`, executa-se `free(raiz)` e retorna-se `filho`, reconectando a subarvore diretamente ao avo.
  3. **Caso 3 — No com 2 filhos:** utiliza a funcao auxiliar `encontrarMinimo(raiz->direita)` para caminhar o maximo possivel a esquerda na subarvore direita, encontrando o **sucessor em-ordem** (menor ID maior que o no removido). Em seguida, copia-se a struct `Livro` do sucessor para `raiz->dado` e chama-se `bst_remover(raiz->direita, sucessor->dado.id)` recursivamente para remover o no original do sucessor (que tera no maximo 1 filho a direita).
- **Liberacao de Memoria (`bst_liberar`)**: executada ao encerrar o programa (Opcao 0) em **Pos-Ordem** (`esquerda -> direita -> free(raiz)`), garantindo que nenhum ponteiro filho seja perdido antes da liberacao do no pai.

---

## 3. Funcionamento da Estrutura Linear Dinamica (Fila / Queue)

A fila implementada em `src/estrutura.c` segue a politica **FIFO (*First In, First Out*)**: o primeiro ID inserido e obrigatoriamente o primeiro a ser removido.

- **Inicializacao (`fila_inicializar`)**: define `f->inicio = NULL` e `f->fim = NULL` ao iniciar a funcao `main`.
- **Insercao (`fila_enqueue` — Opcao 5)**: aloca um `NoFila` com `malloc`, atribui o `id` validado e define `proximo = NULL`. Se a fila estiver vazia (`f->inicio == NULL`), ambos os ponteiros `inicio` e `fim` passam a apontar para o novo no. Caso contrario, o novo no e encadeado em `f->fim->proximo = novo` e `f->fim` passa a apontar para ele.
- **Remocao (`fila_dequeue` — Opcao 6)**: se `f->inicio == NULL`, retorna `0` para sinalizar fila vazia sem interromper o programa. Caso contrario, salva o `id` do primeiro no em `*id_removido`, avanca `f->inicio = removido->proximo`, verifica se `f->inicio` se tornou `NULL` (ajustando `f->fim = NULL` para manter a consistencia da fila vazia) e libera o no removido com `free`.
- **Exibicao e Liberacao (`fila_exibir` e `fila_liberar`)**: percorrem os nos sequencialmente a partir de `f->inicio` usando um ponteiro auxiliar ate atingir `NULL`. Na opcao `0`, `fila_liberar` percorre todos os nos restantes aplicando `free` e zera `inicio` e `fim`.

**Independencia em relacao a BST:** a fila armazena apenas inteiros positivos (`int id`). Ela nao consulta a BST ao enfileirar, aceita IDs que nao estao cadastrados na arvore (como `999` nos testes), permite IDs repetidos (como enfileirar `30` duas vezes) e preserva seus nos mesmo quando um livro com o mesmo ID e removido da arvore.

---

## 4. Exemplos de Testes Executados

Todos os cenarios foram documentados em `tests/roteiro_testes.md` e validados de forma reproduzivel por meio de redirecionamento de entrada com o arquivo `tests/entradas_teste.txt`.

| Caso | Cenario Testado | Entradas Principais | Resultado Obtido |
| :--- | :--- | :--- | :--- |
| **CT01** | Estruturas vazias | Opcoes `2`, `3`, `4` (BST) e `6`, `7` (Fila) no inicio | Mensagens informando catalogo/fila vazios sem erros ou travamentos |
| **CT02** | Validacao de ID | `0`, `-10`, `"abc"`, `"12abc"`, `999999999999`, depois `50` | Valores invalidos e overflow rejeitados; apenas `50` aceito |
| **CT03** | Validacao de texto e ano | Titulo vazio, so espacos, 80 chars, `"  Algoritmos...  "`, anos `0`, `-5`, `2027`, `"dois mil"`, `2009` | Rejeitou invalidos, limpou `stdin` no excesso, aplicou *trim* no titulo e salvou `2009` |
| **CT04** | Construcao da BST | IDs `50`, `30`, `70`, `20`, `40`, `80` | Todos inseridos formando nos folha (`20, 40, 80`), 1 filho (`70`) e 2 filhos (`30, 50`) |
| **CT05** | ID duplicado na BST | Opcao `1` com ID `30` | Cadastro cancelado imediatamente com aviso de ID duplicado |
| **CT06** | Listagem Em-Ordem | Opcao `4` | Exibicao em ordem crescente: `20, 30, 40, 50, 70, 80` |
| **CT07** | Busca na BST | Opcao `2` com ID `40` e ID `99` | Exibiu dados completos do `40`; informou que `99` nao foi encontrado |
| **CT08** | Remocao dos 3 casos | Opcao `3` com `99`, `20` (folha), `70` (1 filho) e `50` (2 filhos) + Opcao `4` | `50` substituido pelo sucessor `80`; nova listagem em-ordem exibiu `30, 40, 80` |
| **CT09** | Fila independente (`enqueue`) | Opcao `5` com `30`, `999`, `30`, `50` + Opcao `7` | Aceitou repetido e inexistente na BST; exibiu `[30] -> [999] -> [30] -> [50]` |
| **CT10** | Remocao FIFO (`dequeue`) | Opcao `6` repetida + Opcao `7` + `6` em fila vazia | Removeu exatamente `30`, `999`, `30`, `50`; ao esvaziar informou fila vazia |
| **CT11** | Encerramento seguro | Insercao de `100` na Fila seguida da Opcao `0` | Executou `bst_liberar` e `fila_liberar`, encerrando com codigo `0` |

---

## 5. Analise de Complexidade Assintotica (Big-O)

Considerando $n$ o numero de elementos armazenados na respectiva estrutura no momento da operacao:

| Estrutura | Operacao | Caso Medio | Pior Caso | Observacao |
| :--- | :--- | :---: | :---: | :--- |
| **BST** | Insercao (`bst_inserir`) | $O(\log n)$ | $O(n)$ | Depende da altura $h$ da arvore |
| **BST** | Busca (`bst_buscar`) | $O(\log n)$ | $O(n)$ | Descarta metade dos nos por nivel se equilibrada |
| **BST** | Remocao (`bst_remover`) | $O(\log n)$ | $O(n)$ | Inclui busca do no e eventual busca do sucessor |
| **BST** | Listagem Em-Ordem (`bst_listar_em_ordem`) | $O(n)$ | $O(n)$ | Visita obrigatoriamente todos os $n$ nos |
| **BST** | Liberacao (`bst_liberar`) | $O(n)$ | $O(n)$ | Percorre e libera todos os $n$ nos em pos-ordem |
| **Fila** | Insercao no fim (`fila_enqueue`) | $O(1)$ | $O(1)$ | Acesso direto via ponteiro `fim` |
| **Fila** | Remocao do inicio (`fila_dequeue`) | $O(1)$ | $O(1)$ | Acesso direto via ponteiro `inicio` |
| **Fila** | Exibicao (`fila_exibir`) / Liberacao (`fila_liberar`) | $O(n)$ | $O(n)$ | Percorre linearmente do `inicio` ate `NULL` |

### Explicacao das Complexidades

- **BST — $O(\log n)$ vs. $O(n)$:** Quando os IDs sao inseridos de forma distribuida, a altura da arvore fica proxima de $\log_2 n$, fazendo com que cada comparacao elimine aproximadamente metade dos nos restantes e garanta desempenho $O(\log n)$ na busca, insercao e remocao. Contudo, no **pior caso** (insercao de IDs em ordem estritamente crescente ou decrescente, como `1, 2, 3, ..., n`), a BST degenera em uma lista encadeada de altura $n$, elevando essas operacoes para $O(n)$. Ja a listagem em-ordem e a liberacao sempre visitam todos os elementos existentes, resultando em $O(n)$ em qualquer cenario.
- **Fila — $O(1)$ e $O(n)$:** Como a struct `Fila` mantem simultaneamente os ponteiros `inicio` e `fim`, nao e necessario percorrer a lista para inserir no final (`enqueue`) nem para remover do comeco (`dequeue`). Ambas as operacoes executam um numero fixo de atribuicoes de ponteiros, mantendo complexidade constante $O(1)$. Apenas a exibicao completa e a liberacao final exigem percorrer todos os nos armazenados, com custo linear $O(n)$.

---

## 6. Conclusao, Limitacoes e Declaracao de Uso de IA

### 6.1 Conclusao
O desenvolvimento do projeto permitiu consolidar na pratica conceitos fundamentais de Estrutura de Dados e programacao em C: manipulacao segura de ponteiros, alocacao e liberacao dinamica de memoria (`malloc` e `free`), uso de recursao em arvores binarias (especialmente na identificacao de sucessores e reconexao de subarvores na remocao), alem da modularizacao de codigo em arquivos `.h` e `.c` e do tratamento defensivo de entradas de terminal.

### 6.2 Limitacoes do Projeto
1. **Armazenamento apenas em memoria volatil (RAM):** nao ha persistencia em arquivos (`.txt`, `.csv` ou binario); ao encerrar o programa, os registros cadastrados sao descartados apos a liberacao da memoria.
2. **Ausencia de balanceamento automatico na BST:** a arvore segue o modelo classico nao balanceado. Caso o usuario cadastre muitos livros em ordem sequencial de ID, a arvore pode ficar desbalanceada (diferentemente de arvores AVL ou Rubro-Negras).

### 6.3 Declaracao de Uso de Inteligencia Artificial
Em conformidade com as diretrizes academicas da disciplina, declara-se que ferramentas de Inteligencia Artificial foram utilizadas como **apoio didatico** durante o desenvolvimento deste trabalho — especificamente para revisao da organizacao modular dos cabeçalhos, estruturacao das rotinas de validacao segura de entrada com `fgets`/`strtol`, elaboracao do roteiro de testes automatizados e revisao textual da documentacao. Toda a logica das estruturas de dados, os casos de teste e o comportamento do codigo foram integralmente revisados, testados e compreendidos pelo autor para apresentacao e defesa tecnica.
