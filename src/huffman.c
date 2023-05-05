#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <huffman.h>

#define SHORT_LENGTH_FOR_8X8_BLOCK 16
#define HUFFMAN_TABLE_SIZE 16

extern bool verbose;


// structure d'un noeud de l'arbre de Huffman
struct node {
    unsigned char symbol;
    struct node *left;
    struct node *right;
};


// fonction qui crée un noeud de l'arbre de Huffman
struct node* create_node(unsigned char symbol) {
    struct node *new_node = malloc(sizeof(struct node));
    new_node->symbol = symbol;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}


// fonction qui parcours un arbre binaire de gauche à droite et qui affiche les symboles
void print_tree(struct node *root) {
    if (root->left != NULL) {
        print_tree(root->left);
    }
    if (root->right != NULL) {
        print_tree(root->right);
    }
    if (root->symbol != NULL) {
        printf("%x\n", root->symbol);
    }
}


// fonction qui fait une rotation à gauche d'un short
short rotate_left_short(short code, int taille) {
    return (code << taille) | (code >> (SHORT_LENGTH_FOR_8X8_BLOCK-taille));
}


// fonction qui construit l'arbre de Huffman à partir de la table de Huffman
void huffman2(int* huff_table, int *pData) {
    struct node *root = create_node(NULL);
    struct node *current_node = root;
    int* current_symbol_address = pData;
    int code =0;

    for (int i= 0; i<HUFFMAN_TABLE_SIZE ; i++) {
        int taille = i+1;
        int nb_element = huff_table[i];
        for (int j=0 ; j<(int)nb_element; j++){
            
            int current_code = rotate_left_short(current_code, SHORT_LENGTH_FOR_8X8_BLOCK-taille+1);
            int taille_locale = taille;
            while(nb_element !=0 & taille_locale != 0){
                if (current_code & 1) {
                    if (current_node->right == NULL) {
                        current_node->right = create_node(NULL);
                    }
                    current_node = current_node->right;
                } else {
                    if (current_node->left == NULL) {
                        current_node->left = create_node(NULL);
                    }
                    current_node = current_node->left;
                }
                taille_locale--;
                current_code = rotate_left_short(current_code, 1);
            }
            current_node->symbol = *current_symbol_address;
            
            verbose ? printf("Le symbole %x est associé au code %d.\n", *current_symbol_address++, code++):0;
            current_node = root;
            current_code++;
        }
        code <<= 1;
    }
    verbose ? print_tree(root):0;
}
