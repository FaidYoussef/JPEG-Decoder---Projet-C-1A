#include <stdlib.h>
#include <stdbool.h>

#include <IDCT.h>
#include <utils.h>
#include <verbose.h>


const int16_t expected_data[64] = {
    147, 123, 132, 127, 130, 128, 129, 129,
    107, 134, 123, 130, 126, 128, 127, 127,
    136, 126, 130, 127, 129, 128, 129, 128,
    121, 130, 126, 129, 127, 128, 128, 128,
    132, 127, 129, 128, 128, 128, 128, 128,
    125, 129, 127, 128, 128, 128, 128, 128,
    129, 128, 128, 128, 128, 128, 128, 128,
    127, 128, 128, 128, 128, 128, 128, 128
};



// tests IDCT
int main(int argc, char **argv) {

    // Mode verbose
    if (argc > 1 && strcmp(argv[1], "-hv") == 0) setHighlyVerbose(true);

    //*************************************************************************************************
    // TEST HEADER
    fprintf(stderr, "\n");
    fprintf(stderr, YELLOW("================== TESTS IDCT ==================\n\n"));


    //*************************************************************************************************
    // test 1 : IDCT
    
    int16_t *initial_data = (int16_t *) malloc(64 * sizeof(int16_t));
    for (int8_t i = 0; i < 8; i++) {
        for (int8_t j = 0; j < 8; j++)
            initial_data[i*8+j] = i;
    }
    getHighlyVerbose() ? fprintf(stderr, "MCU avant IDCT\n"):0;
    print_block(initial_data, 0, 0);
    getHighlyVerbose() ? fprintf(stderr, "Expected output\n"):0;
    print_block((int16_t *) expected_data, 0, 0);


    fast_IDCT_function((int16_t **) &initial_data);


    // Affiche la matrice de sortie
    getHighlyVerbose() ? fprintf(stderr, "MCU après IDCT\n"):0;
    if (getHighlyVerbose()){
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                printf("%x ", initial_data[i * 8 + j]);
            }
            printf("\n");
        }
        printf("\n");
    }

    // On génère le message relatif au test
    bool result = true;
    for(int i = 0; i < 64; i++){
        if(initial_data[i] != expected_data[i]) result = false;
    }
    result ? fprintf(stderr, GREEN("test : OK\n")) : fprintf(stderr, RED("test : KO !!!\n"));
    

    fprintf(stderr, YELLOW("\n================================================\n"));


    // On libère la mémoire
    free(initial_data);

    return EXIT_SUCCESS;
}