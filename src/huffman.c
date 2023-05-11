#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <huffman.h>
#include <utils.h>


struct node {
    unsigned char symbol;
    struct node *left;
    struct node *right;
};


// Crée un nouveau noeud
struct node *create_node(unsigned char symbol, struct node *left, struct node *right) {
    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    check_memory_allocation((void *) new_node);

    new_node->symbol = symbol;
    new_node->left = left;
    new_node->right = right;
    
    return new_node;
}


// Affiche la représentation binaire d'un entier
void print_binary(uint16_t value, int length) {
    for (int i = length ; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
}


// Affiche la représentation binaire d'un code de huffman
void print_huffman_codes(int *bit_lengths, unsigned char *symbols, int n) {
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


// Construit l'arbre de huffman à partir de la table de huffman
struct node * build_huffman_tree(unsigned char *ht_data) {
    struct node *root, *current_node;
    root = create_node(0, NULL, NULL);
    int pos = 16;
    uint16_t code = 0;

    for (int i = 1; i <= 16; i++) {
        for (int j = 0; j < ht_data[i - 1]; j++) {
            current_node = root;
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
            current_node->symbol = ht_data[pos++];
            code++;
        }
        code <<= 1;
    }
    return root;
}


// Décode un bitstream à partir de la table de huffman
unsigned char *decode_bitstream(unsigned char *ht_DC_data, unsigned char *ht_AC_data, unsigned char *bitstream) {
    struct node *root_ht_DC_data, *root_ht_AC_data, *current_node;
    root_ht_DC_data = build_huffman_tree(ht_DC_data);
    root_ht_AC_data = build_huffman_tree(ht_AC_data);

    size_t max_output_size = 1024;
    unsigned char *decoded_bitstream = (unsigned char *)malloc(max_output_size);
    check_memory_allocation((void *) decoded_bitstream);

    size_t output_pos = 0;

    current_node = root_ht_DC_data;
    for (size_t i = 0; i<1; i++) {
        current_node = (bitstream[i] == '1') ? current_node->right : current_node->left;

        if (!current_node->left && !current_node->right) {
            decoded_bitstream[output_pos++] = current_node->symbol;
            current_node = root_ht_AC_data;

            if (output_pos >= max_output_size) {
                max_output_size *= 2;
                decoded_bitstream = (unsigned char *)realloc(decoded_bitstream, max_output_size);
            }
        }
    }

    current_node = root_ht_AC_data;
    for (size_t i = 1; bitstream[i] != '\0'; i++) {
        current_node = (bitstream[i] == '1') ? current_node->right : current_node->left;

        if (!current_node->left && !current_node->right) {
            decoded_bitstream[output_pos++] = current_node->symbol;
            current_node = root_ht_AC_data;

            if (output_pos >= max_output_size) {
                max_output_size *= 2;
                decoded_bitstream = (unsigned char *)realloc(decoded_bitstream, max_output_size);
            }
        }
    }

    decoded_bitstream[output_pos] = '\0';
    return decoded_bitstream;
}


// Teste la fonction decode_bitstream
void test_decode_bitstream(unsigned char *bitstream, unsigned char *expected_output) {
    unsigned char ht[] = {0, 2, 1, 2, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x11, 0x12, 0xf, 0x8, 0x13, 0x0, 0x9, 0xd, 0x17, 0x1c, 0x1d, 0x25};
    // 17, 18, 15, 8, 19, 0, 9, 13, 23, 28, 29, 37
    // unsigned char ht[] = {0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xd, 0xe, 0xa, 0xc, 0xb};
    unsigned char *decoded_bitstream = decode_bitstream(ht, ht, bitstream);

    if (strcmp((char *)decoded_bitstream, (char *)expected_output) == 0) {
        printf("Test successful!\n");
        printf("Decoded bitstream: ");
        for (size_t i = 0; decoded_bitstream[i] != '\0'; i++) {
            printf("%x", decoded_bitstream[i]);
        }
        printf("\n");
    } else {
        printf("Test failed!\n");
        printf("Expected output: ");
        for (size_t i = 0; expected_output[i] != 0x0; i++) {
            printf("%x", expected_output[i]);
        }
        printf("\n");
        printf("Decoded bitstream: ");
        for (size_t i = 0; decoded_bitstream[i] != 0x0; i++) {
            printf("%x", decoded_bitstream[i]);
        }
        printf("\n");
    }

    free(decoded_bitstream);
}

unsigned char * decode_bitstream2(unsigned char *ht_DC_data, unsigned char *ht_AC_data, unsigned char *bitstream, size_t bitstream_size_in_bits) {
    getVerbose() ? fprintf(stderr, "Decoding bitstream:"):0;
    struct node *root_ht_DC_data, *root_ht_AC_data, *current_node;
    root_ht_DC_data = build_huffman_tree(ht_DC_data);
    root_ht_AC_data = build_huffman_tree(ht_AC_data);

    size_t max_output_size = 1024;
    unsigned char *decoded_bitstream = (unsigned char *) malloc(max_output_size);
    check_memory_allocation((void *) decoded_bitstream);
    size_t output_pos = 0;
    current_node = root_ht_DC_data;
    size_t current_pos = 0;

    for (size_t i = 0; i < bitstream_size_in_bits; i++) {
        // détermine le bit actuel en inspectant l'octet approprié dans bitstream
        // puis en décalant et en masquant le bit approprié
        unsigned char current_bit = (bitstream[i / 8] >> (7 - (i % 8))) & 1;

        current_node = (current_bit == 1) ? current_node->right : current_node->left;

        if (!current_node->left && !current_node->right) {
            // On a lu le code de Huffman et on récupère le symbole associé (Run/Size)
            int magnitude = current_node->symbol;
            getVerbose() ? fprintf(stderr, "%x", current_node->symbol):0;
            // Il faut maintenant lire le bon nombre de bit pour reconstruire la valeur de DC
            int nb_de_bits_a_lire = magnitude;  // attention il faut lire en decimal et non pas en hexadécimal >>> à modifier !!!
            int vraie_valeur_reconstruite = 0;
            for (size_t j = 0; j < nb_de_bits_a_lire; j++){
                vraie_valeur_reconstruite = bitstream[i + 1 + j];
                vraie_valeur_reconstruite <<= 1;
            }
            // transcrire cette valeur décimale en héxadécimal !!!



            decoded_bitstream[output_pos++] = vraie_valeur_reconstruite;
            // On prépare la suite en repositionnant le current_node sur la racine de l'arbre des AC
            current_node = root_ht_AC_data;
            // Et on réaffecte la position courante dans le bitstream pour la suite
            current_pos = i+1;

            if (output_pos >= max_output_size) {
                max_output_size *= 2;
                decoded_bitstream = (unsigned char *)realloc(decoded_bitstream, max_output_size);
            }
            break;
        }
    }

    for (size_t i = current_pos; i < bitstream_size_in_bits; i++) {
        // détermine le bit actuel en inspectant l'octet approprié dans bitstream
        // puis en décalant et en masquant le bit approprié
        unsigned char current_bit = (bitstream[(int)(i / 8)] >> (7 - (i % 8))) & 1;

        current_node = (current_bit == 1) ? current_node->right : current_node->left;

        if (!current_node->left && !current_node->right) {
            decoded_bitstream[output_pos++] = current_node->symbol;
            getVerbose() ? fprintf(stderr, "%x", current_node->symbol):0;
            current_node = root_ht_AC_data;

            if (output_pos >= max_output_size) {
                max_output_size *= 2;
                decoded_bitstream = (unsigned char *)realloc(decoded_bitstream, max_output_size);
            }
        }
    }

    decoded_bitstream[output_pos] = '\0';
    return decoded_bitstream;
}
