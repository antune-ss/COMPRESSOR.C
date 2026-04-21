#include <stdlib.h>
#include "bit_io.h"

BitBuffer *iniciar_buffer(FILE *arquivo) {
    BitBuffer *empacotador = (BitBuffer *) malloc(sizeof(BitBuffer));
    if (empacotador == NULL) return NULL;

    empacotador->arquivo = arquivo;
    empacotador->buffer = 0;
    empacotador->contagem = 0;
    empacotador->posicao_atual = -1;

    return empacotador;
}

void escrever_bit (BitBuffer *empacotador, int bit) {
    empacotador->buffer = (empacotador->buffer << 1) | bit;  
    empacotador->contagem++;

    if (empacotador->contagem == 8) {
        fwrite(&(empacotador->buffer), 1, 1, empacotador->arquivo);
        
        empacotador->buffer = 0;
        empacotador->contagem = 0;
    }
}

void finalizar_escrita(BitBuffer *empacotador) {
    if (empacotador->contagem > 0) {
        empacotador->buffer <<= 8 - empacotador->contagem;
        fwrite(&(empacotador->buffer), 1, 1, empacotador->arquivo);
    }

    free(empacotador);
}

int ler_bit (BitBuffer *empacotador) {
    if (empacotador->posicao_atual == -1) {
        size_t blocos_lidos = fread(&empacotador->buffer, 1, 1, empacotador->arquivo);
        if (blocos_lidos == 0) return -1;

        empacotador->posicao_atual = 7;
    }

    int bit = (empacotador->buffer >> empacotador->posicao_atual) & 1;
    empacotador->posicao_atual--;

    return bit;
}