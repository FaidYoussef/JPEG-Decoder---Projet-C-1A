#include <extract.h>

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

unsigned char read_byte(FILE *input, unsigned char *buffer){
    // Lecture et renvoie d'un octet
    fread(buffer, 1, 1, input);
    return buffer[0];
}

void ignore_bytes(FILE *input, int nb_bytes){
    unsigned char buffer[nb_bytes];
    fread(buffer, nb_bytes, 1, input);
}

void get_qt(FILE *input, unsigned char *buffer, unsigned char *qt_luminance, unsigned char *qt_chrominance) {
    // On souhaite récupérer les tables de quantification
    getVerbose() ? printf("Quantization table\n"):0;
    short length = 0;
    length = two_bytes_to_dec(input);
    length = length - 2 - 1;

    fread(buffer, 1, 1, input);
    if (buffer[0] == LUMINANCE_ID) {
        fread(qt_luminance, length, 1, input);
        // Affichage des tables de quantification
        for (int i=0; i<length; i++){
            getVerbose() ? printf("%x", qt_luminance[i]):0;
        }
        getVerbose() ? printf("\n"):0;

        free(qt_luminance);

    } else if (buffer[0] == CHROMINANCE_ID) {
        fread(qt_chrominance, length, 1, input);
        // Affichage des tables de quantification
        for (int i=0; i<length; i++){
            getVerbose() ? printf("%x", qt_chrominance[i]):0;
        }
        getVerbose() ? printf("\n"):0;

        free(qt_chrominance);

    } else {
        fprintf(stderr, "Erreur dans la lecture des tables de quantification\n");
    }
}

bool extract(char *filename) { 
    // Ouverture du fichier et vérification de la conformité du type
    FILE *input;
    if( (input = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", filename);
        return false;
    }

    // Vérification JPEG Magic number 
    unsigned char first3bytes[THREE_BYTES_LONG];
    fread(first3bytes, sizeof(first3bytes), 1, input);
    unsigned char JPEG_magic_Number[THREE_BYTES_LONG] = {SEGMENT_START, SOI, SEGMENT_START};

    for (int i=0; i<3; i++){
        if (first3bytes[i] != JPEG_magic_Number[i]){
            fprintf(stderr, "Le fichier %s n'est pas un fichier JPEG\n", filename);
            return false;
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
                unsigned char *quantization_table_luminance = malloc(64*sizeof(unsigned char));
                unsigned char *quantization_table_chrominance = malloc(64*sizeof(unsigned char));

                get_qt(input, buffer, quantization_table_luminance, quantization_table_chrominance);

            } else if (id[0] == SOF_0){
                getVerbose() ? printf("Start of frame\n"):0;
                ignore_bytes(input, 3); // On ignore la longueur et la précision

                short height = two_bytes_to_dec(input);
                short width = two_bytes_to_dec(input);
                getVerbose() ? printf("Hauteur : %d\n", height):0;
                getVerbose() ? printf("Largeur : %d\n", width):0;

                unsigned char nb_components = read_byte(input, buffer);
                getVerbose() ? printf("Nombre de composantes : %d\n", nb_components):0;

                getVerbose() ? printf("Composantes :\n"):0;
                for (int i=0; i<nb_components; i++){
                    unsigned char id_component = read_byte(input, buffer); // ID composante
                    
                    // Facteur d'échantillonnage
                    unsigned char sampling_factor = read_byte(input, buffer); // Il faut faire une conversion car c'est un octet et on veut deux bits

                    unsigned char sampling_factor_x = sampling_factor >> 4;
                    unsigned char sampling_factor_y = sampling_factor << 4;
                    sampling_factor_y = sampling_factor_y >> 4;

                    unsigned char num_quantization_table = read_byte(input, buffer); // Tables de quantification
                    getVerbose() ? printf("ID composante : %d\n", id_component):0;
                    getVerbose() ? printf("Facteur d'échantillonnage X : %d\n", sampling_factor_x):0;
                    getVerbose() ? printf("Facteur d'échantillonnage Y : %d\n", sampling_factor_y):0;
                    getVerbose() ? printf("Numéro de la table de quantification : %d\n", num_quantization_table):0;
                }

            } else if (id[0] == DHT){
                getVerbose() ? printf("Huffman table\n"):0;
                short length = two_bytes_to_dec(input); // Longueur du segment
                length = length - 2 - 1; // On enlève la longueur du segment et l'octet de précision 
                
                unsigned char id_table = read_byte(input, buffer); // ID de la table
                // Les 4 bits de poids fort indiquent le type de table (DC ou AC)
                // Les 4 bits de poids faible indiquent le numéro de la table
                unsigned char type_table = id_table >> 4;
                unsigned char num_table = id_table << 4;
                num_table = num_table >> 4;
                getVerbose() ? printf("Type de table : %d\n", type_table):0;
                getVerbose() ? printf("Numéro de table : %d\n", num_table):0;

                // Contenu de la table
                unsigned char *huffman = malloc(length*sizeof(unsigned char));
                fread(huffman, length, 1, input);
                // Affichage des tables de Huffman
                for (int i=0; i<length; i++){
                    getVerbose() ? printf("%x", huffman[i]):0;
                }
                getVerbose() ? printf("\n"):0;

                free(huffman);

            } else if (id[0] == SOS){
                getVerbose() ? printf("Start of scan + data\n"):0;
                ignore_bytes(input, 2); // Longueur du segment (ignoré)

                unsigned char nb_components = read_byte(input, buffer); // Nombre de composantes
                getVerbose() ? printf("Nombre de composantes : %d\n", nb_components):0;

                // Composantes
                for (int i=0; i<nb_components; i++){
                    unsigned char id_component = read_byte(input, buffer); // ID composante
                    
                    unsigned char id_table = read_byte(input, buffer); // ID de la table
                    unsigned char type_table = id_table >> 4;
                    unsigned char num_table = id_table << 4;
                    num_table = num_table >> 4;
                    getVerbose() ? printf("ID composante : %d\n", id_component):0;
                    getVerbose() ? printf("Type de table : %d\n", type_table):0;
                    getVerbose() ? printf("Numéro de table : %d\n", num_table):0;
                }

                // Paramètres ignorés
                ignore_bytes(input, 3); // Octet de début de spectre, octet de fin de spectre, approximation (ignorés)

                // On essaie une nouvelle méthode pour lire la data, pour enlever les 0
                unsigned char *data = malloc(sizeof(unsigned char)); // Le malloc n'est pas bon car on ne connait pas la taille de la data
                int nb_data = 0;
                while (1){
                    fread(buffer, 1, 1, input);
                    if (buffer[0] == SEGMENT_START){
                        fread(buffer, 1, 1, input);
                        if (buffer[0] == 0x00){
                            data[nb_data] = SEGMENT_START;
                            nb_data++;
                        } else if (buffer[0] == EOI){
                            // On a fini la lecture des données
                            printf("Taille de la data : %d\n", nb_data);
                            free(data);
                            getVerbose() ? printf("Fin du fichier\n"):0;
                            return true;
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
                        getVerbose() ? fprintf(stderr, "Erreur de réallocation mémoire\n"):0;
                        free(data);
                        return false;
                    }               
                    data = tmp;
                }

                // On n'arrive jamais ici (impossible de break correctement)

                // free(data);

            } else if (id[0] == EOI){
                getVerbose() ? printf("Fin du fichier\n"):0;
                break;
            }
        }
    }
    return true;
}