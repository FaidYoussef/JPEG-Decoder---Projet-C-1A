#include <stretch.h>


void transformY(int16_t src[SIZE*SIZE], int16_t matA[SIZE*SIZE], int16_t matB[SIZE*SIZE]) {
    int dest[SIZE * SIZE * 2];
    for(int8_t i = 0; i < SIZE; i++) {
        for(int8_t j = 0; j < SIZE; j++) {
            dest[2*i*SIZE+j] = src[i*SIZE+j];
            dest[(2*i + 1)*SIZE+j] = src[i*SIZE+j];
        }
    }

    for(int8_t i = 0; i < 2*SIZE; i++) {
        for(int8_t j = 0; j < SIZE; j++) {
            if(i < SIZE) {
                matA[i*SIZE+j] = dest[i*SIZE+j];
            } else {
                matB[(i - SIZE)*SIZE+j] = dest[i*SIZE+j];
            }
        }
    }
}

void transformX(int16_t src[SIZE*SIZE], int16_t matA[SIZE*SIZE], int16_t matB[SIZE*SIZE]) {


    int dest[SIZE*2*SIZE];
    for(int8_t i = 0; i < SIZE; i++) {
        for(int8_t j = 0; j < SIZE; j++) {
            dest[i*2*SIZE+2*j] = src[i*SIZE+j];
            dest[i*2*SIZE+2*j + 1] = src[i*SIZE+j];
        }
    }

    for(int8_t i = 0; i < SIZE; i++) {
        for(int8_t j = 0; j < 2*SIZE; j++) {
            if(j < SIZE) {
                matA[i*SIZE+j] = dest[i*2*SIZE+j];
            } else {
                matB[i*SIZE+j - SIZE] = dest[i*2*SIZE+j];
            }
        }
    }
}

void print_matrix(int16_t matrix[SIZE*SIZE]) {
    for(int8_t i = 0; i < SIZE; i++) {
        for(int8_t j = 0; j < SIZE; j++) {
            printf("%x ", matrix[i*SIZE+j]);
        }
        printf("\n");
    }
}

void stretch_function(struct JPEG *jpeg) {
    
    int16_t** MCUs_Cb = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), COMPONENT_1_INDEX));
    int16_t** MCUs_Cr = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), COMPONENT_2_INDEX));

    int8_t Y_sampling_factor_horizontal = get_JPEG_Sampling_Factor_X(jpeg);
    int8_t Y_sampling_factor_vertical = get_JPEG_Sampling_Factor_Y(jpeg);


    size_t nb_mcu_to_stretch = get_JPEG_nb_Mcu_Width_Strechted(jpeg) * get_JPEG_nb_Mcu_Height_Strechted(jpeg);

    if (Y_sampling_factor_horizontal == 2) {
        for (size_t i = 0; i < nb_mcu_to_stretch; i+=Y_sampling_factor_horizontal) {
    
            transformX(MCUs_Cb[i], MCUs_Cb[i], MCUs_Cb[i+1]);
            transformX(MCUs_Cr[i], MCUs_Cr[i], MCUs_Cr[i+1]);
        }
    }     

    if (Y_sampling_factor_vertical == 2) {
        for (size_t i = 0; i < nb_mcu_to_stretch - get_JPEG_nb_Mcu_Width_Strechted(jpeg); i++) {
            if (i == nb_mcu_to_stretch - get_JPEG_nb_Mcu_Width_Strechted(jpeg) ){
                break;
            }
            if (i % get_JPEG_nb_Mcu_Width_Strechted(jpeg) == 0 && i != 0 ) {
                i += get_JPEG_nb_Mcu_Width_Strechted(jpeg); 
            }
            transformY(MCUs_Cb[i], MCUs_Cb[i], MCUs_Cb[i+ get_JPEG_nb_Mcu_Width_Strechted(jpeg)]);
            transformY(MCUs_Cr[i], MCUs_Cr[i], MCUs_Cr[i+ get_JPEG_nb_Mcu_Width_Strechted(jpeg)]);
        }
    }
}
