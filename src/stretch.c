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
    int dest[2*SIZE*SIZE+2*SIZE];
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
    int dest[(2*SIZE)*SIZE+SIZE];
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
    fprintf(stderr, "Y_sampling_factor_horizontal = %d\n", Y_sampling_factor_horizontal);
    fprintf(stderr, "Y_sampling_factor_vertical = %d\n", Y_sampling_factor_vertical);
    fprintf(stderr, "get_JPEG_nb_Mcu_Width_Strechted(jpeg)= %ld\n", get_JPEG_nb_Mcu_Width_Strechted(jpeg));
    fprintf(stderr, "get_JPEG_nb_Mcu_Height_Strechted(jpeg)= %ld\n", get_JPEG_nb_Mcu_Height_Strechted(jpeg));

    size_t nb_mcu_to_stretch = get_JPEG_nb_Mcu_Width_Strechted(jpeg) * get_JPEG_nb_Mcu_Height_Strechted(jpeg);
    fprintf(stderr, "nb_mcu_to_stretch = %ld\n", nb_mcu_to_stretch);

    fprintf(stderr, "+++++++++++++++++++++++++++++\n");
    print_matrix(MCUs_Cb[0]);
    fprintf(stderr, "\n\n");
    print_matrix(MCUs_Cb[1]);
    fprintf(stderr, "\n\n");
    print_matrix(MCUs_Cb[2]);
    fprintf(stderr, "\n\n");
    print_matrix(MCUs_Cb[3]);
    fprintf(stderr, "\n\n");
    print_matrix(MCUs_Cr[0]);
    fprintf(stderr, "\n\n");
    print_matrix(MCUs_Cr[1]);
    fprintf(stderr, "\n\n");
    print_matrix(MCUs_Cr[2]);
    fprintf(stderr, "\n\n");
    print_matrix(MCUs_Cr[3]);

    for (long long i = 0; i < nb_mcu_to_stretch; i+=Y_sampling_factor_horizontal) {
        fprintf(stderr, "i = %lld\n", i);
        // print_matrix(MCUs_Cb[2]);
        // fprintf(stderr, "\n\n");
        // print_matrix(MCUs_Cr[2]);
        // fprintf(stderr, "\n\n");
        transformX(MCUs_Cb[i], MCUs_Cb[i], MCUs_Cb[i+1]);
        transformX(MCUs_Cr[i], MCUs_Cr[i], MCUs_Cr[i+1]);
        
    }

    fprintf(stderr, "===================================\n");
    print_matrix(MCUs_Cb[0]);
    fprintf(stderr, "\n\n");
    print_matrix(MCUs_Cb[1]);
    fprintf(stderr, "\n\n");
    print_matrix(MCUs_Cb[2]);
    fprintf(stderr, "\n\n");
    print_matrix(MCUs_Cb[3]);
    fprintf(stderr, "\n\n");
    print_matrix(MCUs_Cr[0]);
    fprintf(stderr, "\n\n");
    print_matrix(MCUs_Cr[1]);
    fprintf(stderr, "\n\n");
    print_matrix(MCUs_Cr[2]);
    fprintf(stderr, "\n\n");
    print_matrix(MCUs_Cr[3]);
}

// int main() {
//     int matrix[SIZE][SIZE] = {
//         {0,1,2,3,4,5,6,7},
//         {8,9,10,11,12,13,14,15},
//         {16,17,18,19,20,21,22,23},
//         {24,25,26,27,28,29,30,31},
//         {32,33,34,35,36,37,38,39},
//         {40,41,42,43,44,45,46,47},
//         {48,49,50,51,52,53,54,55},
//         {56,57,58,59,60,61,62,63}};
    
//     int matA[SIZE][SIZE];
//     int matB[SIZE][SIZE];
//     int matC[SIZE][SIZE];
//     int matD[SIZE][SIZE];
    
//     // transform(matrix, matA, matB, matC, matD);

//     // printf("Matrix A:\n");
//     // print_matrix(matA);
//     // printf("\n");

//     // printf("Matrix B:\n");
//     // print_matrix(matB);
//     // printf("\n");

//     // printf("Matrix C:\n");
//     // print_matrix(matC);
//     // printf("\n");

//     // printf("Matrix D:\n");
//     // print_matrix(matD);
//     // printf("\n");

//     transformY(matrix, matA, matB);
//     printf("Matrix A:\n");
//     print_matrix(matA);
//     printf("\n");

//     printf("Matrix B:\n");
//     print_matrix(matB);
//     printf("\n");

    
//     return 0;
// }
