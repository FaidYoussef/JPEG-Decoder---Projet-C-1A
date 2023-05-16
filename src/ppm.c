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
            fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", filename);
            return EXIT_FAILURE;
        }

        // On écrit l'en-tête du fichier PGM
        fprintf(output_file, "P5\n%d %d\n255\n", width, height);


    } else if (nb_components == 3) {
        // On vérifie que le fichier a bien été créé/ouvert
        output_file = fopen("YOUSSEF.ppm", "wb");
        if (!output_file) {
            fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", filename);
            return EXIT_FAILURE;
        }

        // On écrit l'en-tête du fichier PPM
        fprintf(output_file, "P6\n%d %d\n255\n", width, height);
    }
    

    // On récupère les pointeurs vers les MCUs des composantes disponibles
    int16_t** MCUs_component0 = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), COMPONENT_0_INDEX));
    int16_t** MCUs_component1;
    int16_t** MCUs_component2;

    if (nb_components > 1) {
        MCUs_component1 = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), COMPONENT_1_INDEX));
        MCUs_component2 = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), COMPONENT_2_INDEX));
    }


    // On récupère le nombre de mcus en largeur et hauteur
    size_t nb_mcu_width = 0;
    size_t nb_mcu_height = 0;
    if (width % 8 == 0) {
        nb_mcu_width =  width / 8;
    } else {
        nb_mcu_width = (width / 8) + 1;
    }
    if (height % 8 == 0) {
        nb_mcu_height =  height / 8;
    } else {
        nb_mcu_height = (height / 8) + 1;
    }
    fprintf(stderr, "nb_mcu_width = %ld\n", nb_mcu_width);
    fprintf(stderr, "nb_mcu_height = %ld\n", nb_mcu_height);


    // On écrit les valeurs dans le fichier de sortie
    int16_t cpt_x = 0;
    int16_t cpt_y = 0;
    fprintf(stderr, "cpt_x = %d\n", cpt_x);
    fprintf(stderr, "cpt_y = %d\n\n", cpt_y);

    for (size_t i = 0; i < nb_mcu_height ; i++){
        for (int8_t l = 0; l < 8; l++) {
            for (size_t j = 0; j < nb_mcu_width ; j++){
                for (int8_t k = 0; k < 8; k++){
                    fprintf(output_file, "%c", MCUs_component0[j + i * 2][k+l*8]);
                    fprintf(stderr, "cpt_x = %d\n", cpt_x);
                    cpt_x++;
                }
                if (cpt_x == width){
                    cpt_x = 0;
                    break;
                }
            }
            cpt_y++;
            fprintf(stderr, "cpt_y = %d\n", cpt_y);
        }
        if (cpt_y == height){
            cpt_y = 0;
            fprintf(output_file, "\n");
            break;
        }
        
    }

    fclose(output_file);
    return EXIT_SUCCESS;

}