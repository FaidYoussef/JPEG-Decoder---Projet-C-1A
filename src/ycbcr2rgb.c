#include <ycbcr2rgb.h>


// Fonction pour saturer les valeurs entre 0 et 255
uint8_t saturer(int valeur) {
    if (valeur < 0) {
        return 0;
    } else if (valeur > 255) {
        return 255;
    } else {
        return (uint8_t) valeur;
    }
}


// Fonction pour convertir un pixel YCbCr en pixel RGB
// Si on a une seule composante, on ne met à jour que la composante 0
// Si on a plusieurs composantes, on met à jour en lieu et place des composantes 0, 1 et 2 les valeurs R, G et B
void ycbcr_vers_rgb(struct JPEG *jpeg, size_t MCU_number) {
    int16_t* MCU_component0 = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), COMPONENT_0_INDEX))[MCU_number];
    int16_t* MCU_component1;
    int16_t* MCU_component2;

    if (get_sos_nb_components(get_JPEG_sos(jpeg)[0]) > 1){
        MCU_component1 = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), COMPONENT_1_INDEX))[MCU_number];
        MCU_component2 = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), COMPONENT_2_INDEX))[MCU_number];
    }

    for (int8_t i = 0; i < 64; i++){
        int r = MCU_component0[i];
        int g;
        int b;

        if (get_sos_nb_components(get_JPEG_sos(jpeg)[0]) > 1){
            r = MCU_component0[i] + 1.402 * (MCU_component2[i] - 128);
            g = MCU_component0[i] - 0.34414 * (MCU_component1[i] - 128) - 0.71414 * (MCU_component2[i] - 128);
            b = MCU_component0[i] + 1.772 * (MCU_component1[i] - 128);
        }

        MCU_component0[i] = saturer(r);

        if (get_sos_nb_components(get_JPEG_sos(jpeg)[0]) > 1){
            MCU_component1[i] = saturer(g);
            MCU_component2[i] = saturer(b);
        }
    }
    
}


int8_t YCbCr2RGB(struct JPEG *jpeg){
    size_t nb_mcu_width = 0;
    size_t nb_mcu_height = 0;
    if (get_JPEG_width(jpeg) % 8 == 0) {
        nb_mcu_width =  get_JPEG_width(jpeg) / 8;
    } else {
        nb_mcu_width = (get_JPEG_width(jpeg) / 8) + 1;
    }
    if (get_JPEG_height(jpeg) % 8 == 0) {
        nb_mcu_height =  get_JPEG_height(jpeg) / 8;
    } else {
        nb_mcu_height = (get_JPEG_height(jpeg) / 8) + 1;
    }

    // On parcours tous les MCUs de l'image
    for (size_t i = 0; i < nb_mcu_width * nb_mcu_height; i++){
        // Prévoir possibilité de reset-er les données `previous_DC_values` dans le cas où l'on a
        // plusieurs scans/frames ---> mode progressif

        ycbcr_vers_rgb(jpeg, i);
    }
    return EXIT_SUCCESS;
}