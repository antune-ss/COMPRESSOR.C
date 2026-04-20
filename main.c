#include <stdio.h>
#include <locale.h>
#include "./modules/compressor.h"

int main () {
    setlocale(LC_ALL,"pt_BR.UTF-8");

    FILE *origem = fopen("teste.txt", "r");
    FILE *destino = fopen("saida.huff", "wb");

    if (origem == NULL || destino == NULL) {
        printf("Erro: Não foi possivel abrir os arquivos!\n");
        return 1;
    }

    compactar_arquivo(origem, destino);

    return 0;
}