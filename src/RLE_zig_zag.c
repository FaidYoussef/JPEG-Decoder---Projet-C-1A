#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>



/*C'est plus sympa de faire RLE et zigzag dans une seule fonction*/

// Ordre de lecture en zigzag pour un bloc 8x8
const int zigzag_order[64] = {
    0,  1,  5,  6, 14, 15, 27, 28,
    2,  4,  7, 13, 16, 26, 29, 42,
    3,  8, 12, 17, 25, 30, 41, 43,
    9, 11, 18, 24, 31, 40, 44, 53,
    10, 19, 23, 32, 39, 45, 52, 54,
    20, 22, 33, 38, 46, 51, 55, 60,
    21, 34, 37, 47, 50, 56, 59, 61,
    35, 36, 48, 49, 57, 58, 62, 63
};

void zigzag_rle_decode(int *huffman_decoded_data, int *decoded_output) {
    // RLE
    int rle_data[64] = {0};
    int rle_index = 0;

    // Décompression RLE
    for (int i = 0; i < 64; i++) {
        // On lit l'index dans l'ordre de lecture en zigzag
        int index = zigzag_order[i];
        // On lit la valeur du coefficient
        rle_data[index] = huffman_decoded_data[rle_index++];
        // Si la valeur est nulle, on lit le nombre de zéros qui suivent
        if (rle_data[index] == 0) {
            // On lit le nombre de zéros qui suivent
            int run_length = huffman_decoded_data[rle_index++];
            // Si le nombre de zéros est nul, on a fini le bloc
            if (run_length == 0) {
                // On sort de la boucle
                break; // Fin du bloc
            }
            // Sinon, on ajoute les zéros
            for (int j = 0; j < run_length - 1; j++) {
                // On incrémente l'index dans l'ordre de lecture en zigzag
                rle_data[zigzag_order[++i]] = 0;
            }
        }
    }

    for (int i = 0; i < 64; i++) {
        decoded_output[i] = rle_data[i];
    }
}

int main() {
    int huffman_decoded_data[] = {};
    int decoded_output[64] = {0};

    zigzag_rle_decode(huffman_decoded_data, decoded_output);

    printf("Bloc 8x8 décodé:\n");
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%4d ", decoded_output[i * 8 + j]);
        }
        printf("\n");
    }

    return 0;
}
