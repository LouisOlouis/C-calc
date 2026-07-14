#include "utils.h"
#include <stddef.h>

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

void substituir_na_memoria(char *str, size_t posicao, size_t tamanho_antigo, const char *texto_novo) {
    size_t tamanho_str = strlen(str);

    if (posicao < 0 || posicao > tamanho_str)
        return;

    if (posicao + tamanho_antigo > tamanho_str)
        return;

    size_t tamanho_novo = strlen(texto_novo);
    
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
        case '%': case '!': case '^':
            return true;
        default:
            return false;
    }
}

bool eh_precedente_negativo(char c) {
    switch (c) {
        case '+': case '-': case '*': case '/': 
        case '%': case '(': case '^':
            return true;
        default:
            return false;
    }
}