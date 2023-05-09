#include <extract.h>

// Quantization tables

struct QuantizationTable {
    unsigned char id;
    unsigned char *data;
};

// Start of Frame

struct ComponentSOF {
    unsigned char id;
    unsigned char sampling_factor_x;
    unsigned char sampling_factor_y;
    unsigned char num_quantization_table;
};

struct StartOfFrame {
    unsigned short height;
    unsigned short width;
    unsigned char nb_components;
    struct ComponentSOF *components;
};

// DHT (Tables de Huffman)

struct HuffmanTable {
    unsigned char type_table;
    unsigned char num_table;
    unsigned char *data;
};

// Start of Scan

struct ComponentSOS {
    unsigned char id_table;
    unsigned char type_table;
    unsigned char num_table;
};

struct StartOfScan {
    unsigned char nb_components;
    struct ComponentSOS *components;
};

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

struct QuantizationTable get_qt(FILE *input, unsigned char *buffer) {
    // On souhaite récupérer les tables de quantification
    getVerbose() ? printf("Quantization table\n"):0;
    short length = 0;
    length = two_bytes_to_dec(input);
    length = length - 2 - 1;

    struct QuantizationTable qt;
    unsigned char *data = malloc(length*sizeof(unsigned char));

    fread(buffer, 1, 1, input);
    if (buffer[0] == LUMINANCE_ID) {
        fread(data, length, 1, input);
        // Affichage des tables de quantification
        for (int i=0; i<length; i++){
            getVerbose() ? printf("%x", data[i]):0;
        }
        getVerbose() ? printf("\n"):0;
        qt.id = LUMINANCE_ID;
        qt.data = data;

    } else if (buffer[0] == CHROMINANCE_ID) {
        fread(data, length, 1, input);
        // Affichage des tables de quantification
        for (int i=0; i<length; i++){
            getVerbose() ? printf("%x", data[i]):0;
        }
        getVerbose() ? printf("\n"):0;
        qt.id = CHROMINANCE_ID;
        qt.data = data;

    } else {
        fprintf(stderr, "Erreur dans la lecture des tables de quantification\n");
    }

    return qt;
}

struct StartOfFrame get_SOF(FILE *input, unsigned char *buffer) {
    getVerbose() ? printf("Start of frame\n"):0;
    ignore_bytes(input, 3); // On ignore la longueur et la précision

    short height = two_bytes_to_dec(input);
    short width = two_bytes_to_dec(input);
    getVerbose() ? printf("Hauteur : %d\n", height):0;
    getVerbose() ? printf("Largeur : %d\n", width):0;

    unsigned char nb_components = read_byte(input, buffer);
    getVerbose() ? printf("Nombre de composantes : %d\n", nb_components):0;

    struct ComponentSOF *components = malloc(nb_components*sizeof(struct ComponentSOF));

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

        components[i].id = id_component;
        components[i].sampling_factor_x = sampling_factor_x;
        components[i].sampling_factor_y = sampling_factor_y;
        components[i].num_quantization_table = num_quantization_table;
    }

    struct StartOfFrame sof;
    sof.height = height;
    sof.width = width;
    sof.nb_components = nb_components;
    sof.components = components;

    return sof;
}

struct HuffmanTable get_DHT(FILE *input, unsigned char *buffer) {
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

    struct HuffmanTable huffman_table;
    huffman_table.type_table = type_table;
    huffman_table.num_table = num_table;
    huffman_table.data = huffman;

    return huffman_table;
} 

struct StartOfScan getSOS(FILE *input, unsigned char *buffer){
    getVerbose() ? printf("Start of scan + data\n"):0;
    ignore_bytes(input, 2); // Longueur du segment (ignoré)

    unsigned char nb_components = read_byte(input, buffer); // Nombre de composantes
    getVerbose() ? printf("Nombre de composantes : %d\n", nb_components):0;

    struct ComponentSOS *components = malloc(nb_components*sizeof(struct ComponentSOS));
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

        components[i].id_table = id_component;
        components[i].type_table = type_table;
        components[i].num_table = num_table;
    }

    // Paramètres ignorés
    ignore_bytes(input, 3); // Octet de début de spectre, octet de fin de spectre, approximation (ignorés)

    struct StartOfScan sos;
    sos.nb_components = nb_components;
    sos.components = components;

    return sos;
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
                
                // Pour le moment, on ne lit qu'une Quantization table (on écrase l'ancienne)
                // A voir pour le comportement final
                struct QuantizationTable quantization_table = get_qt(input, buffer);
                free(quantization_table.data);

            } else if (id[0] == SOF_0){

                struct StartOfFrame sof = get_SOF(input, buffer);
                free(sof.components);

            } else if (id[0] == DHT){
                
                // Pour le moment, on ne lit qu'une seule table de Huffman (on écrase l'ancienne)
                // A voir pour le comportement final
                struct HuffmanTable huffman_table = get_DHT(input, buffer);
                free(huffman_table.data);

            } else if (id[0] == SOS){
                
                struct StartOfScan sos = getSOS(input, buffer);

                // On lit la data en enlevant les 0 (à cause du byte stuffing)
                unsigned char *data = malloc(sizeof(unsigned char)); // Le malloc n'est pas bon car on ne connait pas la taille de la data
                int nb_data = 0;
                while (1){
                    fread(buffer, 1, 1, input);
                    if (buffer[0] == SEGMENT_START){
                        fread(buffer, 1, 1, input);
                        if (buffer[0] == 0x00){
                            // pour le moment on a le dernier FF dans les données
                            data[nb_data] = SEGMENT_START;
                            nb_data++;
                        } else if (buffer[0] == EOI){
                            // On a fini la lecture des données
                            getVerbose() ? printf("Taille de la data : %d\n", nb_data):0;
                            getVerbose() ? printf("Fin du fichier\n"):0;
                            free(sos.components);
                            free(data);
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