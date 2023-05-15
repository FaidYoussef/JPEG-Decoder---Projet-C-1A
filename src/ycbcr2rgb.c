#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <ycbcr2rgb.h>

// Fonction pour saturer les valeurs entre 0 et 255
uint8_t saturer(int valeur) {
    if (valeur < 0) {
        return 0;
    } else if (valeur > 255) {
        return 255;
    } else {
        return (uint8_t) valeur;
    }
}

// Fonction pour convertir un pixel YCbCr en pixel RGB
// Si une seule composan (Y) >>> donner en entrée la même valeur '128' pour Cb et Cr
void ycbcr_vers_rgb(uint8_t Y, uint8_t Cb, uint8_t Cr, uint8_t *R, uint8_t *G, uint8_t *B) {
    int r = Y + 1.402 * (Cr - 128);
    int g = Y - 0.34414 * (Cb - 128) - 0.71414 * (Cr - 128);
    int b = Y + 1.772 * (Cb - 128);

    *R = saturer(r);
    *G = saturer(g);
    *B = saturer(b);
}
