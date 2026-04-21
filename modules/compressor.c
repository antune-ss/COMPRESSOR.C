#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "compressor.h"
#include "priority_queue.h"
#include "huffman_tree.h"
#include "bit_io.h"

void gerar_dicionario(char dictionary[256][COLUNAS], Node *raiz, char *caminho, int profundidade) {
    if (raiz == NULL) return;

    if (raiz->dir == NULL && raiz->esq == NULL) {
        caminho[profundidade] = '\0';
        strcpy(dictionary[(unsigned char)raiz->caractere], caminho);
        return;
    }

    caminho[profundidade] = '0';
    gerar_dicionario(dictionary, raiz->esq, caminho, profundidade + 1);

    caminho[profundidade] = '1';
    gerar_dicionario(dictionary, raiz->dir, caminho, profundidade + 1);
}

void serializar_arvore(Node *raiz, BitBuffer *empacotador) {
    if (raiz == NULL) return;

    if (raiz->dir == NULL && raiz->esq == NULL) {
        escrever_bit(empacotador, 1);

        for(int i = 7; i >= 0; i--) {
            int bit_isolado = (raiz->caractere >> i) & 1;

            escrever_bit(empacotador, bit_isolado);
        }
    } else {
        escrever_bit(empacotador, 0);

        serializar_arvore(raiz->esq, empacotador);
        serializar_arvore(raiz->dir, empacotador);
    }
}

Node* desserializar_arvore(BitBuffer *empacotador) {
    int tipo_no = ler_bit(empacotador);
    if (tipo_no == -1) return NULL;

    if (tipo_no == 1) {
        unsigned char letra = 0;

        for (int i = 7; i >= 0; i--) {
            int bit_lido = ler_bit(empacotador);
            letra = letra | (bit_lido << i);
        }

        return criarNode(letra, 0);
    } else {
        Node *novo_no = criarNode('\0', 0);

        novo_no->esq = desserializar_arvore(empacotador);
        novo_no->dir = desserializar_arvore(empacotador);

        return novo_no;
    }
}

int compactar_arquivo(FILE *arquivo_origem, FILE *arquivo_destino) {
    if (!arquivo_origem || !arquivo_destino) return 0;

    unsigned int frequencias[256] = {0};
    unsigned int tamanho_original = 0;

    int lenght_array = sizeof(frequencias) / sizeof(int);
    ListaNode *fila_prioridade = NULL;

    int caractere = fgetc(arquivo_origem);

    while (caractere != EOF) {
        frequencias[caractere]++;
        tamanho_original++;
        caractere = fgetc(arquivo_origem);
    }

    for (int i = 0; i < lenght_array; i++) {
        if (frequencias[i]) {
            Node *node_arvore = criarNode((char)i, frequencias[i]);
            fila_prioridade = enqueue(fila_prioridade, node_arvore);
        }
    }

    while (tamanho_lista(fila_prioridade) > 1) {
        Node *no_primario = dequeue(&fila_prioridade);
        Node *no_secundario = dequeue(&fila_prioridade);

        Node *no_saida = fundirNode(no_primario, no_secundario);

        fila_prioridade = enqueue(fila_prioridade, no_saida);
    }

    Node *raiz = dequeue(&fila_prioridade);

    fwrite(&tamanho_original, sizeof(unsigned int), 1, arquivo_destino);

    BitBuffer *empacotador = iniciar_buffer(arquivo_destino);
    serializar_arvore(raiz, empacotador);

    char dictionary[256][COLUNAS];
    char caminho[COLUNAS];
    gerar_dicionario(dictionary, raiz, caminho, 0);

    rewind(arquivo_origem);

    caractere = fgetc(arquivo_origem);

    while (caractere != EOF) {
        int i = 0;
        while (dictionary[caractere][i] != '\0') {
            int bit_real = dictionary[caractere][i] - '0';
            escrever_bit(empacotador, bit_real);

            i++;
        }
        
        caractere = fgetc(arquivo_origem);
    }

    finalizar_escrita(empacotador);
    
    fclose(arquivo_origem);
    fclose(arquivo_destino);
    
    return 1;
}

int descompactar_arquivo(FILE *arquivo_origem, FILE *arquivo_destino) {
    if (!arquivo_origem || !arquivo_destino) return 0;

    unsigned int tamanho_original;
    if (fread(&tamanho_original, sizeof(unsigned int), 1, arquivo_origem) != 1) return 0; // Falha na leitura

    BitBuffer *empacotador = iniciar_buffer(arquivo_origem);
    Node *raiz = desserializar_arvore(empacotador);

    if (raiz == NULL) {
        free(empacotador);
        return 0;
    }

    for (unsigned int i = 0; i < tamanho_original; i++) {
        Node *atual = raiz;

        if (atual->esq == NULL && atual->dir == NULL) {
            fputc(atual->caractere, arquivo_destino);
            continue;
        }

        while (atual->esq != NULL && atual->dir != NULL) {
            int bit_atual = ler_bit(empacotador);

            if (bit_atual == 0) {
                atual = atual->esq;
            } else if (bit_atual == 1){
                atual = atual->dir;
            } else {
                break;
            }
        }

        fputc(atual->caractere, arquivo_destino);
    }

    liberarArvore(raiz);
    free(empacotador);

    return 1;
}