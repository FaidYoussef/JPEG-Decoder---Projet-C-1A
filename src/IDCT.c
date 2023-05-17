#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include <IDCT.h>

#define PI 3.14159265358979323846
#define N 8
#define NN N*N


double C[N];
double C_cos_values[N][N][N][N];


// Fonction qui calcule les valeurs de C_cos_values et les enregistre dans un fichier
void initialize_C_cos_values() {
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


// Fonction qui vérifie si le fichier C_cos_values.dat existe et le crée si besoin
// Puis on charge les valeurs de C_cos_values en mémoire
int8_t load_C_cos_values() {
    FILE *file = fopen("C_cos_values.dat", "rb");

    // On vérifie si le fichier existe déjà
    if(file  == NULL) {
        // Si le fichier n'existe pas, on le crée
        initialize_C_cos_values();
        // Et on re-vérifie qu'il a bien été créé
        file = fopen("C_cos_values.dat", "rb");
        if(file == NULL) {
            fprintf(stderr, "Impossible d'ouvrir le fichier C_cos_values.dat !\n");
            return EXIT_FAILURE;
        }
    }

    fread(C_cos_values, sizeof(double), N*N*N*N, file);
    fclose(file);

    return EXIT_SUCCESS;
}


// Inverse Discrete Cosine Transform function
int8_t IDCT_function(int16_t **mcu){

    int16_t *output = (int16_t *) malloc(NN * sizeof(int16_t));
    if (check_memory_allocation(output)) return EXIT_FAILURE;

    for (int8_t x = 0; x < N; x++) {
        for (int8_t y = 0; y < N; y++) { 
            double sum = 0.0;

            for (int8_t u = 0; u < N; u++) {
                for (int8_t v = 0; v < N; v++) {
                    int16_t dct_uv = (*mcu)[u * N + v];
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

    free(*mcu);
    *mcu = output;

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

    // On vérifie si le fichier C_cos_values.dat existe déjà et on le recrée si besoin
    // Et on le charge en mémoire
    if (load_C_cos_values()) return EXIT_FAILURE;

    // On parcours toutes les composantes
    for (int8_t i = 0; i < get_sos_nb_components(get_JPEG_sos(jpeg)[0]); i++) {   // attention ici l'index 0 correspond au 1er scan/frame ... prévoir d'intégrer un index pour le mode progressifi
        
        int16_t** MCUs = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), i));

        // On parcours tous les MCUs de l'image
        for (size_t j = 0; j < nb_mcu_width * nb_mcu_height; j++){
            // Prévoir possibilité de reset-er les données `previous_DC_values` dans le cas où l'on a
            // plusieurs scans/frames ---> mode progressif
        
            if (IDCT_function(&(MCUs[j])) ) return EXIT_FAILURE;
            
            getHighlyVerbose() ? fprintf(stderr, "MCU après IDCT\n"):0;
            print_block(MCUs[j], j);
            
        }
    }
    return EXIT_SUCCESS;
}
