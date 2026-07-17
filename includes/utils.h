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
#define negativo '_'
#define MAX_EXPR_SIZE 256
#define MAX_FATORIAL_SIZE 3
#define PRECISION "%.15lf"
#define EPSILON 1e-9

#include "debug.h"

typedef struct SeparatorReturns {
    double number;
    int position;
    bool acabou;
} SeparatorReturns;


void substituir_na_memoria(char *str, size_t posicao, size_t tamanho_antigo, const char *texto_novo);
void separator(char *s, SeparatorReturns *expr, int pos);
void num_fmt(char *buffer, size_t size, double num);
bool eh_precedente_negativo(char c);
double operador(char *s);
void remover_espacos(char *s);
double fatoracao(double n);
bool eh_operador(char c);

#endif