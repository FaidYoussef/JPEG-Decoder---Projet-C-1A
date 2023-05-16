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
// Si une seule composante (Y) >>> donner en entrée la même valeur '128' pour Cb et Cr
void ycbcr_vers_rgb(uint8_t Y, uint8_t Cb, uint8_t Cr, uint8_t *R, uint8_t *G, uint8_t *B) {
// void ycbcr_vers_rgb(struct JPEG *jpeg, size_t MCU_number) {
    // int16_t* MCU_component0 = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), 0))[MCU_number];
    // int16_t* MCU_component1;
    // int16_t* MCU_component2;

    // if (get_sos_nb_components(get_JPEG_sos(jpeg)[0]) > 1){
    //     int16_t* MCU_component1 = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), 1))[MCU_number];
    //     int16_t* MCU_component2 = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), 2))[MCU_number];
    // }


    int r = Y + 1.402 * (Cr - 128);
    int g = Y - 0.34414 * (Cb - 128) - 0.71414 * (Cr - 128);
    int b = Y + 1.772 * (Cb - 128);

    *R = saturer(r);
    *G = saturer(g);
    *B = saturer(b);
}


int8_t YCbCr2RGB(struct JPEG *jpeg){
    // size_t nb_mcu_width = 0;
    // size_t nb_mcu_height = 0;
    // if (get_JPEG_width(jpeg) % 8 == 0) {
    //     nb_mcu_width =  get_JPEG_width(jpeg) / 8;
    // } else {
    //     nb_mcu_width = (get_JPEG_width(jpeg) / 8) + 1;
    // }
    // if (get_JPEG_height(jpeg) % 8 == 0) {
    //     nb_mcu_height =  get_JPEG_height(jpeg) / 8;
    // } else {
    //     nb_mcu_height = (get_JPEG_height(jpeg) / 8) + 1;
    // }

    // // On parcours tous les MCUs de l'image
    // for (size_t i = 0; i < nb_mcu_width * nb_mcu_height; i++){
    //     // Prévoir possibilité de reset-er les données `previous_DC_values` dans le cas où l'on a
    //     // plusieurs scans/frames ---> mode progressif

    //     ycbcr_vers_rgb(jpeg, i);
        
    //     // // On parcours toutes les composantes
    //     // for (int8_t j = 0; j < get_sos_nb_components(get_JPEG_sos(jpeg)[0]); ++j) {   // attention ici l'index 0 correspond au 1er scan/frame ... prévoir d'intégrer un index pour le mode progressif
    //     //     if (ycbcr_vers_rgb(jpeg, i, j) ) return EXIT_FAILURE;
    //     // }
    // }
    return EXIT_SUCCESS;
}