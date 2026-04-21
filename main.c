#include <stdio.h>
#include <windows.h>
#include <locale.h>
#include <dirent.h>
#include "./modules/compressor.h"

char show_files(int modo) {
    DIR *dir = modo == 1 ? opendir("./inputs") : opendir("./compressed");
    struct dirent *entrada;

    if (dir) {
        int i = 1;

        while ((entrada = readdir(dir)) != NULL) {
            printf("[%d] - %s\n", i, entrada->d_name);
            i++;
        }
    }
}

void compressor (int modo) {

}

int main () {
    SetConsoleOutputCP(65001);
    setlocale(LC_ALL, "pt_BR.UTF-8"); 
    
    int opcao;

    do {
        printf("1. Compactar Arquivo\n");
        printf("2. Descompactar Arquivo\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        switch (opcao) {
            case 1:
                /* code */
                break;
            case 2:
                /* code */
                break;
            case 0:
                /* code */
                break;
            default:
                printf("Opção inválida!\n");
        }

    } while (opcao != 0);
}