#include <ppm.h>

int8_t write_ppm(const char *filename, struct JPEG *jpeg) {

    int8_t nb_components = get_sof_nb_components(get_JPEG_sof(jpeg)[0]);
    int16_t width = get_JPEG_width(jpeg);
    int16_t height = get_JPEG_height(jpeg);

    // On prépare le fichier de sortie
    FILE *output_file;
    if (nb_components == 1) {
        // On vérifie que le fichier a bien été créé/ouvert
        output_file = fopen("YOUSSEF.pgm", "wb");
        if (!output_file) {
            fprintf(stderr, RED("Erreur lors de l'ouverture du fichier %s\n"), filename);
            return EXIT_FAILURE;
        }

        // On écrit l'en-tête du fichier PGM
        fprintf(output_file, "P5\n%d %d\n255\n", width, height);


    } else if (nb_components == 3) {
        // On vérifie que le fichier a bien été créé/ouvert
        output_file = fopen("YOUSSEF.ppm", "wb");
        if (!output_file) {
            fprintf(stderr, RED("Erreur lors de l'ouverture du fichier %s\n"), filename);
            return EXIT_FAILURE;
        }

        // On écrit l'en-tête du fichier PPM
        fprintf(output_file, "P6\n%d %d\n255\n", width, height);
    }
    

    // On récupère les pointeurs vers les MCUs des composantes disponibles
    int16_t** MCUs_component0 = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), COMPONENT_0_INDEX));
    int16_t** MCUs_component1 = NULL;
    int16_t** MCUs_component2 = NULL;

    if (nb_components > 1) {
        MCUs_component1 = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), COMPONENT_1_INDEX));
        MCUs_component2 = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), COMPONENT_2_INDEX));
    }


    // On récupère le nombre de mcus en largeur et hauteur
    size_t nb_mcu_width = (width + 7) / 8;
    size_t nb_mcu_height = (height + 7) / 8;
    



    for (size_t i = 0; i < nb_mcu_height ; i++){
        for (int8_t l = 0; l < 8; l++) {
            for (size_t j = 0; j < nb_mcu_width ; j++){
                for (int8_t k = 0; k < 8; k++){
                    if (j * 8 + k < (size_t) width && i * 8 + l < (size_t)height) {
                        fprintf(output_file, "%c", MCUs_component0[j + i * nb_mcu_width][k+l*8]);
                        if (nb_components > 1) {
                            fprintf(output_file, "%c", MCUs_component1[j + i * nb_mcu_width][k+l*8]);
                            fprintf(output_file, "%c", MCUs_component2[j + i * nb_mcu_width][k+l*8]);
                        }
                    }
                    
                }
            }
        }
    }

    fclose(output_file);
    return EXIT_SUCCESS;

}