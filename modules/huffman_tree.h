#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

typedef struct Node {
   char caractere; 
   unsigned int frequencia;
   struct Node *esq;
   struct Node *dir;
} Node;

Node *criarNode (char caractere, unsigned int frequencia);
Node *fundirNode (Node *no_primario, Node *no_secundario);
Node *liberarArvore(Node *raiz);

#endif