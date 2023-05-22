// #include <ycbcr2rgb.h>


// // Fonction pour saturer les valeurs entre 0 et 255
// uint8_t saturer(int16_t valeur) {
//     if (valeur < 0) {
//         return 0;
//     } else if (valeur > 255) {
//         return 255;
//     } else {
//         return (uint8_t) valeur;
//     }
// }

// // Fonction pour convertir un pixel YCbCr en pixel RGB
// // Si 1 composante : on met à jour en lieu et place des composantes 0, 1 et 2 avec la valeur de la luminance uniquement
// // Si 3 composantes : on met à jour en lieu et place des composantes 0, 1 et 2 avec les valeurs R, G et B
// void pixel_YCbCr2RGB(int16_t *pixel_Y, int16_t *pixel_Cb, int16_t *pixel_Cr, int8_t nb_components, bool force_grayscale) {

//         int16_t r = *pixel_Y;
//         int16_t g = *pixel_Y;
//         int16_t b = *pixel_Y;

//         if (!force_grayscale && nb_components > 1){
//             r += 1.402 * (*pixel_Cr - 128);
//             g += - 0.34414 * (*pixel_Cb - 128) - 0.71414 * (*pixel_Cr - 128);
//             b += 1.772 * (*pixel_Cb - 128);
//         }

//         *pixel_Y = saturer(r);

//         if (!force_grayscale && nb_components > 1){
//             *pixel_Cb = saturer(g);
//             *pixel_Cr = saturer(b);
//         }
    
// }


// // Fonction pour convertir un MCU YCbCr en pixel RGB
// void MCU_YCbCr2RGB(int16_t *MCU_Y, int16_t *MCU_Cb, int16_t *MCU_Cr, const uint8_t Sampling_Factor_X, const uint8_t Sampling_Factor_Y, const uint8_t v, const uint8_t h, int8_t nb_components, bool force_grayscale) {

//     for (uint8_t y = 7; y < 8; y--){
//         for (uint8_t x = 7; x < 8; x--){
//             const uint8_t pixel_index = y * 8 + x;
//             // const uint8_t CbCr_pixel_row = y / Sampling_Factor_Y + 4 * v;
//             // const uint8_t CbCr_pixel_col = x / Sampling_Factor_X + 4 * h;
//             // const uint8_t CbCr_pixel_index = CbCr_pixel_row * 8 + CbCr_pixel_col;
//             // fprintf(stderr, "pixel_index = %u\n", pixel_index);
//             // fprintf(stderr, "CbCr_pixel_row = %u\n", CbCr_pixel_row);
//             // fprintf(stderr, "CbCr_pixel_col = %u\n", CbCr_pixel_col);
//             // fprintf(stderr, "CbCr_pixel_index = %u\n", CbCr_pixel_index);
//             pixel_YCbCr2RGB(&MCU_Y[pixel_index], &MCU_Cb[pixel_index], &MCU_Cr[pixel_index], nb_components, force_grayscale);
//         }
//     }
    
// }


// int8_t YCbCr2RGB(struct JPEG *jpeg, bool force_grayscale){

//     const uint8_t Sampling_Factor_X = get_JPEG_Sampling_Factor_X(jpeg);
//     const uint8_t Sampling_Factor_Y = get_JPEG_Sampling_Factor_Y(jpeg);

//     // On parcours tous les MCUs de l'image
//     for (size_t y = 0; y < get_JPEG_nb_Mcu_Height(jpeg); y+= Sampling_Factor_Y){
//         for (size_t x = 0; x < get_JPEG_nb_Mcu_Width(jpeg); x+= Sampling_Factor_X){

//             int16_t *MCU_Cb = NULL;
//             int16_t *MCU_Cr = NULL;

//             if (!force_grayscale && get_sos_nb_components(get_JPEG_sos(jpeg)[0]) > 1){
//                 MCU_Cb = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), COMPONENT_1_INDEX))[y * get_JPEG_nb_Mcu_Width_Strechted(jpeg) + x];
//                 MCU_Cr = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), COMPONENT_2_INDEX))[y * get_JPEG_nb_Mcu_Width_Strechted(jpeg) + x];

//             }
            
//             for (uint8_t v = Sampling_Factor_Y -1; v < Sampling_Factor_Y; v--){
//                 for (uint8_t h = Sampling_Factor_X -1; h < Sampling_Factor_X; h--){

//                     // fprintf(stderr, "index MCU = %zu\n", (y + v) * get_JPEG_nb_Mcu_Width_Strechted(jpeg) + (x + h));
//                     // fprintf(stderr, "index CbCr = %zu\n", (y + v) * 8 + (x + h));
//                     // fprintf(stderr, "y = %zu\n", y);
//                     // fprintf(stderr, "x = %zu\n", x);
//                     // fprintf(stderr, "v = %u\n", v);
//                     // fprintf(stderr, "h = %u\n", h);
//                     // fprintf(stderr, "Sampling_Factor_X = %u\n", Sampling_Factor_X);
//                     // fprintf(stderr, "Sampling_Factor_Y = %u\n", Sampling_Factor_Y);
                    
//                     int16_t *MCU_Y = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), COMPONENT_0_INDEX))[(y + v) * get_JPEG_nb_Mcu_Width_Strechted(jpeg) + (x + h)];

//                     MCU_YCbCr2RGB(MCU_Y, MCU_Cb, MCU_Cr, Sampling_Factor_X, Sampling_Factor_Y, v, h, get_sos_nb_components(get_JPEG_sos(jpeg)[0]), force_grayscale);
//                 }
//             }


//         }
//     }
//     return EXIT_SUCCESS;
// }



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
            r += 1.402 * (*pixel_Cr - 128);
            g += - 0.34414 * (*pixel_Cb - 128) - 0.71414 * (*pixel_Cr - 128);
            b += 1.772 * (*pixel_Cb - 128);
        }

        *pixel_Y = saturer(r);

        if (!force_grayscale && nb_components > 1){
            *pixel_Cb = saturer(g);
            *pixel_Cr = saturer(b);
        }
    
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