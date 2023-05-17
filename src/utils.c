#include <utils.h>

int8_t check_memory_allocation(void *allocated_data){
    if(allocated_data == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void print_block(int16_t *block, size_t MCU_number){
    getHighlyVerbose() ? fprintf(stderr, "\nMCU#%ld :\n", MCU_number):0;
    for (int i = 0; i < 64; i++) {
        getHighlyVerbose() ? fprintf(stderr, "%hx ", block[i]):0;
        getHighlyVerbose() ? (i % 8 == 7) ? fprintf(stderr, "\n"):0:0;
    }
    getHighlyVerbose() ? printf("\n"):0;
}
