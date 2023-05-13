
#include <huffman.h>


//**********************************************************************************************************************
// Quantization tables
struct QuantizationTable{
    unsigned char id;
    size_t length;
    unsigned char *data;
};

void initialize_qt(struct QuantizationTable *qt){
    qt->id = 0;
    qt->length = 0;
    qt->data = NULL;
}

unsigned char get_qt_id(struct QuantizationTable *qt){
    return qt->id;
}

size_t get_qt_length(struct QuantizationTable *qt){
    return qt->length;
}

unsigned char * get_qt_data(struct QuantizationTable *qt){
    return qt->data;
}


//**********************************************************************************************************************
// Start of Frame
struct ComponentSOF {
    int8_t id;
    int8_t DC_huffman_table_id;
    int8_t AC_huffman_table_id;
    unsigned char sampling_factor_x;
    unsigned char sampling_factor_y;
    unsigned char num_quantization_table;
    int16_t *MCUs[64];
};

void initialize_component_sof(struct ComponentSOF *component){
    component->id = -1;
    component->DC_huffman_table_id = -1;
    component->AC_huffman_table_id = -1;
    component->sampling_factor_x = -1;
    component->sampling_factor_y = -1;
    component->num_quantization_table = 0;
}

int8_t get_id(struct ComponentSOF *component){
    return component->id;
}

int8_t get_DC_huffman_table_id(struct ComponentSOF *component){
    return component->DC_huffman_table_id;
}

int8_t get_AC_huffman_table_id(struct ComponentSOF *component){
    return component->AC_huffman_table_id;
}

int16_t **get_MCUs(struct ComponentSOF *component){
    return component->MCUs;
}

// int16_t *get_MCU(struct ComponentSOF *component, int index_of_mcu){
//     return component->MCUs[index_of_mcu];
// }

void set_value_in_MCU(struct ComponentSOF *component, int index_of_mcu, int index_of_pixel_in_mcu, int16_t value){
        component->MCUs[index_of_mcu][index_of_pixel_in_mcu] = value;
}

struct StartOfFrame {
    unsigned int nb_components;
    struct ComponentSOF *components;
};

void initialize_sof(struct StartOfFrame *sof){
    sof->nb_components = 0;
    sof->components = NULL;
}

unsigned char get_sof_nb_components(struct StartOfFrame *sof){
    return sof->nb_components;
}

struct ComponentSOF * get_sof_components(struct StartOfFrame *sof){
    return sof->components;
}


//**********************************************************************************************************************
// DHT (Tables de Huffman)
struct HuffmanTable {
    unsigned char class;
    unsigned char destination;
    size_t length;
    unsigned char *data;
    struct node * huffman_tree;
    bool set;   // permet de savoir si la table de Huffman a été définie dans le header
};

void initialize_ht(struct HuffmanTable *ht){
    ht->length = 0;
    ht->data = NULL;
    ht->huffman_tree = NULL;
    ht->set = false;
}

unsigned char get_ht_class(struct HuffmanTable *ht){
    return ht->class;
}

unsigned char get_ht_destination(struct HuffmanTable *ht){
    return ht->destination;
}

size_t get_ht_length(struct HuffmanTable *ht){
    return ht->length;
}

unsigned char * get_ht_data(struct HuffmanTable *ht){
    return ht->data;
}

struct node * get_ht_tree(struct HuffmanTable *ht){
    return ht->huffman_tree;
}


bool get_ht_set(struct HuffmanTable *ht){
    return ht->set;
}


//**********************************************************************************************************************
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

void initialize_sos(struct StartOfScan *sos){
    sos->nb_components = 0; // 1 = greyscale, 3 = YCbCr or YIQ
    sos->components = NULL;
}

unsigned char get_sos_nb_components(struct StartOfScan *sos){
    return sos->nb_components;
}

struct ComponentSOS * get_sos_components(struct StartOfScan *sos){
    return sos->components;
}


//**********************************************************************************************************************
// Structure JPEG
struct JPEG {
    unsigned short height;
    unsigned short width;
    struct QuantizationTable **quantization_tables;
    struct StartOfFrame **start_of_frame;
    struct HuffmanTable **huffman_tables;
    struct StartOfScan *start_of_scan;
    unsigned char *image_data;
    size_t image_data_size_in_bits;
};

void initialize_JPEG_struct(struct JPEG *jpeg){
    jpeg->height = 0;

    jpeg->width = 0;

    jpeg->quantization_tables = (struct QuantizationTable **) malloc(2 * sizeof(struct QuantizationTable *));
    check_memory_allocation((void *) jpeg->quantization_tables);
    for (int i=0; i<2; i++){
        jpeg->quantization_tables[i] = (struct QuantizationTable *) malloc(sizeof(struct QuantizationTable));
        check_memory_allocation((void *) jpeg->quantization_tables[i]);
        initialize_qt(jpeg->quantization_tables[i]);
    }

    jpeg->start_of_frame = (struct StartOfFrame **) malloc(1 * sizeof(struct StartOfFrame *));
    check_memory_allocation((void *) jpeg->start_of_frame);
    initialize_sof(jpeg->start_of_frame[0]);

    jpeg->huffman_tables = (struct HuffmanTable **) malloc(4 * sizeof(struct HuffmanTable *));
    check_memory_allocation((void *) jpeg->huffman_tables);
    for (int i=0; i<4; i++){
        jpeg->huffman_tables[i] = (struct HuffmanTable *) malloc(sizeof(struct HuffmanTable));
        check_memory_allocation((void *) jpeg->huffman_tables[i]);
        initialize_ht(jpeg->huffman_tables[i]);
    }

    jpeg->start_of_scan = (struct StartOfScan *) malloc(sizeof(struct StartOfScan));
    check_memory_allocation((void *) jpeg->start_of_scan);

    jpeg->image_data = (unsigned char *) malloc(INITIAL_DATA_SIZE * sizeof(unsigned char));
    check_memory_allocation((void *) jpeg->image_data);

    jpeg->image_data_size_in_bits = 0;
}

unsigned short get_JPEG_height(struct JPEG *jpeg){
    return jpeg->height;
}

unsigned short get_JPEG_width(struct JPEG *jpeg){
    return jpeg->width;
}

struct QuantizationTable ** get_JPEG_qt(struct JPEG *jpeg){
    return jpeg->quantization_tables;
}

struct StartOfFrame ** get_JPEG_sof(struct JPEG *jpeg){
    return jpeg->start_of_frame;
}

struct HuffmanTable ** get_JPEG_ht(struct JPEG *jpeg){
    return jpeg->huffman_tables;
}

struct StartOfScan * get_JPEG_sos(struct JPEG *jpeg){
    return jpeg->start_of_scan;
}

unsigned char * get_JPEG_image_data(struct JPEG* jpeg){
    return jpeg->image_data;
}

size_t get_JPEG_image_data_size_in_bits(struct JPEG* jpeg){
    return jpeg->image_data_size_in_bits;
}




//**********************************************************************************************************************
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


//**********************************************************************************************************************
// Récupère les données de la table de quantification
struct QuantizationTable * get_qt(FILE *input, unsigned char *buffer) {
    // On souhaite récupérer les tables de quantification
    getVerbose() ? printf("Quantization table\n"):0;
    short length = 0;
    length = two_bytes_to_dec(input);
    length = length - 2 - 1;

    struct QuantizationTable *qt = (struct QuantizationTable *) malloc(sizeof(struct QuantizationTable));
    check_memory_allocation((void *) qt);
    qt->data = (unsigned char *) malloc(length * sizeof(unsigned char *));
    check_memory_allocation((void *) qt->data);
    
    fread(buffer, 1, 1, input);
    if (buffer[0] == LUMINANCE_ID) {
        fread(qt->data, length, 1, input);
        // Affichage des tables de quantification
        for (int i=0; i<length; i++){
            getVerbose() ? printf("%x", qt->data[i]):0;
        }
        getVerbose() ? printf("\n"):0;
        qt->id = LUMINANCE_ID;

    } else if (buffer[0] == CHROMINANCE_ID) {
        fread(qt->data, length, 1, input);
        // Affichage des tables de quantification
        for (int i=0; i<length; i++){
            getVerbose() ? printf("%x", qt->data[i]):0;
        }
        getVerbose() ? printf("\n"):0;
        qt->id = CHROMINANCE_ID;

    } else {
        fprintf(stderr, "Erreur dans la lecture des tables de quantification\n");
    }
    qt->length = length;

    return qt;
}


//**********************************************************************************************************************
// Récupère les données du segment Start_Of_Frame
int get_SOF(FILE *input, unsigned char *buffer, struct JPEG *jpeg) {
    getVerbose() ? printf("Start of frame\n"):0;
    ignore_bytes(input, 3); // On ignore la longueur et la précision

    short height = two_bytes_to_dec(input);
    short width = two_bytes_to_dec(input);
    // On stocke ces valeurs dans la super structure JPEG plutôt que dans le SOF ...
    // si on a plusieur frames ils seront de toute façon de la même taille ...
    jpeg->height = height;
    jpeg->width = width;
    getVerbose() ? printf("Hauteur de l'image en pixel : %d\n", height):0;
    getVerbose() ? printf("Largeur de l'image en pixel : %d\n", width):0;

    unsigned char nb_components = read_byte(input, buffer);
    getVerbose() ? printf("Nombre de composantes : %d\n", nb_components):0;

    struct ComponentSOF *components = (struct ComponentSOF *) malloc(nb_components*sizeof(struct ComponentSOF));
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

    // On supprime les données précédentes
    free(jpeg->start_of_frame[0]->components);

    // Et on met à jour les données
    jpeg->start_of_frame[0]->nb_components = nb_components;
    jpeg->start_of_frame[0]->components = components;

    return EXIT_SUCCESS;
}


//**********************************************************************************************************************
// Récupère les données de la table de Huffman
struct HuffmanTable * get_DHT(FILE *input, unsigned char *buffer) {
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
    unsigned char *huffman_data = (unsigned char *) malloc(length*sizeof(unsigned char));
    check_memory_allocation((void *) huffman_data);

    fread(huffman_data, length, 1, input);

    // Affichage des tables de Huffman
    for (int i=0; i<length; i++){
        getVerbose() ? printf("%x", huffman_data[i]):0;
    }
    getVerbose() ? printf("\n"):0;

    struct HuffmanTable *huffman_table = (struct HuffmanTable *) malloc(sizeof(struct HuffmanTable));
    check_memory_allocation((void *) huffman_table);
    huffman_table->class = class;
    huffman_table->destination = destination;
    huffman_table->length = length;
    huffman_table->data = huffman_data;
    huffman_table->huffman_tree = build_huffman_tree(huffman_data);
    huffman_table->set = true;

    return huffman_table;
} 


//**********************************************************************************************************************
// Récupère les données du segment Start_Of_Scan
int getSOS(FILE *input, unsigned char *buffer, struct JPEG *jpeg){
    getVerbose() ? printf("Start of scan + data\n"):0;
    ignore_bytes(input, 2); // Longueur du segment (ignoré)

    unsigned char nb_components = read_byte(input, buffer); // Nombre de composantes
    getVerbose() ? printf("Nombre de composantes : %d\n", nb_components):0;

    struct ComponentSOS *components = (struct ComponentSOS *) malloc(nb_components*sizeof(struct ComponentSOS));
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

    // On supprime les données précédentes
    free(jpeg->start_of_scan->components);

    // Et on met à jour les données
    jpeg->start_of_scan->nb_components = nb_components;
    jpeg->start_of_scan->components = components;

    return EXIT_SUCCESS;
}


//**********************************************************************************************************************
// Récupère les données du fichier JPEG
struct JPEG * extract(char *filename) {

    // Ouverture et vérification de la présence du fichier
    FILE *input;
    if( (input = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Vérification conformité fichier via JPEG Magic number 
    unsigned char first3bytes[THREE_BYTES_LONG];
    fread(first3bytes, sizeof(first3bytes), 1, input);
    unsigned char JPEG_magic_Number[THREE_BYTES_LONG] = {SEGMENT_START, SOI, SEGMENT_START};

    for (int i=0; i<3; i++){
        if (first3bytes[i] != JPEG_magic_Number[i]){
            fprintf(stderr, "Le fichier %s n'est pas un fichier JPEG\n", filename);
            exit(EXIT_FAILURE);
        }
    }

    // Ajouter la vérification de la présence de JFIF dans le marker APP0


    // Récupération des données de l'en-tête
    unsigned char buffer[1];    // Buffer
    unsigned char id[1];        // Buffer de lecture pour déterminer le type de segments

    struct JPEG *jpeg = (struct JPEG *) malloc(1 * sizeof(struct JPEG));
    check_memory_allocation((void *) jpeg);
    initialize_JPEG_struct(jpeg);

    while (!feof(input)){ // On arrête la boucle si on arrive à la fin du fichier sans avoir lu de marker EOF
        fread(buffer, 1, 1, input);

        if (buffer[0] == SEGMENT_START){
            fread(id, 1, 1, input);
            //**********************************************************************************************************************
            if (id[0] == DQT){
                
                // BREAKING-UPDATE : on possède au maximum 2 tables de quantification (luminance et chrominance)
                // index 0 : luminance
                // index 1 : chrominance
                struct QuantizationTable *quantization_table = get_qt(input, buffer);
                
                if(quantization_table->id == LUMINANCE_ID) {
                    if (jpeg->quantization_tables[0]->data != NULL) free(jpeg->quantization_tables[0]->data);
                    jpeg->quantization_tables[0] = quantization_table;
                } else {
                    if (jpeg->quantization_tables[1]->data != NULL) free(jpeg->quantization_tables[1]->data);
                    jpeg->quantization_tables[1] = quantization_table;
                }

            //**********************************************************************************************************************
            } else if (id[0] == SOF_0){

                get_SOF(input, buffer, jpeg);

            //**********************************************************************************************************************
            } else if (id[0] == DHT){
                
                // BREAKING-UPDATE : on possède au maximum 4 tables de Huffman
                // index 0 >>> class|destination : 00 >>> luminance|DC
                // index 1 >>> class|destination : 01 >>> chrominance|DC
                // index 2 >>> class|destination : 10 >>> luminance|AC
                // index 3 >>> class|destination : 11 >>> chrominance|AC
                // Si une nouvelle table de Huffman redéfinie une table déjà existante, on supprime l'ancienne
                struct HuffmanTable *huffman_table = get_DHT(input, buffer);
                
                if (huffman_table->class == 0) {
                    if (huffman_table->destination == 0) {
                        free(jpeg->huffman_tables[0]->data);
                        free(jpeg->huffman_tables[0]->huffman_tree);
                        free(jpeg->huffman_tables[0]);
                        jpeg->huffman_tables[0] = huffman_table;
                    } else {
                        free(jpeg->huffman_tables[1]->data);
                        free(jpeg->huffman_tables[1]->huffman_tree);
                        free(jpeg->huffman_tables[1]);
                        jpeg->huffman_tables[0] = huffman_table;
                    }
                } else {
                    if (huffman_table->destination == 0) {
                        free(jpeg->huffman_tables[2]->data);
                        free(jpeg->huffman_tables[2]->huffman_tree);
                        free(jpeg->huffman_tables[2]);
                        jpeg->huffman_tables[0] = huffman_table;
                    } else {
                        free(jpeg->huffman_tables[3]->data);
                        free(jpeg->huffman_tables[3]->huffman_tree);
                        free(jpeg->huffman_tables[3]);
                        jpeg->huffman_tables[0] = huffman_table;
                    }
                }

            //**********************************************************************************************************************
            } else if (id[0] == SOS){
                
                getSOS(input, buffer, jpeg);

                // On lit la data en enlevant les 0 (à cause du byte stuffing)
                int data_size = INITIAL_DATA_SIZE; // On donne une estimation de la taille des données à récupérer

                int nb_data = 0;

                while (!feof(input)){ // On arrête la boucle si on arrive à la fin du fichier sans avoir lu de marker EOF
                    fread(buffer, 1, 1, input);

                    // On réalloue de la mémoire si on a besoin
                    if(nb_data >= data_size -1){
                        data_size *= 2;
                        jpeg->image_data = realloc(jpeg->image_data, data_size * sizeof(unsigned char *));
                        check_memory_allocation((void *) jpeg->image_data);
                    }

                    if (buffer[0] == SEGMENT_START){
                        fread(buffer, 1, 1, input);

                        if (buffer[0] == 0x00){ // On a du byte stuffing et on le supprime
                            jpeg->image_data[nb_data] = SEGMENT_START;
                            nb_data++;

                        } else if (buffer[0] == EOI){   // On supprimer le dernier 0xff du marker EOI
                            jpeg->image_data[nb_data -1] = 0x0;
                            jpeg->image_data_size_in_bits = 8 * nb_data;
                            // On a fini la lecture des données
                            getVerbose() ? printf("Taille de la data : %d\n", nb_data):0;
                            getVerbose() ? printf("Fin du fichier\n"):0;
                            fclose(input);
                            return jpeg;

                        } else if (feof(input)){    // On atteint la fin du fichier avant d'avoir lu un marker EOI
                            getVerbose() ? printf("Fin du fichier atteinte avant d'avoir lu un EOI !!!\n"):0;
                            fclose(input);
                            exit(EXIT_FAILURE);

                        } else {    // On a autre chose que du byte stuffing ou un marker autre que EOI ********** Remarque : si on a un autre marker que EOI, on ne le traite pas
                            jpeg->image_data[nb_data] = 0xFF;
                            nb_data++;
                            jpeg->image_data[nb_data] = buffer[0];
                            nb_data++;
                        }

                    } else {
                        jpeg->image_data[nb_data] = buffer[0];
                        nb_data++;
                    }
                }

                // On n'arrive jamais ici (impossible de break correctement)

            //**********************************************************************************************************************
            } else if (id[0] == EOI){
                getVerbose() ? printf("Fin du fichier\n"):0;
                fclose(input);
                break;
            }
        }
    }
    fclose(input);
    return jpeg;
}
