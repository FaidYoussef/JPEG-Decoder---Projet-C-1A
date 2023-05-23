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

    int16_t *initial_data = (int16_t *) malloc(64 * sizeof(int16_t));
    initial_data[0] = 0;
    initial_data[1] = 1;
    initial_data[2] = 8;
    initial_data[3] = 16;
    initial_data[4] = 9;
    initial_data[5] = 2;
    initial_data[6] = 3;
    initial_data[7] = 10;
    initial_data[8] = 17;
    initial_data[9] = 24;
    initial_data[10] = 32;
    initial_data[11] = 25;
    initial_data[12] = 18;
    initial_data[13] = 11;
    initial_data[14] = 4;
    initial_data[15] = 5;
    initial_data[16] = 12;
    initial_data[17] = 19;
    initial_data[18] = 26;
    initial_data[19] = 33;
    initial_data[20] = 40;
    initial_data[21] = 48;
    initial_data[22] = 41;
    initial_data[23] = 34;
    initial_data[24] = 27;
    initial_data[25] = 20;
    initial_data[26] = 13;
    initial_data[27] = 6;
    initial_data[28] = 7;
    initial_data[29] = 14;
    initial_data[30] = 21;
    initial_data[31] = 28;
    initial_data[32] = 35;
    initial_data[33] = 42;
    initial_data[34] = 49;
    initial_data[35] = 56;
    initial_data[36] = 57;
    initial_data[37] = 50;
    initial_data[38] = 43;
    initial_data[39] = 36;
    initial_data[40] = 29;
    initial_data[41] = 22;
    initial_data[42] = 15;
    initial_data[43] = 23;
    initial_data[44] = 30;
    initial_data[45] = 37;
    initial_data[46] = 44;
    initial_data[47] = 51;
    initial_data[48] = 58;
    initial_data[49] = 59;
    initial_data[50] = 52;
    initial_data[51] = 45;
    initial_data[52] = 38;
    initial_data[53] = 31;
    initial_data[54] = 39;
    initial_data[55] = 46;
    initial_data[56] = 53;
    initial_data[57] = 60;
    initial_data[58] = 61;
    initial_data[59] = 54;
    initial_data[60] = 47;
    initial_data[61] = 55;
    initial_data[62] = 62;
    initial_data[63] = 63;


    //*************************************************************************************************
    // test 1 : on dé_zig-zag
    getHighlyVerbose() ? fprintf(stderr, "MCU avant IZZ\n"):0;
    print_block(initial_data, 0, 0);
    getHighlyVerbose() ? fprintf(stderr, "Expected output\n"):0;
    print_block(expected_data, 0, 0);

    IZZ_function((int16_t **) &initial_data);

    getHighlyVerbose() ? fprintf(stderr, "MCU après IZZ\n"):0;
    print_block(initial_data, 0, 0);

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
