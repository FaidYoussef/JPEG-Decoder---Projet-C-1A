#include <ycbcr2rgb.h>


// Fonction pour saturer les valeurs entre 0 et 255
uint8_t saturer(int16_t valeur) {
    if (valeur < 0) {
        return 0;
    } else if (valeur > 255) {
        return 255;
    } else {
        return (uint8_t) valeur;
    }
}

// Fonction pour convertir un pixel YCbCr en pixel RGB
// Si 1 composante : on met à jour en lieu et place des composantes 0, 1 et 2 avec la valeur de la luminance uniquement
// Si 3 composantes : on met à jour en lieu et place des composantes 0, 1 et 2 avec les valeurs R, G et B
void pixel_YCbCr2RGB(int16_t *pixel_Y, int16_t *pixel_Cb, int16_t *pixel_Cr, int8_t nb_components, bool force_grayscale) {

        int16_t r = *pixel_Y;
        int16_t g = *pixel_Y;
        int16_t b = *pixel_Y;

        if (!force_grayscale && nb_components > 1){
            
            r += round(-0.0009267 * (*pixel_Cb - 128) + 1.4016868 * (*pixel_Cr - 128));
            g += round(- 0.3436954 * (*pixel_Cb - 128) - 0.7141690 * (*pixel_Cr - 128));
            b += round(1.7721604 * (*pixel_Cb - 128) + 0.0009902 * (*pixel_Cr - 128));

            // Forumles simplifiées
            // r += 1.402 * (*pixel_Cr - 128);
            // g += - 0.34414 * (*pixel_Cb - 128) - 0.71414 * (*pixel_Cr - 128);
            // b += 1.772 * (*pixel_Cb - 128);
        }

        *pixel_Y = saturer(r);
        *pixel_Cb = saturer(g);
        *pixel_Cr = saturer(b);
    
}


// Fonction pour convertir un MCU YCbCr en pixel RGB
void MCU_YCbCr2RGB(int16_t *MCU_Y, int16_t *MCU_Cb, int16_t *MCU_Cr, int8_t nb_components, bool force_grayscale) {

    for (int8_t i = 0; i < 64; i++){
        pixel_YCbCr2RGB(&MCU_Y[i], &MCU_Cb[i], &MCU_Cr[i], nb_components, force_grayscale);
    }
    
}


int8_t YCbCr2RGB(struct JPEG *jpeg, bool force_grayscale){
    size_t nb_mcu_width = (get_JPEG_width(jpeg) + 7) / 8;
    size_t nb_mcu_height = (get_JPEG_height(jpeg) + 7) / 8;


    // On parcours tous les MCUs de l'image
    for (size_t i = 0; i < nb_mcu_width * nb_mcu_height; i++){
        // Prévoir possibilité de reset-er les données `previous_DC_values` dans le cas où l'on a
        // plusieurs scans/frames ---> mode progressif

        int16_t* MCU_Y = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), COMPONENT_0_INDEX))[i];
        int16_t* MCU_Cb = NULL;
        int16_t* MCU_Cr = NULL;

        if (!force_grayscale && get_sos_nb_components(get_JPEG_sos(jpeg)[0]) > 1){
            MCU_Cb = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), COMPONENT_1_INDEX))[i];
            MCU_Cr = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), COMPONENT_2_INDEX))[i];
        }

        MCU_YCbCr2RGB(MCU_Y, MCU_Cb, MCU_Cr, get_sos_nb_components(get_JPEG_sos(jpeg)[0]), force_grayscale);
    }
    return EXIT_SUCCESS;
}