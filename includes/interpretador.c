#include "interpretador.h"

void interpretar_negativos(char *s) {
    for(int i = 0; s[i] != '\0'; i++) {
        if (s[i] == '-') {
            if (i == 0 || eh_precedente_negativo(s[i - 1])) {
                s[i] = negativo;
            }
        }
    }
}

// parenteses
void iniciar_captura_parenteses(ExpressaoParenteses *expr, int posicao) {
    expr->capturando = true;
    expr->inicio = posicao;
    expr->profundidade = 1;
    expr->tamanho = 0;
    expr->expressao[0] = '\0';
}

void adicionar_caractere_parenteses(ExpressaoParenteses *expr, char c) {
    if(expr->tamanho >= MAX_SIZE - 1)
        return;

    expr->expressao[expr->tamanho++] = c;
    expr->expressao[expr->tamanho] = '\0';
}

void resolver_parenteses(char *s, ExpressaoParenteses *expr, int fechamento) {
    DEBUG_LOG("resolver_parenteses: inicio=%d fechamento=%d profundidade=%d expressao='%s'", expr->inicio, fechamento, expr->profundidade, expr->expressao);
    interpretador_parenteses(expr->expressao);
    interpretador_fatorial(expr->expressao);
    interpretador_prioritario(expr->expressao);

    double resultado = operador(expr->expressao);

    char resultado_str[20];

    num_fmt(
        resultado_str,
        sizeof(resultado_str),
        resultado
    );

    substituir_na_memoria(
        s,
        expr->inicio,
        fechamento - expr->inicio + 1,
        resultado_str
    );

    DEBUG_LOG("resultado parenteses: %s", resultado_str);
    expr->capturando = false;
}


void interpretador_parenteses(char *s) {
    ExpressaoParenteses expr = {0};

    for (int i = 0; s[i] != '\0'; i++) {
        if (!expr.capturando) {
            if (s[i] == '(') {
                iniciar_captura_parenteses(&expr, i);
            }
        } else {
            if (s[i] == '(') {
                expr.profundidade++;

                adicionar_caractere_parenteses(&expr, s[i]);
            } else if (s[i] == ')') {
                expr.profundidade--;

                if (expr.profundidade == 0) {
                    resolver_parenteses(s, &expr, i);
                    i = expr.inicio - 1;
                }
                else {
                    adicionar_caractere_parenteses(&expr, s[i]);
                }
            } else {
                adicionar_caractere_parenteses(&expr, s[i]);
            }
        }
    }
}

// fatorial
void iniciar_captura_fatorial(ExpressaoFatorial *expr, int posicao) {
    expr->capturando = true;
    expr->inicio = posicao;
    expr->tamanho = 0;
}

void adicionar_caractere_fatorial(ExpressaoFatorial *expr, char c) {
    if(expr->tamanho >= MAX_FATORIAL_SIZE - 1)
        return;

    expr->expressao[expr->tamanho++] = c;
}

void resolver_expressao_fatorial(char *s, ExpressaoFatorial *expr) {
    expr->expressao[expr->tamanho] = '\0';
    DEBUG_LOG("resolver_expressao_fatorial: inicio=%d expressao='%s'", expr->inicio, expr->expressao);

    double resultado = operador(expr->expressao);

    char texto_resultado[MAX_RESULT];

    num_fmt(
        texto_resultado,
        sizeof(texto_resultado),
        resultado
    );

    substituir_na_memoria(
        s,
        expr->inicio,
        strlen(expr->expressao),
        texto_resultado
    );

    DEBUG_LOG("resultado expressao fatorial: %s", texto_resultado);
}

void interpretador_fatorial(char *s) {
    ExpressaoFatorial expr = {0};

    int ultimo_operador = 0;
    DEBUG_LOG("interpretador_fatorial: entrada='%s'", s);

    for (int i = 0; s[i] != '\0'; i++) {
        if (eh_operador(s[i])) {
            if(s[i] == '!') {
                if(!expr.capturando) {
                    int inicio_operando = (ultimo_operador != 0) ? ultimo_operador + 1 : 0;
                    iniciar_captura_fatorial(&expr, inicio_operando);
                    i = inicio_operando;
                } else {
                    adicionar_caractere_fatorial(&expr, '!');
                    resolver_expressao_fatorial(s, &expr);

                    expr.capturando = false;

                    i = expr.inicio;
                }
            } else {
                ultimo_operador = i;
            }
        }

        if(expr.capturando){
            adicionar_caractere_fatorial(&expr, s[i]);}
        }
}


// prioritario
bool eh_expressao_prioritaria(char operador) {
    return operador == '*' || operador == '/' || operador == '^' || operador == '~';
}

void resolver_expressao(char *s, ExpressaoPrioritaria *expr) {
    expr->expressao[expr->tamanho] = '\0';
    DEBUG_LOG("resolver_expressao: inicio=%d expressao='%s'", expr->inicio, expr->expressao);

    double resultado = operador(expr->expressao);

    char texto_resultado[MAX_RESULT];

    num_fmt(
        texto_resultado,
        sizeof(texto_resultado),
        resultado
    );

    substituir_na_memoria(
        s,
        expr->inicio,
        strlen(expr->expressao),
        texto_resultado
    );

    DEBUG_LOG("resultado expressao prioritaria: %s", texto_resultado);
}

void iniciar_captura(ExpressaoPrioritaria *expr, int inicio) {
    expr->capturando = true;
    expr->inicio = inicio;
    expr->tamanho = 0;
}

void adicionar_caractere(ExpressaoPrioritaria *expr, char c) {
    if(expr->tamanho >= MAX_EXPR_SIZE - 1)
        return;

    expr->expressao[expr->tamanho++] = c;
}

void interpretador_prioritario(char *s) {
    ExpressaoPrioritaria expr = {0};

    int ultimo_operador = 0;
    DEBUG_LOG("interpretador_prioritario: entrada='%s'", s);

    for(int i = 0; s[i] != '\0'; i++) {
        if(eh_operador(s[i])) {
            if(!expr.capturando) {
                if(eh_expressao_prioritaria(s[i])) {
                    int inicio_operando = (ultimo_operador != 0) ? ultimo_operador + 1 : 0;
                    iniciar_captura(&expr, inicio_operando);
                    i = inicio_operando; 
                } else {
                    ultimo_operador = i;
                }
            } else {
                if(s[i] != '*' && s[i] != '/' && s[i] != '^' && s[i] != '~') {
                    resolver_expressao(s, &expr);

                    expr.capturando = false;

                    i = expr.inicio;
                }
            }
        }

        if(expr.capturando){
            adicionar_caractere(&expr, s[i]);}
    }
    if (expr.capturando) {
        resolver_expressao(s, &expr);
    }
}
