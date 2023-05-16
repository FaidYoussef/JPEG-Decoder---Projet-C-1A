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


int8_t check_memory_allocation(void *allocated_data);

// Display block in matrix format
void print_block(int16_t *block);

#endif
