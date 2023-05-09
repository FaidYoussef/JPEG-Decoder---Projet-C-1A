#ifndef _RLE_DECODE_H_
#define _RLE_DECODE_H_
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


void rle_decode(unsigned char *huffman_decoded_data, unsigned char *decoded_output);

void print_RLE_result(unsigned char *decoded_output);

#endif
