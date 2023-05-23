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
        "./tests/images-tests/poupoupidou_invalid_huffman_table_invalid_level_number2___ERROR_-_INCONSISTENT_DATA_-_extract.c_get_DHT_huffman_table_build_huffman_tree.jpg",    // segfault
        "./tests/images-tests/poupoupidou_invalid_huffman_table_invalid_not_enough_symbols___ERROR_-_INCONSISTENT_DATA_-_extract.c_get_DHT_huffman_table_build_huffman_tree.jpg",   // segfault
        "./tests/images-tests/poupoupidou_no_huffman_tables___ERROR_-_INCONSISTENT_DATA_-_huffman.c_build_huffman_tree.jpg" // segfault
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

//++++++++++++++++++++++++++++













    
//     //**************************************************************************************************************************
//     // Test 1
//     if ((fils1 = fork()) == -1) {
//         fprintf(stderr, RED("Error can't fork !"));
//     } else {
//     execv(programme1, arguments);
//     // fprintf(stderr, RED("Test 1 KO - Error in execution of jpeg2ppm\n"));
//     // exit(EXIT_FAILURE); // normalement pas besoin, si on revient ici c'est que execv s'est mal passé
//     }

//     wait(&fils1);
//     fprintf(stderr, GREEN("Test 1 OK\n"));


//     **************************************************************************************************************************
//     Test 2
//     if ((fils2 = fork()) == -1) {
//         fprintf(stderr, RED("Error can't fork !"));
//     } else {
//         execv(programme2, arguments);
//         fprintf(stderr, RED("Test 2 KO - Error in execution of jpeg2ppm\n"));
//         exit(EXIT_FAILURE); // normalement pas besoin, si on revient ici c'est que execv s'est mal passé
//     }

//     wait(&fils2);
//     fprintf(stderr, GREEN("Test 2 OK\n"));


//     //**************************************************************************************************************************
//     // Test 3
//     if ((fils3 = fork()) == -1) {
//         fprintf(stderr, RED("Error can't fork !"));
//     } else {
//         execv(programme3, arguments);
//         fprintf(stderr, RED("Test 3 KO - Error in execution of jpeg2ppm\n"));
//         exit(EXIT_FAILURE); // normalement pas besoin, si on revient ici c'est que execv s'est mal passé
//     }

//     wait(&fils3);
//     fprintf(stderr, GREEN("Test 3 OK\n"));


//     //**************************************************************************************************************************
//     // Test 4
//     if ((fils4 = fork()) == -1) {
//         fprintf(stderr, RED("Error can't fork !"));
//     } else {
//         execv(programme4, arguments);
//         fprintf(stderr, RED("Test 4 KO - Error in execution of jpeg2ppm\n"));
//         exit(EXIT_FAILURE); // normalement pas besoin, si on revient ici c'est que execv s'est mal passé
//     }

//     wait(&fils4);
//     fprintf(stderr, GREEN("Test 4 OK\n"));


//     //**************************************************************************************************************************
//     // Test 5
//     if ((fils5 = fork()) == -1) {
//         fprintf(stderr, RED("Error can't fork !"));
//     } else {
//         execv(programme5, arguments);
//         fprintf(stderr, RED("Test 5 KO - Error in execution of jpeg2ppm\n"));
//         exit(EXIT_FAILURE); // normalement pas besoin, si on revient ici c'est que execv s'est mal passé
//     }

//     wait(&fils5);
//     fprintf(stderr, GREEN("Test 5 OK\n"));


//     //**************************************************************************************************************************
//     // Test 6
//     if ((fils6 = fork()) == -1) {
//         fprintf(stderr, RED("Error can't fork !"));
//     } else {
//         execv(programme6, arguments);
//         fprintf(stderr, RED("Test 6 KO - Error in execution of jpeg2ppm\n"));
//         exit(EXIT_FAILURE); // normalement pas besoin, si on revient ici c'est que execv s'est mal passé
//     }

//     wait(&fils6);
//     fprintf(stderr, GREEN("Test 6 OK\n"));

//     //**************************************************************************************************************************
//     // Test 7
//     if ((fils7 = fork()) == -1) {
//         fprintf(stderr, RED("Error can't fork !"));
//     } else {
//         execv(programme7, arguments);
//         fprintf(stderr, RED("Test 7 KO - Error in execution of jpeg2ppm\n"));
//         exit(EXIT_FAILURE); // normalement pas besoin, si on revient ici c'est que execv s'est mal passé
//     }

//     wait(&fils7);
//     fprintf(stderr, GREEN("Test 7 OK\n"));

//     //**************************************************************************************************************************
//     // Test 8
//     if ((fils8 = fork()) == -1) {
//         fprintf(stderr, RED("Error can't fork !"));
//     } else {
//         execv(programme8, arguments);
//         fprintf(stderr, RED("Test 8 KO - Error in execution of jpeg2ppm\n"));
//         exit(EXIT_FAILURE); // normalement pas besoin, si on revient ici c'est que execv s'est mal passé
//     }

//     wait(&fils8);
//     fprintf(stderr, GREEN("Test 8 OK\n"));

//     //**************************************************************************************************************************
//     // Test 9
//     if ((fils9 = fork()) == -1) {
//         fprintf(stderr, RED("Error can't fork !"));
//     } else {
//         execv(programme9, arguments);
//         fprintf(stderr, RED("Test 9 KO - Error in execution of jpeg2ppm\n"));
//         exit(EXIT_FAILURE); 
//     }

//     wait(&fils9);
//     fprintf(stderr, GREEN("Test 9 OK\n"));


//     //**************************************************************************************************************************
//     // Test 10
//     if ((fils10 = fork()) == -1) {
//         fprintf(stderr, RED("Error can't fork !"));
//     } else {
//         execv(programme10, arguments);
//         fprintf(stderr, RED("Test 10 KO - Error in execution of jpeg2ppm\n"));
//         exit(EXIT_FAILURE); 
//     }

//     wait(&fils10);
//     fprintf(stderr, GREEN("Test 10 OK\n"));

//     //**************************************************************************************************************************
//     // Test 11
//     if ((fils11 = fork()) == -1) {
//         fprintf(stderr, RED("Error can't fork !"));
//     } else {
//         execv(programme11, arguments);
//         fprintf(stderr, RED("Test 11 KO - Error in execution of jpeg2ppm\n"));
//         exit(EXIT_FAILURE); 
//     }

//     wait(&fils11);
//     fprintf(stderr, GREEN("Test 11 OK\n"));

//     //**************************************************************************************************************************
//     // Test 12
//     if ((fils12 = fork()) == -1) {
//         fprintf(stderr, RED("Error can't fork !"));
//     } else {
//         execv(programme12, arguments);
//         fprintf(stderr, RED("Test 12 KO - Error in execution of jpeg2ppm\n"));
//         exit(EXIT_FAILURE); 
//     }

//     wait(&fils12);
//     fprintf(stderr, GREEN("Test 12 OK\n"));

//     //**************************************************************************************************************************
//     // Test 13
//     if ((fils13 = fork()) == -1) {
//         fprintf(stderr, RED("Error can't fork !"));
//     } else {
//         execv(programme13, arguments);
//         fprintf(stderr, RED("Test 13 KO - Error in execution of jpeg2ppm\n"));
//         exit(EXIT_FAILURE); 
//     }

//     wait(&fils13);
//     fprintf(stderr, GREEN("Test 13 OK\n"));

//     //**************************************************************************************************************************
//     // Test 14
//     if ((fils14 = fork()) == -1) {
//         fprintf(stderr, RED("Error can't fork !"));
//     } else {
//         execv(programme14, arguments);
//         fprintf(stderr, RED("Test 14 KO - Error in execution of jpeg2ppm\n"));
//         exit(EXIT_FAILURE); 
//     }

//     wait(&fils14);
//     fprintf(stderr, GREEN("Test 14 OK\n"));

//     //**************************************************************************************************************************
//     // Test 15
//     if ((fils15 = fork()) == -1) {
//         fprintf(stderr, RED("Error can't fork !"));
//     } else {
//         execv(programme15, arguments);
//         fprintf(stderr, RED("Test 15 KO - Error in execution of jpeg2ppm\n"));
//         exit(EXIT_FAILURE); 
//     }

//     wait(&fils15);
//     fprintf(stderr, GREEN("Test 15 OK\n"));

//     //**************************************************************************************************************************
//     // Test 16
//     if ((fils16 = fork()) == -1) {
//         fprintf(stderr, RED("Error can't fork !"));
//     } else {
//         execv(programme16, arguments);
//         fprintf(stderr, RED("Test 16 KO - Error in execution of jpeg2ppm\n"));
//         exit(EXIT_FAILURE); 
//     }

//     wait(&fils16);
//     fprintf(stderr, GREEN("Test 16 OK\n"));

//     //**************************************************************************************************************************
//     // Test 17
//     if ((fils17 = fork()) == -1) {
//         fprintf(stderr, RED("Error can't fork !"));
//     } else {
//         execv(programme17, arguments);
//         fprintf(stderr, RED("Test 17 KO - Error in execution of jpeg2ppm\n"));
//         exit(EXIT_FAILURE); 
//     }

//     wait(&fils17);
//     fprintf(stderr, GREEN("Test 17 OK\n"));

//     //**************************************************************************************************************************
//     // Test 18
//     if ((fils18 = fork()) == -1) {
//         fprintf(stderr, RED("Error can't fork !"));
//     } else {
//         execv(programme18, arguments);
//         fprintf(stderr, RED("Test 18 KO - Error in execution of jpeg2ppm\n"));
//         exit(EXIT_FAILURE); 
//     }

//     wait(&fils18);
//     fprintf(stderr, GREEN("Test 18 OK\n"));

//     //**************************************************************************************************************************
//     // Test 19
//     if ((fils19 = fork()) == -1) {
//         fprintf(stderr, RED("Error can't fork !"));
//     } else {
//         execv(programme19, arguments);
//         fprintf(stderr, RED("Test 19 KO - Error in execution of jpeg2ppm\n"));
//         exit(EXIT_FAILURE); 
//     }

//     wait(&fils19);
//     fprintf(stderr, GREEN("Test 19 OK\n"));

//     //**************************************************************************************************************************
//     // Test 20
//     if ((fils20 = fork()) == -1) {
//         fprintf(stderr, RED("Error can't fork !"));
//     } else {
//         execv(programme20, arguments);
//         fprintf(stderr, RED("Test 20 KO - Error in execution of jpeg2ppm\n"));
//         exit(EXIT_FAILURE); 
//     }

//     wait(&fils20);
//     fprintf(stderr, GREEN("Test 20 OK\n"));

//     //**************************************************************************************************************************
//     // Test 21
//     if ((fils21 = fork()) == -1) {
//         fprintf(stderr, RED("Error can't fork !"));
//     } else {
//         execv(programme21, arguments);
//         fprintf(stderr, RED("Test 21 KO - Error in execution of jpeg2ppm\n"));
//         exit(EXIT_FAILURE); 
//     }

//     wait(&fils21);
//     fprintf(stderr, GREEN("Test 21 OK\n"));

//     //**************************************************************************************************************************
//     // Test 22
//     if ((fils22 = fork()) == -1) {
//         fprintf(stderr, RED("Error can't fork !"));
//     } else {
//         execv(programme22, arguments);
//         fprintf(stderr, RED("Test 22 KO - Error in execution of jpeg2ppm\n"));
//         exit(EXIT_FAILURE); 
//     }

//     wait(&fils22);
//     fprintf(stderr, GREEN("Test 22 OK\n"));

//     //**************************************************************************************************************************
//     // Test 23
//     if ((fils23 = fork()) == -1) {
//         fprintf(stderr, RED("Error can't fork !"));
//     } else {
//         execv(programme23, arguments);
//         fprintf(stderr, RED("Test 23 KO - Error in execution of jpeg2ppm\n"));
//         exit(EXIT_FAILURE); 
//     }

//     wait(&fils23);
//     fprintf(stderr, GREEN("Test 23 OK\n"));

//     //**************************************************************************************************************************
//     // Test 24
//     if ((fils24 = fork()) == -1) {
//         fprintf(stderr, RED("Error can't fork !"));
//     } else {
//         execv(programme24, arguments);
//         fprintf(stderr, RED("Test 24 KO - Error in execution of jpeg2ppm\n"));
//         exit(EXIT_FAILURE); 
//     }

//     wait(&fils24);
//     fprintf(stderr, GREEN("Test 24 OK\n"));

//     //**************************************************************************************************************************
//     // Test 25
//     if ((fils25 = fork()) == -1) {
//         fprintf(stderr, RED("Error can't fork !"));
//     } else {
//         execv(programme25, arguments);
//         fprintf(stderr, RED("Test 25 KO - Error in execution of jpeg2ppm\n"));
//         exit(EXIT_FAILURE); 
//     }

//     wait(&fils25);
//     fprintf(stderr, GREEN("Test 25 OK\n"));


//     return EXIT_SUCCESS;
// }