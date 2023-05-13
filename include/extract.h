#ifndef _EXTRACT_H_
#define _EXTRACT_H_
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
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

#define INITIAL_DATA_SIZE 1024

//**********************************************************************************************************************
struct QuantizationTable;
void initialize_qt(struct QuantizationTable *qt);
unsigned char get_qt_id(struct QuantizationTable *qt);
size_t get_qt_length(struct QuantizationTable *qt);
unsigned char * get_qt_data(struct QuantizationTable *qt);

//**********************************************************************************************************************
struct ComponentSOF;
void initialize_component_sof(struct ComponentSOF *component);
int8_t get_id(struct ComponentSOF *component);
int8_t get_DC_huffman_table_id(struct ComponentSOF *component);
int8_t get_AC_huffman_table_id(struct ComponentSOF *component);
int16_t **get_MCUs(struct ComponentSOF *component);
// int16_t *get_MCU(struct ComponentSOF *component, int index_of_mcu);
void set_value_in_MCU(struct ComponentSOF *component, int index_of_mcu, int index_of_pixel_in_mcu, int16_t value);

struct StartOfFrame;
void initialize_sof(struct StartOfFrame *sof);
unsigned char get_sof_nb_components(struct StartOfFrame *sof);
struct ComponentSOF ** get_sof_components(struct StartOfFrame *sof);

//**********************************************************************************************************************
struct HuffmanTable;
void initialize_ht(struct HuffmanTable *ht);
unsigned char get_ht_class(struct HuffmanTable *ht);
unsigned char get_ht_destination(struct HuffmanTable *ht);
size_t get_ht_length(struct HuffmanTable *ht);
unsigned char * get_ht_data(struct HuffmanTable *ht);
struct node * get_ht_tree(struct HuffmanTable *ht);
bool get_ht_set(struct HuffmanTable *ht);

//**********************************************************************************************************************
struct ComponentSOS;
struct StartOfScan;
void initialize_sos(struct StartOfScan *sos);
unsigned char get_sos_nb_components(struct StartOfScan *sos);
struct ComponentSOS * get_sos_components(struct StartOfScan *sos);

//**********************************************************************************************************************
struct JPEG;
void initialize_JPEG_struct(struct JPEG *jpeg);
unsigned short get_JPEG_height(struct JPEG *jpeg);
unsigned short get_JPEG_width(struct JPEG *jpeg);
struct QuantizationTable ** get_JPEG_qt(struct JPEG *jpeg);
struct StartOfFrame ** get_JPEG_sof(struct JPEG *jpeg);
struct HuffmanTable ** get_JPEG_ht(struct JPEG *jpeg);
struct StartOfScan * get_JPEG_sos(struct JPEG *jpeg);
unsigned char * get_JPEG_image_data(struct JPEG* jpeg);
size_t get_JPEG_image_data_size_in_bits(struct JPEG* jpeg);

//**********************************************************************************************************************
short two_bytes_to_dec(FILE *input);

unsigned char read_byte(FILE *input, unsigned char *buffer);

void ignore_bytes(FILE *input, int nb_bytes);

struct QuantizationTable * get_qt(FILE *input, unsigned char *buffer, struct JPEG *jpeg);

int get_SOF(FILE *input, unsigned char *buffer, struct JPEG *jpeg);

struct HuffmanTable * get_DHT(FILE *input, unsigned char *buffer, struct JPEG *jpeg);

int getSOS(FILE *input, unsigned char *buffer, struct JPEG *jpeg);

struct JPEG * extract(char *filename);

#endif
