#ifndef _QUANT_ZIGZAG_
#define _QUANT_ZIGZAG_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <extract.h>
#include <utils.h>

// Fonction qui permet de savoir si on a un overflow
bool isOverflow(int16_t a, int16_t b);

// Inverse quantization function
void IQ(int16_t *mcu, const uint8_t *qtable);

// Fonction qui récupère les données de la structure JPEG et qui procède à la quantification inverse
int8_t inv_quantize(struct JPEG * jpeg);

#endif
