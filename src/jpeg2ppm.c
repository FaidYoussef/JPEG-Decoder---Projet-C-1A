#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char **argv) {
    if (argc != 2) {
    	/* 
            Si y'a pas au moins un argument en ligne de commandes, on
    	    boude. 
        */
    	fprintf(stderr, "Usage: %s fichier.jpeg\n", argv[0]);
    	return EXIT_FAILURE;
    }

    // Ouverture du fichier et vérification de la conformité du type
    FILE *input;
    if( (input = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    
    // Vérification JPEG Magic number 
    unsigned char first3bytes[3];
    fread(first3bytes, sizeof(first3bytes), 1, input);
    unsigned char JPEG_magic_Number[3] = {0xff, 0xd8, 0xff};

    for (int i=0; i<3; i++){
        if (first3bytes[i] != JPEG_magic_Number[i]){
            fprintf(stderr, "Le fichier %s n'est pas un fichier JPEG\n", argv[1]);
            return EXIT_FAILURE;
        }
    }

    // Récupération données en-tête
    //unsigned char bflecture[2];
    unsigned char bf[1]; // Buffer
    unsigned char id[1]; // Buffer de lecture pour déterminer le type de données

    while (1){
        fread(bf, 1, 1, input);
        //printf("%x\n", bf[0]);
        if (bf[0] == 0xff){
            fread(id, 1, 1, input);
            if (id[0] == 0xdb){
                printf("Quantization table\n");

            } else if (id[0] == 0xc0){
                printf("Start of frame\n");

            } else if (id[0] == 0xc4){
                printf("Huffman table\n");

            } else if (id[0] == 0xda){
                printf("Start of scan + data\n");

            } else if (id[0] == 0xd9){
                printf("Fin du fichier\n");
                break;
            }
        }
    }

    return EXIT_SUCCESS;
}
