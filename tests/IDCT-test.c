#include <stdlib.h>
#include <stdbool.h>

#include <IDCT.h>


int main() {

    //*************************************************************************************************
    // TEST HEADER
    fprintf(stderr, "\n");
    fprintf(stderr, YELLOW("================== TESTS IDCT ==================\n\n"));
    
    int16_t input[N * N] = {0,0,0,0,0,0,0,0,
                        1,1,1,1,1,1,1,1,
                        2,2,2,2,2,2,2,2,
                        3,3,3,3,3,3,3,3,
                        4,4,4,4,4,4,4,4,
                        5,5,5,5,5,5,5,5,
                        6,6,6,6,6,6,6,6,
                        7,7,7,7,7,7,7,7};

    int8_t res = fast_IDCT_function((int16_t **) &input);

    if (res != 0) {
        printf("Erreur lors de l'IDCT\n");
        return EXIT_FAILURE;
    } else {
        // Affiche la matrice de sortie
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                printf("%d ", input[i * 8 + j]);
            }
            printf("\n");
        }
    }

    fprintf(stderr, YELLOW("\n================================================\n"));

    return EXIT_SUCCESS;
}