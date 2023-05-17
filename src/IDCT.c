#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include <IDCT.h>


double C[N];
double C_cos_values[N][N][N][N];

void initialize() {
    FILE *file;

    for (int i = 0; i < N; i++) {
        C[i] = (i == 0) ? 0.5 / sqrt(2) : 0.5;
    }
    

    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            for (int u = 0; u < N; u++) {
                for (int v = 0; v < N; v++) {
                    C_cos_values[x][y][u][v] = C[u] * C[v] * cos(((2 * x + 1) * u * PI) / (2 * N)) * cos(((2 * y + 1) * v * PI) / (2 * N));
                }
            }
        }
    }
    file = fopen("C_cos_values.dat", "wb");
    fwrite(C_cos_values, sizeof(double), N*N*N*N, file);
    fclose(file);
}


int8_t load() {
    FILE *file;

    // On vérifie si le fichier existe déjà
    if( (file = fopen("C_cos_values.dat", "rb")) == NULL) {
        // Si le fichier n'existe pas, on le crée
        initialize();
        // Et on re-vérifie qu'il a bien été créé
        if( (file = fopen("C_cos_values.dat", "rb")) == NULL) {
            fprintf(stderr, "Impossible d'ouvrir le fichier C_cos_values.dat !\n", );
            return EXIT_FAILURE;
        }
    }

    fread(C_cos_values, sizeof(double), N*N*N*N, file);
    fclose(file);

    return EXIT_SUCCESS;
}


// Inverse Discrete Cosine Transform function
int8_t IDCT_function(struct JPEG *jpeg, size_t MCU_number, int8_t component_index){

    int16_t** MCUs = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), component_index));

    
    if (load) return EXIT_FAILURE;

    int16_t *output = (int16_t *) malloc(NN * sizeof(int16_t));
    if (check_memory_allocation(output)) return EXIT_FAILURE;

    for (int8_t x = 0; x < N; x++) {
        for (int8_t y = 0; y < N; y++) { 
            double sum = 0.0;

            for (int8_t u = 0; u < N; u++) {
                for (int8_t v = 0; v < N; v++) {
                    int16_t dct_uv = MCUs[MCU_number][u * N + v];
                    sum +=  dct_uv * C_cos_values[x][y][u][v];
                }
            }

            if (round(sum) + 128 < 0) {
                output[x * N + y] = 0;
            } else if (round(sum) + 128 > 255) {
                output[x * N + y] = 255;
            } else {
                output[x * N + y] = round(sum) + 128;
            }
        }
    }
    free(MCUs[MCU_number]);
    MCUs[MCU_number] = output;
    getHighlyVerbose() ? fprintf(stderr, "MCU après IDCT\n"):0;
    print_block(MCUs[MCU_number], MCU_number);

    return EXIT_SUCCESS;
}


int8_t IDCT(struct JPEG * jpeg) {
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
        
        // On parcours toutes les composantes
        for (int8_t j = 0; j < get_sos_nb_components(get_JPEG_sos(jpeg)[0]); ++j) {   // attention ici l'index 0 correspond au 1er scan/frame ... prévoir d'intégrer un index pour le mode progressif
            if (IDCT_function(jpeg, i, j) ) return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
