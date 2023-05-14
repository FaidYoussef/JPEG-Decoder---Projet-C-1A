#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <huffman.h>

#define ONE 0x1
#define EOB 0x00
#define ZRL 0x78



//**********************************************************************************************************************
// Structure représentant un noeud de l'arbre de Huffman
struct node {
    int8_t symbol;
    struct node *left;
    struct node *right;
};


// Crée un nouveau noeud de l'arbre de Huffman
struct node *create_node(int8_t symbol, struct node *left, struct node *right) {
    struct node *new_node = (struct node *) malloc(sizeof(struct node));
    if (check_memory_allocation((void *) new_node)) return NULL;

    new_node->symbol = symbol;
    new_node->left = left;
    new_node->right = right;
    
    return new_node;
}


//**********************************************************************************************************************
// Construit l'arbre de huffman à partir de la table de huffman
struct node * build_huffman_tree(unsigned char *ht_data) {
    getHighlyVerbose() ? fprintf(stderr, "\tHuffman Tree :\n"):0;
    // On vérifie que la table de Huffman est valide
    if (ht_data == NULL) {
        fprintf(stderr, "Error: invalid Huffman table\n");
        return NULL;
    }
    // Il faut également vérifier que le nombre de code pour chaque longueur est valide
    for (uint16_t i = 0; i < 16; i++) {
        uint16_t nb_max_symbols_per_level = (ONE << (i+1)) - 1;
        getHighlyVerbose() ? fprintf(stderr, "\t\tnb_max_symbols_per_level: %d\n", nb_max_symbols_per_level):0;
        getHighlyVerbose() ? fprintf(stderr, "\t\tNombre de codes: %d\n", ht_data[i]):0;
        if (ht_data[i] > nb_max_symbols_per_level) {
            fprintf(stderr, "Error: invalid Huffman table - too much symbols per level\n");
            return NULL;
        }
    }

    struct node *root, *current_node;
    if ( (root = create_node(0, NULL, NULL)) == NULL) return NULL;
    int pos = 16;
    uint16_t code = 0;

    getHighlyVerbose() ? fprintf(stderr, "\t\tSymbol(s): "):0;
    for (int i = 1; i <= 16; i++) {
        for (int j = 0; j < ht_data[i - 1]; j++) {
            current_node = root;
            for (int k = i - 1; k >= 0; k--) {
                if (code & (1 << k)) {
                    if (!current_node->right) {
                        if ( (current_node->right = create_node(0, NULL, NULL)) == NULL) {
                            free_huffman_tree(root);
                            return NULL;
                        }
                    }
                    current_node = current_node->right;
                } else {
                    if (!current_node->left) {
                        if ( (current_node->left = create_node(0, NULL, NULL)) == NULL) {
                            free_huffman_tree(root);
                            return NULL;
                        }
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
            getHighlyVerbose() ? fprintf(stderr, " '%x' ", current_node->symbol):0;
            code++;
        }
        code <<= 1;
    }
    getHighlyVerbose() ? fprintf(stderr, "\n"):0;
    return root;
}


// Fonction qui free l'arbre de Huffman
void free_huffman_tree(struct node *root) {
    if (root == NULL) {
        return;
    }
    free_huffman_tree(root->left);
    free_huffman_tree(root->right);
    free(root);
}


// Affiche la représentation binaire d'un entier
void print_binary(uint16_t value, int length) {
    for (int i = length ; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
}


// Affiche la représentation binaire d'un code de huffman
void print_huffman_codes(int *bit_lengths, int8_t *symbols, int n) {
    uint16_t code = 0;
    int symbol_index = 0;

    for (int i = 1; i < n; i++) {
        int bit_length = bit_lengths[i];

        for (int j = 0; j < bit_length; j++) {
            getHighlyVerbose() ? printf("Symbol: %d, Code: ", symbols[symbol_index]):0;
            getHighlyVerbose() ? print_binary(code, i):0;
            getHighlyVerbose() ? printf("\n"):0;
            code++;
            symbol_index++;
        }

        code <<= 1;
    }
}


//**********************************************************************************************************************
// Renvoie la valeur du coefficient DC à partir de sa magnitude et de son indice dans la classe de magnitude
int16_t recover_DC_coeff_value(int8_t magnitude, int16_t indice_dans_classe_magnitude) {
    if (indice_dans_classe_magnitude < 0){
        fprintf(stderr, "Error: invalid DC coefficient\n");
        exit(EXIT_FAILURE);
    }
    if (magnitude != 0 && indice_dans_classe_magnitude < (1 << (magnitude - 1))){
        indice_dans_classe_magnitude -= (1 << magnitude) - 1;
    }
    return indice_dans_classe_magnitude;
}


// Renvoie la valeur du coefficient AC à partir de sa magnitude et de son indice dans la classe de magnitude
int16_t recover_AC_coeff_value(int8_t magnitude, int16_t indice_dans_classe_magnitude) {
    if (indice_dans_classe_magnitude == -1){
        fprintf(stderr, "Error: invalid AC coefficient\n");
        exit(EXIT_FAILURE);
    }
    if (magnitude != 0 && indice_dans_classe_magnitude < (1 << (magnitude - 1))){
        indice_dans_classe_magnitude -= (1 << magnitude) - 1;
    }
    return indice_dans_classe_magnitude;
}


//**********************************************************************************************************************
// Décode un MCU
// utilise les tables de Huffman de la composante
// puis récupère les valeurs à encoder via RLE et encodage via magnitude
int8_t decode_MCU(struct JPEG *jpeg, size_t MCU_number, int8_t component_index, int* previous_DC_value) {
    // On récupère les 64 valeurs du bloc 8x8
    struct ComponentSOS *component = get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), component_index);
    unsigned char *bitstream = get_JPEG_image_data(jpeg);
    size_t bitstream_size_in_bits = get_JPEG_image_data_size_in_bits(jpeg);

    struct node *current_node = get_ht_tree(get_JPEG_ht(jpeg, get_DC_huffman_table_id(component)));
    int8_t nombre_de_valeurs_decodees = 0;
    size_t current_pos = 0;

    getHighlyVerbose() ? fprintf(stderr, "Decoding MCUs:\n"):0;
    getHighlyVerbose() ? fprintf(stderr, "\tMCU#%ld:\n", MCU_number):0;
    getHighlyVerbose() ? fprintf(stderr, "\t\tComponent#%d\n", component_index):0;
    getHighlyVerbose() ? fprintf(stderr, "\t\t\tDC huffman table id: %d\n", get_DC_huffman_table_id(component)):0;
    getHighlyVerbose() ? fprintf(stderr, "\t\t\tAC huffman table id: %d\n", get_AC_huffman_table_id(component)):0;

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
            return EXIT_FAILURE;
        }
        
        if (!current_node->left && !current_node->right) {  // On est sur une feuille
            // (2) On récupère la valeur de magnitude associée
            int8_t magnitude_DC = current_node->symbol;
            getHighlyVerbose() ? fprintf(stderr, "\t\t\tmagnitude_DC :%x\n", magnitude_DC):0;

            if (magnitude_DC < 0) {
                fprintf(stderr, "Error: invalid DC magnitude - negative value\n");
                return EXIT_FAILURE;
            } else if (magnitude_DC > 11) {
                fprintf(stderr, "Error: invalid DC magnitude - value over 11\n");
                return EXIT_FAILURE;
            }
            
            // (3) On récupère l'indice dans la classe de magnitude associé
            // Il faut lire le bon nombre de bit(s) ... et reconstruire l'indice bit après bit
            int16_t indice_dans_classe_magnitude_DC = 0;
            for (int8_t j = 0; j < magnitude_DC; j++){
                indice_dans_classe_magnitude_DC <<= 1;
                indice_dans_classe_magnitude_DC += (bitstream[(i + 1 + j) / 8] >> (7 - ((i + 1 + j) % 8))) & 1;
            }

            // (4) On récupère finalement la valeur du coefficient DC à partir de la magnitude et de l'indice dans la classe de magnitude
            int16_t DC_value = recover_DC_coeff_value(magnitude_DC, indice_dans_classe_magnitude_DC) - *previous_DC_value;
            set_value_in_MCU(component, MCU_number, nombre_de_valeurs_decodees++, DC_value);
            *previous_DC_value = get_MCUs(component)[MCU_number][DC_VALUE_INDEX];
            getHighlyVerbose() ? fprintf(stderr, "\t\t| %x-%d | \n", DC_value, nombre_de_valeurs_decodees):0;

            // On prépare la suite en repositionnant le current_node sur la racine de l'arbre des coefficients AC
            current_node = get_ht_tree(get_JPEG_ht(jpeg, get_AC_huffman_table_id(component)));

            // Et on réaffecte la position courante dans le bitstream pour la suite
            current_pos = i + magnitude_DC + 1;

            break;  // On a récupéré la valeur du coefficient DC, on peut passer à la suite
        }

        // On prévoit le cas où on a atteint la fin du bitstream sans avoir trouvé les 64 valeurs du MCU en cours de décodage
            if (nombre_de_valeurs_decodees < 64 && current_pos == bitstream_size_in_bits - 1) {
                fprintf(stderr, "Error: invalid bitstream - does not contain enough values for current MCU#%ld\n", MCU_number);
                return EXIT_FAILURE;
            }
    }

    // On décode pour trouver les 63 valeurs des coefficients AC
    while(nombre_de_valeurs_decodees < 64){
        for (size_t i = current_pos; i < bitstream_size_in_bits; i++) {
            getHighlyVerbose() ? fprintf(stderr, "current_pos = %ld\n", current_pos):0;
            // (1) On lit le code de Huffman
            // On détermine le bit actuel en inspectant l'octet approprié dans bitstream
            // puis en décalant et en masquant le bit approprié
            unsigned char current_bit = (bitstream[(int)(i / 8)] >> (7 - (i % 8))) & 1;

            // On parcours l'arbre de Huffman à la recherche d'un mot de code
            current_node = (current_bit == 1) ? current_node->right : current_node->left;

            // On détecte une erreur éventuelle d'encodage
            if (!current_node) {
                fprintf(stderr, "Error: invalid Huffman code\n");
                return EXIT_FAILURE;
            }

            // On est sur une feuille
            if (!current_node->left && !current_node->right) {
                // (2) On récupère le Run/Size associé pour déterminer :
                // >>> 4 MSB : combien de coefficients nuls précèdent ce coefficient AC
                // >>> 4 LSB : la magnitude du coefficient AC (Note: valeur comprise entre 0 et A >>> prévoir vérification de la conformité de la valeur lue)
                uint8_t run_and_size = current_node->symbol;
                getHighlyVerbose() ? fprintf(stderr, ">>> run_and_size = %x\n", run_and_size):0;

                
                // (3) On récupère la valeur du coefficient AC à partir du Run/Size
                if (run_and_size == EOB){   // (3a) On gère le cas spécial EOB
                    for (int8_t j = 0; j < (64 - nombre_de_valeurs_decodees); j++){
                        set_value_in_MCU(component, MCU_number, nombre_de_valeurs_decodees++, 0);
                        getHighlyVerbose() ? fprintf(stderr, "%x-%d | ", 0x0, nombre_de_valeurs_decodees):0;

                        // On réaffecte la position courante dans le bitstream pour la suite
                        current_pos++;
                    }
                } else if (run_and_size == ZRL){   // (3b) On gère le cas spécial ZRL
                    for (int j = 0; j < 16; j++){
                        set_value_in_MCU(component, MCU_number, nombre_de_valeurs_decodees++, 0);
                        getHighlyVerbose() ? fprintf(stderr, "%x-%d | ", 0x0, nombre_de_valeurs_decodees):0;

                        if (nombre_de_valeurs_decodees > 64){
                            fprintf(stderr, "Error: invalid number of decoded values - RLE exeeded MCU size\n");
                            return EXIT_FAILURE;
                        }

                        // On réaffecte la position courante dans le bitstream pour la suite
                        current_pos++;
                    }
                } else {    // (3c) Sinon On ajoute le bon nombre de coefficients nuls avant le coefficient AC
                    uint8_t nb_de_coeff_nuls_a_ajouter_avant = run_and_size >> 4;
                    for (int8_t j = 0; j < nb_de_coeff_nuls_a_ajouter_avant; j++){
                        set_value_in_MCU(component, MCU_number, nombre_de_valeurs_decodees++, 0);
                        getHighlyVerbose() ? fprintf(stderr, "\t\t\t| %x-%d |\n", 0x0, nombre_de_valeurs_decodees):0;
                    }

                    // (4) Puis on récupère la magnitude du coefficient AC
                    uint8_t magnitude_AC = run_and_size & 0x0F; // on récupère les 4 LSB en appliquant un masque
                    if (magnitude_AC > 16){
                        fprintf(stderr, "Error: invalid magnitude_AC value - value over 10\n");
                        return EXIT_FAILURE;
                    }
                    getHighlyVerbose() ? fprintf(stderr, "magnitude_AC = %x - ", magnitude_AC):0;
                    int16_t indice_dans_classe_magnitude_AC = 0;
                    for (uint8_t j = 0; j < magnitude_AC; j++){
                        indice_dans_classe_magnitude_AC <<= 1;
                        indice_dans_classe_magnitude_AC = (bitstream[(int)((current_pos + 1 + j) / 8)] >> (7 - ((current_pos + 1 + j) % 8))) & 1;
                        getHighlyVerbose() ? fprintf(stderr, "%x", indice_dans_classe_magnitude_AC):0;
                    }

                    // (5) On récupère finalement la valeur du coefficient AC à partir de la magnitude et de l'indice dans la classe de magnitude
                    int16_t AC_value = recover_AC_coeff_value(magnitude_AC, indice_dans_classe_magnitude_AC);
                    set_value_in_MCU(component, MCU_number, nombre_de_valeurs_decodees++, AC_value);
                    getHighlyVerbose() ? fprintf(stderr, "\t\t%x-%d | \n", AC_value, nombre_de_valeurs_decodees):0;

                    // On réaffecte la position courante dans le bitstream pour la suite
                    current_pos = i + magnitude_AC;
                    i += magnitude_AC;

                }

                // On prépare la suite en repositionnant le current_node sur la racine de l'arbre des AC
                current_node = get_ht_tree(get_JPEG_ht(jpeg, get_AC_huffman_table_id(component)));
            }
            current_pos++;

            // On prévoit le cas où on a atteint la fin du bitstream sans avoir trouvé les 64 valeurs du MCU en cours de décodage
            if (nombre_de_valeurs_decodees < 64 && current_pos == bitstream_size_in_bits - 1) {
                fprintf(stderr, "Error: invalid bitstream - does not contain enough values for current MCU#%ld\n", MCU_number);
                exit(EXIT_FAILURE);
            }
        }

        
    }
    return EXIT_SUCCESS;
}


//**********************************************************************************************************************
// Décode le bitstream et récupère les MCU de chacune des composantes
int8_t decode_bitstream(struct JPEG * jpeg){
    unsigned int nb_mcu_width = (get_JPEG_width(jpeg) + 7) / 8;
    unsigned int nb_mcu_heigth = (get_JPEG_height(jpeg) + 7) / 8;

    int previous_DC_values[3] = {0};

    // On parcours tous les MCUs de l'image
    for (unsigned int i = 0; i < nb_mcu_width * nb_mcu_heigth; i++){
        // Prévoir possibilité de reset-er les données `previous_DC_values` dans le cas où l'on a
        // plusieurs scans/frames ---> mode progressif
        
        // On parcours toutes les composantes
        for (unsigned int j = 0; j < get_sos_nb_components(get_JPEG_sos(jpeg)[0]); ++j) {   // attention ici l'index 0 correspond au 1er scan/frame ... prévoir d'intégrer un index pour le mode progressif
            if (!decode_MCU(jpeg, i, j, &previous_DC_values[j])) {
                // free_memory(jpeg);
                return EXIT_FAILURE;
            }
        }

    }
    return EXIT_SUCCESS;
}
