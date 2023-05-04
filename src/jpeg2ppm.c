#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


#define THREE_BYTES_LONG 3

#define SEGMENT_START 0xff  // Segment start marker
#define SOI 0xd8            // Start of Image
#define SOF_0 0xc0          // Baseline DCT
#define SOF_1 0xc1          // Extended sequential DCT
#define SOF_2 0xc2          // Progressive DCT
#define SOF_3 0xc3          // Lossless (sequential)
#define DHT 0xc4            // Define Huffman Table(s)
#define DQT 0xdb            // Define Quantization Table(s)
#define SOS 0xda            // Start of Scan
#define EOI 0xd9            // End of Image

#define LUMINANCE_ID 0x00
#define CHROMINANCE_ID 0x01


short size_seg(FILE *input){
    short length = 0;
    short length2 = 0;
    fread(&length, 1, 1, input);
    length = length << 8;
    fread(&length2, 1, 1, input);
    length = length + length2;
    return length;
}

int ipow(int b, int e)
{
  if (e==0) {
    return 1;
  } else if (b==0) {
    return 0;
  } else if (e%2==0) {
    int tmp=ipow(b,e/2);
    return tmp*tmp;
  } else {
    int tmp=ipow(b,(e-1)/2);
    return b*tmp*tmp;
  }
}

int main(int argc, char **argv) {
    if (argc != 2) {
    	/* 
            Si y'a pas au moins un argument en ligne de commandes, on
    	    boude. 
        */
    	fprintf(stderr, "Usage: %s fichier.jpeg\n", argv[0]);
    	return EXIT_FAILURE;
    }

    // Ouverture du fichier et vérification de la conformité du type
    FILE *input;
    if( (input = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    
    // Vérification JPEG Magic number 
    unsigned char first3bytes[THREE_BYTES_LONG];
    fread(first3bytes, sizeof(first3bytes), 1, input);
    unsigned char JPEG_magic_Number[THREE_BYTES_LONG] = {SEGMENT_START, SOI, SEGMENT_START};

    for (int i=0; i<3; i++){
        if (first3bytes[i] != JPEG_magic_Number[i]){
            fprintf(stderr, "Le fichier %s n'est pas un fichier JPEG\n", argv[1]);
            return EXIT_FAILURE;
        }
    }

    // Récupération données en-tête
    unsigned char buffer[1];    // Buffer
    unsigned char id[1];        // Buffer de lecture pour déterminer le type de segments

    while (1){
        fread(buffer, 1, 1, input);
        if (buffer[0] == SEGMENT_START){
            fread(id, 1, 1, input);
            if (id[0] == DQT){
                printf("Quantization table\n");
                short length = 0;
                length = size_seg(input);
                length = length - 2 - 1;

                fread(buffer, 1, 1, input);
                if (buffer[0] == LUMINANCE_ID) {
                    unsigned char *quantization_table_luminance = malloc(length*sizeof(unsigned char));
                    fread(quantization_table_luminance, length, 1, input);
                    // Affichage des tables de quantification
                    for (int i=0; i<64; i++){
                        printf("%x", quantization_table_luminance[i]);
                    }
                    printf("\n");
                } else if (buffer[0] == CHROMINANCE_ID) {
                    unsigned char *quantization_table_chrominance = malloc(length*sizeof(unsigned char));
                    fread(quantization_table_chrominance, length, 1, input);
                    // Affichage des tables de quantification
                    for (int i=0; i<64; i++){
                        printf("%x", quantization_table_chrominance[i]);
                    }
                    printf("\n");
                } else {
                    fprintf(stderr, "Erreur dans la lecture des tables de quantification\n");
                    return EXIT_FAILURE;
                }

            } else if (id[0] == SOF_0){
                printf("Start of frame\n");
                short length = 0;
                length = size_seg(input);
                fread(buffer, 1, 1, input); // Précision
                short height = 0;
                short width = 0;
                height = size_seg(input); // Size seg renvoie la valeur numérique issue de la lecture des 2 octets
                width = size_seg(input); // Il faudrait modifier le nom de la fonction pour plus de clarté
                printf("Hauteur : %d\n", height);
                printf("Largeur : %d\n", width);
                fread(buffer, 1, 1, input); // Nombre de composantes
                short nb_components = 0;
                nb_components = buffer[0];
                printf("Nombre de composantes : %d\n", nb_components);
                printf("Composantes :\n");
                for (int i=0; i<nb_components; i++){
                    fread(buffer, 1, 1, input); // ID composante
                    short id_component = 0;
                    id_component = buffer[0];
                    fread(buffer, 1, 1, input); // Facteur d'échantillonnage
                    short sampling_factor = 0;
                    sampling_factor = buffer[0]; // Il faut faire une conversion car c'est un octet et on veut deux bits

                    // On convertit en binaire
                    char *sampling_factor_bin = malloc(8*sizeof(char));
                    for (int i=0; i<8; i++){
                        sampling_factor_bin[i] = '0';
                    }
                    int j = 7;
                    while (sampling_factor != 0){
                        if (sampling_factor % 2 == 1){
                            sampling_factor_bin[j] = '1';
                        }
                        sampling_factor = sampling_factor / 2;
                        j--;
                    }
                    // On convertit en décimal par paquets de 4 bits
                    int sampling_factor_x = 0;
                    int sampling_factor_y = 0;
                    for (int i=0; i<4; i++){
                        if (sampling_factor_bin[i] == '1'){
                            sampling_factor_x = sampling_factor_x + ipow(2, 3-i);
                        }
                    }
                    for (int i=4; i<8; i++){
                        if (sampling_factor_bin[i] == '1'){
                            sampling_factor_y = sampling_factor_y + ipow(2, 7-i);
                        }
                    }

                    fread(buffer, 1, 1, input); // Tables de quantification
                    short quantization_table = 0;
                    quantization_table = buffer[0];
                    printf("ID composante : %d\n", id_component);
                    printf("Facteur d'échantillonnage X : %d\n", sampling_factor_x);
                    printf("Facteur d'échantillonnage Y : %d\n", sampling_factor_y);
                }

            } else if (id[0] == DHT){
                printf("Huffman table\n");

            } else if (id[0] == SOS){
                printf("Start of scan + data\n");

            } else if (id[0] == EOI){
                printf("Fin du fichier\n");
                break;
            }
        }
    }

    

    return EXIT_SUCCESS;
}
