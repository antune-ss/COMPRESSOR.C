#include <stdio.h>
#include <windows.h>
#include <locale.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include "./modules/compressor.h"

char* show_files(int modo) {
    static char caminho_final[256];

    DIR *dir = modo == 1 ? opendir("./inputs") : opendir("./compressed");
    struct dirent *entrada;

    if (dir == NULL) {
        if (modo == 1) {
            mkdir("./inputs");
            dir = opendir("./inputs");
        } else {
            mkdir("./compressed");
            dir = opendir("./compressed");
        }

        if (dir == NULL) {
            printf("Erro fatal: Nao foi possivel criar ou acessar as pastas.\n");
            return NULL; 
        }
    }

    if (dir) {
        int i = 1;

        while ((entrada = readdir(dir)) != NULL) {
            if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0) continue;

            printf("[%d] - %s\n", i, entrada->d_name);
            i++;
        }

        if (i == 1) {
            printf("  (Nenhum arquivo encontrado nesta pasta)\n");
        }
        printf("[0] Digitar um caminho manualmente\n");
    }

    int escolha;
    printf("Escolha uma opção: ");
    scanf("%d", &escolha);

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    if (escolha == 0) {
        printf("Digite o caminho completo do arquivo: ");
        fgets(caminho_final, sizeof(caminho_final), stdin);
        caminho_final[strcspn(caminho_final, "\n")] = '\0';
    } else {
        rewinddir(dir);
        int cont = 1;
        while ((entrada = readdir(dir)) != NULL) {
            if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0) continue;

            if (cont == escolha) {
                sprintf(caminho_final, modo == 1 ? "./inputs/%s" : "./compressed/%s", entrada->d_name);
                break;
            }

            cont++;
        }
    }

    closedir(dir);
    return caminho_final;
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
            case 1: {
                printf("\n--- COMPACTAÇÃO ---\n");
                
                char *caminho_origem = show_files(1);
                if (caminho_origem == NULL) break;

                char *nome_arquivo = strrchr(caminho_origem, '/');
                if (nome_arquivo != NULL) {
                    nome_arquivo++;
                } else {
                    nome_arquivo = caminho_origem;
                }

                char caminho_destino[256];

                sprintf(caminho_destino, "./compressed/%s.huff", nome_arquivo);
                printf("Destino gerado automaticamente: %s\n", caminho_destino);

                FILE *origem = fopen(caminho_origem, "rb");
                FILE *destino = fopen(caminho_destino, "wb");

                if (!origem || !destino) {
                    printf("Erro fatal: Nao foi possivel abrir os arquivos de leitura/escrita.\n");
                    if (origem) fclose(origem);
                    if (destino) fclose(destino);
                    break;
                }

                printf("Aguarde, compactando...\n");

                clock_t inicio = clock();

                compactar_arquivo(origem, destino);

                clock_t fim = clock();

                fseek(origem, 0, SEEK_END); 
                long tam_origem = ftell(origem);
                
                fseek(destino, 0, SEEK_END);
                long tam_destino = ftell(destino);

                double tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
                double taxa_compressao = (1.0 - ((double)tam_destino / tam_origem)) * 100.0;

                printf("\n=====================================\n");
                printf("       RELATÓRIO DE COMPRESSÃO       \n");
                printf("=====================================\n");
                printf(" Arquivo: %s\n", nome_arquivo);
                printf(" Tamanho Original : %ld Bytes\n", tam_origem);
                printf(" Tamanho Final    : %ld Bytes\n", tam_destino);
                printf(" Taxa de Redução  : %.2f%%\n", taxa_compressao);
                printf(" Tempo de Execução: %.3f segundos\n", tempo_gasto);
                printf("=====================================\n\n");

                fclose(origem);
                fclose(destino);

                break;
            }
            case 2: {
                printf("\n--- DESCOMPACTAÇÃO ---\n");
                
                char *caminho_origem = show_files(2);
                if (caminho_origem == NULL) break;

                char *nome_arquivo = strrchr(caminho_origem, '/');
                if (nome_arquivo != NULL) {
                    nome_arquivo++;
                } else {
                    nome_arquivo = caminho_origem;
                }

                char nome_original[256];
                strcpy(nome_original, nome_arquivo);

                char *extensao = strstr(nome_original, ".huff");
                if (extensao != NULL) {
                    *extensao = '\0';
                }

                mkdir("./output");
                char caminho_destino[256];
                sprintf(caminho_destino, "./output/%s", nome_original);
                
                printf("Destino gerado automaticamente: %s\n", caminho_destino);

                FILE *origem = fopen(caminho_origem, "rb");
                FILE *destino = fopen(caminho_destino, "wb");

                if (!origem || !destino) {
                    printf("Erro fatal: Nao foi possivel abrir os arquivos de leitura/escrita.\n");
                    if (origem) fclose(origem);
                    if (destino) fclose(destino);
                    break;
                }

                printf("Aguarde, descompactando...\n");
                
                clock_t inicio = clock();
                
                int sucesso = descompactar_arquivo(origem, destino); 
                
                clock_t fim = clock();

                if (!sucesso) {
                    printf("Erro: O arquivo .huff parece estar corrompido ou invalido.\n");
                    fclose(origem);
                    fclose(destino);
                    break;
                }

                fseek(origem, 0, SEEK_END); 
                long tam_origem = ftell(origem);
                
                fseek(destino, 0, SEEK_END);
                long tam_destino = ftell(destino);

                double tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;

                printf("\n=====================================\n");
                printf("      RELATÓRIO DE DESCOMPRESSÃO     \n");
                printf("=====================================\n");
                printf(" Arquivo Recuperado: %s\n", nome_original);
                printf(" Tamanho Compactado: %ld Bytes\n", tam_origem);
                printf(" Tamanho Restaurado: %ld Bytes\n", tam_destino);
                printf(" Tempo de Execução : %.3f segundos\n", tempo_gasto);
                printf("=====================================\n\n");

                fclose(origem);
                fclose(destino);

                break;
            }
            case 0: {
                printf("Saindo...");
                break;
            }
            default:
                printf("Opção inválida!\n");
        }

    } while (opcao != 0);
}