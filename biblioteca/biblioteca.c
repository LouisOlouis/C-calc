#include <stdio.h>
#include <stdbool.h>
#include <string.h>
typedef struct {
    int id;
    char titulo[100];
    char autor[100];
    int ano;
    bool emprestado;
} Livro;

void clean_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    return;
}

int buscaid(FILE *arquivo){
    Livro l;
    int id = 1;

    while (fread(&l, sizeof(Livro), 1, arquivo) == 1) {
        id ++;
    }
    return id;
}

void cadastrabook(void) {
    printf("Cadastramento de livros\n\n");

    Livro l;
    printf("Digite o titulo do livro:   ");
    fgets(l.titulo, sizeof(l.titulo), stdin);
    l.titulo[strcspn(l.titulo, "\n")] = '\0';

    printf("Digite o autor do livro:   ");
    fgets(l.autor, sizeof(l.autor), stdin);
    l.autor[strcspn(l.autor, "\n")] = '\0';


    printf("Digite o ano do livro:   ");
    scanf("%d", &l.ano);
    clean_stdin();

    FILE *livros = fopen("livros.dat", "rb");

    if (livros != NULL) {
        l.id = buscaid(livros);
        fclose(livros);
    } else {
        l.id = 1;
    }

    l.emprestado = false;

    livros = fopen("livros.dat", "ab");

    fwrite(&l, sizeof(Livro), 1, livros);
    fclose(livros);

    printf("O livro foi cadastrado\n");
    printf("O id do seu livro e:    %d\n", l.id);
    return;
}

void listabook(void) {
    int bookid;
    printf("Digite o id do livro:   ");
    scanf("%d", &bookid);
    clean_stdin();

    Livro l;
    bool encontrado = false;

    FILE *arquivo = fopen("livros.dat", "rb");
    if (!arquivo) {
        printf("Nenhum livro cadastrado\n");
        return;
    }

    while (fread(&l, sizeof(Livro), 1, arquivo) == 1) {
        if (l.id == bookid) {
            printf("%s - %s - %d\n", l.titulo, l.autor, l.ano);
            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        printf("o livro nao existe\n");
    }

    fclose(arquivo);
}

void editabook(void){
    int bookid;
    printf("Digite o id do livro:   ");
    scanf("%d", &bookid);
    clean_stdin();

    Livro l;

    FILE *arquivo = fopen("livros.dat", "rb+");

    if (!arquivo) {
        printf("Nenhum livro cadastrado");
        return;
    }

    while (fread(&l, sizeof(Livro), 1, arquivo) == 1) {
        if(l.id == bookid){
            printf("Digite o titulo do livro:   ");
            fgets(l.titulo, sizeof(l.titulo), stdin);
            l.titulo[strcspn(l.titulo, "\n")] = '\0';

            printf("Digite o autor do livro:   ");
            fgets(l.autor, sizeof(l.autor), stdin);
            l.autor[strcspn(l.autor, "\n")] = '\0';

            printf("Digite o ano do livro:   ");
            scanf("%d", &l.ano);
            clean_stdin();


            if (fseek(arquivo, -(long)sizeof(Livro), SEEK_CUR) != 0) {
                printf("Erro ao posicionar no arquivo\n");
                fclose(arquivo);
                return;
            }
            if (fwrite(&l, sizeof(Livro), 1, arquivo) != 1) {
                printf("Erro ao salvar alterações\n");
                fclose(arquivo);
                return;
            }
            fclose(arquivo);

            printf("O livro foi editado\n");
            return;
        }
    }
    printf("o livro nao existe\n");
    fclose(arquivo);
}

void emprestabook(void){
int bookid;
    printf("Digite o id do livro:   ");
    scanf("%d", &bookid);
    clean_stdin();

    Livro l;

    FILE *arquivo = fopen("livros.dat", "rb+");

    if (!arquivo) {
        printf("Nenhum livro cadastrado");
        return;
    }
    while (fread(&l, sizeof(Livro), 1, arquivo) == 1) {
        if(l.id == bookid){
            if(l.emprestado == true){
                printf("Esse livro ja foi emprestado para outra pessoa\n");
                return;
            }
            l.emprestado = true;

            if (fseek(arquivo, -(long)sizeof(Livro), SEEK_CUR) != 0) {
                printf("Erro ao posicionar no arquivo\n");
                fclose(arquivo);
                return;
            }
            if (fwrite(&l, sizeof(Livro), 1, arquivo) != 1) {
                printf("Erro ao salvar alterações\n");
                fclose(arquivo);
                return;
            }
            fclose(arquivo);

            printf("O livro foi emprestado\n");
            return;
        }
    }
    printf("o livro nao existe\n");
    fclose(arquivo);
}

void devolvebook(void){
int bookid;
    printf("Digite o id do livro:   ");
    scanf("%d", &bookid);
    clean_stdin();

    Livro l;

    FILE *arquivo = fopen("livros.dat", "rb+");

    if (!arquivo) {
        printf("Nenhum livro cadastrado");
        return;
    }
    while (fread(&l, sizeof(Livro), 1, arquivo) == 1) {
        if(l.id == bookid){
            if(l.emprestado == false){
                printf("Esse livro ainda nao foi emprestado\n");
                return;
            }
            l.emprestado = false;

            if (fseek(arquivo, -(long)sizeof(Livro), SEEK_CUR) != 0) {
                printf("Erro ao posicionar no arquivo\n");
                fclose(arquivo);
                return;
            }
            if (fwrite(&l, sizeof(Livro), 1, arquivo) != 1) {
                printf("Erro ao salvar alterações\n");
                fclose(arquivo);
                return;
            }
            fclose(arquivo);

            printf("O livro foi devolvido\n");
            return;
        }
    }
    printf("o livro nao existe\n");
    fclose(arquivo);
}

int main(void) {
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
        clean_stdin();

        switch(opcao){
            case 1:
                cadastrabook();
                break;
            case 2:
                listabook();
                break;
            case 3:
                editabook();
                break;
            case 4:
                emprestabook();
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
    return 0;
}