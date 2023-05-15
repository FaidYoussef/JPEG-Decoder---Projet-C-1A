#include <IQ.h>

const int16_t test_qt2[64] = {
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2
};

const int16_t test_qt_max_value[64] = {
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535
};

const int16_t test_qt0[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};


// Inverse quantization function using quant_table
// qblock : les données qui sont quantifiées
int8_t inv_quantize(struct JPEG * jpeg) {
    size_t nb_mcu_width = 0;
    size_t nb_mcu_height = 0;
    if (get_JPEG_width(jpeg) % 8 == 0) {
        nb_mcu_width =  get_JPEG_width(jpeg) / 8;
    } else {
        nb_mcu_width = (get_JPEG_width(jpeg) / 8) + 1;
    }
    if (get_JPEG_width(jpeg) % 8 == 0) {
        nb_mcu_height =  get_JPEG_width(jpeg) / 8;
    } else {
        nb_mcu_height = (get_JPEG_width(jpeg) / 8) + 1;
    }

    // On parcours toutes les composantes
    for (int8_t i = 0; i < get_sos_nb_components(get_JPEG_sos(jpeg)[0]); i++) {   // attention ici l'index 0 correspond au 1er scan/frame ... prévoir d'intégrer un index pour le mode progressif
        // On récupère la table de quantification associée à la composante
        int8_t qt_index = get_num_quantization_table(get_sof_components((get_JPEG_sof(jpeg)[0]))[i]);
        fprintf(stderr, "qt_index : %d\n", qt_index);
        struct QuantizationTable *qt = get_JPEG_qt(jpeg)[qt_index];
        unsigned char *qt_table = get_qt_data(qt);
        int16_t** MCUs = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), i));

        // On parcours tous les MCUs de l'image
        for (size_t j = 0; j < nb_mcu_width * nb_mcu_height; j++){
            fprintf(stderr, "Block avant IQ\n");
            print_block(MCUs[j]);
            for (int8_t k = 0; k < 64; k++) {
                MCUs[j][k] = MCUs[j][k] * qt_table[k];
            }
            fprintf(stderr, "Block après IQ\n");
            print_block(MCUs[j]);
        }
    }
    return EXIT_SUCCESS;
}


