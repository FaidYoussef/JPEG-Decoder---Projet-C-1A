#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

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
void huffman(int* huff_table, int *pData);

#endif
