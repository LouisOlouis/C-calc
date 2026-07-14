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

// #define TESTING_DEBUG
// #define OPERATION_DEBUG

#define MAX_SIZE 256
#define MAX_RESULT 32
#define negativo '_'
#define MAX_EXPR_SIZE 256
#define MAX_FATORIAL_SIZE 3
#define PRECISION "%.15lf"
#define EPSILON 1e-9

typedef struct SeparatorReturns {
    double number;
    int position;
    bool acabou;
} SeparatorReturns;

typedef struct ExpressaoParenteses{
    int inicio;
    int profundidade;
    int tamanho;
    char expressao[MAX_SIZE];
    bool capturando;
} ExpressaoParenteses;

typedef struct ExpressaoPrioritaria{
    int inicio;
    int tamanho;
    char expressao[MAX_EXPR_SIZE];
    bool capturando;
} ExpressaoPrioritaria;

typedef struct ExpressaoFatorial{
    int inicio;
    int tamanho;
    char expressao[MAX_FATORIAL_SIZE];
    bool capturando;
} ExpressaoFatorial;

void substituir_na_memoria(char *str, size_t posicao, size_t tamanho_antigo, const char *texto_novo);
void resolver_parenteses(char *s, ExpressaoParenteses *expr, int fechamento);
void iniciar_captura_parenteses(ExpressaoParenteses *expr, int posicao);
void adicionar_caractere_parenteses(ExpressaoParenteses *expr, char c);
void iniciar_captura_fatorial(ExpressaoFatorial *expr, int posicao);
void adicionar_caractere_fatorial(ExpressaoFatorial *expr, char c);
void resolver_expressao_fatorial(char *s, ExpressaoFatorial *expr);
void resolver_expressao(char *s, ExpressaoPrioritaria *expr);
void iniciar_captura(ExpressaoPrioritaria *expr, int inicio);
void adicionar_caractere(ExpressaoPrioritaria *expr, char c);
SeparatorReturns separator(char s[MAX_SIZE], int pos);
void num_fmt(char *buffer, size_t size, double num);
bool eh_expressao_prioritaria(char operador);
void interpretador_prioritario(char *s);
void interpretador_parenteses(char *s);
void interpretador_fatorial(char *s);
void interpretar_negativos(char *s);
bool eh_precedente_negativo(char c);
double operador(char *s);
void remover_espacos(char *s);
double fatoracao(double n);
bool eh_operador(char c);

#ifdef OPERATION_DEBUG
    #define DEBUG_LOG(...) do { printf("[DEBUG] " __VA_ARGS__); printf("\n"); } while (0)
#else
    #define DEBUG_LOG(...) ((void)0)
#endif

#ifdef TESTING_DEBUG

    void testar(char *expressao, double esperado) {
        char s[MAX_SIZE];
        strncpy(s, expressao, MAX_SIZE - 1);
        s[MAX_SIZE - 1] = '\0';

        remover_espacos(s);
        interpretar_negativos(s);
        interpretador_parenteses(s);
        interpretador_fatorial(s);
        interpretador_prioritario(s);
        double resultado = operador(s);

        bool passou = fabs(resultado - esperado) < EPSILON;

        printf("[%s] %-20s -> obtido: %-25.15g esperado: %g\n",
            passou ? "OK    " : "FALHOU",
            expressao,
            resultado,
            esperado
        );
    }

    void rodar_testes(void) {
        printf("\n========== TESTES ==========\n");

        printf("\n-- Basicos --\n");
        testar("2+3",          5);
        testar("10-4",         6);
        testar("3*4",          12);
        testar("10/4",         2.5);
        testar("10%3",         1);

        printf("\n-- Precedencia --\n");
        testar("1+2*3",        7);
        testar("2*3+1",        7);
        testar("2*3+4*5",      26);
        testar("10-2*3+1",     5);

        printf("\n-- Parenteses --\n");
        testar("(2+3)*4",      20);
        testar("2*(3+4)",      14);
        testar("(1+2)*(3+4)",  21);
        testar("1+2*(3+4)",    15);

        printf("\n-- Negativos --\n");
        testar("-5+3",         -2);
        testar("5*-3",         -15);
        testar("(-3+5)*2",     4);
        testar("-3*-3",        9);

        printf("\n-- Floats --\n");
        testar("1/2",          0.5);
        testar("1.5*2",        3);
        testar("10/3",         3.33333333333333);

        printf("\n-- Fatorial --\n");
        testar("5!",           120);
        testar("0!",           1);
        testar("3!+2",         8);
        testar("2*3!",         12);
        testar("(3+2)!",       120);
        testar("3!*2!",        12);

        printf("\n-- Elevados --\n");
        testar("2^3",          8);
        testar("3^2",          9);
        testar("2^3+1",        9);
        testar("2^3^2",       64);

        printf("\n============================\n\n");
    }

#else
    #define rodar_testes() ((void)0)
#endif

#endif