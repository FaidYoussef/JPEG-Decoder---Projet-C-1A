#include <stdio.h>
#include <stdlib.h>

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

int main() {
    // Remplacez ceci par les données de votre image décodée
    int width = 4;
    int height = 4;
    unsigned char image_data[] = {
        255, 0, 0,   0, 255, 0,   0, 0, 255,   255, 255, 0,
        255, 255, 255,   128, 128, 128,   255, 128, 128,   128, 255, 128,
        0, 255, 255,   255, 0, 255,   255, 255, 128,   128, 128, 0,
        128, 0, 128,   0, 128, 0,   128, 128, 255,   0, 128, 255
    };

    const char *filename = "output.ppm";
    write_ppm(filename, image_data, width, height);

    printf("L'image PPM a été écrite dans %s\n", filename);
    return 0;
}
