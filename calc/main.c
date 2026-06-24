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
        case '&': case '|':
            return true;
        default:
            return false;
    }
}

typedef struct Separator_returns {
    int number;
    int position;
} separator_returns;

separator_returns res;


separator_returns separator(char s[100], int pos){
    
    char buffer[100] = "";
    int j = 0;

    #define p res.position

    for (p = pos; s[p] != '\n'; p++) {
        if (isdigit(s[p])) {
            buffer[j++] = s[p];
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
    if(n2 == 0) return 0;
    int buffer = n1/n2;
    return buffer;
}

int modu(int n1, int n2){
    if(n2 == 0) return 0;
    int buffer = n1%n2;
    return buffer;
}

int interpretador(){
    separator_returns guarda_separador;
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
                break;
        }
    }

    return result;
}

int main() {
    char s[100];

    fgets(s, sizeof(s), stdin);

    int result = interpretador();
    printf("Resultado:  %d\n", result);

    return 0;
}


