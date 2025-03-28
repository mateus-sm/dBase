#include <stdio.h>
#include <string.h>
#include <conio2.h>
#include <stdlib.h>
#include <time.h>

#include "Tad.h"

//Tratar comando
char validaComando(char str[]);
void lerComando(char str[]);
int chaveComando(char str[]);
void extrairParametro(char str[], char str2[]) ;
void extrairParametros(char mat[5][15], char comando[]);

//Funções gerais
void print2(int x, int y, char str[]);
char compare(char str[], char str2[]);
void strSplit(char str[], char str2[], char del);

//Interface Gráfica
void moldura(int xi, int yi, int xf, int yf);
void linhaVertical(int xi, int yi, int yf);
void caixa1();
void caixa2();
void caixa3(); 
void caixa4();
void instrucoes();
void base();
void baseDir();
void baseCmd();
void baseDBF();
void createCampos();

DBF* buscaDBF(char str[], DBF *dbf) {
    DBF *aux = dbf, *ret = NULL;
    char flag = 0;

    //Ir para o começo dos DBF
    while(aux->ant != NULL) {
        aux = aux->ant;
    }

    //Procurar o DBF passado
    if (compare(aux->nomeDBF, str)) {
        ret = aux;
    }
    while(aux->prox != NULL) {
        if (compare(aux->nomeDBF, str)) {
            ret = aux;
        }
        aux = aux->prox;
    }

    return ret;
}

int main() {
    Unidade *unid = NULL;
    DBF *dbf = NULL, *aberto = NULL;

    char comando[50], cmd[15], arg[15];
    char args[4][15];

    clrscr();
    instrucoes();
    base();
    baseCmd("Command Line");
    baseDir(" ");

    do {
        lerComando(comando);
        strSplit(comando, cmd, ' ');
        // gotoxy(1, 1);
        // printf("String digitada: %s\n", comando);
        // printf("Comando extraido: %s\n", cmd);
        // printf("Chave do Comando: %d\n", chaveComando(cmd));

        switch (chaveComando(cmd)) {
            case 0:
                //Foi digitado comando que começa com "SET"
                extrairParametros(args, comando);
                //"SET DEFAULT ..."
                if (compare(args[1], "DEFAULT") && compare(args[2], "TO")) {
                    if (compare(args[3], "C:") || compare(args[3], "D:")) {
                        setDefaltTo(&unid, args[3]);
                        baseDir(unid->und);
                        baseDBF("");
                    }
                }
            break;

            case 1:
                //Foi digitado comando que começa com "CREATE"
                extrairParametro(comando, arg);
                strSplit(arg, cmd, '.');
                baseDBF(cmd);
                baseCmd("CREATE");
                createCampos();
                Create(&unid, &dbf, arg);
            break;

            case 2:
                //Foi digitado o comando "DIR"
                Dir(&unid);
            break;

            case 3:
                //Foi digitado o comando "QUIT" irá sair do loop do while
                quit();
            break;

            case 4:
                //Foi digitado o comando "USE"
                extrairParametro(comando, arg);

                if (buscaDBF(arg, dbf) != NULL) {
                    USE(&aberto, buscaDBF(arg, dbf));
                    baseDBF(arg);
                }
            break;

            case 5:
                //Foi digitado comando que começa com "LIST"
                extrairParametros(args, comando);
                if (compare(args[1], "STRUCTURE")) {
                    ListStructure(unid, dbf);
                }
            break;

            default:
                printf("Opcao invalida.\n");
        }
        
    } while (chaveComando(cmd) != 3);

    return 0;
}

void print2(int x, int y, char str[]) {
    gotoxy(x, y);
    printf("%s", str);
}

char validaComando(char str[]) {
    char flag = 0;
    char mat[17][20] = {"SET", "CREATE", "DIR", "QUIT", "USE", "LIST", "CLEAR",
                        "DISPLAY", "EDIT", "DELETE", "RECALL", "PACK", "ZAP", 
                        "MODIFY", "SORT"};

    for (int i = 0; i < 17; i++) {
        if (strcmp(str, mat[i]) == 0) {
            flag = 1;
        }
    }

    return flag;
}

void base() {
    textcolor(BLACK); textbackground(LIGHTGRAY);
    //Barra completa
    for (int i = 5; i < 89; i++) {
        gotoxy(i, 19); printf("%c", ' ');
	}
    textcolor(LIGHTGRAY); textbackground(BLACK);
}

void baseCmd(char cmd[]) {
    textcolor(BLACK); textbackground(LIGHTGRAY);
    for (int i = 0; i < 15; i++) {
        gotoxy(i + 5, 19); printf("%c", ' ');
	}
    //Linha de comando
    print2(5, 19, cmd);
    textcolor(LIGHTGRAY); textbackground(BLACK);
}

void baseDir(char dir[]) {
    textcolor(BLACK); textbackground(LIGHTGRAY);
    //Diretório atual
    gotoxy(21, 19); printf("%c", 186);
    if (dir != " ") {
        gotoxy(22, 19); printf("<%s>", dir);
    }
    gotoxy(26, 19); printf("%c", 186);
    textcolor(LIGHTGRAY); textbackground(BLACK);
}

void baseDBF(char DBF[]) {
    textcolor(BLACK); textbackground(LIGHTGRAY);
    for (int i = 0; i < 15; i++) {
        gotoxy(i + 27, 19); printf("%c", ' ');
	}
    //Linha dbf
    print2(27, 19, DBF);
    textcolor(LIGHTGRAY); textbackground(BLACK);
}

//Divide a string str baseada no delimitador del e guarda na string str2
void strSplit(char str[], char str2[], char del) {
    int i;

    for (i = 0; str[i] != del && str[i] != '\0'; i++) {
        str2[i] = str[i];
    }
    str2[i] = '\0';
}

void lerComando(char str[]) {
    char cmd[50];
    int i;

    do {
        print2(5, 18, ". ");

        strSplit(gets(str), cmd, ' ');

        gotoxy(5, 18);
        clreol();
    } while (!validaComando(cmd));
}

int chaveComando(char str[]) {
    char flag = 0;
    char mat[17][20] = {"SET", "CREATE", "DIR", "QUIT", "USE", "LIST", "CLEAR",
                        "DISPLAY", "EDIT", "DELETE", "RECALL", "PACK", "ZAP", 
                        "MODIFY", "SORT"};

    for (int i = 0; i < 17; i++) {
        if (strcmp(str, mat[i]) == 0) {
            flag = i;
        }
    } 

    return flag;
}

//Retorna tudo depois do primeiro espaço
void extrairParametro(char str[], char str2[]) {
    int i = 0, j;
    while (str[i] != ' ') {
        i++;
    }
    i++;

    for (j = 0; str[i] != '\0'; i++, j++) {
        str2[j] = str[i];
    }
    
    str2[j] = '\0';
}

void extrairParametros(char mat[5][15], char comando[]) {
    int i, j, k;
    for (i = 0, j = 0, k = 0; comando[i] != '\0'; i++) {
        if (comando[i] == ' ') {
            mat[j++][k] = '\0';
            k = 0;
        } else {
            mat[j][k++] = comando[i];
        }
    }
    mat[j][k] = '\0';
}

char compare(char str[], char str2[]) {
    return (strcmp(str, str2) == 0);
}

void moldura(int xi, int yi, int xf, int yf) {
    gotoxy(xi, yi); printf("%c", 201); // Canto superior esquerdo
    gotoxy(xf, yi); printf("%c", 187); // Canto superior direito
    gotoxy(xi, yf); printf("%c", 200); // Canto inferior esquerdo
    gotoxy(xf, yf); printf("%c", 188); // Canto inferior direito

    for (int a = xi + 1; a < xf; a++) {
        gotoxy(a, yi); printf("%c", 205); // Linha superior
        gotoxy(a, yf); printf("%c", 205); // Linha inferior
    }

    for (int b = yi + 1; b < yf; b++) {
        gotoxy(xi, b); printf("%c", 186); // Linha lateral esquerda
        gotoxy(xf, b); printf("%c", 186); // Linha lateral direita
    }
}

void linhaVertical(int xi, int yi, int yf) {
    gotoxy(xi, yi++); printf("%c", 203);
    gotoxy(xi, yf); printf("%c", 202);

    while(yi < yf) {
        gotoxy(xi, yi++); printf("%c", 186);
    }
}

void caixa1() {
    print2(7, 3, "CURSOR  <-- -->");
    print2(8, 4, "Char:   <- ->");
    print2(8, 5, "Word: Home End");
    print2(8, 6, "Pan:   ^<- ^->");
    linhaVertical(23, 2, 7);
}

void caixa2() {
    print2(30, 3, "INSERT");
    print2(26, 4, "Char:   Ins");
    print2(26, 5, "Field:  ^N");
    print2(26, 6, "Help:   F1");
    linhaVertical(39, 2, 7);
}

void caixa3() {
    print2(45, 3, "DELETE");
    print2(42, 4, "Char:    Del");
    print2(42, 5, "Word:   ^Y");
    print2(42, 6, "Field:  ^U");
    linhaVertical(55, 2, 7);
}

void caixa4() {
    print2(58, 3, "Up a field:    /\\");
    print2(58, 4, "Down a field:  \\/");
    print2(58, 5, "Exit/Save:    ^End");
    print2(58, 6, "Abort:        Esc");
}

void instrucoes() {
    //73 de largura
    //5 de altura
    moldura(5, 2, 78, 7);
    caixa1();
    caixa2();
    caixa3();
    caixa4();    
}

void createCampos() {
    print2(10, 8, "Field Name  Type      Width  Dec");
    gotoxy(10, 9);
    for (int i = 0; i < 32; i++) {
        printf("%c", 205);
    }
}