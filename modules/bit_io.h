#ifndef BIT_IO_H
#define BIT_IO_H

#include <stdio.h>

typedef struct {
    FILE *arquivo;
    unsigned char buffer;
    int contagem;
    int posicao_atual;
} BitBuffer;

BitBuffer *iniciar_buffer(FILE *arquivo);
void escrever_bit (BitBuffer *empacotador, int bit);
void finalizar_escrita(BitBuffer *empacotador);
int ler_bit(BitBuffer *empacotador);


#endif