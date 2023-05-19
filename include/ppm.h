#ifndef _PPM_H_
#define _PPM_H_

#include <stdio.h>
#include <stdlib.h>

#include <extract.h>
#include <utils.h>

int8_t write_ppm(const char *output_filename, struct JPEG *jpeg);

#endif
