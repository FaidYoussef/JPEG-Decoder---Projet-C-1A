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
    unsigned char magicNumber[3] = {0xff, 0xd8, 0xff};

    for (int i=0; i<3; i++){
        if (first3bytes[i] != magicNumber[i]){
            fprintf(stderr, "Le fichier %s n'est pas un fichier JPEG\n", argv[1]);
            return EXIT_FAILURE;
        }
    }

    // Récupération données en-tête



    return EXIT_SUCCESS;
}
