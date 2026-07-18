#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

#define MAX_SIZE 256
#define MAX_RESULT 32
#define STRING_SIZE 64
#define negativo '_'
#define MAX_EXPR_SIZE 256
#define MAX_FATORIAL_SIZE 3
#define PRECISION "%.15lf"
#define EPSILON 1e-9

typedef enum {
    ERRO_NENHUM = 0,
    ERRO_SINTAXE,
    ERRO_CONVERSAO,
    ERRO_PARENTESES,
    ERRO_DIV_ZERO,
    ERRO_FATORIAL_NEGATIVO,
    ERRO_EXPRESSAO_VAZIA
} TipoErro;

typedef struct ERRO {
    TipoErro tipo;
    char detalhe[STRING_SIZE];
} Erro;


typedef struct SeparatorReturns {
    double number;
    int position;
    bool acabou;
} SeparatorReturns;


void substituir_na_memoria(char *str, size_t posicao, size_t tamanho_antigo, const char *texto_novo);
void separator(char *s, SeparatorReturns *expr, int pos, Erro *erro);
void num_fmt(char *buffer, size_t size, double num);
bool eh_precedente_negativo(char c);
double operador(char *s, Erro *erro);
void remover_espacos(char *s);
double fatoracao(double n, Erro *erro);
bool eh_operador(char c);

void limpa_erro(Erro *e);
void seta_erro(Erro *e, TipoErro tipo, const char *fmt, ...);

#endif