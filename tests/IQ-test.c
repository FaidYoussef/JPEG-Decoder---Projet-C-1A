#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <IQ.h>
#include <utils.h>
#include <verbose.h>

bool highly_verbose = true;


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

void re_initialize_data(int16_t data[64]){
    for (int i = 0; i < 64; i++) {
        data[i] = 1;
    }
}



// tests inverse quantization
int main(void) {

    int16_t initial_data[64] = {
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1
    };

    //*************************************************************************************************
    // test 1 : on multiplie par 0
    getHighlyVerbose() ? fprintf(stderr, "MCU avant IQ\n"):0;
    print_block(initial_data, 0, 0);

    IQ_function(initial_data, test_qt0);

    getHighlyVerbose() ? fprintf(stderr, "MCU après IQ\n"):0;
    print_block(initial_data, 0, 0);


    //*************************************************************************************************
    // test 2 : on multiplie par 1
    re_initialize_data(initial_data);


    getHighlyVerbose() ? fprintf(stderr, "MCU avant IQ\n"):0;
    print_block(initial_data, 0, 0);

    IQ_function(initial_data, test_qt1);

    getHighlyVerbose() ? fprintf(stderr, "MCU après IQ\n"):0;
    print_block(initial_data, 0, 0);


    //*************************************************************************************************
    // test 3 : on multiplie par 2
    re_initialize_data(initial_data);


    getHighlyVerbose() ? fprintf(stderr, "MCU avant IQ\n"):0;
    print_block(initial_data, 0, 0);

    IQ_function(initial_data, test_qt2);

    getHighlyVerbose() ? fprintf(stderr, "MCU après IQ\n"):0;
    print_block(initial_data, 0, 0);


    //*************************************************************************************************
    // test 4 : on multiplie par 1 et par 2
    re_initialize_data(initial_data);


    getHighlyVerbose() ? fprintf(stderr, "MCU avant IQ\n"):0;
    print_block(initial_data, 0, 0);

    IQ_function(initial_data, test_qt1and2);

    getHighlyVerbose() ? fprintf(stderr, "MCU après IQ\n"):0;
    print_block(initial_data, 0, 0);


    //*************************************************************************************************
    // test 5 : on multiplie par 255
    re_initialize_data(initial_data);


    getHighlyVerbose() ? fprintf(stderr, "MCU avant IQ\n"):0;
    print_block(initial_data, 0, 0);

    IQ_function(initial_data, test_qt255);

    getHighlyVerbose() ? fprintf(stderr, "MCU après IQ\n"):0;
    print_block(initial_data, 0, 0);


    //*************************************************************************************************
    // test 6 : on multiplie par 255 et on évite les overflows
    re_initialize_data(initial_data);
    initial_data[0] = -32768;
    initial_data[63] = 32767;


    getHighlyVerbose() ? fprintf(stderr, "MCU avant IQ\n"):0;
    print_block(initial_data, 0, 0);

    IQ_function(initial_data, test_qt255);

    getHighlyVerbose() ? fprintf(stderr, "MCU après IQ\n"):0;
    print_block(initial_data, 0, 0);



    return EXIT_SUCCESS;
}
