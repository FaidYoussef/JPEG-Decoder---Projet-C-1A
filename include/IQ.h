#ifndef _QUANT_ZIGZAG_
#define _QUANT_ZIGZAG_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <extract.h>
#include <utils.h>


// Inverse quantization function
void IQ_function(int16_t *mcu, const uint8_t *qtable);

// Fonction qui récupère les données de la structure JPEG et qui procède à la quantification inverse
int8_t IQ(struct JPEG * jpeg);

#endif
