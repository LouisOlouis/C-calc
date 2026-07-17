#include "matematica.h"

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

double elevado(double n1, double n2) {
    double buffer = pow(n1, n2);
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
