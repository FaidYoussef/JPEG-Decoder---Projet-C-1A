#include <stretch.h>

// # include <stdint.h>
// # include <stdlib.h>
// #include <stdio.h>

// void stretch(int16_t ** MCUs, int8_t sampling_factor_x, size_t nb_mcu_width, size_t nb_mcu_height){
//     for (size_t i = nb_mcu_width * nb_mcu_height / sampling_factor_x - 1; i < nb_mcu_width * nb_mcu_height / sampling_factor_x; i--){
//         printf("%zu\n", i);
//         for (size_t j = 0; j < 4; j++){
//             if (j % 2 == 1){
//                 printf("======================%zu %zu\n", 2*i + 1, j- 1);
//                 MCUs[2*i + 1][j] = MCUs[i][j-1];
//                 MCUs[2*i + 1][j+1] = MCUs[i][j-1];
//             } else {
//                 printf("============================%zu %zu\n", 2*i, j);
//                 MCUs[2*i][j] = MCUs[i][j];
//                 MCUs[2*i][j+1] = MCUs[i][j];
//             }        
//         }
//     }
// }

// int main(int argc, char *argv[]){
//     int16_t ** MCUs = malloc(4 * sizeof(int16_t *));
//     for (size_t i = 0; i < 4; i++){
//         MCUs[i] = malloc(4 * sizeof(int16_t));
//         for (size_t j = 0; j < 4; j++){
//             MCUs[i][j] = j;
//         }
//     }
//     stretch(MCUs, 2, 2, 2);
//     for (size_t i = 0; i < 4; i++){
//         for (size_t j = 0; j < 4; j++){
//             printf("%d ", MCUs[i][j]);
//         }
//         printf("\n");
//     }
//     return 0;
// }

// #include <stdio.h>

// void transformMatrix(int mat[4][4], int rows, int cols) {
//     int i, j;
//     for (i = 0; i < rows; i++) {
//         for (j = 0; j < cols; j += 2) {
//             mat[i][j + 1] = mat[i][j];
//         }
//     }
// }

// void printMatrix(int mat[4][4], int rows, int cols) {
//     int i, j;
//     for (i = 0; i < rows; i++) {
//         for (j = 0; j < cols; j++) {
//             printf("%d ", mat[i][j]);
//         }
//         printf("\n");
//     }
// }

// int main() {
//     int mat[4][4] = {{0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11}, {12, 13, 14, 15}};

//     printf("Initial matrix:\n");
//     printMatrix(mat, 4, 4);

//     transformMatrix(mat, 4, 4);

//     printf("Transformed matrix:\n");
//     printMatrix(mat, 4, 4);

//     return 0;
// }



void transformXY(int16_t src[SIZE*SIZE], int16_t matA[SIZE*SIZE], int16_t matB[SIZE*SIZE], int16_t matC[SIZE*SIZE], int16_t matD[SIZE*SIZE]) {
    int dest[2*SIZE*SIZE*2];
    for(int8_t i = 0; i < SIZE; i++) {
        for(int8_t j = 0; j < SIZE; j++) {
            dest[2*i*SIZE+2*j] = src[i*SIZE+j];
            dest[2*i*SIZE+2*j + 1] = src[i*SIZE+j];
            dest[(2*i + 1)*SIZE+2*j] = src[i*SIZE+j];
            dest[(2*i + 1)*SIZE+2*j + 1] = src[i*SIZE+j];
        }
    }

    for(int8_t i = 0; i < 2*SIZE; i++) {
        for(int8_t j = 0; j < 2*SIZE; j++) {
            if(i < SIZE && j < SIZE) {
                matA[i*SIZE+j] = dest[i*SIZE+j];
            } else if(i < SIZE && j >= SIZE) {
                matB[i*SIZE+j - SIZE] = dest[i*SIZE+j];
            } else if(i >= SIZE && j < SIZE) {
                matC[(i - SIZE)*SIZE+j] = dest[i*SIZE+j];
            } else {
                matD[(i - SIZE)*SIZE+j - SIZE] = dest[i*SIZE+j];
            }
        }
    }
}

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
    // int dest[SIZE*SIZE+2*SIZE];
    // for(int8_t i = 0; i < SIZE; i++) {
    //     for(int8_t j = 0; j < SIZE; j++) {
    //         dest[i*SIZE+2*j] = src[i*SIZE+j];
    //         dest[i*SIZE+2*j + 1] = src[i*SIZE+j];
    //     }
    // }

    // for(int8_t i = 0; i < SIZE; i++) {
    //     for(int8_t j = 0; j < 2*SIZE; j++) {
    //         if(j < SIZE) {
    //             matA[i*SIZE+j] = dest[i*SIZE+j];
    //         } else {
    //             matB[i*SIZE+j - SIZE] = dest[i*SIZE+j];
    //         }
    //     }
    // }

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
        for (long long i = 0; i < nb_mcu_to_stretch; i+=Y_sampling_factor_horizontal) {
    
            transformX(MCUs_Cb[i], MCUs_Cb[i], MCUs_Cb[i+1]);
            transformX(MCUs_Cr[i], MCUs_Cr[i], MCUs_Cr[i+1]);
        }
    }     

    if (Y_sampling_factor_vertical == 2) {
        for (long long i = 0; i < nb_mcu_to_stretch - get_JPEG_nb_Mcu_Width_Strechted(jpeg); i++) {
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
    // for (long long i = 0; i < nb_mcu_to_stretch - get_JPEG_nb_Mcu_Width_Strechted(jpeg); i+=2) {
    //     // fprintf(stderr, "i = %lld ", i);
    //     if (i == nb_mcu_to_stretch - get_JPEG_nb_Mcu_Width_Strechted(jpeg) ){
    //         break;
    //     }
        
    //     if (i % get_JPEG_nb_Mcu_Width_Strechted(jpeg) == 0 && i != 0 ) {
    //         i += get_JPEG_nb_Mcu_Width_Strechted(jpeg); 
    //     }
    //     // print_matrix(MCUs_Cb[2]);
    //     // fprintf(stderr, "\n\n");
    //     // print_matrix(MCUs_Cr[2]);
    //     // fprintf(stderr, "\n\n");
    //     transformXY(MCUs_Cb[i], MCUs_Cb[i],MCUs_Cb[i+1], MCUs_Cb[i+ get_JPEG_nb_Mcu_Width_Strechted(jpeg)], MCUs_Cb[i+ get_JPEG_nb_Mcu_Width_Strechted(jpeg) + 1]);
    //     transformXY(MCUs_Cr[i], MCUs_Cr[i],MCUs_Cr[i+1], MCUs_Cr[i+ get_JPEG_nb_Mcu_Width_Strechted(jpeg)], MCUs_Cr[i+ get_JPEG_nb_Mcu_Width_Strechted(jpeg) + 1]);
        
    // }

}
