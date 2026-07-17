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
    SeparatorReturns expr;

    bool *acabou = &expr.acabou;
    *acabou = false;

    separator(s, &expr, 0);
    double result = expr.number;
    char op;
    DEBUG_LOG("operador: valor inicial=%.15f", result);

    while(!*acabou) {
        op = s[expr.position];
        if(op == '!') {
            DEBUG_LOG("operador: operador='%c'", op);
            result = fatoracao(result);
            break;
        }
        separator(s, &expr, expr.position + 1);
        DEBUG_LOG("operador: operador='%c' proximo=%.15f", op, expr.number);

        switch(op) {
            case '+':
                result = sum(result, expr.number);
                break;
            case '-':
                result = sub(result, expr.number);
                break;
            case '*':
                result = mult(result, expr.number);
                break;
            case '/':
                result = divi(result, expr.number);
                break;
            case '%':
                result = modu(result, expr.number);
                break;
            case '^':
                result = elevado(result, expr.number);
                break;
            case '~':
                result = raiz(result, expr.number);
                break;
            default:
            printf("Operador nao reconhecido");
                break;
        }
    }

    DEBUG_LOG("operador: resultado final=%.15f", result);
    return result;
}


void separator(char *s, SeparatorReturns *expr, int pos) {
    char buffer[MAX_SIZE] = {0};
    int j = 0;

    expr->acabou = false;
    expr->position = -1;

    DEBUG_LOG("separator: pos=%d", pos);
    for (int i = pos; s[i] != '\0'; i++) {
        if (isdigit((unsigned char)s[i]) || s[i] == '.') {
            if (j < MAX_SIZE - 1) {
                buffer[j++] = s[i];
            }
        } else if (eh_operador(s[i])) {
            buffer[j] = '\0';
            expr->number = atof(buffer);
            expr->position = i;
            return;
        } else if (s[i] == negativo) {
            if (j < MAX_SIZE - 1) {
                buffer[j++] = '-';
            }
        }
    }

    buffer[j] = '\0';
    expr->number = atof(buffer);
    expr->acabou = true;
    return;
}

