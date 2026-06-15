#include <stdio.h>

typedef struct {
    int id;
    char titulo[100];
    char autor[100];
    int ano;
    int emprestado;
} Livro;

void cadastrabook() {
    printf("Cadastramento de livros\n\n");

    // char titulo[100];
    // char autor[100];
    // int ano

    Livro l
    printf("Digite o titulo do livro:   ")
    fgets(l.titulo, sizeof(l.titulo), stdin);

    printf("Digite o autor do livro:   ")
    fgets(l.autor, sizeof(l.autor), stdin);



    FILE *livros = fopen("arquivos/livros.dat", "wb");
    if (arquivo == NULL) {
        return 1;
    }

    return;
}

void listabook(){
    return;
}

void editabook(){
    return;
}

void emprestabook(){
    return;
}

void devolvebook(){
    return;
}

int main() {
    int opcao;
    bool loop = true;
    while(loop){
        printf("SISTEMA BIBLIOTECA\n");
        printf("1 - Cadastrar livro\n"
                "2 - Listar livros\n"
                "3 - Editar livro\n"
                "4 - Emprestar livro\n"
                "5 - Devolver livro\n"
                "6 - Sair\n");
        scanf("%d", &opcao);
        switch(opcao){
            case 1:
                cadastrabook();
                break;
            case 2:
                listabook();
                break;
            case 3:
                buscabook();
                break;
            case 4:
                emprestabook():
                break;
            case 5:
                devolvebook();
                break;
            case 6:
                printf("tchau\n\n");
                loop = false;
                break;
            default:
                printf("Caso invalido\n\n");
                break;
        }
    }
    return 0
}