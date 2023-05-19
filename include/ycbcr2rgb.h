#ifndef _YCBCR2RGB_H_
#define _YCBCR2RGB_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <extract.h>
#include <utils.h>

// Fonction pour saturer les valeurs entre 0 et 255
uint8_t saturer(int16_t valeur);

// Fonction pour convertir un pixel YCbCr en pixel RGB
// Si on a une seule composante, on ne met à jour que la composante 0
// Si on a plusieurs composantes, on met à jour en lieu et place des composantes 0, 1 et 2 les valeurs R, G et B
void pixel_YCbCr2RGB(int16_t *pixel_Y, int16_t *pixel_Cb, int16_t *pixel_Cr, int8_t nb_components);

// Fonction pour convertir un MCU YCbCr en pixel RGB
void MCU_YCbCr2RGB(int16_t *MCU_Y, int16_t *MCU_Cb, int16_t *MCU_Cr, int8_t nb_components);

int8_t YCbCr2RGB(struct JPEG *jpeg);

#endif
