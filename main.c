#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

#define TESTING_DEBUG
// #define OPERATION_DEBUG

#ifdef OPERATION_DEBUG
    #define DEBUG_LOG(...) do { printf("[DEBUG] " __VA_ARGS__); printf("\n"); } while (0)
#else
    #define DEBUG_LOG(...) ((void)0)
#endif

#define MAX_SIZE 100
#define negativo '_'
#define MAX_EXPR_SIZE 100
#define MAX_RESULT_SIZE 40
#define MAX_FATORIAL_SIZE 6
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

void substituir_na_memoria(char *str, int posicao, int tamanho_antigo, const char *texto_novo);
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
double operador(char s[MAX_SIZE]);
void remover_espacos(char *s);
double fatoracao(double n);
bool eh_operador(char c);

double mult(double n1, double n2);
double divi(double n1, double n2);
double modu(double n1, double n2);
double sum(double n1, double n2);
double sub(double n1, double n2);


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

        printf("\n============================\n\n");
    }

#else
    #define rodar_testes() ((void)0)
#endif



int main(void) {
    char s[MAX_SIZE];

    rodar_testes();

    printf("Calculadora inteligente\n");
    while(true){

        printf(">    ");
        if(fgets(s, sizeof(s), stdin) == NULL){
            printf("Operacao nula");
            return 0;
        }

        s[strcspn(s, "\n")] = '\0';
        DEBUG_LOG("Entrada recebida: %s", s);

        remover_espacos(s);
        DEBUG_LOG("Depois de remover espacos: %s", s);

        interpretar_negativos(s);
        DEBUG_LOG("Depois de interpretar negativos: %s", s);

        interpretador_parenteses(s);
        DEBUG_LOG("Depois de resolver parenteses: %s", s);

        interpretador_fatorial(s);
        DEBUG_LOG("Depois de resolver fatoriais: %s",s);

        interpretador_prioritario(s);
        DEBUG_LOG("Depois de resolver prioridades: %s", s);



        char buffer[32] = {0};
        num_fmt(buffer, sizeof(buffer), operador(s));

        printf("Resultado: %s\n", buffer);
    }
    return 0;
}

void num_fmt(char *buffer, size_t size, double num) {

    DEBUG_LOG("Entrada recebida num_fmt: "PRECISION"\n", num);
    snprintf(buffer, size, PRECISION, num);
    DEBUG_LOG("buffer num_fmt: %s\n", buffer);

    size_t end = strlen(buffer) - 1;

    while (end > 0 && buffer[end] == '0') {
        buffer[end--] = '\0';
    }
    if (end > 0 && buffer[end] == '.') {
        buffer[end] = '\0';
    }

    DEBUG_LOG("pos while buffer num_fmt: %s\n", buffer);
}

void remover_espacos(char *s) {
    int write = 0;
    for (size_t i = 0; s[i] != '\0'; i++) {
        if (s[i] != ' ') {
            s[write++] = s[i];
        }
    }
    s[write] = '\0';
}

void interpretar_negativos(char *s) {
    for(int i = 0; s[i] != '\0'; i++) {
        if (s[i] == '-') {
            if (i == 0 || eh_precedente_negativo(s[i - 1])) {
                s[i] = negativo;
            }
        }
    }
}

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

    char texto_resultado[MAX_RESULT_SIZE];

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



bool eh_expressao_prioritaria(char operador) {
    return operador == '*' || operador == '/';
}

void resolver_expressao(char *s, ExpressaoPrioritaria *expr) {
    expr->expressao[expr->tamanho] = '\0';
    DEBUG_LOG("resolver_expressao: inicio=%d expressao='%s'", expr->inicio, expr->expressao);

    double resultado = operador(expr->expressao);

    char texto_resultado[MAX_RESULT_SIZE];

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
                if(s[i] != '*' && s[i] != '/') {
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


double operador(char s[MAX_SIZE]) {
    SeparatorReturns guarda_separador;

    bool *acabou = &guarda_separador.acabou;
    *acabou = false;

    guarda_separador = separator(s, 0);
    double result = guarda_separador.number;
    char operador;
    DEBUG_LOG("operador: valor inicial=%.15f", result);

    while(!*acabou) {
        operador = s[guarda_separador.position];
        if(operador == '!') {
            DEBUG_LOG("operador: operador='%c'", operador);
            result = fatoracao(result);
            break;
        }
        guarda_separador = separator(s, guarda_separador.position + 1);
        DEBUG_LOG("operador: operador='%c' proximo=%.15f", operador, guarda_separador.number);

        switch(operador) {
            case '+':
                result = sum(result, guarda_separador.number);
                break;
            case '-':
                result = sub(result, guarda_separador.number);
                break;
            case '*':
                result = mult(result, guarda_separador.number);
                break;
            case '/':
                result = divi(result, guarda_separador.number);
                break;
            case '%':
                result = modu(result, guarda_separador.number);
                break;
            default:
            //operadores reconhecidos nao tratados serao tratados em atualizaçoes futuras
                break;
        }
    }

    DEBUG_LOG("operador: resultado final=%.15f", result);
    return result;
}


SeparatorReturns separator(char s[MAX_SIZE], int pos) {
    SeparatorReturns res = {0};
    char buffer[MAX_SIZE] = "";
    int j = 0;

    DEBUG_LOG("separator: pos=%d", pos);
    for (res.position = pos; s[res.position] != '\0'; res.position++) {
        if (isdigit(s[res.position]) || s[res.position] == '.') {
            buffer[j] = s[res.position];
            j++;
        } else if (eh_operador(s[res.position])) {
            buffer[j] = '\0';
            res.number = atof(buffer);
            return res;
        } else if(s[res.position] == negativo){
            buffer[j] = '-';
            j++;
        }
    }
    buffer[j] = '\0';
    res.number = atof(buffer);
    res.acabou = true;
    return res;
}

double fatoracao(double n) {
    long long ni = (long long)round(n);
    if (ni < 0) {
        printf("Fatorial indefinido para negativos\n");
        return 0;
    }
    if (ni == 0 || ni == 1) return 1;
    double resultado = 1;
    for (long long i = 2; i <= ni; i++) resultado *= i;
    return resultado;
}


double sum(double n1, double n2) {
    double buffer = n1 + n2;
    return buffer;
}

double sub(double n1, double n2) {
    double buffer = n1 - n2;
    return buffer;
}

double mult(double n1, double n2) {
    double buffer = n1 * n2;
    return buffer;
}

double divi(double n1, double n2) {
    if(fabs(n2) < EPSILON) {
        printf("Impossivel dividir por 0\n");
        return 0;
    }
    double buffer = n1 / n2;
    return buffer;
}

double modu(double n1, double n2) {
    if(fabs(n2) < EPSILON) {
        printf("Impossivel dividir por 0\n");
        return 0;
    }
    double buffer = fmod(n1,n2);
    return buffer;
}


void substituir_na_memoria(char *str, int posicao, int tamanho_antigo, const char *texto_novo) {
    int tamanho_str = strlen(str);

    if (posicao < 0 || posicao > tamanho_str)
        return;

    if (posicao + tamanho_antigo > tamanho_str)
        return;

    int tamanho_novo = strlen(texto_novo);
    
    char *ponto_insercao = str + posicao;

    char *resto_texto = ponto_insercao + tamanho_antigo;
    
    if(tamanho_str - tamanho_antigo + tamanho_novo >= MAX_SIZE) {
        printf("A nova string operada estoura o limite de 100 caracteres por operacao completa");
        return;
    }
    memmove(ponto_insercao + tamanho_novo, resto_texto, strlen(resto_texto) + 1);

    memcpy(ponto_insercao, texto_novo, tamanho_novo);
}

bool eh_operador(char c) {
    switch (c) {
        case '+': case '-': case '*': case '/': 
        case '%': case '!':
            return true;
        default:
            return false;
    }
}

bool eh_precedente_negativo(char c) {
    switch (c) {
        case '+': case '-': case '*': case '/': 
        case '%': case '(':
            return true;
        default:
            return false;
    }
}