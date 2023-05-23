#include <stdio.h>
#include <stdint.h>

#include <verbose.h>
#include <ycbcr2rgb.h>



const int16_t expected_output1[3] = {255, 255, 255};

const int16_t expected_output2[3] = {255, 255, 255};

const int16_t expected_output3[3] = {0, 0, 0};

const int16_t expected_output4[3] = {0, 0, 0};

const int16_t expected_output5[3] = {82, 82, 82};

const int16_t expected_output6[3] = {239, 15, 15};

const int16_t expected_output7[3] = {145, 145, 145};

const int16_t expected_output8[3] = {13, 238, 14};

const int16_t expected_output9[3] = {41, 41, 41};

const int16_t expected_output10[3] = {16, 15, 239};


void initialize_values_white(int16_t *Y, int16_t *Cb, int16_t *Cr){
    *Y  = 255;
    *Cb = 128;
    *Cr = 128;
}

void initialize_values_black(int16_t *Y, int16_t *Cb, int16_t *Cr){
    *Y  = 0;
    *Cb = 128;
    *Cr = 128;
}

void initialize_values_red(int16_t *Y, int16_t *Cb, int16_t *Cr){
    *Y  = 82;
    *Cb = 90;
    *Cr = 240;
}

void initialize_values_green(int16_t *Y, int16_t *Cb, int16_t *Cr){
    *Y  = 145;
    *Cb = 54;
    *Cr = 34;
}

void initialize_values_blue(int16_t *Y, int16_t *Cb, int16_t *Cr){
    *Y  = 41;
    *Cb = 240;
    *Cr = 110;
}


int main(int argc, char **argv) {

    // Mode verbose
    if (argc > 1 && strcmp(argv[1], "-hv") == 0) setHighlyVerbose(true);

    int16_t Y, Cb, Cr;


    //*************************************************************************************************
    // test 1 : blanc, 1 composante = niveau de gris

    initialize_values_white(&Y, &Cb, &Cr);

    getHighlyVerbose() ? fprintf(stderr, "Valeurs des composantes avant conversion YCbCr vers RGB : Y=%d Cb=%d Cr=%d\n", Y, Cb, Cr):0;
    getHighlyVerbose() ? fprintf(stderr, "Valeurs attendues après conversion YCbCr vers RGB       : R=%d G=%d B=%d\n", expected_output1[0], expected_output1[1], expected_output1[2]):0;
    
    pixel_YCbCr2RGB(&Y, &Cb, &Cr, 1, 0);

    getHighlyVerbose() ? fprintf(stderr, "Valeurs des composantes après conversion YCbCr vers RGB : R=%d G=%d B=%d\n", Y, Cb, Cr):0;

    bool result = true;
    if(Y != expected_output1[0] || Cb != expected_output1[1] || Cr != expected_output1[2]) result = false;
    result ? fprintf(stderr, GREEN("test 1 : OK\n")) : fprintf(stderr, RED("test 1 : KO\n"));


     //*************************************************************************************************
    // test 2 : blanc, 3 composantes = couleur

    initialize_values_white(&Y, &Cb, &Cr);

    getHighlyVerbose() ? fprintf(stderr, "Valeurs des composantes avant conversion YCbCr vers RGB : Y=%d Cb=%d Cr=%d\n", Y, Cb, Cr):0;
    getHighlyVerbose() ? fprintf(stderr, "Valeurs attendues après conversion YCbCr vers RGB       : R=%d G=%d B=%d\n", expected_output2[0], expected_output2[1], expected_output2[2]):0;
    
    pixel_YCbCr2RGB(&Y, &Cb, &Cr, 3, 0);

    getHighlyVerbose() ? fprintf(stderr, "Valeurs des composantes après conversion YCbCr vers RGB : R=%d G=%d B=%d\n", Y, Cb, Cr):0;

    result = true;
    if(Y != expected_output2[0] || Cb != expected_output2[1] || Cr != expected_output2[2]) result = false;
    result ? fprintf(stderr, GREEN("test 2 : OK\n")) : fprintf(stderr, RED("test 2 : KO\n"));


    //*************************************************************************************************
    // test 3 : noir, 1 composante = niveau de gris

    initialize_values_black(&Y, &Cb, &Cr);

    getHighlyVerbose() ? fprintf(stderr, "Valeurs des composantes avant conversion YCbCr vers RGB : Y=%d Cb=%d Cr=%d\n", Y, Cb, Cr):0;
    getHighlyVerbose() ? fprintf(stderr, "Valeurs attendues après conversion YCbCr vers RGB       : R=%d G=%d B=%d\n", expected_output3[0], expected_output3[1], expected_output3[2]):0;


    pixel_YCbCr2RGB(&Y, &Cb, &Cr, 1, 0);

    getHighlyVerbose() ? fprintf(stderr, "Valeurs des composantes après conversion YCbCr vers RGB : R=%d G=%d B=%d\n", Y, Cb, Cr):0;

    result = true;
    if(Y != expected_output3[0] || Cb != expected_output3[1] || Cr != expected_output3[2]) result = false;
    result ? fprintf(stderr, GREEN("test 3 : OK\n")) : fprintf(stderr, RED("test 3 : KO\n"));


    //*************************************************************************************************
    // test 4 : noir, 3 composantes = couleur

    initialize_values_black(&Y, &Cb, &Cr);

    getHighlyVerbose() ? fprintf(stderr, "Valeurs des composantes avant conversion YCbCr vers RGB : Y=%d Cb=%d Cr=%d\n", Y, Cb, Cr):0;
    getHighlyVerbose() ? fprintf(stderr, "Valeurs attendues après conversion YCbCr vers RGB       : R=%d G=%d B=%d\n", expected_output4[0], expected_output4[1], expected_output4[2]):0;
    
    pixel_YCbCr2RGB(&Y, &Cb, &Cr, 3, 0);

    getHighlyVerbose() ? fprintf(stderr, "Valeurs des composantes après conversion YCbCr vers RGB : R=%d G=%d B=%d\n", Y, Cb, Cr):0;

    result = true;
    if(Y != expected_output4[0] || Cb != expected_output4[1] || Cr != expected_output4[2]) result = false;
    result ? fprintf(stderr, GREEN("test 4 : OK\n")) : fprintf(stderr, RED("test 4 : KO\n"));


    //*************************************************************************************************
    // test 5 : rouge, 1 composante = niveau de gris

    initialize_values_red(&Y, &Cb, &Cr);

    getHighlyVerbose() ? fprintf(stderr, "Valeurs des composantes avant conversion YCbCr vers RGB : Y=%d Cb=%d Cr=%d\n", Y, Cb, Cr):0;
    getHighlyVerbose() ? fprintf(stderr, "Valeurs attendues après conversion YCbCr vers RGB       : R=%d G=%d B=%d\n", expected_output5[0], expected_output5[1], expected_output5[2]):0;
    
    pixel_YCbCr2RGB(&Y, &Cb, &Cr, 1, 0);

    getHighlyVerbose() ? fprintf(stderr, "Valeurs des composantes après conversion YCbCr vers RGB : R=%d G=%d B=%d\n", Y, Cb, Cr):0;

    result = true;
    if(Y != expected_output5[0] || Cb != expected_output5[1] || Cr != expected_output5[2]) result = false;
    result ? fprintf(stderr, GREEN("test 5 : OK\n")) : fprintf(stderr, RED("test 5 : KO\n"));


    //*************************************************************************************************
    // test 6 : rouge, 3 composantes = couleur

    initialize_values_red(&Y, &Cb, &Cr);

    getHighlyVerbose() ? fprintf(stderr, "Valeurs des composantes avant conversion YCbCr vers RGB : Y=%d Cb=%d Cr=%d\n", Y, Cb, Cr):0;
    getHighlyVerbose() ? fprintf(stderr, "Valeurs attendues après conversion YCbCr vers RGB       : R=%d G=%d B=%d\n", expected_output6[0], expected_output6[1], expected_output6[2]):0;
    
    pixel_YCbCr2RGB(&Y, &Cb, &Cr, 3, 0);

    getHighlyVerbose() ? fprintf(stderr, "Valeurs des composantes après conversion YCbCr vers RGB : R=%d G=%d B=%d\n", Y, Cb, Cr):0;

    result = true;
    if(Y != expected_output6[0] || Cb != expected_output6[1] || Cr != expected_output6[2]) result = false;
    result ? fprintf(stderr, GREEN("test 6 : OK\n")) : fprintf(stderr, RED("test 6 : KO\n"));


    //*************************************************************************************************
    // test 7 : vert, 1 composante = niveau de gris

    initialize_values_green(&Y, &Cb, &Cr);

    getHighlyVerbose() ? fprintf(stderr, "Valeurs des composantes avant conversion YCbCr vers RGB : Y=%d Cb=%d Cr=%d\n", Y, Cb, Cr):0;
    getHighlyVerbose() ? fprintf(stderr, "Valeurs attendues après conversion YCbCr vers RGB       : R=%d G=%d B=%d\n", expected_output7[0], expected_output7[1], expected_output7[2]):0;

    pixel_YCbCr2RGB(&Y, &Cb, &Cr, 1, 0);

    getHighlyVerbose() ? fprintf(stderr, "Valeurs des composantes après conversion YCbCr vers RGB : R=%d G=%d B=%d\n", Y, Cb, Cr):0;

    result = true;
    if(Y != expected_output7[0] || Cb != expected_output7[1] || Cr != expected_output7[2]) result = false;
    result ? fprintf(stderr, GREEN("test 7 : OK\n")) : fprintf(stderr, RED("test 7 : KO\n"));


    //*************************************************************************************************
    // test 8 : vert, 3 composantes = couleur

    initialize_values_green(&Y, &Cb, &Cr);

    getHighlyVerbose() ? fprintf(stderr, "Valeurs des composantes avant conversion YCbCr vers RGB : Y=%d Cb=%d Cr=%d\n", Y, Cb, Cr):0;
    getHighlyVerbose() ? fprintf(stderr, "Valeurs attendues après conversion YCbCr vers RGB       : R=%d G=%d B=%d\n", expected_output8[0], expected_output8[1], expected_output8[2]):0;

    pixel_YCbCr2RGB(&Y, &Cb, &Cr, 3, 0);

    getHighlyVerbose() ? fprintf(stderr, "Valeurs des composantes après conversion YCbCr vers RGB : R=%d G=%d B=%d\n", Y, Cb, Cr):0;

    result = true;
    if(Y != expected_output8[0] || Cb != expected_output8[1] || Cr != expected_output8[2]) result = false;
    result ? fprintf(stderr, GREEN("test 8 : OK\n")) : fprintf(stderr, RED("test 8 : KO\n"));

    //*************************************************************************************************
    // test 9 : bleu, 1 composante = niveau de gris

    initialize_values_blue(&Y, &Cb, &Cr);

    getHighlyVerbose() ? fprintf(stderr, "Valeurs des composantes avant conversion YCbCr vers RGB : Y=%d Cb=%d Cr=%d\n", Y, Cb, Cr):0;
    getHighlyVerbose() ? fprintf(stderr, "Valeurs attendues après conversion YCbCr vers RGB       : R=%d G=%d B=%d\n", expected_output9[0], expected_output9[1], expected_output9[2]):0;

    pixel_YCbCr2RGB(&Y, &Cb, &Cr, 1, 0);

    getHighlyVerbose() ? fprintf(stderr, "Valeurs des composantes après conversion YCbCr vers RGB : R=%d G=%d B=%d\n", Y, Cb, Cr):0;

    result = true;
    if(Y != expected_output9[0] || Cb != expected_output9[1] || Cr != expected_output9[2]) result = false;
    result ? fprintf(stderr, GREEN("test 9 : OK\n")) : fprintf(stderr, RED("test 9 : KO\n"));

    //*************************************************************************************************
    // test 10 : bleu, 3 composantes = couleur

    initialize_values_blue(&Y, &Cb, &Cr);

    getHighlyVerbose() ? fprintf(stderr, "Valeurs des composantes avant conversion YCbCr vers RGB : Y=%d Cb=%d Cr=%d\n", Y, Cb, Cr):0;
    getHighlyVerbose() ? fprintf(stderr, "Valeurs attendues après conversion YCbCr vers RGB       : R=%d G=%d B=%d\n", expected_output10[0], expected_output10[1], expected_output10[2]):0;

    pixel_YCbCr2RGB(&Y, &Cb, &Cr, 3, 0);

    getHighlyVerbose() ? fprintf(stderr, "Valeurs des composantes après conversion YCbCr vers RGB : R=%d G=%d B=%d\n", Y, Cb, Cr):0;

    result = true;
    if(Y != expected_output10[0] || Cb != expected_output10[1] || Cr != expected_output10[2]) result = false;
    result ? fprintf(stderr, GREEN("test 10 : OK\n")) : fprintf(stderr, RED("test 10 : KO\n"));


    return EXIT_SUCCESS;
}