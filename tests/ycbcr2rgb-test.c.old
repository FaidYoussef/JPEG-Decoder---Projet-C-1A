#include <stdio.h>
#include <stdint.h>

#include <ycbcr2rgb.h>

int main() {
    uint8_t Y, Cb, Cr;
    uint8_t R, G, B;

    printf("Entrez les valeurs de Y, Cb et Cr (0-255) séparées par des espaces : ");
    scanf("%hhu %hhu %hhu", &Y, &Cb, &Cr);

    ycbcr_vers_rgb(Y, Cb, Cr, &R, &G, &B);

    printf("Les valeurs RGB correspondantes sont : R=%d, G=%d, B=%d\n", R, G, B);

    return 0;
}