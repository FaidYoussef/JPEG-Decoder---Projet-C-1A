#include <ppm.h>

int8_t write_ppm(const char *filename, struct JPEG *jpeg) {

    int8_t nb_components = get_sof_nb_components(get_JPEG_sof(jpeg)[0]);
    int16_t width = get_JPEG_width(jpeg);
    int16_t height = get_JPEG_height(jpeg);

    FILE *output_file = fopen("YOUSSEF.pgm", "wb");
    if (!output_file) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", filename);
        return EXIT_FAILURE;
    }

    fprintf(output_file, "P5\n%d %d\n255\n", width, height);
    if (nb_components == 1) {
        
        int16_t* image_data = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), 0))[0];

        for (int8_t i = 0 ; i < 8 ; i++){
            for (int8_t j = 0 ; j < 8 ; j++){
                fprintf(output_file, "%c", image_data[i * 8 + j]);
            }
        }
        

        fclose(output_file);

        fprintf(stderr, "L'image PPM a été écrite dans `YOUSSEF.pgm`\n");

    }

    if (nb_components == 3){
        
        int16_t* image_data = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), 0))[0];
        int16_t* image_data2 = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), 1))[0];
        int16_t* image_data3 = get_MCUs(get_sos_component(get_sos_components(get_JPEG_sos(jpeg)[0]), 2))[0];

        FILE *output_file = fopen("YOUSSEF.ppm", "wb");
        if (!output_file) {
            fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", filename);
            return EXIT_FAILURE;
        }

        fprintf(output_file, "P6\n%d %d\n255\n", width, height);

        size_t nb_mcu_width = 0;
        size_t nb_mcu_height = 0;
        size_t width_reste = get_JPEG_width(jpeg) % 8;
        size_t height_reste = get_JPEG_height(jpeg) % 8;

        if (width_reste == 0) {

            nb_mcu_width =  get_JPEG_width(jpeg) / 8;

        } else {

            nb_mcu_width = (get_JPEG_width(jpeg) / 8) + 1;

        }
        if (height_reste == 0) {

            nb_mcu_height =  get_JPEG_height(jpeg) / 8;

        } else {

            nb_mcu_height = (get_JPEG_height(jpeg) / 8) + 1;
        }
        
        for (int8_t height_mcu_index = 0; height_mcu_index < nb_mcu_height ; height_mcu_index++){
        
            for (int8_t width_mcu_index =0; width_mcu_index < nb_mcu_width; width_mcu_index++){

                for (int8_t pixel_index = 0; pixel_index < 8; pixel_index++){
                    fprintf(output_file, "%c", image_data[width_mcu_index * 8 + pixel_index]);
                    fprintf(output_file, "%c", image_data2[width_mcu_index * 8 + pixel_index]);
                    fprintf(output_file, "%c", image_data3[width_mcu_index * 8 + pixel_index]);
                }

            }
                for (int8_t pixel_index = 0; pixel_index < width_reste; pixel_index++){
                    fprintf(output_file, "%c", image_data[(nb_mcu_width - 1)  * 8 + pixel_index]);
                    fprintf(output_file, "%c", image_data2[(nb_mcu_width - 1) * 8 + pixel_index]);
                    fprintf(output_file, "%c", image_data3[(nb_mcu_width - 1) * 8 + pixel_index]);
                }          
            }
        

        for (int8_t pixel_index = 0; pixel_index < height_reste; pixel_index++){
            for (int8_t width_mcu_index =0; width_mcu_index < nb_mcu_width; width_mcu_index++){
                fprintf(output_file, "%c", image_data[width_mcu_index * 8 + pixel_index]);
                fprintf(output_file, "%c", image_data2[width_mcu_index * 8 + pixel_index]);
                fprintf(output_file, "%c", image_data3[width_mcu_index * 8 + pixel_index]);
            }

            for (int8_t pixel_index = 0; pixel_index < width_reste; pixel_index++){
                fprintf(output_file, "%c", image_data[(nb_mcu_width - 1)  * 8 + pixel_index]);
                fprintf(output_file, "%c", image_data2[(nb_mcu_width - 1) * 8 + pixel_index]);
                fprintf(output_file, "%c", image_data3[(nb_mcu_width - 1) * 8 + pixel_index]);
            }
        }
    }

    fclose(output_file);
    return EXIT_SUCCESS;

}