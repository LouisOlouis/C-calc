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
void resolver_parenteses(char *s, ExpressaoParenteses *expr, int fechamento, Erro *erro);
void interpretador_parenteses(char *s, Erro *erro);

void iniciar_captura_fatorial(ExpressaoFatorial *expr, int posicao);
void adicionar_caractere_fatorial(ExpressaoFatorial *expr, char c);
void resolver_expressao_fatorial(char *s, ExpressaoFatorial *expr, Erro *erro);
void interpretador_fatorial(char *s, Erro *erro);

bool eh_expressao_prioritaria(char operador);
void iniciar_captura(ExpressaoPrioritaria *expr, int inicio);
void adicionar_caractere(ExpressaoPrioritaria *expr, char c);
void resolver_expressao(char *s, ExpressaoPrioritaria *expr, Erro *erro);
void interpretador_prioritario(char *s, Erro *erro);



void interpretar_negativos(char *s);
#ifdef __cplusplus
}
#endif
#endif