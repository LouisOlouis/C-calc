#ifndef DEBUG_H
#define DEBUG_H

#include "utils.h"
#include "interpretador.h"

// #define TESTING_DEBUG
// #define OPERATION_DEBUG

#ifdef OPERATION_DEBUG
    #define DEBUG_LOG(...) do { printf("[DEBUG] " __VA_ARGS__); printf("\n"); } while (0)
#else
    #define DEBUG_LOG(...) ((void)0)
#endif

#ifdef TESTING_DEBUG

    void testar(char *expressao, double esperado);

    void rodar_testes(void);

#else
    #define rodar_testes() ((void)0)
#endif

#endif