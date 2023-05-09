#include <stdlib.h>
#include <stdbool.h>

#include <RLE_decode.h>

int main() {
    unsigned char *huffman_decoded_data = malloc(8 * sizeof(char));
    huffman_decoded_data[0] = 0x1;
    huffman_decoded_data[1] = 0x2;
    huffman_decoded_data[2] = 0x3;
    huffman_decoded_data[3] = 0x0;
    huffman_decoded_data[4] = 0x3;
    huffman_decoded_data[5] = 0x4;
    huffman_decoded_data[6] = 0x0;
    huffman_decoded_data[7] = 0x39;

    unsigned char *decoded_output = calloc(64, sizeof(char));

    rle_decode(huffman_decoded_data, decoded_output);
    print_RLE_result(decoded_output);
    

    unsigned char *huffman_decoded_data2 = malloc(12 * sizeof(char));
    huffman_decoded_data2[0] = 0x1;
    huffman_decoded_data2[1] = 0x2;
    huffman_decoded_data2[2] = 0x3;
    huffman_decoded_data2[3] = 0x0;
    huffman_decoded_data2[4] = 0x4;
    huffman_decoded_data2[5] = 0x5;
    huffman_decoded_data2[6] = 0x6;
    huffman_decoded_data2[7] = 0x0;
    huffman_decoded_data2[8] = 0x3;
    huffman_decoded_data2[9] = 0x9;
    huffman_decoded_data2[10] = 0x0;
    huffman_decoded_data2[11] = 0x51;

    unsigned char *decoded_output2 = calloc(64, sizeof(char));

    rle_decode(huffman_decoded_data2, decoded_output2);
    print_RLE_result(decoded_output2);

    // On libère la mémoire
    free(huffman_decoded_data);
    free(decoded_output);
    free(huffman_decoded_data2);
    free(decoded_output2);

    return EXIT_SUCCESS;
}
