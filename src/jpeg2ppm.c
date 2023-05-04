#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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


short two_bytes_to_dec(FILE *input){
    // Lecture et renvoie de la valeur décimale de deux octets
    short length = 0;
    short length2 = 0;
    fread(&length, 1, 1, input);
    length = length << 8;
    fread(&length2, 1, 1, input);
    length = length + length2;
    return length;
}

void ignore_bytes(FILE *input, int nb_bytes){
    unsigned char buffer[nb_bytes];
    fread(buffer, nb_bytes, 1, input);
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
                length = two_bytes_to_dec(input);
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

                    free(quantization_table_luminance);

                } else if (buffer[0] == CHROMINANCE_ID) {
                    unsigned char *quantization_table_chrominance = malloc(length*sizeof(unsigned char));
                    fread(quantization_table_chrominance, length, 1, input);
                    // Affichage des tables de quantification
                    for (int i=0; i<64; i++){
                        printf("%x", quantization_table_chrominance[i]);
                    }
                    printf("\n");

                    free(quantization_table_chrominance);

                } else {
                    fprintf(stderr, "Erreur dans la lecture des tables de quantification\n");
                    return EXIT_FAILURE;
                }

            } else if (id[0] == SOF_0){
                printf("Start of frame\n");
                ignore_bytes(input, 3); // On ignore la longueur et la précision

                short height = two_bytes_to_dec(input);
                short width = two_bytes_to_dec(input);
                printf("Hauteur : %d\n", height);
                printf("Largeur : %d\n", width);

                fread(buffer, 1, 1, input); // Nombre de composantes
                short nb_components = buffer[0];
                printf("Nombre de composantes : %d\n", nb_components);

                printf("Composantes :\n");
                for (int i=0; i<nb_components; i++){
                    fread(buffer, 1, 1, input); // ID composante
                    unsigned char id_component = buffer[0];
                    
                    fread(buffer, 1, 1, input); // Facteur d'échantillonnage
                    unsigned char sampling_factor = buffer[0]; // Il faut faire une conversion car c'est un octet et on veut deux bits

                    unsigned char sampling_factor_x = sampling_factor >> 4;
                    unsigned char sampling_factor_y = sampling_factor << 4;
                    sampling_factor_y = sampling_factor_y >> 4;

                    fread(buffer, 1, 1, input); // Tables de quantification
                    short num_quantization_table = buffer[0];
                    printf("ID composante : %d\n", id_component);
                    printf("Facteur d'échantillonnage X : %d\n", sampling_factor_x);
                    printf("Facteur d'échantillonnage Y : %d\n", sampling_factor_y);
                    printf("Numéro de la table de quantification : %d\n", num_quantization_table);
                }

            } else if (id[0] == DHT){
                printf("Huffman table\n");
                short length = two_bytes_to_dec(input); // Longueur du segment
                length = length - 2 - 1; // On enlève la longueur du segment et l'octet de précision

                fread(buffer, 1, 1, input); // ID de la table
                // Les 4 bits de poids fort indiquent le type de table (DC ou AC)
                // Les 4 bits de poids faible indiquent le numéro de la table
                unsigned char id_table = buffer[0];
                unsigned char type_table = id_table >> 4;
                unsigned char num_table = id_table << 4;
                num_table = num_table >> 4;
                printf("Type de table : %d\n", type_table);
                printf("Numéro de table : %d\n", num_table);

                // Contenu de la table
                unsigned char *huffman = malloc(length*sizeof(unsigned char));
                fread(huffman, length, 1, input);
                // Affichage des tables de Huffman
                for (int i=0; i<length; i++){
                    printf("%x", huffman[i]);
                }
                printf("\n");

                free(huffman);

            } else if (id[0] == SOS){
                printf("Start of scan + data\n");
                ignore_bytes(input, 2); // Longueur du segment (ignoré)

                unsigned char nb_components = 0;
                fread(buffer, 1, 1, input); // Nombre de composantes
                nb_components = buffer[0];
                printf("Nombre de composantes : %d\n", nb_components);

                // Composantes
                for (int i=0; i<nb_components; i++){
                    fread(buffer, 1, 1, input); // ID composante
                    unsigned char id_component = buffer[0];
                    
                    fread(buffer, 1, 1, input); // Tables de Huffman
                    unsigned char id_table = buffer[0];
                    unsigned char type_table = id_table >> 4;
                    unsigned char num_table = id_table << 4;
                    num_table = num_table >> 4;
                    printf("ID composante : %d\n", id_component);
                    printf("Type de table : %d\n", type_table);
                    printf("Numéro de table : %d\n", num_table);
                }

                // Paramètres ignorés
                ignore_bytes(input, 3); // Octet de début de spectre, octet de fin de spectre, approximation (ignorés)

                // On essaie une nouvelle méthode pour lire la data, pour enlever les 0
                unsigned char *data = malloc(sizeof(unsigned char)); // Le malloc n'est pas bon car on ne connait pas la taille de la data
                int nb_data = 0;
                while (1){
                    fread(buffer, 1, 1, input);
                    if (buffer[0] == 0xFF){
                        fread(buffer, 1, 1, input);
                        if (buffer[0] == 0x00){
                            data[nb_data] = 0xFF;
                            nb_data++;
                        } else if (buffer[0] == EOI){
                            // On a fini la lecture des données
                            free(data);
                            printf("Fin du fichier\n");
                            return EXIT_SUCCESS;
                        } else {
                            data[nb_data] = 0xFF;
                            nb_data++;
                            data[nb_data] = buffer[0];
                            nb_data++;
                        }
                    } else {
                        data[nb_data] = buffer[0];
                        nb_data++;
                    }

                    unsigned char *tmp = realloc(data, (nb_data+1)*sizeof(unsigned char));
                    if (tmp == NULL)
                    {
                        fprintf(stderr, "Erreur de réallocation mémoire\n");
                        free(data);
                        return EXIT_FAILURE;
                    }               
                    data = tmp;
                }

                // On n'arrive jamais ici (impossible de break correctement)

                // free(data);

            } else if (id[0] == EOI){
                printf("Fin du fichier\n");
                break;
            }
        }
    }
    return EXIT_SUCCESS;
}