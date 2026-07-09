# C-calc

Calculadora de expressões aritméticas feita em C puro, sem bibliotecas de parsing externas. O programa lê uma expressão digitada pelo usuário (ex: `2+3*(4-1)!`) e resolve respeitando precedência de operadores, parênteses aninhados, números negativos e fatorial.

## Funcionalidades

- **Operadores básicos**: `+`, `-`, `*`, `/`, `%`
- **Precedência de operadores**: multiplicação e divisão são resolvidas antes de soma e subtração
- **Parênteses aninhados**: `(1+2)*(3+4)`, incluindo parênteses dentro de parênteses
- **Números negativos**: unário, tanto no início da expressão quanto após outro operador ou parêntese (`-5+3`, `5*-3`, `(-3+5)*2`)
- **Fatorial**: `5!`, incluindo fatorial de sub-expressões entre parênteses (`(3+2)!`)
- **Números com ponto flutuante**: `1.5*2`, `10/3`
- **Formatação de saída**: remove zeros e pontos decimais desnecessários no resultado (ex: `6` em vez de `6.000000000000000`)
- **Suíte de testes embutida**: roda automaticamente na inicialização (pode ser desligada, veja abaixo)

## Como funciona

A expressão passa por uma sequência de "interpretadores", cada um responsável por resolver uma camada da expressão antes de passar a próxima:

1. `remover_espacos` — remove espaços em branco da entrada
2. `interpretar_negativos` — marca sinais de menos que representam número negativo (não subtração), trocando o caractere por um marcador interno (`_`)
3. `interpretador_parenteses` — resolve recursivamente parênteses, do mais interno para o mais externo
4. `interpretador_fatorial` — resolve expressões com `!`
5. `interpretador_prioritario` — resolve multiplicações e divisões antes de somas/subtrações
6. `operador` — percorre o que sobrou da expressão da esquerda para a direita, aplicando os operadores restantes (`+`, `-`, `%`) em sequência

Cada sub-expressão resolvida é substituída na string original (função `substituir_na_memoria`), e o processo se repete até restar apenas um número.

## Como compilar e rodar

Requer apenas um compilador C (gcc, clang, etc.) e a biblioteca matemática padrão:

```bash
gcc -o calc main.c -lm
./calc
```

Exemplo de uso:

```
Calculadora inteligente
>    2*(3+4)
Resultado: 14
>    5!+1
Resultado: 121
>    -3*-3
Resultado: 9
```

## Modo de testes e debug

O arquivo define algumas flags no topo que controlam o comportamento em tempo de compilação:

```c
#define TESTING_DEBUG
// #define OPERATION_DEBUG
```

- **`TESTING_DEBUG`**: quando definida, roda automaticamente uma bateria de testes (`rodar_testes`) antes de abrir o prompt interativo, cobrindo operações básicas, precedência, parênteses, negativos, floats e fatorial. Comente a linha para pular os testes.
- **`OPERATION_DEBUG`**: quando definida, imprime logs detalhados (`DEBUG_LOG`) de cada etapa da interpretação da expressão — útil para depurar o parser. Descomente a linha para ativar.

## Limitações conhecidas

- Tamanho máximo de expressão: 100 caracteres (`MAX_SIZE`)
- Divisão e módulo por zero são tratados com mensagem de erro e retornam `0`, mas não interrompem o programa
- Fatorial de número negativo retorna `0` com mensagem de aviso
- Fatorial é calculado com `double`, então valores muito grandes podem perder precisão

## Estrutura do projeto

```
C-calc/
├── main.c      # Todo o código-fonte da calculadora
└── README.md
```

## Possíveis melhorias futuras

- Suporte a expoentes (`^` ou `**`)
- Suporte a funções matemáticas (`sin`, `cos`, `sqrt`, etc.)
- Separar o código em múltiplos arquivos (`.h`/`.c`) por responsabilidade
- Tratar erros de sintaxe da expressão de forma mais robusta