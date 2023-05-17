#include <IZZ.h>

const uint8_t zigzag[]={
    0, 1, 8, 16, 9, 2, 3, 10,
    17, 24, 32, 25, 18, 11, 4, 5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13, 6, 7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63
};


// Fonction qui permet de dé-zigzaguer un bloc
int8_t IZZ_function(struct JPEG *jpeg,size_t MCU_number, int8_t component_index){
    int16_t *block = (int16_t *) malloc(64 * sizeof(int16_t));
    if (check_memory_allocation(block)) return EXIT_FAILURE;

    int16_t **MCUs = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), component_index));

    for (int8_t i = 0; i < 64; i++) {
        block[zigzag[i]] = MCUs[MCU_number][i];    
    }

    free(MCUs[MCU_number]);
    MCUs[MCU_number] = block;

    getHighlyVerbose() ? fprintf(stderr, "MCU après IZZ\n"):0;
    print_block(MCUs[MCU_number], MCU_number, component_index);

    return EXIT_SUCCESS;
}


int8_t IZZ(struct JPEG * jpeg) {
    size_t nb_mcu_width = (get_JPEG_width(jpeg) + 7) / 8;
    size_t nb_mcu_height = (get_JPEG_height(jpeg) + 7) / 8;

    // On parcourt tous les MCUs de l'image
    for (size_t i = 0; i < nb_mcu_width * nb_mcu_height; i++){
        // Prévoir possibilité de reset-er les données `previous_DC_values` dans le cas où l'on a
        // plusieurs scans/frames ---> mode progressif
        
        // On parcourt toutes les composantes
        for (int8_t j = 0; j < get_sos_nb_components(get_JPEG_sos(jpeg)[0]); ++j) {   // attention ici l'index 0 correspond au 1er scan/frame ... prévoir d'intégrer un index pour le mode progressif
            if (IZZ_function(jpeg, i, j) ) return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
