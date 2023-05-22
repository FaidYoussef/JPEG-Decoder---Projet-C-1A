#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <extract.h>


int main() {

    //**************************************************************************************************************************
    // On active les modes verboses pour l'affichage sur stdout et stderr
    setVerbose(true);
    setHighlyVerbose(true);


    //**************************************************************************************************************************
    // On prépare les threads d'exécution
    int fils1;
    int fils2;
    int fils3;
    int fils4;
    int fils5;
    int fils6;
    int fils7;
    int fils8;
    int fils9;
    int fils10;
    int fils11;
    int fils12;
    int fils13;
    int fils14;
    int fils15;
    int fils16;
    int fils17;
    int fils18;
    int fils19;
    int fils20;
    int fils21;
    int fils22;
    int fils23;
    int fils24;
    int fils25;


    char *programme1 = "jpeg2ppm tests/images/poupoupidou_invalid_component_id_in_QT___ERROR_-_INCONSISTENT_DATA_-_extract.c_get_qt.jpg";
    char *programme2 = "jpeg2ppm tests/images/poupoupidou_invalid_nb_of_components_2_in_SOF___ERROR_-_INCONSISTENT_DATA_-_extract.c_get_SOF_nb_components.jpg";
    char *programme3 = "jpeg2ppm tests/images/poupoupidou_invalid_nb_of_components_4_in_SOF___ERROR_-_INCONSISTENT_DATA_-_extract.c_get_SOF_nb_components.jpg";
    char *programme4 = "jpeg2ppm tests/images/poupoupidou_invalid_nb_of_components_7_in_SOF___ERROR_-_INCONSISTENT_DATA_-_extract.c_get_SOF_nb_components.jpg";
    char *programme5 = "jpeg2ppm tests/images/poupoupidou_invalid_huffman_table_invalid_level_number___ERROR_-_INCONSISTENT_DATA_-_extract.c_get_DHT_huffman_table_build_huffman_tree.jpg";
    char *programme6 = "jpeg2ppm tests/images/poupoupidou_invalid_huffman_table_invalid_too_much_symbols___NO-ERROR.jpg";   // génère bien le fichier
    char *programme7 = "jpeg2ppm tests/images/poupoupidou_invalid_nb_of_components_2_in_SOS___ERROR_-_INCONSISTENT_DATA_-_extract.c_get_SOS_nb_components.jpg";
    char *programme8 = "jpeg2ppm tests/images/poupoupidou_invalid_nb_of_components_4_in_SOS___ERROR_-_INCONSISTENT_DATA_-_extract.c_get_SOS_nb_components.jpg";
    char *programme9 = "jpeg2ppm tests/images/poupoupidou_invalid_nb_of_components_7_in_SOS___ERROR_-_INCONSISTENT_DATA_-_extract.c_get_SOS_nb_components.jpg";
    char *programme10 = "jpeg2ppm tests/images/not_a_jpeg_file___ERROR_-_FORMAT_-_extract.c_extract_with_file.pdf";
    char *programme11 = "jpeg2ppm tests/images/poupoupidou_not_a_JFIF_file_no_APP0_segment___ERROR_-_FORMAT_-_extract.c_extract__with_file.jpg";
    char *programme12 = "jpeg2ppm tests/images/poupoupidou_no_EOI_segment___ERROR_-_INCONSISTENT_DATA_-_extract.c_extract_EOI_marker_is_missing.jpg";
    char *programme13 = "jpeg2ppm tests/images/invader_invalid_encoded_data_DC___ERROR_-_INCONSISTENT_DATA_-_huffman.c_decode_MCU_invalid_huffman_code.jpeg";
    char *programme14 = "jpeg2ppm tests/images/invader_invalid_encoded_data_AC___ERROR_-_INCONSISTENT_DATA_-_huffman.c_decode_MCU_invalid_huffman_code.jpeg";
    char *programme15 = "jpeg2ppm tests/images/invader_invalid_encoded_data_DC_magnitude_over_11___ERROR_-_INCONSISTENT_DATA_-_huffman.c_decode_MCU_magnitude_over_11.jpeg";
    char *programme16 = "jpeg2ppm tests/images/invader_invalid_encoded_data_not_enough_values_before_eof___ERROR_-_INCONSISTENT_DATA_-_huffman.c_decode_MCU_not_enough_values_for_current_MCU.jpeg";
    char *programme17 = "jpeg2ppm tests/images/invader_invalid_encoded_data_wrong_RLE_use___ERROR_-_INCONSISTENT_DATA_-_huffman.c_decode_MCU_RLE_exceeded_MCU_size.jpeg";
    char *programme18 = "jpeg2ppm tests/images/invader_invalid_encoded_data_not_enough_values_before_eof2___ERROR_-_INCONSISTENT_DATA_-_huffman.c_decode_MCU_not_enough_values_for_current_MCU.jpeg";
    char *programme19 = "jpeg2ppm tests/images/invader_invalid_marker_length.jpeg";
    char *programme20 = "jpeg2ppm tests/images/polyglot_JFIF_PDF.pdf";
    char *programme21 = "jpeg2ppm tests/images/poupoupidou_invalid_sampling_factor___ERROR_-_INCONSISTENT_DATA_-_extract.c_get_SOF_sampling_factor.jpg"; // erreur pas relevée par le programme
    char *programme22 = "jpeg2ppm tests/images/poupoupidou_invalid_huffman_table_invalid_level_number2___ERROR_-_INCONSISTENT_DATA_-_extract.c_get_DHT_huffman_table_build_huffman_tree.jpg";    // segfault
    char *programme23 = "jpeg2ppm tests/images/poupoupidou_invalid_huffman_table_invalid_not_enough_symbols___ERROR_-_INCONSISTENT_DATA_-_extract.c_get_DHT_huffman_table_build_huffman_tree.jpg";   // segfault
    char *programme24 = "jpeg2ppm tests/images/poupoupidou_no_huffman_tables___ERROR_-_INCONSISTENT_DATA_-_huffman.c_build_huffman_tree.jpg";   // segfault
    char *programme25 = "jpeg2ppm tests/images/invader_no_data_between_SOI_APP0_and_EOI.jpeg";  // segfault

    char *arguments[] = {NULL};
    
    
    //**************************************************************************************************************************
    // Test 1
    switch (fils1 = fork()) {
        case -1:
            erreur("Error can't fork !");
        case 0:
            execv(programme1, arguments);
            fprintf(stderr, RED("Test 1 KO - Error in execution of jpeg2ppm\n"));
            exit(EXIT_FAILURE); // normalement pas besoin, si on revient ici c'est que execv s'est mal passé
    }

    wait(&fils1);
    fprintf(stderr, GREEN("Test 1 OK\n"));


    //**************************************************************************************************************************
    // Test 1
    switch (fils2 = fork()) {
        case -1:
            erreur("Error can't fork !");
        case 0:
            execv(programme2, arguments);
            fprintf(stderr, RED("Test 2 KO - Error in execution of jpeg2ppm\n"));
            exit(EXIT_FAILURE); // normalement pas besoin, si on revient ici c'est que execv s'est mal passé
    }

    wait(&fils2);
    fprintf(stderr, GREEN("Test 2 OK\n"));


    //**************************************************************************************************************************
    // Test 3
    switch (fils3 = fork()) {
        case -1:
            erreur("Error can't fork !");
        case 0:
            execv(programme3, arguments);
            fprintf(stderr, RED("Test 3 KO - Error in execution of jpeg2ppm\n"));
            exit(EXIT_FAILURE); // normalement pas besoin, si on revient ici c'est que execv s'est mal passé
    }

    wait(&fils2);
    fprintf(stderr, GREEN("Test 3 OK\n"));


    //**************************************************************************************************************************
    // Test 4
    switch (fils4 = fork()) {
        case -1:
            erreur("Error can't fork !");
        case 0:
            execv(programme4, arguments);
            fprintf(stderr, RED("Test 4 KO - Error in execution of jpeg2ppm\n"));
            exit(EXIT_FAILURE); // normalement pas besoin, si on revient ici c'est que execv s'est mal passé
    }

    wait(&fils2);
    fprintf(stderr, GREEN("Test 4 OK\n"));


    //**************************************************************************************************************************
    // Test 5
    switch (fils5 = fork()) {
        case -1:
            erreur("Error can't fork !");
        case 0:
            execv(programme5, arguments);
            fprintf(stderr, RED("Test 5 KO - Error in execution of jpeg2ppm\n"));
            exit(EXIT_FAILURE); // normalement pas besoin, si on revient ici c'est que execv s'est mal passé
    }

    wait(&fils5);
    fprintf(stderr, GREEN("Test 5 OK\n"));


    //**************************************************************************************************************************
    // Test 6
    switch (fils6 = fork()) {
        case -1:
            erreur("Error can't fork !");
        case 0:
            execv(programme6, arguments);
            fprintf(stderr, RED("Test 6 KO - Error in execution of jpeg2ppm\n"));
            exit(EXIT_FAILURE); // normalement pas besoin, si on revient ici c'est que execv s'est mal passé
    }

    wait(&fils6);
    fprintf(stderr, GREEN("Test 6 OK\n"));

    //**************************************************************************************************************************
    // Test 7
    switch (fils7 = fork()) {
        case -1:
            erreur("Error can't fork !");
        case 0:
            execv(programme7, arguments);
            fprintf(stderr, RED("Test 7 KO - Error in execution of jpeg2ppm\n"));
            exit(EXIT_FAILURE); // normalement pas besoin, si on revient ici c'est que execv s'est mal passé
    }

    wait(&fils7);
    fprintf(stderr, GREEN("Test 7 OK\n"));

    //**************************************************************************************************************************
    // Test 8
    switch (fils8 = fork()) {
        case -1:
            erreur("Error can't fork !");
        case 0:
            execv(programme8, arguments);
            fprintf(stderr, RED("Test 8 KO - Error in execution of jpeg2ppm\n"));
            exit(EXIT_FAILURE); // normalement pas besoin, si on revient ici c'est que execv s'est mal passé
    }

    wait(&fils8);
    fprintf(stderr, GREEN("Test 8 OK\n"));

    //**************************************************************************************************************************
    // Test 9
    switch (fils9 = fork()) {
        case -1:
            erreur("Error can't fork !");
        case 0:
            execv(programme9, arguments);
            fprintf(stderr, RED("Test 9 KO - Error in execution of jpeg2ppm\n"));
            exit(EXIT_FAILURE); 
    }

    wait(&fils9);
    fprintf(stderr, GREEN("Test 9 OK\n"));


    //**************************************************************************************************************************
    // Test 10
    switch (fils10 = fork()) {
        case -1:
            erreur("Error can't fork !");
        case 0:
            execv(programme10, arguments);
            fprintf(stderr, RED("Test 10 KO - Error in execution of jpeg2ppm\n"));
            exit(EXIT_FAILURE); 
    }

    wait(&fils10);
    fprintf(stderr, GREEN("Test 10 OK\n"));

    //**************************************************************************************************************************
    // Test 11
    switch (fils11 = fork()) {
        case -1:
            erreur("Error can't fork !");
        case 0:
            execv(programme11, arguments);
            fprintf(stderr, RED("Test 11 KO - Error in execution of jpeg2ppm\n"));
            exit(EXIT_FAILURE); 
    }

    wait(&fils11);
    fprintf(stderr, GREEN("Test 11 OK\n"));

    //**************************************************************************************************************************
    // Test 12
    switch (fils12 = fork()) {
        case -1:
            erreur("Error can't fork !");
        case 0:
            execv(programme12, arguments);
            fprintf(stderr, RED("Test 12 KO - Error in execution of jpeg2ppm\n"));
            exit(EXIT_FAILURE); 
    }

    wait(&fils12);
    fprintf(stderr, GREEN("Test 12 OK\n"));

    //**************************************************************************************************************************
    // Test 13
    switch (fils13 = fork()) {
        case -1:
            erreur("Error can't fork !");
        case 0:
            execv(programme13, arguments);
            fprintf(stderr, RED("Test 13 KO - Error in execution of jpeg2ppm\n"));
            exit(EXIT_FAILURE); 
    }

    wait(&fils13);
    fprintf(stderr, GREEN("Test 13 OK\n"));

    //**************************************************************************************************************************
    // Test 14
    switch (fils14 = fork()) {
        case -1:
            erreur("Error can't fork !");
        case 0:
            execv(programme14, arguments);
            fprintf(stderr, RED("Test 14 KO - Error in execution of jpeg2ppm\n"));
            exit(EXIT_FAILURE); 
    }

    wait(&fils14);
    fprintf(stderr, GREEN("Test 14 OK\n"));

    //**************************************************************************************************************************
    // Test 15
    switch (fils15 = fork()) {
        case -1:
            erreur("Error can't fork !");
        case 0:
            execv(programme15, arguments);
            fprintf(stderr, RED("Test 15 KO - Error in execution of jpeg2ppm\n"));
            exit(EXIT_FAILURE); 
    }

    wait(&fils15);
    fprintf(stderr, GREEN("Test 15 OK\n"));

    //**************************************************************************************************************************
    // Test 16
    switch (fils16 = fork()) {
        case -1:
            erreur("Error can't fork !");
        case 0:
            execv(programme16, arguments);
            fprintf(stderr, RED("Test 16 KO - Error in execution of jpeg2ppm\n"));
            exit(EXIT_FAILURE); 
    }

    wait(&fils16);
    fprintf(stderr, GREEN("Test 16 OK\n"));

    //**************************************************************************************************************************
    // Test 17
    switch (fils17 = fork()) {
        case -1:
            erreur("Error can't fork !");
        case 0:
            execv(programme17, arguments);
            fprintf(stderr, RED("Test 17 KO - Error in execution of jpeg2ppm\n"));
            exit(EXIT_FAILURE); 
    }

    wait(&fils17);
    fprintf(stderr, GREEN("Test 17 OK\n"));

    //**************************************************************************************************************************
    // Test 18
    switch (fils18 = fork()) {
        case -1:
            erreur("Error can't fork !");
        case 0:
            execv(programme18, arguments);
            fprintf(stderr, RED("Test 18 KO - Error in execution of jpeg2ppm\n"));
            exit(EXIT_FAILURE); 
    }

    wait(&fils18);
    fprintf(stderr, GREEN("Test 18 OK\n"));

    //**************************************************************************************************************************
    // Test 19
    switch (fils19 = fork()) {
        case -1:
            erreur("Error can't fork !");
        case 0:
            execv(programme19, arguments);
            fprintf(stderr, RED("Test 19 KO - Error in execution of jpeg2ppm\n"));
            exit(EXIT_FAILURE); 
    }

    wait(&fils19);
    fprintf(stderr, GREEN("Test 19 OK\n"));

    //**************************************************************************************************************************
    // Test 20
    switch (fils20 = fork()) {
        case -1:
            erreur("Error can't fork !");
        case 0:
            execv(programme20, arguments);
            fprintf(stderr, RED("Test 20 KO - Error in execution of jpeg2ppm\n"));
            exit(EXIT_FAILURE); 
    }

    wait(&fils20);
    fprintf(stderr, GREEN("Test 20 OK\n"));

    //**************************************************************************************************************************
    // Test 21
    switch (fils21 = fork()) {
        case -1:
            erreur("Error can't fork !");
        case 0:
            execv(programme21, arguments);
            fprintf(stderr, RED("Test 21 KO - Error in execution of jpeg2ppm\n"));
            exit(EXIT_FAILURE); 
    }

    wait(&fils21);
    fprintf(stderr, GREEN("Test 21 OK\n"));

    //**************************************************************************************************************************
    // Test 22
    switch (fils22 = fork()) {
        case -1:
            erreur("Error can't fork !");
        case 0:
            execv(programme22, arguments);
            fprintf(stderr, RED("Test 22 KO - Error in execution of jpeg2ppm\n"));
            exit(EXIT_FAILURE); 
    }

    wait(&fils22);
    fprintf(stderr, GREEN("Test 22 OK\n"));

    //**************************************************************************************************************************
    // Test 23
    switch (fils23 = fork()) {
        case -1:
            erreur("Error can't fork !");
        case 0:
            execv(programme23, arguments);
            fprintf(stderr, RED("Test 23 KO - Error in execution of jpeg2ppm\n"));
            exit(EXIT_FAILURE); 
    }

    wait(&fils23);
    fprintf(stderr, GREEN("Test 23 OK\n"));

    //**************************************************************************************************************************
    // Test 24
    switch (fils24 = fork()) {
        case -1:
            erreur("Error can't fork !");
        case 0:
            execv(programme24, arguments);
            fprintf(stderr, RED("Test 24 KO - Error in execution of jpeg2ppm\n"));
            exit(EXIT_FAILURE); 
    }

    wait(&fils24);
    fprintf(stderr, GREEN("Test 24 OK\n"));

    //**************************************************************************************************************************
    // Test 25
    switch (fils25 = fork()) {
        case -1:
            erreur("Error can't fork !");
        case 0:
            execv(programme25, arguments);
            fprintf(stderr, RED("Test 25 KO - Error in execution of jpeg2ppm\n"));
            exit(EXIT_FAILURE); 
    }

    wait(&fils25);
    fprintf(stderr, GREEN("Test 25 OK\n"));


    return EXIT_SUCCESS;
}