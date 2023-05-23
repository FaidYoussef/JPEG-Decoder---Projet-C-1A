#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <extract.h>
#include <huffman.h>
#include <IQ.h>
#include <utils.h>
#include <verbose.h>


const uint8_t test_qt0[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

const uint8_t test_qt1[64] = {
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1
};

const int16_t expected_result1[64] = {
    4, 8, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1
};

const uint8_t test_qt2[64] = {
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2
};

const int16_t expected_result2[64] = {
    8, 16, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2
};

const uint8_t test_qt1and2[64] = {
    1, 2, 1, 2, 1, 2, 1, 2,
    2, 1, 2, 1, 2, 1, 2, 1,
    1, 2, 1, 2, 1, 2, 1, 2,
    2, 1, 2, 1, 2, 1, 2, 1,
    1, 2, 1, 2, 1, 2, 1, 2,
    2, 1, 2, 1, 2, 1, 2, 1,
    1, 2, 1, 2, 1, 2, 1, 2,
    2, 1, 2, 1, 2, 1, 2, 1
};

const uint8_t expected_result1and2[64] = {
    4, 16, 1, 2, 1, 2, 1, 2,
    2, 1, 2, 1, 2, 1, 2, 1,
    1, 2, 1, 2, 1, 2, 1, 2,
    2, 1, 2, 1, 2, 1, 2, 1,
    1, 2, 1, 2, 1, 2, 1, 2,
    2, 1, 2, 1, 2, 1, 2, 1,
    1, 2, 1, 2, 1, 2, 1, 2,
    2, 1, 2, 1, 2, 1, 2, 1
};


const uint8_t test_qt255[64] = {
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255
};

const int16_t expected_result255[64] = {
    1020, 2040, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255
};

const int16_t expected_result255_no_overflow[64] = {
    -32768, 32767, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255
};


void re_initialize_data(int16_t data[64]){
    data[0] = 4;
    data[1] = 8;
    for (int i = 2; i < 64; i++) {
        data[i] = 1;
    }
}



// tests inverse quantization
int main(int argc, char **argv) {

    // Mode verbose
    if (argc > 1 && strcmp(argv[1], "-hv") == 0) setHighlyVerbose(true);


    int16_t initial_data[64] = {
        4, 8, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1
    };

    //*************************************************************************************************
    // TEST HEADER
    fprintf(stderr, "\n");
    fprintf(stderr, YELLOW("========= TESTS QUANTIFICATION INVERSE =========\n\n"));

    //*************************************************************************************************
    // test 1 : on multiplie par 0
    getHighlyVerbose() ? fprintf(stderr, "MCU avant IQ\n"):0;
    print_block(initial_data, 0, 0);
    getHighlyVerbose() ? fprintf(stderr, "Table de quantification utilisée :\n"):0;
    print_block((int16_t *) test_qt0, 0, 0);

    IQ_function(initial_data, test_qt0);

    getHighlyVerbose() ? fprintf(stderr, "MCU après IQ\n"):0;
    print_block(initial_data, 0, 0);

    bool result = true;
    for(int i = 0; i < 64; i++){
        if(initial_data[i] != test_qt0[i]) result = false;
    }
    result ? fprintf(stderr, GREEN("\t\ttest_1 : OK\n")) : fprintf(stderr, RED("\t\ttest_1 : KO\n"));


    //*************************************************************************************************
    // test 2 : on multiplie par 1
    re_initialize_data(initial_data);


    getHighlyVerbose() ? fprintf(stderr, "MCU avant IQ\n"):0;
    print_block(initial_data, 0, 0);
    getHighlyVerbose() ? fprintf(stderr, "Table de quantification utilisée :\n"):0;
    print_block((int16_t *) test_qt1, 0, 0);

    IQ_function(initial_data, test_qt1);

    getHighlyVerbose() ? fprintf(stderr, "MCU après IQ\n"):0;
    print_block(initial_data, 0, 0);

    result = true;
    for(int i = 0; i < 64; i++){
        if(initial_data[i] != expected_result1[i]) result = false;
    }
    result ? fprintf(stderr, GREEN("\t\ttest_2 : OK\n")) : fprintf(stderr, RED("\t\ttest_2 : KO\n"));


    //*************************************************************************************************
    // test 3 : on multiplie par 2
    re_initialize_data(initial_data);


    getHighlyVerbose() ? fprintf(stderr, "MCU avant IQ\n"):0;
    print_block(initial_data, 0, 0);
    getHighlyVerbose() ? fprintf(stderr, "Table de quantification utilisée :\n"):0;
    print_block((int16_t *) test_qt2, 0, 0);

    IQ_function(initial_data, test_qt2);

    getHighlyVerbose() ? fprintf(stderr, "MCU après IQ\n"):0;
    print_block(initial_data, 0, 0);

    result = true;
    for(int i = 0; i < 64; i++){
        if(initial_data[i] != expected_result2[i]) result = false;
    }
    result ? fprintf(stderr, GREEN("\t\ttest_3 : OK\n")) : fprintf(stderr, RED("\t\ttest_3 : KO\n"));


    //*************************************************************************************************
    // test 4 : on multiplie par 1 et par 2
    re_initialize_data(initial_data);


    getHighlyVerbose() ? fprintf(stderr, "MCU avant IQ\n"):0;
    print_block(initial_data, 0, 0);
    getHighlyVerbose() ? fprintf(stderr, "Table de quantification utilisée :\n"):0;
    print_block((int16_t *) test_qt1and2, 0, 0);

    IQ_function(initial_data, test_qt1and2);

    getHighlyVerbose() ? fprintf(stderr, "MCU après IQ\n"):0;
    print_block(initial_data, 0, 0);

    result = true;
    for(int i = 0; i < 64; i++){
        if(initial_data[i] != expected_result1and2[i]) result = false;
    }
    result ? fprintf(stderr, GREEN("\t\ttest_4 : OK\n")) : fprintf(stderr, RED("\t\ttest_4 : KO\n"));


    //*************************************************************************************************
    // test 5 : on multiplie par 255
    re_initialize_data(initial_data);


    getHighlyVerbose() ? fprintf(stderr, "MCU avant IQ\n"):0;
    print_block(initial_data, 0, 0);
    getHighlyVerbose() ? fprintf(stderr, "Table de quantification utilisée :\n"):0;
    print_block((int16_t *) test_qt255, 0, 0);

    IQ_function(initial_data, test_qt255);

    getHighlyVerbose() ? fprintf(stderr, "MCU après IQ\n"):0;
    print_block(initial_data, 0, 0);

    result = true;
    for(int i = 0; i < 64; i++){
        if(initial_data[i] != expected_result255[i]) result = false;
    }
    result ? fprintf(stderr, GREEN("\t\ttest_5 : OK\n")) : fprintf(stderr, RED("\t\ttest_5 : KO\n"));


    //*************************************************************************************************
    // test 6 : on multiplie par 255 et on évite les overflows
    re_initialize_data(initial_data);
    initial_data[0] = -32768;
    initial_data[1] = 32767;


    getHighlyVerbose() ? fprintf(stderr, "MCU avant IQ\n"):0;
    print_block(initial_data, 0, 0);
    getHighlyVerbose() ? fprintf(stderr, "Table de quantification utilisée :\n"):0;
    print_block((int16_t *) test_qt255, 0, 0);

    IQ_function(initial_data, test_qt255);

    getHighlyVerbose() ? fprintf(stderr, "MCU après IQ\n"):0;
    print_block(initial_data, 0, 0);

    result = true;
    for(int i = 0; i < 64; i++){
        if(initial_data[i] != expected_result255_no_overflow[i]) result = false;
    }
    result ? fprintf(stderr, GREEN("\t\ttest_6 : OK\n")) : fprintf(stderr, RED("\t\ttest_6 : KO\n"));


    fprintf(stderr, YELLOW("\n================================================\n"));

    return EXIT_SUCCESS;
}
