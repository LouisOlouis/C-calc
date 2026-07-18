#include "matematica.h"

double fatoracao(double n, Erro *erro) {
    long long ni = (long long)round(n);
    if (ni < 0) {
        if (erro != NULL) {
            seta_erro(erro, ERRO_FATORIAL_NEGATIVO, "fatorial indefinido para negativos");
        }
        return 0;
    }
    if (ni == 0 || ni == 1) return 1;
    double resultado = 1;
    for (long long i = 2; i <= ni; i++) resultado *= i;
    return resultado;
}

double elevado(double n1, double n2) {
    double buffer = pow(n1, n2);
    return buffer;
}

double raiz(double n1, double n2) {
    double buffer = pow(n1, 1.0 / n2);
    return buffer;
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

double divi(double n1, double n2, Erro *erro) {
    if(fabs(n2) < EPSILON) {
        if (erro != NULL) {
            seta_erro(erro, ERRO_DIV_ZERO, "impossivel dividir por zero");
        }
        return 0;
    }
    double buffer = n1 / n2;
    return buffer;
}

double modu(double n1, double n2, Erro *erro) {
    if(fabs(n2) < EPSILON) {
        if (erro != NULL) {
            seta_erro(erro, ERRO_DIV_ZERO, "impossivel dividir por zero");
        }
        return 0;
    }
    double buffer = fmod(n1,n2);
    return buffer;
}
