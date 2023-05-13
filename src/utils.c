#include <utils.h>

int check_memory_allocation(void *allocated_data, struct JPEG *jpeg){
    if(allocated_data == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}