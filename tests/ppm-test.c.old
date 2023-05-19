#include <stdio.h>
#include <ppm.h>

int main() {
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