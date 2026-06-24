#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

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
    char buffer;
    int position;
} separator_returns;

separator_returns separator(char s[100], int pos){
    separator_returns res;

    #define p res.position

    for (p = pos; s[p] != '\0'; p++) {
        if (isdigit(s[p])) {
            strcat(res.buffer, s[p]);
        } else if (eh_operador(s[p])) {
            return res;
        } 
        // else if (s[*p] == '!'){
        //     return;
        // }
    }
}


int main(int argc, char const *argv[])
{
    if(argc > 1){
        return 0;
    }

    char s[100];

    fgets(s, sizeof(s), stdin);
    s[strcspn(s, "\n")] = '\0';

    separator_returns res = separator(s, 0);

    return 0;
}


