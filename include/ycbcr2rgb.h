#ifndef _YCBCR2RGB_H_
#define _YCBCR2RGB_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <extract.h>

// Fonction pour saturer les valeurs entre 0 et 255
uint8_t saturer(int valeur);

// Fonction pour convertir un pixel YCbCr en pixel RGB
// Si une seule composante (Y) >>> donner en entrée la même valeur '128' pour Cb et Cr
void ycbcr_vers_rgb(uint8_t Y, uint8_t Cb, uint8_t Cr, uint8_t *R, uint8_t *G, uint8_t *B);
// void ycbcr_vers_rgb(struct JPEG *jpeg, size_t MCU_number);

int8_t YCbCr2RGB(struct JPEG *jpeg);

#endif
