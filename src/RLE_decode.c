#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <RLE_decode.h>


void rle_decode(unsigned char *huffman_decoded_data, unsigned char *decoded_output) {
    int compteur=0, cpt2 = 0;
    while (1) {
        decoded_output[compteur] = huffman_decoded_data[cpt2];
        if ((int) decoded_output[compteur] == 0x0) {
            compteur += huffman_decoded_data[++cpt2];
        } else {
            compteur++;
        }
        if(compteur >= 63) {
            break;
        }
        cpt2++;
    }
}

// On affiche le résultat
void print_RLE_result(unsigned char *decoded_output) {
    printf("Bloc 8x8 décodé:\n");
    for (int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++){
            printf("%4d ", decoded_output[i * 8 + j]);
        }
        printf("\n");
    }
}
