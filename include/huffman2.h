#ifndef _HUFFMAN2_H_
#define _HUFFMAN2_H_

#include <stdio.h>
#include <stdlib.h>

#define SHORT_LENGTH 16
#define HUFFMAN_TABLE_SIZE 16


// structure d'un noeud de l'arbre de Huffman
struct node;

// fonction qui crée un noeud de l'arbre de Huffman
struct node* create_node(unsigned char symbol);

// fonction qui parcours un arbre binaire de gauche à droite et qui affiche les symboles
void print_tree(struct node *root);

// fonction qui fait une rotation à gauche d'un short
short rotate_left_short(short code, int taille);

// fonction qui construit l'arbre de Huffman à partir de la table de Huffman
void huffman2(int* huff_table, int *pData);


// void main(void){
//     int* huff_table = calloc(20, sizeof(int));
//     huff_table[0] = 0x0;
//     huff_table[1] = 3;
//     huff_table[2] = 2;
//     huff_table[16] = 0x0d;
//     huff_table[17] = 0x0e;
//     huff_table[18] = 0x0a;
//     huff_table[19] = 0x0c;
//     huff_table[20] = 0x0b;

//     huffman2(huff_table, &huff_table[16]);
//     free(huff_table);

// }

#endif