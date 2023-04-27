#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
    	/* 
            Si y'a pas au moins un argument en ligne de commandes, on
    	    boude. 
        */
    	fprintf(stderr, "Usage: %s fichier.jpeg\n", argv[0]);
    	return EXIT_FAILURE;
    }

    /* On se congratule. */
    return EXIT_SUCCESS;
}
