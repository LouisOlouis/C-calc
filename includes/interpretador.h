#ifndef INTERPRETADOR_H
#define INTERPRETADOR_H

#include "utils.h"

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

typedef struct ExpressaoParenteses{
    int inicio;
    int profundidade;
    int tamanho;
    char expressao[MAX_SIZE];
    bool capturando;
} ExpressaoParenteses;

void iniciar_captura_parenteses(ExpressaoParenteses *expr, int posicao);
void adicionar_caractere_parenteses(ExpressaoParenteses *expr, char c);
void resolver_parenteses(char *s, ExpressaoParenteses *expr, int fechamento);
void interpretador_parenteses(char *s);

void iniciar_captura_fatorial(ExpressaoFatorial *expr, int posicao);
void adicionar_caractere_fatorial(ExpressaoFatorial *expr, char c);
void resolver_expressao_fatorial(char *s, ExpressaoFatorial *expr);
void interpretador_fatorial(char *s);

bool eh_expressao_prioritaria(char operador);
void iniciar_captura(ExpressaoPrioritaria *expr, int inicio);
void adicionar_caractere(ExpressaoPrioritaria *expr, char c);
void resolver_expressao(char *s, ExpressaoPrioritaria *expr);
void interpretador_prioritario(char *s);



void interpretar_negativos(char *s);
#endif