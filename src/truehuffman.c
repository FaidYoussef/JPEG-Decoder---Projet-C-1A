#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Les bits et les valeurs de la DHT pour les coefficients DC de la composante de luminance (Y)
const uint8_t bits[] = {0, 2, 1, 2, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const uint8_t values[] = {17, 18, 15, 8, 19, 0, 9, 13, 23, 28, 29, 37};

// 0008 01 01 00 00 3f 00 7c  d1 ca ca dc 76 da 4d 6a  
//   00 15 ed 41 f1 2d 3a dc  70 8b 16 be 4c c9 bb b3  
//   4f fb 35 b8 7d 13 ab 12  9d 0f 0e 1f 4e 1d e3 ff  
//   d9

// Le bitstream à décompresser
const char *bitstream = "01000110011111100110011011";

// void decode_bitstream(const char *bitstream, const uint8_t *bits, const uint8_t *values) {
//     int value_index = 0;
//     int current_bit_length = 1;

//     while (*bitstream) {
//         int found = 0;

//         for (int i = 0; i < bits[current_bit_length - 1]; ++i) {
//             const char *current_code = bitstream;
//             uint8_t value_copy = values[value_index];

//             for (int j = 0; j < current_bit_length; ++j) {
//                 if (*current_code != (value_copy & 1) + '0') {
//                     break;
//                 }
//                 ++current_code;
//                 value_copy >>= 1;
//             }

//             if (current_code - bitstream == current_bit_length) {
//                 found = 1;
//                 printf("%.*s -> %d\n", current_bit_length, bitstream, values[value_index]);
//                 bitstream += current_bit_length;
//                 break;
//             }

//             ++value_index;
//         }

//         if (found) {
//             current_bit_length = 1;
//             value_index = 0;
//         } else {
//             ++current_bit_length;
//             if (current_bit_length > 16) {
//                 fprintf(stderr, "Erreur : code Huffman non trouvé.\n");
//                 break;
//             }
//         }
//     }
// }

// int main() {
//     printf("Séquence de symboles décompressés :\n");
//     decode_bitstream(bitstream, bits, values);
//     return 0;
// }



