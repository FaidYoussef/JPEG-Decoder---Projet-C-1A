#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <extract.h>

#define NB_VALUES_IN_8x8_BLOCK 64
#define COMPONENT_0_INDEX 0
#define COMPONENT_1_INDEX 1
#define COMPONENT_2_INDEX 2

#define RED(string) "\x1b[31m" string "\x1b[0m"
#define GREEN(string) "\x1b[32m" string "\x1b[0m"
#define YELLOW(string) "\x1b[33m" string "\x1b[0m"
#define BLUE(string) "\x1b[34m" string "\x1b[0m"
#define MAGENTA(string) "\x1b[35m" string "\x1b[0m"
#define CYAN(string) "\x1b[36m" string "\x1b[0m"


// Check if memory allocation was successful
int8_t check_memory_allocation(void *allocated_data);

// Display block in matrix format
void print_block(int16_t *block, size_t MCU_number, int8_t component_index);

#endif
