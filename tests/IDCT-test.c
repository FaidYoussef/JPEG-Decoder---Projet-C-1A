#include <stdlib.h>
#include <stdbool.h>

#include <IDCT.h>

bool verbose = 1;

int main() {
    
    int input[N * N] = {0,0,0,0,0,0,0,0,
                        1,1,1,1,1,1,1,1,
                        2,2,2,2,2,2,2,2,
                        3,3,3,3,3,3,3,3,
                        4,4,4,4,4,4,4,4,
                        5,5,5,5,5,5,5,5,
                        6,6,6,6,6,6,6,6,
                        7,7,7,7,7,7,7,7};

    int *output = idct(input);

    // Affiche la matrice de sortie
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", output[i * 8 + j]);
        }
        printf("\n");
    }

    // On libère la mémoire
    free(output);

    return EXIT_SUCCESS;
}