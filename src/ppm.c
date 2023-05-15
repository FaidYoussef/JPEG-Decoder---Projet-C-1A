#include <ppm.h>

int8_t write_ppm(const char *filename, struct JPEG *jpeg) {

    int16_t width = get_JPEG_width(jpeg);
    int16_t height = get_JPEG_height(jpeg);
    int16_t* image_data = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), 0))[0];

    FILE *output_file = fopen("YOUSSEF.pgm", "wb");
    if (!output_file) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", filename);
        return EXIT_FAILURE;
    }

    fprintf(output_file, "P5\n%d %d\n255\n", width, height);
    for (int8_t i = 0 ; i < 8 ; i++){
        for (int8_t j = 0 ; j < 8 ; j++){
            fprintf(output_file, "%c", image_data[i * 8 + j]);
        }
    }
    

    fclose(output_file);

    fprintf(stderr, "L'image PPM a été écrite dans `YOUSSEF.pgm`\n");

    return EXIT_SUCCESS;
}
