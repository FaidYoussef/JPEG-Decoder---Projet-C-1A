// #include <stdlib.h>
// #include <stdbool.h>
// #include <stdio.h>
// #include <math.h>



// // // Inverse Discrete Cosine Transform function
// // int8_t IDCT_function(int16_t **mcu){

// //     int16_t *output = (int16_t *) malloc(NN * sizeof(int16_t));
// //     if (check_memory_allocation(output)) return EXIT_FAILURE;

// //     for (int8_t x = 0; x < N; x++) {
// //         for (int8_t y = 0; y < N; y++) { 
// //             double sum = 0.0;

// //             for (int8_t u = 0; u < N; u++) {
// //                 for (int8_t v = 0; v < N; v++) {
// //                     int16_t dct_uv = (*mcu)[u * N + v];
// //                     sum +=  dct_uv * C_cos_values[x][y][u][v];
// //                 }
// //             }

// //             if (round(sum) + 128 < 0) {
// //                 output[x * N + y] = 0;
// //             } else if (round(sum) + 128 > 255) {
// //                 output[x * N + y] = 255;
// //             } else {
// //                 output[x * N + y] = round(sum) + 128;
// //             }
// //         }
// //     }

// //     free(*mcu);
// //     *mcu = output;

// //     return EXIT_SUCCESS;
// // }

// # include <stdint.h>

// //  cos_table[i] = cos(i*pi/16)
// float cos_table[9] = {1.0,
// 			               0.98078528040323044912618223613,
// 			               0.92387953251128675612818318939,
// 			               0.83146961230254523707878837761,
// 			               0.70170678118654752440084436210,
// 			               0.55557023301960222474283081394,
// 			               0.38268343236508977172845998403,
// 			               0.19509032201612826784828486847,
// 			               0.0};
// float sqrt_2 = 1.41421356237309504880168872420;

// // papillon_inv est l'inverse du module papillon de loeffler
// float* papillon_inv(float O0, float O1){
//     float I0 = (O0 + O1)/2;
//     float I1 = (O0 - O1)/2;

//     float *output = (float *) malloc(2 * sizeof(float));

//     output[0] = I0;
//     output[1] = I1;

//     return output;

// }

// // rotation_inv est l'inverse du module rotation de loeffler
// float* rotation_inv(float O0, float O1, float k, uint8_t num_coef){
//     float I0 = 1.0/k * (O0 *  cos_table[num_coef] - O1 * cos_table[8 - num_coef]);
//     float I1 = 1.0/k * (O1 *  cos_table[num_coef] + O0 * cos_table[8 - num_coef]);

//     float *output = (float *) malloc(2 * sizeof(float));

//     output[0] = I0;
//     output[1] = I1;

//     return output;
// }


// // fidct est l'inverse de la dct en utilisant l'algorithme de loeffler
// int8_t fidct(int16_t ** input){
    
//     // passage en float
//     float input_float[8][8];
//     for (uint8_t i = 0; i<8; i++){
//         for (uint8_t j = 0; j<8; j++){
//             input_float[i][j] = (float) input[i][j];
//         }
//     }

//     for (uint8_t i = 0; i<8; i++){

//         // vecteur temporaire pour stocker les valeurs intermédiaires
//         float temp[8];

//         // 1st step
//         temp[0] = input_float[i][0];
//         temp[1] = input_float[i][4];
//         temp[2] = input_float[i][2];
//         temp[3] = input_float[i][6];
//         temp[4] = papillon_inv(input_float[i][1], input_float[i][7])[1];
//         temp[5] = input_float[i][3] / sqrt_2;
//         temp[6] = input_float[i][5] / sqrt_2;
//         temp[7] = papillon_inv(input_float[i][1], input_float[i][7])[0];



//         // 2nd step
        
//         input_float[i][0] = papillon_inv(temp[0], temp[1])[0];
//         input_float[i][1] = papillon_inv(temp[0], temp[1])[1];
//         input_float[i][2] = rotation_inv(temp[2], temp[3], sqrt_2, 6)[0];
//         input_float[i][3] = rotation_inv(temp[2], temp[3], sqrt_2, 6)[1];
//         input_float[i][4] = papillon_inv(temp[4], temp[6])[0];
//         input_float[i][5] = papillon_inv(temp[7], temp[5])[1];
//         input_float[i][6] = papillon_inv(temp[4], temp[6])[1];
//         input_float[i][7] = papillon_inv(temp[7], temp[5])[0];


//         // // 3rd step
//         temp[0] = papillon_inv(input_float[i][0], input_float[i][3])[0];
//         temp[1] = papillon_inv(input_float[i][1], input_float[i][2])[0];
//         temp[2] = papillon_inv(input_float[i][1], input_float[i][2])[1];
//         temp[3] = papillon_inv(input_float[i][0], input_float[i][3])[1];
//         temp[4] = rotation_inv(input_float[i][4], input_float[i][7], 1.0, 3)[0];
//         temp[5] = rotation_inv(input_float[i][5], input_float[i][6], 1.0, 1)[0];
//         temp[6] = rotation_inv(input_float[i][5], input_float[i][6], 1.0, 1)[1];
//         temp[7] = rotation_inv(input_float[i][4], input_float[i][7], 1.0, 3)[1];

//         // // 4th step
//         input_float[i][0] = papillon_inv(temp[0], temp[7])[0];
//         input_float[i][1] = papillon_inv(temp[1], temp[6])[0];
//         input_float[i][2] = papillon_inv(temp[2], temp[5])[0];
//         input_float[i][3] = papillon_inv(temp[3], temp[4])[0];
//         input_float[i][4] = papillon_inv(temp[3], temp[4])[1];
//         input_float[i][5] = papillon_inv(temp[2], temp[5])[1];
//         input_float[i][6] = papillon_inv(temp[1], temp[6])[1];
//         input_float[i][7] = papillon_inv(temp[0], temp[7])[1];
      
//     }

//     for (uint8_t i = 0; i<8; i++ ){
            
//         // get a temp column
//         float temp[8];

//         // 1st step
//         temp[0] = input_float[0][i];
//         temp[1] = input_float[4][i];
//         temp[2] = input_float[2][i];
//         temp[3] = input_float[6][i];
//         temp[4] = papillon_inv(input_float[1][i], input_float[7][i])[1];
//         temp[5] = input_float[3][i] / sqrt_2;
//         temp[6] = input_float[5][i] / sqrt_2;
//         temp[7] = papillon_inv(input_float[1][i], input_float[7][i])[0];


//         // 2nd step
//         input_float[0][i] = papillon_inv(temp[0], temp[1])[0];
//         input_float[1][i] = papillon_inv(temp[0], temp[1])[1];
//         input_float[2][i] = rotation_inv(temp[2], temp[3], sqrt_2, 6)[0];
//         input_float[3][i] = rotation_inv(temp[2], temp[3], sqrt_2, 6)[1];
//         input_float[4][i] = papillon_inv(temp[4], temp[6])[0];
//         input_float[5][i] = papillon_inv(temp[7], temp[5])[1];
//         input_float[6][i] = papillon_inv(temp[4], temp[6])[1];
//         input_float[7][i] = papillon_inv(temp[7], temp[5])[0];

//         // // 3rd step
//         temp[0] = papillon_inv(input_float[0][i], input_float[3][i])[0];
//         temp[1] = papillon_inv(input_float[1][i], input_float[2][i])[0];
//         temp[2] = papillon_inv(input_float[1][i], input_float[2][i])[1];
//         temp[3] = papillon_inv(input_float[0][i], input_float[3][i])[1];
//         temp[4] = rotation_inv(input_float[4][i], input_float[7][i], 1.0, 3)[0];
//         temp[5] = rotation_inv(input_float[5][i], input_float[6][i], 1.0, 1)[0];
//         temp[6] = rotation_inv(input_float[5][i], input_float[6][i], 1.0, 1)[1];
//         temp[7] = rotation_inv(input_float[4][i], input_float[7][i], 1.0, 3)[1];

//         // // 4th step
//         input_float[0][i] = papillon_inv(temp[0], temp[7])[0];
//         input_float[1][i] = papillon_inv(temp[1], temp[6])[0];
//         input_float[2][i] = papillon_inv(temp[2], temp[5])[0];
//         input_float[3][i] = papillon_inv(temp[3], temp[4])[0];
//         input_float[4][i] = papillon_inv(temp[3], temp[4])[1];
//         input_float[5][i] = papillon_inv(temp[2], temp[5])[1];
//         input_float[6][i] = papillon_inv(temp[1], temp[6])[1];
//         input_float[7][i] = papillon_inv(temp[0], temp[7])[1];
//         // printf("input_float = ");
//         // for (uint8_t j = 0; j<8; j++){
//         //     printf("%f\n ", input_float[j][i]);
//         // }
//         // printf("\n");
//     }


//     // passage en int
//     for (uint8_t i = 0; i<8; i++){
//         for (uint8_t j = 0; j<8; j++){
//             input_float[i][j] = round(8 * input_float[i][j] + 128) < 0 ? 0 : round(8 * input_float[i][j] + 128) > 255 ? 255 : round(8 * input_float[i][j] + 128);
//             input[i][j] = (int16_t) input_float[i][j];
//         }
//     }

//     return EXIT_SUCCESS;
// }

// // int main(){
// //     // // test papillon_inv
// //     // float *output = papillon_inv(1.0, 2.0);
// //     // printf("papillon_inv(1.0, 2.0) = (%f, %f)\n", output[0], output[1]);

// //     // // test rotation_inv
// //     // output = rotation_inv(1.0, 2.0, 1.0, 8);
// //     // printf("rotation_inv(1.0, 2.0, 1.0, 8) = (%f, %f)\n", output[0], output[1]);

// //     // test idct
// //     int** input = (int **) malloc(8 * sizeof(int *));
// //     for (uint8_t i = 0; i<8; i++){
// //         input[i] = (int *) malloc(8 * sizeof(int));
// //     }

// //     for (uint8_t i = 0; i<8; i++){
// //         for (uint8_t j = 0; j<8; j++){
// //             input[i][j] = i;
// //         }
// //     }
// //     // print input
// //     for (uint8_t i = 0; i<8; i++){
// //         for (uint8_t j = 0; j<8; j++){
// //             printf("%d ", input[i][j]);
// //         }
// //         printf("\n");
// //     }

// //     printf("\n*************************\n");

// //     fidct(input);

// //     for (uint8_t i = 0; i<8; i++){
// //         for (uint8_t j = 0; j<8; j++){
// //             printf("%d ", input[i][j]);
// //         }
// //         printf("\n");
// //     }

// //     // print expected output
// //     printf("\n*************************\n");    

// //     return EXIT_SUCCESS;
// // }




