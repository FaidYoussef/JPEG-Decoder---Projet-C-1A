#include <stdio.h>
#include <stdlib.h>

#include <ppm.h>

void write_ppm(const char *filename, unsigned char *image_data, int width, int height) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", filename);
        exit(1);
    }

    fprintf(file, "P6\n%d %d\n255\n", width, height);
    fwrite(image_data, 1, width * height * 3, file);

    fclose(file);
}
