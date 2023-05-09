#ifndef _RLE_DECODE_H_
#define _RLE_DECODE_H_
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


unsigned char* rle_decode(unsigned char *huffman_decoded_data);

void print_RLE_result(unsigned char *decoded_output);

#endif
