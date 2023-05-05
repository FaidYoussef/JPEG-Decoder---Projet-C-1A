#include <stdbool.h>
#include "../include/huffman.h"
#include "../src/huffman.c"

bool verbose = 1;

void main(void){
    verbose = 1;
    
    int* huff_table = calloc(20, sizeof(int));
    huff_table[0] = 0x0;
    huff_table[1] = 3;
    huff_table[2] = 2;
    huff_table[16] = 0x0d;
    huff_table[17] = 0x0e;
    huff_table[18] = 0x0a;
    huff_table[19] = 0x0c;
    huff_table[20] = 0x0b;

    huffman2(huff_table, &huff_table[16]);
    free(huff_table);

}