#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include <IDCT.h>


double C_cos_values[N][N][N][N];
const float sqrt_2 = 1.41421356237309504880168872420;


// //*********************************************************************************************************************************************************************************************
// // IDCT naive

// // Fonction C() dans la formule de l'IDCT// Décommenter cette ligne et commenter celle en ligne 54 pour utiliser la fonction C() dans la fonction IDCT_function_naive()

// double C(int16_t x) {
//     return (x == 0) ? 1 / sqrt_2 : 1;
// }

// IDCT naive
// int8_t IDCT_function_naive(int16_t **input) {
//     int16_t *output = (int16_t *) malloc(NN * sizeof(int16_t));
//     if (check_memory_allocation(output)) return EXIT_FAILURE;

//     for (int8_t x = 0; x < N; x++) {
//         for (int8_t y = 0; y < N; y++) { 
//             double sum = 0.0;
//             double cu = C[u];
//             for (int8_t v = 0; v < N; v++) {
//                 double cv = C[v];
//                 int dct_uv = input[u * N + v];
//                 sum += cu * cv * dct_uv * cos_values[x][y][u][v];
//             }
//         }
//         output[x * N + y] = round(sum * 0.25);
//     }
    
//     free(*input);
//     *input = output;

//     return EXIT_SUCCESS;
// }



// //*********************************************************************************************************************************************************************************************
// // IDCT légèrement optimisée : on pré-calcule les coefficients de la fonction C() dans la formule
// // Penser à décommenter les lignes 312 à 315 pour utiliser ces fonctions !!!

// double C[N];

// // Fonction qui calcule les valeurs de C_cos_values et les enregistre dans un fichier
// int8_t initialize_C_cos_values() {
//     FILE *file;

//     for (int8_t i = 0; i < N; i++) {
//         C[i] = (i == 0) ? 0.5 / sqrt_2 : 0.5;
//     }
    

//     for (int8_t x = 0; x < N; x++) {
//         for (int8_t y = 0; y < N; y++) {
//             for (int8_t u = 0; u < N; u++) {
//                 for (int8_t v = 0; v < N; v++) {
//                     C_cos_values[x][y][u][v] = C[u] * C[v] * cos(((2 * x + 1) * u * PI) / (2 * N)) * cos(((2 * y + 1) * v * PI) / (2 * N));
//                 }
//             }
//         }
//     }
    
//     if( (file = fopen("C_cos_values.dat", "wb")) == NULL) {
//         fprintf(stderr, RED("ERROR - WRITE - IDCT.c > initialize_C_cos_values()\n"));
//         return EXIT_FAILURE;
//     }

//     fwrite(C_cos_values, sizeof(double), N*N*N*N, file);
//     fclose(file);

//     return EXIT_SUCCESS;
// }


// // Fonction qui vérifie si le fichier C_cos_values.dat existe et le crée si besoin
// // Puis on charge les valeurs de C_cos_values en mémoire
// int8_t load_C_cos_values() {
//     FILE *file;

//     // On vérifie si le fichier existe déjà
//     if((file = fopen("C_cos_values.dat", "rb"))  == NULL) {
//         // Si le fichier n'existe pas, on le crée
//         if (initialize_C_cos_values()) return EXIT_FAILURE;
        
//         // On le lit
//         if((file = fopen("C_cos_values.dat", "rb")) == NULL) {
//             fprintf(stderr, RED("ERROR - READ - IDCT.c > load_C_cos_values()\n"));
//             return EXIT_FAILURE;
//         }
//     }

//     fread(C_cos_values, sizeof(double), N*N*N*N, file);
//     fclose(file);

//     return EXIT_SUCCESS;
// }


// // Inverse Discrete Cosine Transform function
// int8_t IDCT_function_optimized_v1(int16_t **input){

//     int16_t *output = (int16_t *) malloc(NN * sizeof(int16_t));
//     if (check_memory_allocation(output)) return EXIT_FAILURE;

//     for (int8_t x = 0; x < N; x++) {
//         for (int8_t y = 0; y < N; y++) { 
//             double sum = 0.0;

//             for (int8_t u = 0; u < N; u++) {
//                 for (int8_t v = 0; v < N; v++) {
//                     int16_t dct_uv = (*input)[u * N + v];
//                     sum +=  dct_uv * C_cos_values[x][y][u][v];
//                 }
//             }

//             if (round(sum) + 128 < 0) {
//                 output[x * N + y] = 0;
//             } else if (round(sum) + 128 > 255) {
//                 output[x * N + y] = 255;
//             } else {
//                 output[x * N + y] = round(sum) + 128;
//             }
//         }
//     }

//     free(*input);
//     *input = output;

//     return EXIT_SUCCESS;
// }



//*********************************************************************************************************************************************************************************************
// FAST IDCT

// cos_table[i] = cos(i*pi/16)
const float cos_table[9] = {1.0,
			               0.98078528040323044912618223613,
			               0.92387953251128675612818318939,
			               0.83146961230254523707878837761,
			               0.70170678118654752440084436210,
			               0.55557023301960222474283081394,
			               0.38268343236508977172845998403,
			               0.19509032201612826784828486847,
			               0.0};


// Inverse du module papillon de loeffler pour récupérer I0
float papillon_inv_I0(float O0, float O1){
    return (O0 + O1)/2;
}


// Inverse du module papillon de loeffler pour récupérer I1
float papillon_inv_I1(float O0, float O1){
    return (O0 - O1)/2;
}


// Inverse du module rotation de loeffler pour récupérer I0
float rotation_inv_I0(float O0, float O1, float k, uint8_t num_coef){
    return 1.0/k * (O0 *  cos_table[num_coef] - O1 * cos_table[8 - num_coef]);
}


// Inverse du module rotation de loeffler pour récupérer I1
float rotation_inv_I1(float O0, float O1, float k, uint8_t num_coef){
    return 1.0/k * (O1 *  cos_table[num_coef] + O0 * cos_table[8 - num_coef]);
}


// Fast Inverse Discrete Cosine Transform function using Loeffler algorithm
int8_t fast_IDCT_function(int16_t **input){
    
    // passage en float
    float input_float[8][8];
    for (uint8_t i = 0; i<8; i++){
        for (uint8_t j = 0; j<8; j++){
            input_float[i][j] = (float) (*input)[i * 8 + j];
        }
    }

    // On applique l'IDCT_1D sur les lignes
    for (uint8_t i = 0; i<8; i++){

        // vecteur temporaire pour stocker les valeurs intermédiaires
        float temp[8];

        // 1st step
        temp[0] = input_float[i][0];
        temp[1] = input_float[i][4];
        temp[2] = input_float[i][2];
        temp[3] = input_float[i][6];
        temp[4] = papillon_inv_I1(input_float[i][1], input_float[i][7]);
        temp[5] = input_float[i][3] / sqrt_2;
        temp[6] = input_float[i][5] / sqrt_2;
        temp[7] = papillon_inv_I0(input_float[i][1], input_float[i][7]);

        // 2nd step
        input_float[i][0] = papillon_inv_I0(temp[0], temp[1]);
        input_float[i][1] = papillon_inv_I1(temp[0], temp[1]);
        input_float[i][2] = rotation_inv_I0(temp[2], temp[3], sqrt_2, 6);
        input_float[i][3] = rotation_inv_I1(temp[2], temp[3], sqrt_2, 6);
        input_float[i][4] = papillon_inv_I0(temp[4], temp[6]);
        input_float[i][5] = papillon_inv_I1(temp[7], temp[5]);
        input_float[i][6] = papillon_inv_I1(temp[4], temp[6]);
        input_float[i][7] = papillon_inv_I0(temp[7], temp[5]);

        // 3rd step
        temp[0] = papillon_inv_I0(input_float[i][0], input_float[i][3]);
        temp[1] = papillon_inv_I0(input_float[i][1], input_float[i][2]);
        temp[2] = papillon_inv_I1(input_float[i][1], input_float[i][2]);
        temp[3] = papillon_inv_I1(input_float[i][0], input_float[i][3]);
        temp[4] = rotation_inv_I0(input_float[i][4], input_float[i][7], 1.0, 3);
        temp[5] = rotation_inv_I0(input_float[i][5], input_float[i][6], 1.0, 1);
        temp[6] = rotation_inv_I1(input_float[i][5], input_float[i][6], 1.0, 1);
        temp[7] = rotation_inv_I1(input_float[i][4], input_float[i][7], 1.0, 3);

        // 4th step
        input_float[i][0] = papillon_inv_I0(temp[0], temp[7]);
        input_float[i][1] = papillon_inv_I0(temp[1], temp[6]);
        input_float[i][2] = papillon_inv_I0(temp[2], temp[5]);
        input_float[i][3] = papillon_inv_I0(temp[3], temp[4]);
        input_float[i][4] = papillon_inv_I1(temp[3], temp[4]);
        input_float[i][5] = papillon_inv_I1(temp[2], temp[5]);
        input_float[i][6] = papillon_inv_I1(temp[1], temp[6]);
        input_float[i][7] = papillon_inv_I1(temp[0], temp[7]);

    }

    // On applique l'IDCT_1D sur les colonnes
    for (uint8_t i = 0; i<8; i++ ){
            
        // vecteur temporaire pour stocker les valeurs intermédiaires
        float temp[8];

        // 1st step
        temp[0] = input_float[0][i];
        temp[1] = input_float[4][i];
        temp[2] = input_float[2][i];
        temp[3] = input_float[6][i];
        temp[4] = papillon_inv_I1(input_float[1][i], input_float[7][i]);
        temp[5] = input_float[3][i] / sqrt_2;
        temp[6] = input_float[5][i] / sqrt_2;
        temp[7] = papillon_inv_I0(input_float[1][i], input_float[7][i]);

        // 2nd step
        input_float[0][i] = papillon_inv_I0(temp[0], temp[1]);
        input_float[1][i] = papillon_inv_I1(temp[0], temp[1]);
        input_float[2][i] = rotation_inv_I0(temp[2], temp[3], sqrt_2, 6);
        input_float[3][i] = rotation_inv_I1(temp[2], temp[3], sqrt_2, 6);
        input_float[4][i] = papillon_inv_I0(temp[4], temp[6]);
        input_float[5][i] = papillon_inv_I1(temp[7], temp[5]);
        input_float[6][i] = papillon_inv_I1(temp[4], temp[6]);
        input_float[7][i] = papillon_inv_I0(temp[7], temp[5]);

        // 3rd step
        temp[0] = papillon_inv_I0(input_float[0][i], input_float[3][i]);
        temp[1] = papillon_inv_I0(input_float[1][i], input_float[2][i]);
        temp[2] = papillon_inv_I1(input_float[1][i], input_float[2][i]);
        temp[3] = papillon_inv_I1(input_float[0][i], input_float[3][i]);
        temp[4] = rotation_inv_I0(input_float[4][i], input_float[7][i], 1.0, 3);
        temp[5] = rotation_inv_I0(input_float[5][i], input_float[6][i], 1.0, 1);
        temp[6] = rotation_inv_I1(input_float[5][i], input_float[6][i], 1.0, 1);
        temp[7] = rotation_inv_I1(input_float[4][i], input_float[7][i], 1.0, 3);

        // 4th step
        input_float[0][i] = papillon_inv_I0(temp[0], temp[7]);
        input_float[1][i] = papillon_inv_I0(temp[1], temp[6]);
        input_float[2][i] = papillon_inv_I0(temp[2], temp[5]);
        input_float[3][i] = papillon_inv_I0(temp[3], temp[4]);
        input_float[4][i] = papillon_inv_I1(temp[3], temp[4]);
        input_float[5][i] = papillon_inv_I1(temp[2], temp[5]);
        input_float[6][i] = papillon_inv_I1(temp[1], temp[6]);
        input_float[7][i] = papillon_inv_I1(temp[0], temp[7]);

    }


    // passage en int
    for (uint8_t i = 0; i<8; i++){
        for (uint8_t j = 0; j<8; j++){
            input_float[i][j] = round(8 * input_float[i][j] + 128) < 0 ? 0 : round(8 * input_float[i][j] + 128) > 255 ? 255 : round(8 * input_float[i][j] + 128);
            (*input)[i * 8 + j] = (int16_t) input_float[i][j];
        }
    }

    return EXIT_SUCCESS;
}


// Fonction qui récupère les données de la structure JPEG et qui procède à l'IDCT inverse
int8_t IDCT(struct JPEG * jpeg) {

    // On parcours tous les MCUs de l'image
    for (size_t y = 0; y < get_JPEG_nb_Mcu_Height(jpeg); y += get_JPEG_Sampling_Factor_Y(jpeg)) {
        for (size_t x = 0; x < get_JPEG_nb_Mcu_Width(jpeg); x += get_JPEG_Sampling_Factor_X(jpeg)) {
            for (int8_t i = 0; i < get_sos_nb_components(get_JPEG_sos(jpeg)[0]); i++) {   // attention ici l'index 0 correspond au 1er scan/frame ... prévoir d'intégrer un index pour le mode progressif
                
                // On récupère les MCUs de la composante
                int16_t** MCUs = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), i));
                
                for (int8_t v = 0; v < get_sampling_factor_y(get_sof_component(get_sof_components(get_JPEG_sof(jpeg)[0]), i)); v++) {
                    for (int8_t h = 0; h < get_sampling_factor_x(get_sof_component(get_sof_components(get_JPEG_sof(jpeg)[0]), i)); h++) {
                        // On récupère le MCU
                        int16_t *mcu = MCUs[(y + v) * get_JPEG_nb_Mcu_Width_Strechted(jpeg) + (x + h)];

                        if (fast_IDCT_function(&mcu )) return EXIT_FAILURE;
                        
                        getHighlyVerbose() ? fprintf(stderr, "MCU après IDCT\n"):0;
                        print_block(mcu, v * get_JPEG_Sampling_Factor_X(jpeg) + h, i);                    }
                }
            }
        }
    }
    return EXIT_SUCCESS;
}
