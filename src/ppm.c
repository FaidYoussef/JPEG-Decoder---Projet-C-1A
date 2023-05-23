#include <ppm.h>



// Fonction qui génère le nom du fichier de sortie
char* generate_output_filename(const char *input_filename, uint8_t nb_components) {
    char *output_filename = malloc(500*sizeof(char)); // Si quelqu'un veut vraiment abuser ...
    char *base_name = basename((char *) input_filename);
    char *dot = strrchr(base_name, '.');
    char *dir_name = dirname((char *) input_filename);

    // copy the directory name to output_filename
    strcpy(output_filename, dir_name);
    strcat(output_filename, "/");

    if (dot) {
        // copy the base name up to the dot
        strncat(output_filename, base_name, dot - base_name);
    } else {
        // if no extension, just copy the whole base name
        strcat(output_filename, base_name);
    }

    // add the new extension
    strcat(output_filename, ".");

    strcat(output_filename, "p");

    if(nb_components == 1) {
        strcat(output_filename, "g");
    } else {
        strcat(output_filename, "p");
    }

    strcat(output_filename, "m");

    return output_filename;
}


int8_t write_ppm(const char *input_filename, struct JPEG *jpeg, bool force_grayscale) {

    int8_t nb_components = get_sof_nb_components(get_JPEG_sof(jpeg)[0]);
    if (force_grayscale) nb_components = 1;

    int16_t width = get_JPEG_width(jpeg);
    int16_t height = get_JPEG_height(jpeg);


    // On prépare le fichier de sortie
    FILE *output_file;
    char* output_filename = generate_output_filename(input_filename, nb_components);


    if (nb_components == 1) {
        // On vérifie que le fichier a bien été créé/ouvert
        output_file = fopen(output_filename, "wb");
        if (!output_file) {
            fprintf(stderr, RED("ERROR : OPEN - ppm.c > write_ppm() %s\n"), output_filename);
            return EXIT_FAILURE;
        }

        // On écrit l'en-tête du fichier PGM
        fprintf(output_file, "P5\n%d %d\n255\n", width, height);


    } else if (nb_components == 3) {
        // On vérifie que le fichier a bien été créé/ouvert
        output_file = fopen(output_filename, "wb");
        if (!output_file) {
            fprintf(stderr, RED("ERROR : OPEN - ppm.c > write_ppm() %s\n"), output_filename);
            return EXIT_FAILURE;
        }

        // On écrit l'en-tête du fichier PPM
        fprintf(output_file, "P6\n%d %d\n255\n", width, height);
        
    } else {
        output_file = NULL;     // On ne se retrouvera jamais ici puisque nb_components est forcément égal à 1 ou 3 après vérification 
        return EXIT_FAILURE;    // dans extract.c > extract()   ........... juste pour virer le warning à la compilation
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
    size_t nb_mcu_width = get_JPEG_nb_Mcu_Width_Strechted(jpeg);
    size_t nb_mcu_height = get_JPEG_nb_Mcu_Height_Strechted(jpeg);
    
    for (size_t i = 0; i < nb_mcu_height ; i++){
        for (int8_t l = 0; l < 8; l++) {
            for (size_t j = 0; j < nb_mcu_width ; j++){
                for (int8_t k = 0; k < 8; k++){
                    if (j * 8 + k < (size_t) width && i * 8 + l < (size_t)height) {
                        fputc(MCUs_component0[j + i * nb_mcu_width][k+l*8], output_file);
                        if (nb_components > 1) {
                            fputc(MCUs_component1[j + i * nb_mcu_width][k+l*8], output_file);
                            fputc(MCUs_component2[j + i * nb_mcu_width][k+l*8], output_file);
                        }
                    }
                    
                }
            }
        }
    }

    fclose(output_file);
    free(output_filename);
    return EXIT_SUCCESS;

}