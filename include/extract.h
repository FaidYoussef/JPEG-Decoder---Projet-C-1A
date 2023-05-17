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

#define MAX_NUMBER_OF_HUFFMAN_TABLES 4
#define MAX_NUMBER_OF_QUANTIZATION_TABLES 3

struct JPEG;

#include <huffman.h>
//**********************************************************************************************************************
struct QuantizationTable;
int8_t initialize_qt(struct QuantizationTable *qt, int8_t id, size_t length, unsigned char *data);
int8_t get_qt_id(struct QuantizationTable *qt);
size_t get_qt_length(struct QuantizationTable *qt);
unsigned char * get_qt_data(struct QuantizationTable *qt);

//**********************************************************************************************************************
struct ComponentSOF;
void initialize_component_sof(struct ComponentSOF *component, int8_t id, int8_t sampling_factor_x, int8_t sampling_factor_y, int8_t num_quantization_table);
int8_t get_id(struct ComponentSOF *component);
int8_t get_sampling_factor_x(struct ComponentSOF *component);
int8_t get_sampling_factor_y(struct ComponentSOF *component);
int8_t get_num_quantization_table(struct ComponentSOF *component);

struct StartOfFrame;
int8_t initialize_sof(struct StartOfFrame *sof, int8_t nb_components, int8_t id, int8_t sampling_factor_x, int8_t sampling_factor_y, int8_t num_quantization_table);
int8_t get_sof_nb_components(struct StartOfFrame *sof);
struct ComponentSOF ** get_sof_components(struct StartOfFrame *sof);

//**********************************************************************************************************************
struct HuffmanTable;
void initialize_ht(struct HuffmanTable *ht, int8_t class, int8_t destination, size_t length, unsigned char *data, struct node * huffman_tree, bool set);
unsigned char get_ht_class(struct HuffmanTable *ht);
unsigned char get_ht_destination(struct HuffmanTable *ht);
size_t get_ht_length(struct HuffmanTable *ht);
unsigned char * get_ht_data(struct HuffmanTable *ht);
struct node * get_ht_tree(struct HuffmanTable *ht);
bool get_ht_set(struct HuffmanTable *ht);

//**********************************************************************************************************************
struct ComponentSOS;
int8_t initialize_component_sos(struct ComponentSOS *component, int8_t id_table, int8_t DC_huffman_table_id, int8_t AC_huffman_table_id, size_t nb_of_MCUs);
int8_t get_DC_huffman_table_id(struct ComponentSOS *component);
int8_t get_AC_huffman_table_id(struct ComponentSOS *component);
int16_t **get_MCUs(struct ComponentSOS *component);
// int16_t *get_MCU(struct ComponentSOS *component, int index_of_mcu);
void set_value_in_MCU(struct ComponentSOS *component, int index_of_mcu, int index_of_pixel_in_mcu, int16_t value);

struct StartOfScan;
int8_t initialize_sos(struct StartOfScan *sos, int8_t nb_components, int8_t id_table, int8_t DC_huffman_table_id, int8_t AC_huffman_table_id, size_t nb_of_MCU);
unsigned char get_sos_nb_components(struct StartOfScan *sos);
struct ComponentSOS * get_sos_components(struct StartOfScan *sos);
struct ComponentSOS * get_sos_component(struct ComponentSOS * components, int8_t index);

//**********************************************************************************************************************

int8_t initialize_JPEG_struct(struct JPEG *jpeg);
void free_JPEG_struct(struct JPEG *jpeg);
int16_t get_JPEG_height(struct JPEG *jpeg);
int16_t get_JPEG_width(struct JPEG *jpeg);
struct QuantizationTable ** get_JPEG_qt(struct JPEG *jpeg);
struct StartOfFrame ** get_JPEG_sof(struct JPEG *jpeg);
struct HuffmanTable * get_JPEG_ht(struct JPEG *jpeg, int8_t index);
struct StartOfScan ** get_JPEG_sos(struct JPEG *jpeg);
unsigned char * get_JPEG_image_data(struct JPEG* jpeg);
size_t get_JPEG_image_data_size_in_bits(struct JPEG* jpeg);

//**********************************************************************************************************************
int16_t two_bytes_to_dec(FILE *input);

int8_t read_byte(FILE *input, unsigned char *buffer);

void ignore_bytes(FILE *input, int nb_bytes);

struct QuantizationTable * get_qt(FILE *input, unsigned char *buffer);

int8_t get_SOF(FILE *input, unsigned char *buffer, struct JPEG *jpeg);

struct HuffmanTable * get_DHT(FILE *input, unsigned char *buffer);

int8_t get_SOS(FILE *input, unsigned char *buffer, struct JPEG *jpeg);

struct JPEG * extract(char *filename);

#endif
