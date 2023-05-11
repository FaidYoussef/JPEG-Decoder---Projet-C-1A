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

// fonction qui convertit une valeur décimale en hexadécimal et la retourne
int dec2hex(int decimal) {
    int hexadecimal = 0, i = 1, reste;
    while (decimal != 0){
        reste = decimal % 16;
        decimal /= 16;
        hexadecimal += reste * i;
        i *= 10;
    }
    return hexadecimal;
}


// Construit l'arbre de huffman à partir de la table de huffman
struct node * build_huffman_tree(unsigned char *ht_data) {
    // On vérifie que la table de Huffman est valide
    if (ht_data == NULL) {
        fprintf(stderr, "Error: invalid Huffman table\n");
        exit(EXIT_FAILURE);
    }
    // Il faut également vérifier que le nombre de code pour chaque longueur est valide

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

            // // On vérifie que le code à enregistrer ne contient pas que des '1'
            // if (code == 1 || code == 3 || code == 7 || code == 15 || code == 31 || code == 63 || code == 127 || code == 255 || code == 511 || code == 1023 || code == 2047 || code == 4095 || code == 8191 || code == 16383 || code == 32767 || code == 65535) {
            //     printf("Code: %d\n", code);
            //     fprintf(stderr, "Error: invalid Huffman code\n");
            //     exit(EXIT_FAILURE);
            // }

            current_node->symbol = ht_data[pos++];
            code++;
        }
        code <<= 1;
    }
    return root;
}


// Décode un bitstream
// utilise les tables de Huffman
// puis récupère les valeurs à encoder via Run/Size data
// La sortie est un vecteur contenant les blocs de 64 valeurs de chacun des MCU les uns à la suite des autres
unsigned char * decode_bitstream(unsigned char *ht_DC_data, unsigned char *ht_AC_data, unsigned char *bitstream, size_t bitstream_size_in_bits) {
    getVerbose() ? fprintf(stderr, "Decoding bitstream:\n"):0;
    struct node *root_ht_DC_data, *root_ht_AC_data, *current_node;
    root_ht_DC_data = build_huffman_tree(ht_DC_data);
    root_ht_AC_data = build_huffman_tree(ht_AC_data);

    size_t max_output_size = 1024;
    unsigned char *decoded_bitstream = (unsigned char *) malloc(max_output_size);
    check_memory_allocation((void *) decoded_bitstream);

    size_t output_pos = 0;
    current_node = root_ht_DC_data;
    size_t current_pos = 0;
    size_t MCU_number = 0;

    // On lit le bitstream jusqu'à la fin
    while(current_pos < bitstream_size_in_bits) {
        // On récupère les 64 valeurs du bloc 8x8 ... avant de passer au prochain MCU
        int nombre_de_valeurs_decodees = 0;

        getVerbose() ? fprintf(stderr, "\tMCU#%ld:", MCU_number):0;

        // On décode pour trouver la valeur du coefficient DC
        for (size_t i = current_pos; i < bitstream_size_in_bits; i++) {
            // (1) On lit le code de Huffman
            // On détermine le bit actuel en inspectant l'octet approprié dans bitstream
            // puis en décalant et en masquant le bit approprié
            unsigned char current_bit = (bitstream[i / 8] >> (7 - (i % 8))) & 1;

            // On parcours l'arbre de Huffman à la recherche d'un mot de code
            current_node = (current_bit == 1) ? current_node->right : current_node->left;

            // On détecte une erreur éventuelle d'encodage
            if (!current_node) {
                fprintf(stderr, "Error: invalid Huffman code\n");
                exit(EXIT_FAILURE);
            }

            if (!current_node->left && !current_node->right) {  // on est sur une feuille
                // (2) On récupère la valeur de magnitude associée
                int magnitude_DC = current_node->symbol;
                
                // (3) On récupère l'indice dans la classe de magnitude associé
                // Il faut lire le bon nombre de bit(s) ... et reconstruire l'indice bit après bit
                int indice_dans_classe_magnitude_DC = 0;
                for (int j = 0; j < magnitude_DC; j++){
                    indice_dans_classe_magnitude_DC += (bitstream[(i + 1 + j) / 8] >> (7 - ((i + 1 + j) % 8))) & 1;    // pas sûr de ce calcul ... à verifier !!!
                    indice_dans_classe_magnitude_DC <<= 1;
                }
                // (4) On récupère finalement la valeur du coefficient DC à partir de la magnitude et de l'indice dans la classe de magnitude
                int DC_value = 0;
                // ***** A COMPLETER *****
                // ***** A COMPLETER *****
                // ***** A COMPLETER *****

                decoded_bitstream[output_pos++] = DC_value;
                nombre_de_valeurs_decodees++;
                getVerbose() ? fprintf(stderr, " | %x-%d | ", DC_value, nombre_de_valeurs_decodees):0;

                // On prépare la suite en repositionnant le current_node sur la racine de l'arbre des coefficients AC
                current_node = root_ht_AC_data;

                // Et on réaffecte la position courante dans le bitstream pour la suite
                current_pos = i + magnitude_DC + 1;

                // On réalloue de la mémoire si nécessaire
                if (output_pos >= max_output_size) {
                    max_output_size *= 2;
                    decoded_bitstream = (unsigned char *)realloc(decoded_bitstream, max_output_size);
                }
                break;  // On a récupéré la valeur du coefficient DC, on peut passer à la suite
            }
        }

        // On décode pour trouver les 63 valeurs des coefficients AC
        while(nombre_de_valeurs_decodees < 64){
            for (size_t i = current_pos; i < bitstream_size_in_bits; i++) {
                // (1) On lit le code de Huffman
                // On détermine le bit actuel en inspectant l'octet approprié dans bitstream
                // puis en décalant et en masquant le bit approprié
                unsigned char current_bit = (bitstream[(int)(i / 8)] >> (7 - (i % 8))) & 1;

                // On parcours l'arbre de Huffman à la recherche d'un mot de code
                current_node = (current_bit == 1) ? current_node->right : current_node->left;

                // On détecte une erreur éventuelle d'encodage
                if (!current_node) {
                    fprintf(stderr, "Error: invalid Huffman code\n");
                    exit(EXIT_FAILURE);
                }

                if (!current_node->left && !current_node->right) {
                    // (2) On récupère le Run/Size associé pour déterminer :
                    // >>> 4 MSB : combien de coefficients nuls précèdent ce coefficient AC
                    // >>> 4 LSB : la magnitude du coefficient AC (Note: valeur comprise entre 0 et A >>> prévoir vérification de la conformité de la valeur lue)
                    int run_and_size = current_node->symbol;

                    // (3) On ajoute le bon nombre de coefficients nuls avant le coefficient AC
                    int nb_de_coeff_nuls_a_ajouter_avant = run_and_size >> 4;
                    for (int j = 0; j < nb_de_coeff_nuls_a_ajouter_avant; j++){
                        decoded_bitstream[output_pos++] = 0;
                        nombre_de_valeurs_decodees++;
                        getVerbose() ? fprintf(stderr, "%x-%d | ", 0x0, nombre_de_valeurs_decodees):0;
                    }

                    // (4) On récupère la magnitude du coefficient AC
                    int magnitude_AC = run_and_size;  // attention il faut lire en decimal et non pas en hexadécimal >>> à modifier !!!
                    magnitude_AC <<= 4;
                    magnitude_AC >>= 4;
                    int indice_dans_classe_magnitude_AC = 0;
                    for (int j = 0; j < magnitude_AC; j++){
                        indice_dans_classe_magnitude_AC = bitstream[i + 1 + j];
                        indice_dans_classe_magnitude_AC <<= 1;
                    }

                    // (5) On récupère finalement la valeur du coefficient AC à partir de la magnitude et de l'indice dans la classe de magnitude
                    int AC_value = 0;
                    // ***** A COMPLETER *****
                    // ***** A COMPLETER *****
                    // ***** A COMPLETER *****

                    decoded_bitstream[output_pos++] = AC_value;
                    nombre_de_valeurs_decodees++;
                    getVerbose() ? fprintf(stderr, "%x-%d | ", AC_value, nombre_de_valeurs_decodees):0;

                    // On prépare la suite en repositionnant le current_node sur la racine de l'arbre des AC
                    current_node = root_ht_AC_data;

                    // Et on réaffecte la position courante dans le bitstream pour la suite
                    current_pos = i + magnitude_AC + 1;

                    // On réalloue de la mémoire si nécessaire
                    if (output_pos >= max_output_size) {
                        max_output_size *= 2;
                        decoded_bitstream = (unsigned char *)realloc(decoded_bitstream, max_output_size);
                    }
                }
            }

            // On prévoit le cas où on a atteint la fin du bitstream sans avoir trouvé les 64 valeurs du MCU en cours de décodage
            if (nombre_de_valeurs_decodees < 64 && current_pos == bitstream_size_in_bits - 1) {
                fprintf(stderr, "Error: invalid bitstream - does not contain enough values for current MCU#%ld\n", MCU_number);
                exit(EXIT_FAILURE);
            }
        }
        MCU_number++;
    }
    return decoded_bitstream;
}


// Teste la fonction decode_bitstream
void test_decode_bitstream(unsigned char *bitstream, size_t bitstream_size_in_bits, unsigned char *expected_output) {
    unsigned char ht[] = {0, 2, 1, 2, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x11, 0x12, 0xf, 0x8, 0x13, 0x0, 0x9, 0xd, 0x17, 0x1c, 0x1d, 0x25};
    // 17, 18, 15, 8, 19, 0, 9, 13, 23, 28, 29, 37
    // unsigned char ht[] = {0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xd, 0xe, 0xa, 0xc, 0xb};
    unsigned char *decoded_bitstream = decode_bitstream(ht, ht, bitstream, bitstream_size_in_bits);

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
