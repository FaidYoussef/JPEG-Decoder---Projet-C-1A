#include <stdlib.h>
#include <stdbool.h>

#include <huffman.h>

bool verbose = 1;

int main(void){
    verbose = 1;
    
    int* huff_table = calloc(20, sizeof(int));
    huff_table[0] = 0x0;
    huff_table[1] = 3;
    huff_table[2] = 2;
    huff_table[16] = 0x0d;
    huff_table[17] = 0x0e;
    huff_table[18] = 0x0a;
    huff_table[19] = 0x0c;
    // huff_table[20] = 0x0b;

    huffman(huff_table, &huff_table[16]);
    free(huff_table);

    return EXIT_SUCCESS;
}