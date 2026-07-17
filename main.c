#include "includes/utils.h"
#include "includes/matematica.h"
#include "includes/interpretador.h"

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



        char buffer[MAX_RESULT] = {0};
        num_fmt(buffer, sizeof(buffer), operador(s));

        printf("Resultado: %s\n", buffer);
    }
    return 0;
}

double operador(char *s) {
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
            case '^':
                result = elevado(result, guarda_separador.number);
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

