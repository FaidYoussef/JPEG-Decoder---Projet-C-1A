#include <IQ.h>


bool isOverflow(int16_t a, int16_t b) {
    // Check if either of them is zero
    if (a == 0 || b == 0)
        return false;
     
    int16_t result = a * b;
    if (a == result / b) {
        return false;
    } else {
        return true;
    }
}


// Inverse quantization function
void IQ(int16_t *mcu, const uint8_t *qtable){
    for (int8_t k = 0; k < 64; k++) {
        if (isOverflow(mcu[k], qtable[k]) && mcu[k] < 0) {
            mcu[k] = -32768;
        } else if (isOverflow(mcu[k], qtable[k]) && mcu[k] > 0) {
            mcu[k] = 32767;
        } else {
            mcu[k] = mcu[k] * qtable[k];
        }
    }
}


// Fonction qui récupère les données de la structure JPEG et qui procède à la quantification inverse
int8_t inv_quantize(struct JPEG * jpeg) {
    size_t nb_mcu_width = (get_JPEG_width(jpeg) + 7) / 8;
    size_t nb_mcu_height = (get_JPEG_height(jpeg) + 7) / 8;

    // On parcours toutes les composantes
    for (int8_t i = 0; i < get_sos_nb_components(get_JPEG_sos(jpeg)[0]); i++) {   // attention ici l'index 0 correspond au 1er scan/frame ... prévoir d'intégrer un index pour le mode progressif
        // On récupère la table de quantification associée à la composante
        int8_t qt_index = get_num_quantization_table(get_sof_components((get_JPEG_sof(jpeg)[0]))[i]);
        getHighlyVerbose() ? fprintf(stderr, "qt_index : %d\n", qt_index):0;
        struct QuantizationTable *qt = get_JPEG_qt(jpeg)[qt_index];
        const uint8_t *qt_table = get_qt_data(qt);
        
        // On récupère les MCUs de la composante
        int16_t** MCUs = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), i));

        // On parcours tous les MCUs de l'image
        for (size_t j = 0; j < nb_mcu_width * nb_mcu_height; j++){
            getHighlyVerbose() ? fprintf(stderr, "MCU avant IQ\n"):0;
            print_block(MCUs[j], j);

            IQ(MCUs[j], qt_table);

            getHighlyVerbose() ? fprintf(stderr, "MCU après IQ\n"):0;
            print_block(MCUs[j], j);
        }
    }
    return EXIT_SUCCESS;
}


