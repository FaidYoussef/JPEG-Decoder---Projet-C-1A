#include <utils.h>

int8_t check_memory_allocation(void *allocated_data){
    if(allocated_data == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void print_block(int16_t *block){
    fprintf(stderr, "\nBlock :\n");
    for (int i = 0; i < 64; i++) {
        fprintf(stderr, "%hx ", block[i]);
        if (i % 8 == 7) fprintf(stderr, "\n");
    }
    printf("\n");
}
