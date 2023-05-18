#include <extract.h>

//**********************************************************************************************************************
// Quantization tables
struct QuantizationTable{
    int8_t id;
    size_t length;
    uint8_t *data;
};

int8_t initialize_qt(struct QuantizationTable *qt, int8_t id, size_t length, unsigned char *data){
    qt->id = id;
    qt->length = length;
    qt->data = data;

    return EXIT_SUCCESS;
}

int8_t get_qt_id(struct QuantizationTable *qt){
    return qt->id;
}

size_t get_qt_length(struct QuantizationTable *qt){
    return qt->length;
}

uint8_t * get_qt_data(struct QuantizationTable *qt){
    return qt->data;
}


//**********************************************************************************************************************
// Start of Frame
struct ComponentSOF {
    int8_t id;
    int8_t sampling_factor_x;
    int8_t sampling_factor_y;
    int8_t num_quantization_table;
};

void initialize_component_sof(struct ComponentSOF *component, int8_t id, int8_t sampling_factor_x, int8_t sampling_factor_y, int8_t num_quantization_table){
    component->id = id;
    component->sampling_factor_x = sampling_factor_x;
    component->sampling_factor_y = sampling_factor_y;
    component->num_quantization_table = num_quantization_table;
}

int8_t get_id(struct ComponentSOF *component){
    return component->id;
}

int8_t get_sampling_factor_x(struct ComponentSOF *component){
    return component->sampling_factor_x;
}

int8_t get_sampling_factor_y(struct ComponentSOF *component){
    return component->sampling_factor_y;
}

int8_t get_num_quantization_table(struct ComponentSOF *component){
    return component->num_quantization_table;
}


struct StartOfFrame {
    int8_t nb_components;
    struct ComponentSOF *components;
};

int8_t initialize_sof(struct StartOfFrame *sof, int8_t nb_components, int8_t id, int8_t sampling_factor_x, int8_t sampling_factor_y, int8_t num_quantization_table){
    sof->nb_components = nb_components; // 1 = greyscale, 3 = YCbCr or YIQ, 4 = CMYK
    
    if (nb_components == 0) {
        sof->components = NULL;
    } else {
        sof->components = (struct ComponentSOF *) malloc(nb_components * sizeof(struct ComponentSOF));
        if(check_memory_allocation((void *) sof->components)) return EXIT_FAILURE;
        for(int i=0; i<nb_components; i++){
            initialize_component_sof(&(sof->components[i]), id, sampling_factor_x, sampling_factor_y, num_quantization_table);
        }
    }

    return EXIT_SUCCESS;
}

int8_t get_sof_nb_components(struct StartOfFrame *sof){
    return sof->nb_components;
}

struct ComponentSOF * get_sof_components(struct StartOfFrame *sof){
    return sof->components;
}

struct ComponentSOF * get_sof_component(struct ComponentSOF * components, int8_t index){
    return &components[index];
}


//**********************************************************************************************************************
// DHT (Tables de Huffman)
struct HuffmanTable {
    int8_t class;   // 0 = DC or lossless, 1 = AC
    int8_t destination;
    size_t length;
    unsigned char *data;
    struct node * huffman_tree;
    bool set;   // permet de savoir si la table de Huffman a été définie dans le header
};

void initialize_ht(struct HuffmanTable *ht, int8_t class, int8_t destination, size_t length, unsigned char *data, struct node * huffman_tree, bool set){
    ht->class = class;
    ht->destination = destination;
    ht->length = length;
    ht->data = data;
    ht->huffman_tree = huffman_tree;
    ht->set = set;
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
    int8_t id_table;
    int8_t DC_huffman_table_id;
    int8_t AC_huffman_table_id;
    size_t nb_of_MCUs;
    int16_t **MCUs;
};

int8_t initialize_component_sos(struct ComponentSOS *component, int8_t id_table, int8_t DC_huffman_table_id, int8_t AC_huffman_table_id, size_t nb_of_MCUs){
    component->id_table = id_table;
    component->DC_huffman_table_id = DC_huffman_table_id;
    component->AC_huffman_table_id = AC_huffman_table_id;
    component->nb_of_MCUs = nb_of_MCUs;
    component->MCUs = (int16_t **) malloc(nb_of_MCUs * sizeof(int16_t *));
    if(check_memory_allocation((void *) component->MCUs)) return EXIT_FAILURE;
    for(size_t i=0; i<nb_of_MCUs; i++){
        component->MCUs[i] = (int16_t *) malloc(64 * sizeof(int16_t));
        if(check_memory_allocation((void *) component->MCUs[i])) {
            for(size_t j=0; j<i; j++){
                free(component->MCUs[j]);
            }
            free(component->MCUs);
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int8_t get_DC_huffman_table_id(struct ComponentSOS *component){
    return component->DC_huffman_table_id;
}

int8_t get_AC_huffman_table_id(struct ComponentSOS *component){
    return component->AC_huffman_table_id;
}

int16_t **get_MCUs(struct ComponentSOS *component){
    return component->MCUs;
}

// int16_t *get_MCU(struct ComponentSOF *component, int index_of_mcu){
//     return component->MCUs[index_of_mcu];
// }

void set_value_in_MCU(struct ComponentSOS *component, int index_of_mcu, int index_of_pixel_in_mcu, int16_t value){
        component->MCUs[index_of_mcu][index_of_pixel_in_mcu] = value;
}


struct StartOfScan {
    unsigned char nb_components;
    struct ComponentSOS *components;
};

int8_t initialize_sos(struct StartOfScan *sos, int8_t nb_components, int8_t id_table, int8_t DC_huffman_table_id, int8_t AC_huffman_table_id, size_t nb_of_MCU){
    sos->nb_components = nb_components; // 1 = greyscale, 3 = YCbCr or YIQ, 4 = CMYK

    if (nb_components == 0) {
        sos->components = NULL;
    } else {
        sos->components = (struct ComponentSOS *) malloc(nb_components * sizeof(struct ComponentSOS));
        if(check_memory_allocation((void *) sos->components)) return EXIT_FAILURE;
        for(int i=0; i<nb_components; i++){
            if(initialize_component_sos(&(sos->components[i]), id_table, DC_huffman_table_id, AC_huffman_table_id, nb_of_MCU)) {
                for(int j=0; j<i; j++){
                    for(size_t k=0; k<nb_of_MCU; k++){
                        free(sos->components[j].MCUs[k]);
                    }
                    free(sos->components[j].MCUs);
                }
                free(sos->components);
                return EXIT_FAILURE;
            }
        }
    }
    return EXIT_SUCCESS;
}

unsigned char get_sos_nb_components(struct StartOfScan *sos){
    return sos->nb_components;
}

struct ComponentSOS * get_sos_components(struct StartOfScan *sos){
    return sos->components;
}

struct ComponentSOS * get_sos_component(struct ComponentSOS * components, int8_t index){
    return &components[index];
}


//**********************************************************************************************************************
// Structure JPEG
struct JPEG {
    int16_t width;
    int16_t height;
    struct QuantizationTable **quantization_tables;
    struct StartOfFrame **start_of_frame;
    struct HuffmanTable **huffman_tables;
    struct StartOfScan **start_of_scan;
    unsigned char *image_data;
    unsigned long long image_data_size_in_bits;
};

int8_t initialize_JPEG_struct(struct JPEG *jpeg){
    jpeg->height = 0;

    jpeg->width = 0;

    jpeg->quantization_tables = (struct QuantizationTable **) malloc(MAX_NUMBER_OF_QUANTIZATION_TABLES * sizeof(struct QuantizationTable *));
    if (check_memory_allocation((void *) jpeg->quantization_tables)) {
        free_JPEG_struct(jpeg);
        return EXIT_FAILURE;
    }
    for (int i=0; i < MAX_NUMBER_OF_QUANTIZATION_TABLES; i++){
        jpeg->quantization_tables[i] = (struct QuantizationTable *) malloc(sizeof(struct QuantizationTable));
        if (check_memory_allocation((void *) jpeg->quantization_tables[i])) {
            free_JPEG_struct(jpeg);
            return EXIT_FAILURE;
        }
        initialize_qt(jpeg->quantization_tables[i], -1, 0, NULL);
    }

    jpeg->start_of_frame = (struct StartOfFrame **) malloc(1 * sizeof(struct StartOfFrame *));  // pour l'instant on a un seul frame ... à modifier pour mode progressif
    if (check_memory_allocation((void *) jpeg->start_of_frame)){
        free_JPEG_struct(jpeg);
        return EXIT_FAILURE;
    }
    jpeg->start_of_frame[0] = (struct StartOfFrame *) malloc(sizeof(struct StartOfFrame));
    if (check_memory_allocation((void *) jpeg->start_of_frame[0])) {
        free_JPEG_struct(jpeg);
        return EXIT_FAILURE;
    }
    if (initialize_sof(jpeg->start_of_frame[0], 0, -1, -1, -1, -1)) {
        free_JPEG_struct(jpeg);
        return EXIT_FAILURE;
    }

    jpeg->huffman_tables = (struct HuffmanTable **) malloc(MAX_NUMBER_OF_HUFFMAN_TABLES * sizeof(struct HuffmanTable *));
    if (check_memory_allocation((void *) jpeg->huffman_tables)) {
        free_JPEG_struct(jpeg);
        return EXIT_FAILURE;
    }
    for (int i=0; i<4; i++){
        jpeg->huffman_tables[i] = (struct HuffmanTable *) malloc(sizeof(struct HuffmanTable));
        if (check_memory_allocation((void *) jpeg->huffman_tables[i])) {
            free_JPEG_struct(jpeg);
            return EXIT_FAILURE;
        }
        initialize_ht(jpeg->huffman_tables[i], -1, -1, 0, NULL, NULL, false);
    }

    jpeg->start_of_scan = (struct StartOfScan **) malloc(1 * sizeof(struct StartOfScan *));  // pour l'instant on a un seul scan ... à modifier pour mode progressif
    if (check_memory_allocation((void *) jpeg->start_of_scan)) {
        free_JPEG_struct(jpeg);
        return EXIT_FAILURE;
    }
    jpeg->start_of_scan[0] = (struct StartOfScan *) malloc(sizeof(struct StartOfScan));
    if (check_memory_allocation((void *) jpeg->start_of_scan[0])) {
        free_JPEG_struct(jpeg);
        return EXIT_FAILURE;
    }
    if(initialize_sos(jpeg->start_of_scan[0], 0, -1, -1, -1, 0)) {
        free_JPEG_struct(jpeg);
        return EXIT_FAILURE;
    }

    jpeg->image_data = (unsigned char *) malloc(INITIAL_DATA_SIZE * sizeof(unsigned char));
    if (check_memory_allocation((void *) jpeg->image_data)) {
        free_JPEG_struct(jpeg);
        return EXIT_FAILURE;
    }

    jpeg->image_data_size_in_bits = 0;

    return EXIT_SUCCESS;
}

// Fonction libère proprement toute la mémoire allouée pour une structure JPEG
// Se charge de libérer toutes les allocations internes à ces structures
void free_JPEG_struct(struct JPEG *jpeg){
    if (jpeg == NULL) return;

    // On free les tables de quantification
    if (jpeg->quantization_tables != NULL) {
        for(int8_t i=0; i < MAX_NUMBER_OF_QUANTIZATION_TABLES; i++){
            if (jpeg->quantization_tables[i] != NULL){
                if (jpeg->quantization_tables[i]->data != NULL){
                    free(jpeg->quantization_tables[i]->data);
                }
                free(jpeg->quantization_tables[i]);
            }
        }
        free(jpeg->quantization_tables);
    }
 
    // On free les Start Of Frame
    if (jpeg->start_of_frame != NULL) {
        for(int8_t i=0; i < 1; i++){    // pour l'instant on a un seul frame ... à modifier pour mode progressif
            if (jpeg->start_of_frame[i] != NULL){
                if (jpeg->start_of_frame[i]->components != NULL){
                    free(jpeg->start_of_frame[i]->components);
                }
                free(jpeg->start_of_frame[i]);
            }
            free(jpeg->start_of_frame);
        }
    }

    // On free les tables de Huffman
    if (jpeg->huffman_tables != NULL) {
        for(int8_t i=0; i < MAX_NUMBER_OF_HUFFMAN_TABLES; i++){
            if (jpeg->huffman_tables[i] != NULL){
                if (jpeg->huffman_tables[i]->data != NULL){
                    free(jpeg->huffman_tables[i]->data);
                }
                if (jpeg->huffman_tables[i]->huffman_tree != NULL){
                    free_huffman_tree(jpeg->huffman_tables[i]->huffman_tree);
                }
                free(jpeg->huffman_tables[i]);
            }
        }
        free(jpeg->huffman_tables);
    }

    // On free les Start Of Scan
    if (jpeg->start_of_scan != NULL) {
        for(int8_t i=0; i < 1; i++){    // pour l'instant on a un seul scan ... à modifier pour mode progressif
            if (jpeg->start_of_scan[i] != NULL){
                if ((jpeg->start_of_scan[i])->components != NULL){
                    for (int8_t j=0; j < jpeg->start_of_scan[i]->nb_components; j++){
                        if (&((jpeg->start_of_scan[i])->components[j]) != NULL){
                            for (size_t k=0; k < (&( (jpeg->start_of_scan[i])->components[j] ))->nb_of_MCUs; k++){
                                if ((&((jpeg->start_of_scan[i])->components[j]))->MCUs[k] != NULL){
                                    free(   (&((jpeg->start_of_scan[i])->components[j]))->MCUs[k]);
                                }
                            }
                            free((&((jpeg->start_of_scan[i])->components[j]))->MCUs);
                            free(&((jpeg->start_of_scan[i])->components[j]));
                        }
                    }
                }
                free(jpeg->start_of_scan[i]);
            }
        }
        free(jpeg->start_of_scan);
    }
    
    // On free les données de l'image
    if (jpeg->image_data != NULL) free(jpeg->image_data);

    // On free la structure JPEG
    free(jpeg);
}

int16_t get_JPEG_height(struct JPEG *jpeg){
    return jpeg->height;
}

int16_t get_JPEG_width(struct JPEG *jpeg){
    return jpeg->width;
}

struct QuantizationTable ** get_JPEG_qt(struct JPEG *jpeg){
    return jpeg->quantization_tables;
}

struct StartOfFrame ** get_JPEG_sof(struct JPEG *jpeg){
    return jpeg->start_of_frame;
}

struct HuffmanTable * get_JPEG_ht(struct JPEG *jpeg, int8_t index){
    return jpeg->huffman_tables[index];
}

struct StartOfScan ** get_JPEG_sos(struct JPEG *jpeg){
    return jpeg->start_of_scan;
}

unsigned char * get_JPEG_image_data(struct JPEG* jpeg){
    return jpeg->image_data;
}

unsigned long long get_JPEG_image_data_size_in_bits(struct JPEG* jpeg){
    return jpeg->image_data_size_in_bits;
}


//**********************************************************************************************************************
int16_t two_bytes_to_dec(FILE *input){
    // Lecture et renvoie de la valeur décimale de deux octets
    int16_t length = 0;
    int16_t length2 = 0;
    fread(&length, 1, 1, input);
    length = length << 8;
    fread(&length2, 1, 1, input);
    length = length + length2;
    return length;
}


int8_t read_byte(FILE *input, unsigned char *buffer){
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
    getVerbose() ? printf("\nQuantization table\n"):0;
    getVerbose() ? printf("\tDonnées de la table : "):0;
    int16_t length = 0;
    length = two_bytes_to_dec(input);
    length = length - 2 - 1;

    struct QuantizationTable *qt = (struct QuantizationTable *) malloc(sizeof(struct QuantizationTable));
    if (check_memory_allocation((void *) qt)) return NULL;
    qt->data = (uint8_t *) malloc(length * sizeof(uint8_t *));
    if (check_memory_allocation((void *) qt->data)) {
        free(qt);
        return NULL;
    }
    
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
        for (int16_t i=0; i<length; i++){
            getVerbose() ? printf("%x", qt->data[i]):0;
        }
        getVerbose() ? printf("\n"):0;
        qt->id = CHROMINANCE_ID;

    } else {
        fprintf(stderr, RED("Erreur dans la lecture des tables de quantification\n"));
    }
    qt->length = length;

    return qt;
}


//**********************************************************************************************************************
// Récupère les données du segment Start_Of_Frame
int8_t get_SOF(FILE *input, unsigned char *buffer, struct JPEG *jpeg) {
    getVerbose() ? printf("\nStart of frame\n"):0;
    ignore_bytes(input, 3); // On ignore la longueur et la précision

    int16_t height = two_bytes_to_dec(input);
    int16_t width = two_bytes_to_dec(input);
    // On stocke ces valeurs dans la super structure JPEG plutôt que dans le SOF ...
    // si on a plusieurs frames ils seront de toute façon de la même taille ...
    jpeg->height = height;
    jpeg->width = width;
    getVerbose() ? printf("\tHauteur de l'image en pixel : %d\n", height):0;
    getVerbose() ? printf("\tLargeur de l'image en pixel : %d\n", width):0;

    int8_t nb_components = read_byte(input, buffer);
    if (nb_components > 3 || nb_components == 2 || nb_components <= 0) {
        fprintf(stderr, RED("Erreur : le nombre de composantes n'est pas supporté par jpeg2ppm\n"));
        return EXIT_FAILURE;
    }
    getVerbose() ? printf("\tNombre de composantes : %d\n", nb_components):0;

    // On met à jour le nombre de mcus dans le Start Of Scan s'il existe
    if (jpeg->start_of_scan[0]->nb_components == nb_components) {
        size_t nb_mcu_width = (width + 7) / 8;
        size_t nb_mcu_height = (height + 7) / 8;

        for (int8_t i=0; i < nb_components; i++) {
            (&(jpeg->start_of_scan[0]->components[i]))->nb_of_MCUs = nb_mcu_width * nb_mcu_height;

            (&(jpeg->start_of_scan[0]->components[i]))->MCUs = (int16_t **) malloc(nb_mcu_width * nb_mcu_height * sizeof(int16_t *));
            if (check_memory_allocation((void *) (&(jpeg->start_of_scan[0]->components[i]))->MCUs)) return EXIT_FAILURE;

            for (size_t j=0; j < nb_mcu_width * nb_mcu_height; i++) {
                (&(jpeg->start_of_scan[0]->components[i]))->MCUs[j] = (int16_t *) malloc(64 * sizeof(int16_t));
                if (check_memory_allocation((void *) (&(jpeg->start_of_scan[0]->components[i]))->MCUs[j])) return EXIT_FAILURE;
            }
        }
    }

    struct ComponentSOF *components = (struct ComponentSOF *) malloc(nb_components*sizeof(struct ComponentSOF));
    if (check_memory_allocation((void *) components)) return EXIT_FAILURE;

    getVerbose() ? printf("\tComposantes :\n"):0;
    for (int8_t i=0; i<nb_components; i++){
        int8_t id_component = read_byte(input, buffer); // ID composante
        
        // Facteur d'échantillonnage
        int8_t sampling_factor = read_byte(input, buffer); // Il faut faire une conversion car c'est un octet et on veut deux bits

        int8_t sampling_factor_x = sampling_factor >> 4;
        int8_t sampling_factor_y = sampling_factor & 0x0F;

        int8_t num_quantization_table = read_byte(input, buffer); // Tables de quantification
        getVerbose() ? printf("\t\tID composante : %d\n", id_component):0;
        getVerbose() ? printf("\t\t\tFacteur d'échantillonnage X : %d\n", sampling_factor_x):0;
        getVerbose() ? printf("\t\t\tFacteur d'échantillonnage Y : %d\n", sampling_factor_y):0;
        getVerbose() ? printf("\t\t\tNuméro de la table de quantification : %d\n", num_quantization_table):0;

        components[i].id = id_component;
        components[i].sampling_factor_x = sampling_factor_x;
        components[i].sampling_factor_y = sampling_factor_y;
        components[i].num_quantization_table = num_quantization_table;
    }

    // On supprime les données précédentes
    free(jpeg->start_of_frame[0]->components);  // attention il faudra modifier lorsque l'on aura plusieurs SOF -> mode progessif

    // Et on met à jour les données
    jpeg->start_of_frame[0]->nb_components = nb_components;
    jpeg->start_of_frame[0]->components = components;

    return EXIT_SUCCESS;
}


//**********************************************************************************************************************
// Récupère les données de la table de Huffman
struct HuffmanTable * get_DHT(FILE *input, unsigned char *buffer) {
    getVerbose() ? printf("\nHuffman table\n"):0;
    int16_t length = two_bytes_to_dec(input); // Longueur du segment
    length = length - 2 - 1; // On enlève la longueur du segment et l'octet de précision 
    
    int8_t id_table = read_byte(input, buffer); // ID de la table
    // Les 4 bits de poids fort indiquent la classe de la table (DC ou AC)
    // 0 : DC or lossless table,
    // 1 : AC
    // Les 4 bits de poids faible indiquent la destination de la table (luminance ou chrominance)
    // Specifies one of four possible destinations at the decoder into which the Huffman table shall be installed
    // 0 : luminance
    // 1 : chrominance
    int8_t class = id_table >> 4;
    int8_t destination = id_table & 0x0F;
    getVerbose() ? printf("\tClasse de la table : %d\n", class):0;
    getVerbose() ? printf("\tDestination de la table : %d\n", destination):0;
    getVerbose() ? printf("\tDonnées de la table : %d", destination):0;

    // Contenu de la table
    unsigned char *huffman_data = (unsigned char *) malloc(length*sizeof(unsigned char));
    if (check_memory_allocation((void *) huffman_data)) {
        fprintf(stderr, RED("Erreur dans l'allocation de la mémoire pour les données de la table de Huffman (huffman_data)\n"));
        return NULL;
    }

    fread(huffman_data, length, 1, input);

    // Affichage des tables de Huffman
    for (int i=0; i<length; i++){
        getVerbose() ? printf("%x", huffman_data[i]):0;
    }
    getVerbose() ? printf("\n"):0;

    struct HuffmanTable *huffman_table = (struct HuffmanTable *) malloc(sizeof(struct HuffmanTable));
    if (check_memory_allocation((void *) huffman_table)) {
        fprintf(stderr, RED("Erreur dans l'allocation de la mémoire pour les données de la table de Huffman (huffman_table)\n"));
        free(huffman_data);
        return NULL;
    }
    huffman_table->class = class;
    huffman_table->destination = destination;
    huffman_table->length = length;
    huffman_table->data = huffman_data;
    if ( (huffman_table->huffman_tree = build_huffman_tree(huffman_data)) == NULL) {
        fprintf(stderr, RED("Erreur dans l'allocation de la mémoire pour les données de la table de Huffman (huffman_tree)\n"));
        free(huffman_data);
        free(huffman_table);
        return NULL;
    }
    huffman_table->set = true;

    return huffman_table;
} 


//**********************************************************************************************************************
// Récupère les données du segment Start_Of_Scan
int8_t get_SOS(FILE *input, unsigned char *buffer, struct JPEG *jpeg){
    getVerbose() ? printf("\nStart of scan + data\n"):0;
    ignore_bytes(input, 2); // Longueur du segment (ignoré)

    int8_t nb_components = read_byte(input, buffer); // Nombre de composantes
    if (nb_components > 3 || nb_components == 2 || nb_components <= 0) {
        fprintf(stderr, RED("Erreur : le nombre de composantes n'est pas supporté par jpeg2ppm\n"));
        return EXIT_FAILURE;
    }
    jpeg->start_of_scan[0]->nb_components = nb_components;
    getVerbose() ? printf("\tNombre de composantes : %d\n", nb_components):0;

    struct ComponentSOS *components = (struct ComponentSOS *) malloc(nb_components * sizeof(struct ComponentSOS));
    if (check_memory_allocation((void *) components)) return EXIT_FAILURE;

    // Composantes
    for (int8_t i=0; i < nb_components; i++){
        int8_t id_component = read_byte(input, buffer); // ID composante
        
        int8_t ht_ids = read_byte(input, buffer); // ID des tables de Huffman utilisées pour cette composante
        int8_t DC_huffman_table_id = ht_ids >> 4;
        int8_t AC_huffman_table_id = (ht_ids & 0x0F) + 2;    // On ajoute 2 car les index des tables_AC commencent à 2

        components[i].id_table = id_component;
        components[i].DC_huffman_table_id = DC_huffman_table_id;
        components[i].AC_huffman_table_id = AC_huffman_table_id;

        getVerbose() ? printf("\tID composante : %d\n", id_component):0;
        getVerbose() ? printf("\tDC_huffman_table_id : %d\n", DC_huffman_table_id):0;
        getVerbose() ? printf("\tAC_huffman_table_id : %d\n", AC_huffman_table_id):0;

        // On met à jour le nombre de mcus à partir des informations du Start Of Frame s'il existe
        // (si oui, la donnée de hauteur et largeur de l'image a été mise à jour dans la structure jpeg)
        if (jpeg->start_of_frame[0]->nb_components == nb_components) {
            size_t nb_mcu_width = (jpeg->width + 7) / 8;
            size_t nb_mcu_height = (jpeg->height + 7) / 8;

            components[i].nb_of_MCUs = nb_mcu_width * nb_mcu_height;
            components[i].MCUs = (int16_t **) malloc(nb_mcu_width * nb_mcu_height * sizeof(int16_t *));
            if (check_memory_allocation((void *) components[i].MCUs)) {
                free(components);
                return EXIT_FAILURE;
            }
            for (size_t j=0; j < nb_mcu_width * nb_mcu_height; j++) {
                (components[i].MCUs)[j] = (int16_t *) malloc(64 * sizeof(int16_t));
                if (check_memory_allocation((void *) components[i].MCUs[j])) {
                    free(components);
                    return EXIT_FAILURE;
                }
            }
        }
    }

    // Paramètres ignorés
    ignore_bytes(input, 3); // Octet de début de spectre, octet de fin de spectre, approximation (ignorés)

    jpeg->start_of_scan[0]->components = components;

    return EXIT_SUCCESS;
}


//**********************************************************************************************************************
// Récupère les données du fichier JPEG
struct JPEG * extract(char *filename) {

    // Ouverture et vérification de la présence du fichier
    FILE *input;
    if( (input = fopen(filename, "r")) == NULL) {
        fprintf(stderr, RED("Impossible d'ouvrir le fichier %s\n"), filename);
        return NULL;
    }

    // Vérification conformité fichier via JPEG Magic number 
    unsigned char first4bytes[FOUR_BYTES_LONG];
    fread(first4bytes, sizeof(first4bytes), 1, input);
    unsigned char JPEG_magic_Number[FOUR_BYTES_LONG] = {SEGMENT_START, SOI, SEGMENT_START, APP0};

    for (int i=0; i<4; i++){
        if (first4bytes[i] != JPEG_magic_Number[i]){
            fprintf(stderr, RED("Le fichier %s n'est pas un fichier JPEG\n"), filename);
            fclose(input);
            return NULL;
        }
    }

    ignore_bytes(input, 2); // Ignorer les 2 octets suivants (longueur du segment)

    // Vérification de la conformité du fichier avec JFIF
    unsigned char JFIF[5] = {0x4A, 0x46, 0x49, 0x46, 0x00}; // JFIF suivi de 0
    unsigned char buffer_2[5];
    fread(buffer_2, sizeof(buffer_2), 1, input);

    for (int i=0; i<5; i++){
        if (buffer_2[i] != JFIF[i]){
            fprintf(stderr, RED("Le fichier %s n'est pas un fichier JPEG\n"), filename);
            fclose(input);
            return NULL;
        }
    }

    // Récupération des données de l'en-tête
    unsigned char buffer[1];    // Buffer
    unsigned char id[1];        // Buffer de lecture pour déterminer le type de segments

    struct JPEG *jpeg = (struct JPEG *) malloc(1 * sizeof(struct JPEG));
    if (check_memory_allocation((void *) jpeg)) {
        fclose(input);
        return NULL;
    }
    
    if (initialize_JPEG_struct(jpeg)) {
        fclose(input);
        return NULL;
    }


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
                if (quantization_table == NULL) {
                    fclose(input);
                    free_JPEG_struct(jpeg);
                    return NULL;
                }
                
                if(quantization_table->id == LUMINANCE_ID) {
                    if (jpeg->quantization_tables[0]->data != NULL) free(jpeg->quantization_tables[0]->data);
                    if (jpeg->quantization_tables[0] != NULL) free(jpeg->quantization_tables[0]);
                    jpeg->quantization_tables[0] = quantization_table;
                } else {
                    if (jpeg->quantization_tables[1]->data != NULL) free(jpeg->quantization_tables[1]->data);
                    if (jpeg->quantization_tables[1] != NULL) free(jpeg->quantization_tables[1]);
                    jpeg->quantization_tables[1] = quantization_table;
                }

            //**********************************************************************************************************************
            } else if (id[0] == SOF_0){

                if (get_SOF(input, buffer, jpeg)) {
                    fclose(input);
                    free_JPEG_struct(jpeg);
                    return NULL;
                }

            //**********************************************************************************************************************
            } else if (id[0] == DHT){
                
                // BREAKING-UPDATE : on possède au maximum 4 tables de Huffman
                // index 0 >>> class|destination : 00 >>> DC|luminance
                // index 1 >>> class|destination : 01 >>> DC|chrominance
                // index 2 >>> class|destination : 10 >>> AC|luminance
                // index 3 >>> class|destination : 11 >>> AC|chrominance
                // Si une nouvelle table de Huffman redéfinie une table déjà existante, on supprime l'ancienne
                struct HuffmanTable *huffman_table = get_DHT(input, buffer);
                if (huffman_table == NULL) {
                    fclose(input);
                    free_JPEG_struct(jpeg);
                    return NULL;
                }
                
                if (huffman_table->class == 0) {    // DC
                    if (huffman_table->destination == 0) {  // Luminance
                        getHighlyVerbose() ? fprintf(stderr, "\t\tTable de Huffman - DC Luminance >>> mise à jour !\n") : 0;
                        free(jpeg->huffman_tables[0]->data);
                        free(jpeg->huffman_tables[0]->huffman_tree);
                        free(jpeg->huffman_tables[0]);
                        jpeg->huffman_tables[0] = huffman_table;
                    } else {    // Chrominance
                        getHighlyVerbose() ? fprintf(stderr, "\t\tTable de Huffman - DC Chrominance >>> mise à jour !\n") : 0;
                        free(jpeg->huffman_tables[1]->data);
                        free(jpeg->huffman_tables[1]->huffman_tree);
                        free(jpeg->huffman_tables[1]);
                        jpeg->huffman_tables[1] = huffman_table;
                    }
                } else {    // AC
                    if (huffman_table->destination == 0) {  // Luminance
                        getHighlyVerbose() ? fprintf(stderr, "\t\tTable de Huffman - AC Luminance >>> mise à jour !\n") : 0;
                        free(jpeg->huffman_tables[2]->data);
                        free(jpeg->huffman_tables[2]->huffman_tree);
                        free(jpeg->huffman_tables[2]);
                        jpeg->huffman_tables[2] = huffman_table;
                    } else {    // Chrominance
                        getHighlyVerbose() ? fprintf(stderr, "\t\tTable de Huffman - AC Chrominance >>> mise à jour !\n") : 0;
                        free(jpeg->huffman_tables[3]->data);
                        free(jpeg->huffman_tables[3]->huffman_tree);
                        free(jpeg->huffman_tables[3]);
                        jpeg->huffman_tables[3] = huffman_table;
                    }
                }

                getHighlyVerbose() ? fprintf(stderr, "\t\tTables de Huffman:\n") : 0;
                getHighlyVerbose() ? fprintf(stderr, "\t\t\tDC Luminance   : %p\n", jpeg->huffman_tables[0]) : 0;
                getHighlyVerbose() ? fprintf(stderr, "\t\t\tDC Chrominance : %p\n", jpeg->huffman_tables[1]) : 0;
                getHighlyVerbose() ? fprintf(stderr, "\t\t\tAC Luminance   : %p\n", jpeg->huffman_tables[2]) : 0;
                getHighlyVerbose() ? fprintf(stderr, "\t\t\tAC Chrominance : %p\n", jpeg->huffman_tables[3]) : 0;

            //**********************************************************************************************************************
            } else if (id[0] == SOS){
                
                if (get_SOS(input, buffer, jpeg)) {
                    fclose(input);
                    free_JPEG_struct(jpeg);
                    return NULL;
                }

                // On lit la data en enlevant les 0 (à cause du byte stuffing)
                unsigned long long data_size = INITIAL_DATA_SIZE; // On donne une estimation de la taille des données à récupérer

                unsigned long long nb_data = 0;

                while (!feof(input)){ // On arrête la boucle si on arrive à la fin du fichier sans avoir lu de marker EOF
                    fread(buffer, 1, 1, input);

                    // On réalloue de la mémoire si on a besoin
                    if(nb_data >= data_size -1){
                        data_size *= 2;
                        jpeg->image_data = realloc(jpeg->image_data, data_size * sizeof(unsigned char *));
                        if (check_memory_allocation((void *) jpeg->image_data)) {
                            fclose(input);
                            free_JPEG_struct(jpeg);
                            return NULL;
                        }
                    }

                    if (buffer[0] == SEGMENT_START){
                        fread(buffer, 1, 1, input);

                        if (buffer[0] == 0x00){ // On a du byte stuffing et on le supprime
                            jpeg->image_data[nb_data] = SEGMENT_START;
                            nb_data++;

                        } else if (buffer[0] == EOI){   // On ne prend pas en compte le dernier 0xff du marker EOI
                            jpeg->image_data_size_in_bits = 8 * nb_data;
                            // On a fini la lecture des données
                            getVerbose() ? printf("\tLongueur du bitstream_image_data (bits) : %lld\n", 8 * nb_data):0;
                            getVerbose() ? printf("\tBitstream : "):0;
                            for (size_t i =0; i < nb_data; i++) {
                                getVerbose() ? printf("%x", (jpeg->image_data[i])):0;
                            }
                            getVerbose() ? printf("\nFin du fichier\n\n"):0;
                            fclose(input);
                            return jpeg;

                        } else if (feof(input)){    // On atteint la fin du fichier avant d'avoir lu un marker EOI
                            getVerbose() ? printf(RED("Fin du fichier atteinte avant d'avoir lu un EOI !!!\n")):0;
                            fclose(input);
                            free_JPEG_struct(jpeg);
                            return NULL;

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
