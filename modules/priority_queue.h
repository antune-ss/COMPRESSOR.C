#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "huffman_tree.h"

typedef struct ListaNode {
    Node *data;
    struct ListaNode *prox;
} ListaNode;

ListaNode *enqueue(ListaNode *inicio, Node *no_arvore);
Node *dequeue(ListaNode **inicio);
int tamanho_lista(ListaNode *inicio);
 
#endif 