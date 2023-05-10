#include <extract.h>
#include <utils.h>

// Quantization tables

struct QuantizationTable {
    unsigned char id;
    unsigned char *data;
};

unsigned char get_qt_id(struct QuantizationTable *qt){
    return qt->id;
}

unsigned char * get_qt_data(struct QuantizationTable *qt){
    return qt->data;
}

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
    unsigned char class;
    unsigned char destination;
    unsigned char *data;
};

// Start of Scan

struct ComponentSOS {
    unsigned char id_table;
    unsigned char class;
    unsigned char destination;
};

struct StartOfScan {
    unsigned char nb_components;
    struct ComponentSOS *components;
};

struct JPEG {
    struct QuantizationTable *quantization_tables;
    struct StartOfFrame start_of_frame;
    struct HuffmanTable *huffman_tables;
    struct StartOfScan start_of_scan;
    unsigned char *data;
};

struct QuantizationTable ** get_JPEG_qt(struct JPEG *jpeg){
    return jpeg->quantization_tables;
}

struct StartOfFrame get_JPEG_sof(struct JPEG *jpeg){
    return jpeg->start_of_frame;
}

struct HuffmanTable * get_JPEG_ht(struct JPEG *jpeg){
    return jpeg->huffman_tables;
}

struct StartOfScan get_JPEG_sos(struct JPEG *jpeg){
    return jpeg->start_of_scan;
}

unsigned char * get_JPEG_data(struct JPEG* jpeg){
    return jpeg->data;
}

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
    check_memory_allocation((void *) data);
    
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
    check_memory_allocation((void *) components);

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
    // Les 4 bits de poids fort indiquent la classe de la table (DC ou AC)
    // 0 : DC or lossless table,
    // 1 : AC
    // Les 4 bits de poids faible indiquent la destination de la table (luminance ou chrominance)
    // Specifies one of four possible destinations at the decoder into which the Huffman table shall be installed
    // 0 : luminance
    // 1 : chrominance
    unsigned char class = id_table >> 4;
    unsigned char destination = id_table << 4;
    destination = destination >> 4;
    getVerbose() ? printf("Classe de la table : %d\n", class):0;
    getVerbose() ? printf("Destination de la table : %d\n", destination):0;

    // Contenu de la table
    unsigned char *huffman = malloc(length*sizeof(unsigned char));
    check_memory_allocation((void *) huffman);

    fread(huffman, length, 1, input);

    // Affichage des tables de Huffman
    for (int i=0; i<length; i++){
        getVerbose() ? printf("%x", huffman[i]):0;
    }
    getVerbose() ? printf("\n"):0;

    struct HuffmanTable huffman_table;
    huffman_table.class = class;
    huffman_table.destination = destination;
    huffman_table.data = huffman;

    return huffman_table;
} 

struct StartOfScan getSOS(FILE *input, unsigned char *buffer){
    getVerbose() ? printf("Start of scan + data\n"):0;
    ignore_bytes(input, 2); // Longueur du segment (ignoré)

    unsigned char nb_components = read_byte(input, buffer); // Nombre de composantes
    getVerbose() ? printf("Nombre de composantes : %d\n", nb_components):0;

    struct ComponentSOS *components = malloc(nb_components*sizeof(struct ComponentSOS));
    check_memory_allocation((void *) components);

    // Composantes
    for (int i=0; i<nb_components; i++){
        unsigned char id_component = read_byte(input, buffer); // ID composante
        
        unsigned char id_table = read_byte(input, buffer); // ID de la table
        unsigned char class = id_table >> 4;
        unsigned char destination = id_table << 4;
        destination = destination >> 4;
        getVerbose() ? printf("ID composante : %d\n", id_component):0;
        getVerbose() ? printf("Type de table : %d\n", class):0;
        getVerbose() ? printf("Numéro de table : %d\n", destination):0;

        components[i].id_table = id_component;
        components[i].class = class;
        components[i].destination = destination;
    }

    // Paramètres ignorés
    ignore_bytes(input, 3); // Octet de début de spectre, octet de fin de spectre, approximation (ignorés)

    struct StartOfScan sos;
    sos.nb_components = nb_components;
    sos.components = components;

    return sos;
}

struct JPEG * extract(char *filename) { 
    // Ouverture du fichier et vérification de la conformité du type
    FILE *input;
    if( (input = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Vérification JPEG Magic number 
    unsigned char first3bytes[THREE_BYTES_LONG];
    fread(first3bytes, sizeof(first3bytes), 1, input);
    unsigned char JPEG_magic_Number[THREE_BYTES_LONG] = {SEGMENT_START, SOI, SEGMENT_START};

    for (int i=0; i<3; i++){
        if (first3bytes[i] != JPEG_magic_Number[i]){
            fprintf(stderr, "Le fichier %s n'est pas un fichier JPEG\n", filename);
            exit(EXIT_FAILURE);
        }
    }

    // Récupération données en-tête
    unsigned char buffer[1];    // Buffer
    unsigned char id[1];        // Buffer de lecture pour déterminer le type de segments

    struct JPEG *jpeg = malloc(sizeof(struct JPEG));
    check_memory_allocation((void *) jpeg);
    jpeg->quantization_tables = malloc(2*sizeof(struct QuantizationTable));
    check_memory_allocation((void *) jpeg->quantization_tables);
    jpeg->huffman_tables = malloc(4*sizeof(struct HuffmanTable));
    check_memory_allocation((void *) jpeg->huffman_tables);
    jpeg->data = malloc(sizeof(unsigned char)); // ET LAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAaa
    check_memory_allocation((void *) jpeg->data);

    while (!feof(input)){ // On arrête la boucle si on arrive à la fin du fichier sans avoir lu de marker EOF
        fread(buffer, 1, 1, input);
        if (buffer[0] == SEGMENT_START){
            fread(id, 1, 1, input);
            if (id[0] == DQT){
                
                // BREAKING-UPDATE : on possède au maximum 2 tables de quantification (luminance et chrominance)
                // index 0 : luminance
                // index 1 : chrominance
                struct QuantizationTable quantization_table = get_qt(input, buffer);
                
                if(quantization_table.id == LUMINANCE_ID) {
                    if(jpeg->quantization_tables[0].data != NULL) free(jpeg->quantization_tables[0].data);
                    jpeg->quantization_tables[0] = quantization_table;
                } else {
                    if(jpeg->quantization_tables[1].data != NULL) free(jpeg->quantization_tables[1].data);
                    jpeg->quantization_tables[1] = quantization_table;
                }

                //free(quantization_table.data);

            } else if (id[0] == SOF_0){

                struct StartOfFrame sof = get_SOF(input, buffer);
                jpeg->start_of_frame = sof;

                free(sof.components);

            } else if (id[0] == DHT){
                
                // BREAKING-UPDATE : on possède au maximum 4 tables de Huffman
                // index 0 >>> class|destination : 00 >>> luminance|DC
                // index 1 >>> class|destination : 01 >>> luminance|AC
                // index 2 >>> class|destination : 10 >>> chrominance|DC
                // index 3 >>> class|destination : 11 >>> chrominance|AC
                // Si une nouvelle table de Huffman redéfinie une table déjà existante, on écrase l'ancienne
                struct HuffmanTable huffman_table = get_DHT(input, buffer);
                
                if (huffman_table.class == 0) {
                    if (huffman_table.destination == 0) {
                        if (jpeg->huffman_tables[0].data != NULL) free(jpeg->huffman_tables[0].data);
                        jpeg->huffman_tables[0] = huffman_table;
                    } else {
                        if (jpeg->huffman_tables[1].data != NULL) free(jpeg->huffman_tables[1].data);
                        jpeg->huffman_tables[1] = huffman_table;
                    }
                } else {
                    if (huffman_table.destination == 0) {
                        if (jpeg->huffman_tables[2].data != NULL) free(jpeg->huffman_tables[2].data);
                        jpeg->huffman_tables[2] = huffman_table;
                    } else {
                        if (jpeg->huffman_tables[3].data != NULL) free(jpeg->huffman_tables[3].data);
                        jpeg->huffman_tables[3] = huffman_table;
                    }
                }

                free(huffman_table.data);

            } else if (id[0] == SOS){
                
                struct StartOfScan sos = getSOS(input, buffer);
                jpeg->start_of_scan = sos;

                // On lit la data en enlevant les 0 (à cause du byte stuffing)
                int data_size = 1024; // On donne une estimation de la taille des données à récupérer
                jpeg->data = malloc(data_size * sizeof(unsigned char *));
                check_memory_allocation((void *) jpeg->data);

                int nb_data = 0;

                while (!feof(input)){ // On arrête la boucle si on arrive à la fin du fichier sans avoir lu de marker EOF
                    fread(buffer, 1, 1, input);

                    if(nb_data >= data_size -1){
                        data_size *= 2;
                        jpeg->data = realloc(jpeg->data, data_size * sizeof(unsigned char *));
                        check_memory_allocation((void *) jpeg->data);
                    }

                    if (buffer[0] == SEGMENT_START){
                        fread(buffer, 1, 1, input);
                        if (buffer[0] == 0x00){ // On a du byte stuffing et on le supprime
                            jpeg->data[nb_data] = SEGMENT_START;
                            nb_data++;
                        } else if (buffer[0] == EOI){   // On supprimer le dernier 0xff du marker EOI
                            jpeg->data[nb_data -1] = 0x0;
                            // On a fini la lecture des données
                            getVerbose() ? printf("Taille de la data : %d\n", nb_data):0;
                            getVerbose() ? printf("Fin du fichier\n"):0;
                            free(sos.components);
                            return jpeg;
                        } else if (feof(input)){    // On atteint la fin du fichier avant d'avoir lu un marker EOI
                            getVerbose() ? printf("Fin du fichier atteinte avant d'avoir lu un EOI !!!\n"):0;
                            exit(EXIT_FAILURE);
                        } else {    // On a autre chose que du byte stuffing ou un marker autre que EOI ********** Remarque : si on a un autre marker que EOI, on ne le traite pas
                            jpeg->data[nb_data] = 0xFF;
                            nb_data++;
                            jpeg->data[nb_data] = buffer[0];
                            nb_data++;
                        }
                    } else {
                        jpeg->data[nb_data] = buffer[0];
                        nb_data++;
                    }
                }

                // On n'arrive jamais ici (impossible de break correctement)

                // free(data);

            } else if (id[0] == EOI){
                getVerbose() ? printf("Fin du fichier\n"):0;
                break;
            }
        }
    }
    return jpeg;
}