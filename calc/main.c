#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

bool acabou = false;

char eh_operador(char c) {
    switch (c) {
        case '+': case '-': case '*': case '/': 
        case '%': case '=': case '<': case '>': 
        case '&': case '|': case '(': case ')':
            return true;
        default:
            return false;
    }
}

typedef struct Separator_returns {
    int number;
    int position;
} separator_returns;

separator_returns separator(char s[100], int pos){
    separator_returns res;
    char buffer[100] = "";
    int j = 0;

    #define p res.position

    for (p = pos; s[p] != '\0'; p++) {
        if (isdigit(s[p])) {
            buffer[j] = s[p];
            j++;
        } else if (eh_operador(s[p])) {
            buffer[j] = '\0';
            res.number = atoi(buffer);
            return res;
        } 
    }
    buffer[j] = '\0';
    res.number = atoi(buffer);
    acabou = true;
    return res;
}

int sum(int n1, int n2){
    int buffer = n1+n2;
    return buffer;
}

int sub(int n1, int n2){
    int buffer = n1-n2;
    return buffer;
}

int mult(int n1, int n2){
    int buffer = n1*n2;
    return buffer;
}

int divi(int n1, int n2){
    if(n2 == 0) {
        printf("Impossivel dividir por 0");
        return 0;
    }
    int buffer = n1/n2;
    return buffer;
}

int modu(int n1, int n2){
    if(n2 == 0) {
        printf("Impossivel dividir por 0");
        return 0;
    }
    int buffer = n1%n2;
    return buffer;
}

int operador(char s[100]){
    separator_returns guarda_separador;

    acabou = false;

    guarda_separador = separator(s, 0);
    int result = guarda_separador.number;
    char operador;

    while(!acabou){
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

void substituir_na_memoria(char *str, int posicao, int tamanho_antigo, const char *texto_novo) {
    int tamanho_str = strlen(str);

    if (posicao < 0 || posicao > tamanho_str)
        return;

    if (posicao + tamanho_antigo > tamanho_str)
        return;

    int tamanho_novo = strlen(texto_novo);
    
    // Ponteiro para onde a alteração vai começar
    char *ponto_insercao = str + posicao;
    
    // Ponteiro para o restante do texto que precisa ser movido
    char *resto_texto = ponto_insercao + tamanho_antigo;
    
    // Desloca o "resto do texto" para trás ou para frente usando memmove
    // strlen(resto_texto) + 1 garante que o caractere '\0' também seja movido
    if(strlen(str) - tamanho_antigo + tamanho_novo > 100) {
        printf("A nova string operada estoura o limite de 100 caracteres por operacao completa");
        return;
    }
    memmove(ponto_insercao + tamanho_novo, resto_texto, strlen(resto_texto) + 1);
    
    // Copia o novo texto (menor) para o espaço reservado
    memcpy(ponto_insercao, texto_novo, tamanho_novo);
}

void interpretador(char *s){
    int i = 0;
    int last_operator_pos = 0;

    bool start_semi = false;
    int semi_i = 0;
    char semi_operation[100];

    bool estouro_s = false;

    for(i = 0; s[i] != '\0'; i++) {
        if (eh_operador(s[i])) {
            if(!start_semi){
                if(s[i] == '*' || s[i] == '/') {
                    start_semi = true;
                    semi_operation[0] = '\0';
                    if(last_operator_pos != 0){
                        last_operator_pos += 1;
                    }
                    i = last_operator_pos;
                } else {last_operator_pos = i;};
            } else {
                if((s[i] != '*' && s[i] != '/')|| estouro_s){
                    repete:
                    start_semi = false;
                    semi_operation[semi_i] = '\0';
                    int buffer_semi_operation_operated = operador(semi_operation);
                    semi_i = 0;
                    estouro_s = false;
                    char semi_operation_operated[20];
                    snprintf(semi_operation_operated, sizeof(semi_operation_operated), "%d", buffer_semi_operation_operated);
                    substituir_na_memoria(s, last_operator_pos, strlen(semi_operation), semi_operation_operated);
                    i = last_operator_pos + strlen(semi_operation_operated)-1;
                }
            }
        }

        if(start_semi){
            if(semi_i >= 99){
                estouro_s = true;
            } else {
                semi_operation[semi_i] = s[i];
                semi_i++;
            }
        }
    }
    if(start_semi && s[i] == '\0'){
        goto repete;
    }
}

int main(void) {
    char s[100];

    printf("Calculadora inteligente\n");
    while(true){

        printf(">    ");
        if(fgets(s, sizeof(s), stdin) == NULL){
            printf("Operacao nula");
            return 0;
        }

        s[strcspn(s, "\n")] = '\0';
        interpretador(s);
        printf("Resultado:  %d\n",operador(s));
    }
    return 0;
}


