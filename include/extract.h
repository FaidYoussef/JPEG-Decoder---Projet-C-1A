#ifndef _EXTRACT_H_
#define _EXTRACT_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <utils.h>
#include <verbose.h>

#define THREE_BYTES_LONG 3

#define SEGMENT_START 0xff  // Segment start marker
#define SOI     0xd8        // Start of Image
#define SOF_0   0xc0        // Baseline DCT
#define SOF_1   0xc1        // Extended sequential DCT
#define SOF_2   0xc2        // Progressive DCT
#define SOF_3   0xc3        // Lossless (sequential)
#define DHT     0xc4        // Define Huffman Table(s)
#define DQT     0xdb        // Define Quantization Table(s)
#define SOS     0xda        // Start of Scan
#define EOI     0xd9        // End of Image

#define LUMINANCE_ID 0x00
#define CHROMINANCE_ID 0x01

struct QuantizationTable;
struct ComponentSOF;
struct StartOfFrame;
struct HuffmanTable;
struct ComponentSOS;
struct StartOfScan;
struct JPEG;

short two_bytes_to_dec(FILE *input);

unsigned char read_byte(FILE *input, unsigned char *buffer);

void ignore_bytes(FILE *input, int nb_bytes);

struct QuantizationTable get_qt(FILE *input, unsigned char *buffer);

struct StartOfFrame get_SOF(FILE *input, unsigned char *buffer);

struct HuffmanTable get_DHT(FILE *input, unsigned char *buffer);

struct StartOfScan getSOS(FILE *input, unsigned char *buffer);

struct JPEG * extract(char *filename);

#endif
