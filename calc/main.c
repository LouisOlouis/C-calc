#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_SIZE 100
#define negativo '_'
#define MAX_EXPR_SIZE 100
#define MAX_RESULT_SIZE 20

typedef struct {
    bool capturando;
    int inicio;
    int tamanho;
    char expressao[MAX_EXPR_SIZE];
} ExpressaoPrioritaria;

typedef struct Separator_returns {
    int number;
    int position;
    bool acabou;
} separator_returns;

void substituir_na_memoria(char *str, int posicao, int tamanho_antigo, const char *texto_novo);
void busca_parenteses(char *s,int *abertura,int *fechamento);
separator_returns separator(char s[MAX_SIZE], int pos);
void interpretador_prioritario(char *s);
void interpretador_parenteses(char *s);
void interpretar_negativos(char *s);
int operador(char s[MAX_SIZE]);
void remover_espacos(char *s);
char eh_operador(char c);
int mult(int n1, int n2);
int divi(int n1, int n2);
int modu(int n1, int n2);
int sum(int n1, int n2);
int sub(int n1, int n2);


int main(void) {
    char s[MAX_SIZE];

    printf("Calculadora inteligente\n");
    while(true){

        printf(">    ");
        if(fgets(s, sizeof(s), stdin) == NULL){
            printf("Operacao nula");
            return 0;
        }

        s[strcspn(s, "\n")] = '\0';

        remover_espacos(s);
        interpretar_negativos(s);
        interpretador_parenteses(s);
        interpretador_prioritario(s);
        int resultado = operador(s);

        printf("Resultado: %d\n", resultado);
    }
    return 0;
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
            if (i == 0 || eh_operador(s[i - 1])) {
                s[i] = negativo;
            }
        }
    }
}

void busca_parenteses(char *s, int *abertura, int *fechamento) {
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] == '(') {
            *abertura = i;
        } else if (s[i] == ')' && *abertura != -1) {
            *fechamento = i;
            break;
        }
    }
}

void interpretador_parenteses(char *s) {
    while (1) {
        int abertura = -1;
        int fechamento = -1;

        busca_parenteses(s, &abertura, &fechamento);


        // Não há mais parênteses
        if (abertura == -1 || fechamento == -1) {
            break;
        }

        int tamanho = fechamento - abertura - 1;
        if (tamanho < 0) {
            printf("Erro: parenteses invalidos\n");
            return;
        }

        if (tamanho >= MAX_SIZE) {
            printf("Erro: expressao muito grande\n");
            return;
        }

        char semi_operation[MAX_SIZE];
        strncpy(semi_operation, &s[abertura + 1], tamanho);
        semi_operation[tamanho] = '\0';

        // Resolve a expressão interna
        interpretador_parenteses(semi_operation);
        interpretador_prioritario(semi_operation);
        int resultado = operador(semi_operation);

        char resultado_str[20];
        snprintf(resultado_str, sizeof(resultado_str), "%d", resultado);

        // Substitui "(expressao)" pelo resultado
        substituir_na_memoria(
            s,
            abertura,
            fechamento - abertura + 1,
            resultado_str
        );
    }
}



bool inicia_expressao_prioritaria(char operador) {
    return operador == '*' || operador == '/';
}

void resolver_expressao(char *s, ExpressaoPrioritaria *expr) {
    expr->expressao[expr->tamanho] = '\0';

    int resultado = operador(expr->expressao);

    char texto_resultado[MAX_RESULT_SIZE];

    snprintf(
        texto_resultado,
        sizeof(texto_resultado),
        "%d",
        resultado
    );

    substituir_na_memoria(
        s,
        expr->inicio,
        strlen(expr->expressao),
        texto_resultado
    );
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

    for(int i = 0; s[i] != '\0'; i++) {
        if(eh_operador(s[i])) {
            if(!expr.capturando) {
                if(inicia_expressao_prioritaria(s[i])) {
                    iniciar_captura(&expr, ultimo_operador);

                    if(ultimo_operador != 0) {
                        i = ultimo_operador + 1;}
                    else {
                        i = 0;}
                }
                else {
                    ultimo_operador = i;
                }
            }
            else {
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
}


int operador(char s[MAX_SIZE]) {
    separator_returns guarda_separador;

    bool *acabou = &guarda_separador.acabou;
    *acabou = false;

    guarda_separador = separator(s, 0);
    int result = guarda_separador.number;
    char operador;

    while(!*acabou) {
        operador = s[guarda_separador.position];
        guarda_separador = separator(s, guarda_separador.position + 1);

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

    return result;
}


separator_returns separator(char s[MAX_SIZE], int pos) {
    separator_returns res;
    char buffer[MAX_SIZE] = "";
    int j = 0;

    for (res.position = pos; s[res.position] != '\0'; res.position++) {
        if (isdigit(s[res.position])) {
            buffer[j] = s[res.position];
            j++;
        } else if (eh_operador(s[res.position])) {
            buffer[j] = '\0';
            res.number = atoi(buffer);
            return res;
        } else if(s[res.position] == negativo){
            buffer[j] = '-';
            j++;
        }
    }
    buffer[j] = '\0';
    res.number = atoi(buffer);
    res.acabou = true;
    return res;
}


int sum(int n1, int n2) {
    int buffer = n1 + n2;
    return buffer;
}

int sub(int n1, int n2) {
    int buffer = n1 - n2;
    return buffer;
}

int mult(int n1, int n2) {
    int buffer = n1 * n2;
    return buffer;
}

int divi(int n1, int n2) {
    if(n2 == 0) {
        printf("Impossivel dividir por 0\n");
        return 0;
    }
    int buffer = n1 / n2;
    return buffer;
}

int modu(int n1, int n2) {
    if(n2 == 0) {
        printf("Impossivel dividir por 0\n");
        return 0;
    }
    int buffer = n1 % n2;
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

char eh_operador(char c) {
    switch (c) {
        case '+': case '-': case '*': case '/': 
        case '%': 
            return true;
        default:
            return false;
    }
}
