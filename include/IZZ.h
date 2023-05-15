#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#include <utils.h>


// Fonction qui permet de dé-zigzaguer un bloc
int8_t IZZ_function(struct JPEG *jpeg,size_t MCU_number, int8_t component_index);

int8_t IZZ(struct JPEG * jpeg);