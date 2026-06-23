#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
int main(int argc, char const *argv[])
{
    if(argc > 1){
        return 0;
    }

    char s[100];

    fgets(s, sizeof(s), stdin);

    char b1, b2;

    char operator;

    int result;

    bool ps = false;

    for (int i = 0; s[i] != '\0'; i++) {

        if (isdigit(s[i])) {
            if(!ps){strcat(b1, s[i]);}
            else{strcat(b2,s[i]);};
        } else if (s[i] == '+' ||
                   s[i] == '-' ||
                   s[i] == '*' ||
                   s[i] == '/') {
            operator = s[i];
            ps = true;
        }
    }



    return 0;
}


