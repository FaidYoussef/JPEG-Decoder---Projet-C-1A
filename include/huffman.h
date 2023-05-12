#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <utils.h>
#include <verbose.h>

struct node ;

// Crée un nouveau noeud
struct node *create_node(unsigned char symbol, struct node *left, struct node *right);

// Renvoie la valeur du coefficient à partir de sa magnitude et de son indice dans la classe de magnitude
int recover_coeff_value(int8_t magnitude, int indice_dans_classe_magnitude);

// Affiche la représentation binaire d'un code de huffman
void print_huffman_codes(int *bit_lengths, unsigned char *symbols, int n);

// Conversion decimal vers hexadecimal
int dec2hex(int decimal);

// Construit l'arbre de huffman à partir de la table de huffman
struct node * build_huffman_tree(unsigned char *huff_table) ;

// Décode un bitstream
// utilise les tables de Huffman
// puis récupère les valeurs à encoder via Run/Size data
unsigned char * decode_bitstream(unsigned char *ht_DC, unsigned char *ht_AC, unsigned char *bitstream, size_t bitstream_size_in_bits);

// Teste la fonction decode_bitstream
void test_decode_bitstream(unsigned char *bitstream, size_t bitstream_size_in_bits, unsigned char *expected_output);

#endif
