#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

void print_binary(uint16_t value, int length) {
    for (int i = length ; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
}

void print_huffman_codes(int bit_lengths[], unsigned char symbols[], int n) {
    uint16_t code = 0;
    int symbol_index = 0;

    for (int i = 1; i < n; i++) {
        int bit_length = bit_lengths[i];

        for (int j = 0; j < bit_length; j++) {
            printf("Symbol: %c, Code: ", symbols[symbol_index]);
            print_binary(code, i);
            printf("\n");
            code++;
            symbol_index++;
        }

        code <<= 1;
    }
}

typedef struct node {
    unsigned char symbol;
    struct node *left;
    struct node *right;
} node;

node *create_node(unsigned char symbol, node *left, node *right) {
    node *new_node = (node *)malloc(sizeof(node));
    new_node->symbol = symbol;
    new_node->left = left;
    new_node->right = right;
    return new_node;
}





void build_huffman_tree(node **root, unsigned char *huff_table) {
    node *current_node;
    *root = create_node(0, NULL, NULL);
    int pos = 16;
    uint16_t code = 0;

    for (int i = 1; i <= 16; i++) {
        for (int j = 0; j < huff_table[i - 1]; j++) {
            current_node = *root;
            for (int k = i - 1; k >= 0; k--) {
                if (code & (1 << k)) {
                    if (!current_node->right) {
                        current_node->right = create_node(0, NULL, NULL);
                    }
                    current_node = current_node->right;
                } else {
                    if (!current_node->left) {
                        current_node->left = create_node(0, NULL, NULL);
                    }
                    current_node = current_node->left;
                }
            }
            current_node->symbol = huff_table[pos++];
            code++;
        }
        code <<= 1;
    }
}

unsigned char *decode_bitstream(unsigned char *huff_table, unsigned char *bitstream) {
    node *root, *current_node;
    build_huffman_tree(&root, huff_table);

    size_t max_output_size = 1024;
    unsigned char *decoded_bitstream = (unsigned char *)malloc(max_output_size);
    size_t output_pos = 0;

    current_node = root;
    for (size_t i = 0; bitstream[i] != '\0'; i++) {
        current_node = (bitstream[i] == '1') ? current_node->right : current_node->left;

        if (!current_node->left && !current_node->right) {
            decoded_bitstream[output_pos++] = current_node->symbol;
            current_node = root;

            if (output_pos >= max_output_size) {
                max_output_size *= 2;
                decoded_bitstream = (unsigned char *)realloc(decoded_bitstream, max_output_size);
            }
        }
    }

    decoded_bitstream[output_pos] = '\0';
    return decoded_bitstream;
}


// Insérez ici les fonctions create_node, build_huffman_tree, decode_bitstream et les structures associées.

void test_decode_bitstream() {
    unsigned char huff_table[] = {0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'd', 'e', 'a', 'c', 'b'};
    unsigned char bitstream[] = "111100001010101";
    unsigned char expected_output[] = {'b', 'a', 'd','e','e','e','e', '\0'};

    unsigned char *decoded_bitstream = decode_bitstream(huff_table, bitstream);

    if (strcmp((char *)decoded_bitstream, (char *)expected_output) == 0) {
        printf("Test successful!\n");
        printf("Decoded bitstream: ");
        for (size_t i = 0; decoded_bitstream[i] != '\0'; i++) {
            printf("%c", decoded_bitstream[i]);
        }
        printf("\n");
    } else {
        printf("Test failed!\n");
        printf("Expected output: %s\n", expected_output);
        printf("Decoded bitstream: %s\n", decoded_bitstream);
    }

    free(decoded_bitstream);
}

int main() {
    test_decode_bitstream();
    return 0;
}



// int main() {
//     int bit_lengths[] = {0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//     unsigned char symbols[] = {'d', 'e', 'a', 'c', 'b'};
//     int n = sizeof(bit_lengths) / sizeof(bit_lengths[0]);

//     print_huffman_codes(bit_lengths, symbols, n);

//     return 0;
// }
