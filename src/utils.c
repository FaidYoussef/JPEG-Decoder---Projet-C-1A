#include <utils.h>

void check_memory_allocation(void *allocated_data){
    if(allocated_data == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }
}