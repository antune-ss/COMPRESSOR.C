#include <stdio.h>
#include <stdlib.h>
#include "priority_queue.h"

ListaNode *enqueue(ListaNode *inicio, Node *no_arvore) {
  ListaNode *newNode = (ListaNode *) malloc(sizeof(ListaNode));

  if (newNode == NULL) {
    printf("[ERRO]: Falha na alocação de memória.\n");
    return NULL;
  }

  newNode->data = no_arvore;
  newNode->prox = NULL;

  if (inicio == NULL) return newNode;

  if (newNode->data->frequencia < inicio->data->frequencia) {
    newNode->prox = inicio;
    return newNode;
  }

  ListaNode *atual = inicio;
  while (atual->prox != NULL && atual->prox->data->frequencia <= newNode->data->frequencia) {
    atual = atual->prox;
  }

  newNode->prox = atual->prox;
  atual->prox = newNode;

  return inicio;
}

Node *dequeue(ListaNode **inicio) {
  if (*inicio == NULL) {
    printf("[ERRO]: Fila vazia.\n");
    return NULL;
  }

  ListaNode *nodeRemovido = *inicio;
  Node *nodeArvore = nodeRemovido->data;

  *inicio = nodeRemovido->prox;
  free(nodeRemovido);

  return nodeArvore;
}

int tamanho_lista(ListaNode *inicio) {
  ListaNode *aux = inicio;
  int cont = 0;
  
  while (aux != NULL) {
    aux = aux->prox;
    cont++;
  }

  return cont;
}