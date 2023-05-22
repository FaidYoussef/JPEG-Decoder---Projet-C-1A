#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <IZZ.h>
#include <extract.h>
#include <utils.h>
#include <verbose.h>


const int16_t expected_data[64] = {
    0, 1, 2, 3, 4, 5, 6, 7,
    8, 9,10,11,12,13,14,15,
    16,17,18,19,20,21,22,23,
    24,25,26,27,28,29,30,31,
    32,33,34,35,36,37,38,39,
    40,41,42,43,44,45,46,47,
    48,49,50,51,52,53,54,55,
    56,57,58,59,60,61,62,63
};


// tests inverse zig_zag
int main(int argc, char **argv) {

    // Mode verbose
    if (argc > 1 && strcmp(argv[1], "-hv") == 0) setHighlyVerbose(true);

    //*************************************************************************************************
    // TEST HEADER
    fprintf(stderr, "\n");
    fprintf(stderr, YELLOW("================== TESTS IZZ ===================\n\n"));

    int16_t initial_data[64] = {
        0, 1, 8, 16, 9, 2, 3, 10,
        17, 24, 32, 25, 18, 11, 4, 5,
        12, 19, 26, 33, 40, 48, 41, 34,
        27, 20, 13, 6, 7, 14, 21, 28,
        35, 42, 49, 56, 57, 50, 43, 36,
        29, 22, 15, 23, 30, 37, 44, 51,
        58, 59, 52, 45, 38, 31, 39, 46,
        53, 60, 61, 54, 47, 55, 62, 63
    };


    //*************************************************************************************************
    // test 1 : on dé_zig-zag
    getHighlyVerbose() ? fprintf(stderr, "MCU avant IZZ\n"):0;
    print_block(initial_data, 0, 0);

    IZZ_function((int16_t **) &initial_data);

    getHighlyVerbose() ? fprintf(stderr, "MCU après IZZ\n"):0;
    print_block(initial_data, 0, 0);

    bool result = true;
    for(int i = 0; i < 64; i++){
        if(initial_data[i] != expected_data[i]) result = false;
    }
    result ? fprintf(stderr, GREEN("test : OK\n")) : fprintf(stderr, RED("test : KO !!!\n"));


    fprintf(stderr, YELLOW("\n================================================\n"));

    return EXIT_SUCCESS;
}
