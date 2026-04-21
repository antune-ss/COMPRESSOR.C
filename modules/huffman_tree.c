#include <stdlib.h>
#include <stdio.h>
#include "huffman_tree.h"

Node *criarNode (char caractere, unsigned int frequencia) {
    Node *newNode = (Node *) malloc(sizeof(Node));

    if (newNode == NULL) {
        printf("[ERRO]: Falha na alocação de memória.\n");
        return NULL;
    }

    newNode->caractere = caractere;
    newNode->frequencia = frequencia;

    newNode->dir = NULL;
    newNode->esq = NULL;

    return newNode;
}

Node *fundirNode (Node *no_primario, Node *no_secundario) {
    int frequencia_pai = no_primario->frequencia + no_secundario->frequencia;
    Node *nodePai = criarNode('\0', frequencia_pai);

    nodePai->esq = no_primario;
    nodePai->dir = no_secundario;

    return nodePai;
}

Node *liberarArvore(Node *raiz) {
    if (raiz == NULL) return NULL;

    raiz->esq = liberarArvore(raiz->esq);
    raiz->dir = liberarArvore(raiz->dir);

    free(raiz);

    return NULL;
}