#include <IQ.h>


// Inverse quantization function
void IQ_function(int16_t *mcu, const uint8_t *qtable) {
    for (int8_t k = 0; k < 64; k++) {
        int32_t result = (int32_t)mcu[k] * qtable[k];
        if (result > INT16_MAX)
            mcu[k] = INT16_MAX;
        else if (result < INT16_MIN)
            mcu[k] = INT16_MIN;
        else
            mcu[k] = (int16_t)result;
    }
}


// Fonction qui récupère les données de la structure JPEG et qui procède à la quantification inverse
int8_t IQ(struct JPEG * jpeg) {

    // On parcourt tous les MCUs de l'image
    for (size_t y = 0; y < get_JPEG_nb_Mcu_Height(jpeg); y += get_JPEG_Sampling_Factor_Y(jpeg)) {
        for (size_t x = 0; x < get_JPEG_nb_Mcu_Width(jpeg); x += get_JPEG_Sampling_Factor_X(jpeg)) {

            // On parcourt toutes les composantes
            for (int8_t i = 0; i < get_sos_nb_components(get_JPEG_sos(jpeg)[0]); i++) {   // attention ici l'index 0 correspond au 1er scan/frame ... prévoir d'intégrer un index pour le mode progressif
                
                // On récupère la table de quantification associée à la composante
                int8_t qt_index = get_num_quantization_table(get_sof_component(get_sof_components((get_JPEG_sof(jpeg)[0]) ), i));
                getHighlyVerbose() ? fprintf(stderr, "qt_index : %d\n", qt_index):0;
                struct QuantizationTable *qt = get_JPEG_qt(jpeg)[qt_index];
                const uint8_t *qt_table = get_qt_data(qt);
                
                // On récupère les MCUs de la composante
                int16_t** MCUs = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), i));

                for (int8_t v = 0; v < get_sampling_factor_y(get_sof_component(get_sof_components(get_JPEG_sof(jpeg)[0]), i)); v++) {
                    for (int8_t h = 0; h < get_sampling_factor_x(get_sof_component(get_sof_components(get_JPEG_sof(jpeg)[0]), i)); h++) {
                        // On récupère le MCU
                        int16_t *mcu = MCUs[(y + v) * get_JPEG_nb_Mcu_Width_Strechted(jpeg) + (x + h)];

                        getHighlyVerbose() ? fprintf(stderr, "MCU avant IQ\n"):0;
                        print_block(mcu,(y + v) * get_JPEG_nb_Mcu_Width_Strechted(jpeg) + (x + h) , i);

                        // On applique la quantification inverse
                        IQ_function(mcu, qt_table);

                        getHighlyVerbose() ? fprintf(stderr, "MCU après IQ\n"):0;
                        print_block(mcu, v * get_JPEG_Sampling_Factor_X(jpeg) + h, i);
                    }
                }
            }
        }
    }
    return EXIT_SUCCESS;
}
