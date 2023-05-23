#ifndef _PPM_H_
#define _PPM_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <extract.h>
#include <utils.h>


char* generate_output_filename(const char *input_filename, uint8_t nb_components);

int8_t write_ppm(const char *input_filename, struct JPEG *jpeg, bool force_grayscale);

#endif
