#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define Valor_maximo 100
#define negativo '_'
#define Var_end '\0'


char eh_operador(char c) {
    switch (c) {
        case '+': case '-': case '*': case '/': 
        case '%': case '=': case '<': case '>': 
            return true;
        default:
            return false;
    }
}

typedef struct Separator_returns {
    int number;
    int position;
    bool acabou;
} separator_returns;

separator_returns separator(char s[Valor_maximo], int pos){
    separator_returns res;
    char buffer[Valor_maximo] = "";
    int j = 0;

    #define p res.position

    for (p = pos; s[p] != Var_end; p++) {
        if (isdigit(s[p])) {
            buffer[j] = s[p];
            j++;
        } else if (eh_operador(s[p])) {
            buffer[j] = Var_end;
            res.number = atoi(buffer);
            return res;
        } else if(s[p] == negativo){
            buffer[j] = '-';
            j++;
        }
    }
    buffer[j] = Var_end;
    res.number = atoi(buffer);
    res.acabou = true;
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

int operador(char s[Valor_maximo]){
    separator_returns guarda_separador;

    bool *acabou = &guarda_separador.acabou;
    *acabou = false;

    guarda_separador = separator(s, 0);
    int result = guarda_separador.number;
    char operador;

    while(!*acabou){
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
    // strlen(resto_texto) + 1 garante que o caractere Var_end também seja movido
    if(tamanho_str - tamanho_antigo + tamanho_novo >= Valor_maximo) {
        printf("A nova string operada estoura o limite de 100 caracteres por operacao completa");
        return;
    }
    memmove(ponto_insercao + tamanho_novo, resto_texto, strlen(resto_texto) + 1);
    
    // Copia o novo texto (menor) para o espaço reservado
    memcpy(ponto_insercao, texto_novo, tamanho_novo);
}

void interpretador(char *s){
    //interpretador de multiplicaçao e divisao prioritaria
    bool start_semi = false;
    int semi_i = 0;
    char semi_operation[Valor_maximo];
    bool estouro_s = false;
    int last_operator_pos = 0;
    int i = 0;

    for(i = 0; s[i] != Var_end; i++) {
        if (eh_operador(s[i])) {
            if(!start_semi){
                if(s[i] == '*' || s[i] == '/') {
                    start_semi = true;
                    semi_operation[0] = Var_end;
                    if(last_operator_pos != 0){
                        last_operator_pos += 1;
                    }
                    i = last_operator_pos;
                } else {last_operator_pos = i;};
            } else {
                if((s[i] != '*' && s[i] != '/')|| estouro_s){
                    repete:
                    start_semi = false;
                    semi_operation[semi_i] = Var_end;
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
    if(start_semi && s[i] == Var_end){
        goto repete;
    }
}

void interpretador_parenteses(char *s){
    int last_parenteses_pos = 0;
    //interpretador de parenteses
    for(int i = 0; s[i] != Var_end; i++) {
        if (s[i] == '(') {
            last_parenteses_pos = i;
        }
        if (s[i] == ')'){

            
        }
    }
}



void interpretacao_primaria(char *s){
    //removedor de espaços
    {
        int write = 0;
        for (int i = 0; s[i] != Var_end; i++) {
            if (s[i] != ' ') {
                s[write++] = s[i];
            }
        }
        s[write] = Var_end;
    }
    //interpretador de negativo
    for(int i = 0; s[i] != Var_end; i++) {
        if (s[i] == '-') {
            if (i == 0 || eh_operador(s[i - 1])) {
                s[i] = negativo;
            }
        }
    }
    interpretador(s);
}

int main(void) {
    char s[Valor_maximo];

    printf("Calculadora inteligente\n");
    while(true){

        printf(">    ");
        if(fgets(s, sizeof(s), stdin) == NULL){
            printf("Operacao nula");
            return 0;
        }

        s[strcspn(s, "\n")] = Var_end;
        interpretacao_primaria(s);
        printf("Resultado:  %d\n",operador(s));
    }
    return 0;
}
