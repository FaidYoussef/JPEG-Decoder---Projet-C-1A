#ifndef _YCBCR2RGB_H_
#define _YCBCR2RGB_H_

// Fonction pour saturer les valeurs entre 0 et 255
uint8_t saturer(int valeur);

// Fonction pour convertir un pixel YCbCr en pixel RGB
void ycbcr_vers_rgb(uint8_t Y, uint8_t Cb, uint8_t Cr, uint8_t *R, uint8_t *G, uint8_t *B);

#endif
