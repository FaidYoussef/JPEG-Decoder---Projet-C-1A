#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <extract.h>
#include <utils.h>
#include <verbose.h>


//**********************************************************************************************************************
// Structure représentant un noeud de l'arbre de huffman
struct node;

// Crée un nouveau noeud
struct node * create_node(unsigned char symbol, struct node *left, struct node *right, struct JPEG *jpeg);

//**********************************************************************************************************************
// Construit l'arbre de huffman à partir de la table de huffman
struct node * build_huffman_tree(unsigned char *ht_data, struct JPEG *jpeg);

// Fonction qui free l'arbre de Huffman
void free_huffman_tree(struct node *root);

// Affiche la représentation binaire d'un entier
void print_binary(uint16_t value, int length);

// Affiche la représentation binaire d'un code de huffman
void print_huffman_codes(int *bit_lengths, unsigned char *symbols, int n);

//**********************************************************************************************************************
// Renvoie la valeur du coefficient DC à partir de sa magnitude et de son indice dans la classe de magnitude
int16_t recover_DC_coeff_value(int8_t magnitude, int16_t indice_dans_classe_magnitude);

// Renvoie la valeur du coefficient AC à partir de sa magnitude et de son indice dans la classe de magnitude
int16_t recover_AC_coeff_value(int8_t magnitude, int16_t indice_dans_classe_magnitude);

//**********************************************************************************************************************
// Décode un MCU
// utilise les tables de Huffman de la composante
// puis récupère les valeurs à encoder via RLE et encodage via magnitude
int decode_MCU(struct JPEG *jpeg, size_t MCU_number, int8_t component_index, int* previous_DC_value);

// Décode le bitstream et récupère les MCU de chacune des composantes
int decode_bitstream(struct JPEG * jpeg);

#endif
