#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define Valor_maximo 100
#define negativo '_'
#define Var_end '\0'

typedef struct Separator_returns {
    int number;
    int position;
    bool acabou;
} separator_returns;

char eh_operador(char c);
void substituir_na_memoria(char *str, int posicao, int tamanho_antigo, const char *texto_novo);
void busca_parenteses(char *s,int *abertura,int *fechamento);

void remover_espacos(char *s);
void interpretar_negativos(char *s);
void interpretador_parenteses(char *s);
void interpretador(char *s);

int operador(char s[Valor_maximo]);
separator_returns separator(char s[Valor_maximo], int pos);

int sum(int n1, int n2);
int sub(int n1, int n2);
int mult(int n1, int n2);
int divi(int n1, int n2);
int modu(int n1, int n2);

int main(void)
{
    char s[Valor_maximo];

    printf("Calculadora inteligente\n");
    while(true){

        printf(">    ");
        if(fgets(s, sizeof(s), stdin) == NULL){
            printf("Operacao nula");
            return 0;
        }

        s[strcspn(s, "\n")] = Var_end;

        remover_espacos(s);
        interpretar_negativos(s);
        interpretador_parenteses(s);
        interpretador(s);
        int resultado = operador(s);

        printf("Resultado: %d\n", resultado);
    }
    return 0;
}


void remover_espacos(char *s)
{
    int write = 0;
    for (int i = 0; s[i] != Var_end; i++) {
        if (s[i] != ' ') {
            s[write++] = s[i];
        }
    }
    s[write] = Var_end;
}

void interpretar_negativos(char *s)
{
    for(int i = 0; s[i] != Var_end; i++) {
        if (s[i] == '-') {
            if (i == 0 || eh_operador(s[i - 1])) {
                s[i] = negativo;
            }
        }
    }
}

void busca_parenteses(char *s,int *abertura,int *fechamento)
{
    for (int i = 0; s[i] != Var_end; i++) {
        if (s[i] == '(') {
            *abertura = i;
        } else if (s[i] == ')' && *abertura != -1) {
            *fechamento = i;
            break;
        }
    }
}

void interpretador_parenteses(char *s)
{
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

        if (tamanho >= Valor_maximo) {
            printf("Erro: expressao muito grande\n");
            return;
        }

        char semi_operation[Valor_maximo];
        strncpy(semi_operation, &s[abertura + 1], tamanho);
        semi_operation[tamanho] = Var_end;

        // Resolve a expressão interna
        interpretador_parenteses(semi_operation);
        interpretador(semi_operation);
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

void interpretador(char *s)
{
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

                    snprintf(
                        semi_operation_operated, 
                        sizeof(semi_operation_operated), 
                        "%d", 
                        buffer_semi_operation_operated
                    );

                    substituir_na_memoria(
                        s, 
                        last_operator_pos, 
                        strlen(semi_operation), 
                        semi_operation_operated
                    );
                    
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


int operador(char s[Valor_maximo])
{
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


separator_returns separator(char s[Valor_maximo], int pos)
{
    separator_returns res;
    char buffer[Valor_maximo] = "";
    int j = 0;

    for (res.position = pos; s[res.position] != Var_end; res.position++) {
        if (isdigit(s[res.position])) {
            buffer[j] = s[res.position];
            j++;
        } else if (eh_operador(s[res.position])) {
            buffer[j] = Var_end;
            res.number = atoi(buffer);
            return res;
        } else if(s[res.position] == negativo){
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
        printf("Impossivel dividir por 0\n");
        return 0;
    }
    int buffer = n1/n2;
    return buffer;
}

int modu(int n1, int n2){
    if(n2 == 0) {
        printf("Impossivel dividir por 0\n");
        return 0;
    }
    int buffer = n1%n2;
    return buffer;
}


void substituir_na_memoria(char *str, int posicao, int tamanho_antigo, const char *texto_novo)
{
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

char eh_operador(char c)
{
    switch (c) {
        case '+': case '-': case '*': case '/': 
        case '%': 
            return true;
        default:
            return false;
    }
}
