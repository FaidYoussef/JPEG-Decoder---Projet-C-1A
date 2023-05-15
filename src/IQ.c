#include <IQ.h>

const int8_t zigzag[]={
    0, 1, 8, 16, 9, 2, 3, 10,
    17, 24, 32, 25, 18, 11, 4, 5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13, 6, 7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63
};


//Quantization function using quant_table
int* quantize(int *block, int *quant_table_DC, int *quant_table_AC) {
    int* qblock = malloc(64 * sizeof(int));
    check_memory_allocation((void *) qblock);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if(i==0 && j==0) {
                block[0] = qblock[0] * quant_table_DC[0];
            } else {
                qblock[i * 8 + j] = (block[i * 8 + j] / quant_table_AC[i * 8 + j]);
            }
        }
    }

    return qblock;
}

int8_t IQ_function(struct JPEG *jpeg, int8_t component_index){
    // On récupère la table de quantification associée à la composante
    int8_t qt_index = get_num_quantization_table((&(get_JPEG_sof(jpeg)[0]))->components[component_index])
    struct QuantizationTable *qt = get_JPEG_qt(jpeg)[qt_index];

    for (int i = 0; i < 64; i++) {
        if(i==0 ) {
            block[0] = qblock[0] * quant_table_DC[0];
        } else {
            block[i] = qblock[i] * quant_table_AC[i];
        }
    }
}


// Inverse quantization function using quant_table
// qblock : les données qui sont quantifiées
int8_t inv_quantize(struct JPEG * jpeg) {
    size_t nb_mcu_width = 0;
    size_t nb_mcu_height = 0;
    if (get_JPEG_width(jpeg) % 8 == 0) {
        nb_mcu_width =  get_JPEG_width(jpeg) / 8;
    } else {
        nb_mcu_width = (get_JPEG_width(jpeg) / 8) + 1;
    }
    if (get_JPEG_width(jpeg) % 8 == 0) {
        nb_mcu_height =  get_JPEG_width(jpeg) / 8;
    } else {
        nb_mcu_height = (get_JPEG_width(jpeg) / 8) + 1;
    }

    // On parcours tous les MCUs de l'image
    for (size_t i = 0; i < nb_mcu_width * nb_mcu_height; i++){
        // Prévoir possibilité de reset-er les données `previous_DC_values` dans le cas où l'on a
        // plusieurs scans/frames ---> mode progressif
        
        // On parcours toutes les composantes
        for (int8_t j = 0; j < get_sos_nb_components(get_JPEG_sos(jpeg)[0]); ++j) {   // attention ici l'index 0 correspond au 1er scan/frame ... prévoir d'intégrer un index pour le mode progressif
            if (IQ_function(jpeg, i)) {
                return EXIT_FAILURE;
            }
        }
    }
    return EXIT_SUCCESS;
}

void print_block(int block[8][8]){
    for (int i = 0; i < 8; i++) {
        for (int j =0; j < 8; j++) {
            printf("%d ", block[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
