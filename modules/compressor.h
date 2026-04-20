#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <stdio.h>
#include "huffman_tree.h"
#include "bit_io.h"
#define COLUNAS 267

int compactar_arquivo (FILE *arquivo_origem, FILE *arquivo_destino);
int descompactar_arquivo(FILE *arquivo_origem, FILE *arquivo_destino);

#endif