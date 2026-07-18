
#ifdef TESTING_DEBUG
#include "debug.h"

void testar(char *expressao, double esperado) {
    char s[MAX_SIZE];
    strncpy(s, expressao, MAX_SIZE - 1);
    s[MAX_SIZE - 1] = '\0';

    Erro erro = {0};

    remover_espacos(s);
    interpretar_negativos(s);
    interpretador_parenteses(s, &erro);
    interpretador_fatorial(s, &erro);
    interpretador_prioritario(s, &erro);
    double resultado = operador(s, &erro);

    bool passou = fabs(resultado - esperado) < EPSILON;

    printf("[%s] %-20s -> obtido: %-25.15g esperado: %g\n",
        passou ? "OK    " : "FALHOU",
        expressao,
        resultado,
        esperado
    );
}

void rodar_testes(void) {
    printf("\n========== TESTES ==========");

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

#endif
