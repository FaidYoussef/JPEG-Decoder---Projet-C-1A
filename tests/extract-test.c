#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include <utils.h>
#include <verbose.h>



void sigHandler(int signo) {
    if (signo == SIGSEGV) {
        printf("Child process terminated with a segmentation fault (SIGSEGV).\n");
    }
    exit(0);
}


int main() {

    //**************************************************************************************************************************
    // On active les modes verboses pour l'affichage sur stdout et stderr
    setVerbose(true);
    setHighlyVerbose(true);

    //**************************************************************************************************************************
    // On prépare la liste des fichiers à tester

    char* test_files[] = {   // Array of file names
        "./tests/images-tests/poupoupidou_invalid_component_id_in_QT___ERROR_-_INCONSISTENT_DATA_-_extract.c_get_qt.jpg",
        "./tests/images-tests/poupoupidou_invalid_nb_of_components_2_in_SOF___ERROR_-_INCONSISTENT_DATA_-_extract.c_get_SOF_nb_components.jpg",
        "./tests/images-tests/poupoupidou_invalid_nb_of_components_4_in_SOF___ERROR_-_INCONSISTENT_DATA_-_extract.c_get_SOF_nb_components.jpg",
        "./tests/images-tests/poupoupidou_invalid_nb_of_components_7_in_SOF___ERROR_-_INCONSISTENT_DATA_-_extract.c_get_SOF_nb_components.jpg",
        "./tests/images-tests/poupoupidou_invalid_huffman_table_invalid_level_number___ERROR_-_INCONSISTENT_DATA_-_extract.c_get_DHT_huffman_table_build_huffman_tree.jpg",
        "./tests/images-tests/poupoupidou_invalid_huffman_table_invalid_too_much_symbols___NO-ERROR.jpg",   // génère bien le fichier
        "./tests/images-tests/poupoupidou_invalid_nb_of_components_2_in_SOS___ERROR_-_INCONSISTENT_DATA_-_extract.c_get_SOS_nb_components.jpg",
        "./tests/images-tests/poupoupidou_invalid_nb_of_components_4_in_SOS___ERROR_-_INCONSISTENT_DATA_-_extract.c_get_SOS_nb_components.jpg",
        "./tests/images-tests/poupoupidou_invalid_nb_of_components_7_in_SOS___ERROR_-_INCONSISTENT_DATA_-_extract.c_get_SOS_nb_components.jpg",
        "./tests/images-tests/not_a_jpeg_file___ERROR_-_FORMAT_-_extract.c_extract_with_file.pdf",
        "./tests/images-tests/poupoupidou_not_a_JFIF_file_no_APP0_segment___ERROR_-_FORMAT_-_extract.c_extract__with_file.jpg",
        "./tests/images-tests/poupoupidou_no_EOI_segment___ERROR_-_INCONSISTENT_DATA_-_extract.c_extract_EOI_marker_is_missing.jpg",
        "./tests/images-tests/invader_invalid_encoded_data_DC___ERROR_-_INCONSISTENT_DATA_-_huffman.c_decode_MCU_invalid_huffman_code.jpeg",
        "./tests/images-tests/invader_invalid_encoded_data_AC___ERROR_-_INCONSISTENT_DATA_-_huffman.c_decode_MCU_invalid_huffman_code.jpeg",
        "./tests/images-tests/invader_invalid_encoded_data_DC_magnitude_over_11___ERROR_-_INCONSISTENT_DATA_-_huffman.c_decode_MCU_magnitude_over_11.jpeg",
        "./tests/images-tests/invader_invalid_encoded_data_not_enough_values_before_eof___ERROR_-_INCONSISTENT_DATA_-_huffman.c_decode_MCU_not_enough_values_for_current_MCU.jpeg",
        "./tests/images-tests/invader_invalid_encoded_data_wrong_RLE_use___ERROR_-_INCONSISTENT_DATA_-_huffman.c_decode_MCU_RLE_exceeded_MCU_size.jpeg",
        "./tests/images-tests/invader_invalid_encoded_data_not_enough_values_before_eof2___ERROR_-_INCONSISTENT_DATA_-_huffman.c_decode_MCU_not_enough_values_for_current_MCU.jpeg",
        "./tests/images-tests/invader_invalid_marker_length.jpeg",
        "./tests/images-tests/polyglot_JFIF_PDF.jpeg",  // génère bien le fichier
        "./tests/images-tests/invader_no_data_between_SOI_APP0_and_EOI.jpeg",
        "./tests/images-tests/poupoupidou_invalid_sampling_factor___ERROR_-_INCONSISTENT_DATA_-_extract.c_get_SOF_sampling_factor.jpg",
        "./tests/images-tests/poupoupidou_invalid_huffman_table_invalid_level_number2___ERROR_-_INCONSISTENT_DATA_-_extract.c_get_DHT_huffman_table_build_huffman_tree.jpg",
        "./tests/images-tests/poupoupidou_invalid_huffman_table_invalid_not_enough_symbols___ERROR_-_INCONSISTENT_DATA_-_extract.c_get_DHT_huffman_table_build_huffman_tree.jpg",
        "./tests/images-tests/poupoupidou_no_huffman_tables___ERROR_-_INCONSISTENT_DATA_-_huffman.c_build_huffman_tree.jpg"
    };

    int num_of_tests = sizeof(test_files) / sizeof(test_files[0]); // Calculate the number of files

    //**************************************************************************************************************************
    signal(SIGSEGV, sigHandler); // Set signal handler for SIGSEGV

    //**************************************************************************************************************************
    // On lance les tests

    for (int i = 0; i < num_of_tests; i++) {
        pid_t pid = fork(); // Create a child process

        if (pid < 0) {
            perror("fork() failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            execl("pwd", "your_program", NULL); // Execute your program with file as an argument
            execl("jpeg2ppm", "jpeg2ppm", test_files[i], NULL); // Execute your program with file as an argument
            perror(RED("\nexecl() failed\n"));
            fprintf(stderr, RED("\nTest %d KO\n"), i+1);
            exit(EXIT_FAILURE);
        } else {
            // Parent process
            int status;
             waitpid(pid, &status, 0); // Wait for the child process to finish

            if (WIFEXITED(status)) {
                fprintf(stderr, GREEN("Test %d OK\n\n"), i+1);
            } else if (WIFSIGNALED(status)) {
                int signalType = WTERMSIG(status);
                if (signalType == SIGSEGV) {
                    fprintf(stderr, RED("Child process terminated due to a segmentation fault (SIGSEGV).\n"));
                    fprintf(stderr, RED("Test %d KO\n\n"), i+1);
                } else {
                    fprintf(stderr, RED("Child process terminated due to a signal: %d\n"), signalType);
                    fprintf(stderr, RED("Test %d KO\n\n"), i+1);
                }
            }
        }        
    }

    return EXIT_SUCCESS;
}
