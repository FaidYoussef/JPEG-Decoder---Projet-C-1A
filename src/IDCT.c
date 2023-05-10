// #include <stdlib.h>
// #include <stdbool.h>
// #include <stdio.h>
// #include <math.h>

// #define PI 3.14159265358979323846
// #define N 8
// #define NN N*N

// double C(int x) {
//     return (x == 0) ? 1 / sqrt(2) : 1;
// }

// void idct(int input[NN], int output[NN]) {
//     for (int x = 0; x < N; x++) {
//         for (int y = 0; y < N; y++) {
//             double sum = 0.0;

//             for (int u = 0; u < N; u++) {
//                 for (int v = 0; v < N; v++) {
//                     double cu = C(u);
//                     double cv = C(v);
//                     int dct_uv = input[u*N + v];
//                     double cos_xu = cos(((2 * x + 1) * u * PI) / (2 * N));
//                     double cos_yv = cos(((2 * y + 1) * v * PI) / (2 * N));
//                     sum += cu * cv * dct_uv * cos_xu * cos_yv;
//                 }
//             }

//             output[x*N + y] = round(sum * 0.25);
//         }
//     }
// }

// bool verbose = 1;

// int main() {
//     int input[NN] = {
//         0,0,0,0,0,0,0,0,
//         1,1,1,1,1,1,1,1,
//         2,2,2,2,2,2,2,2,
//         3,3,3,3,3,3,3,3,
//         4,4,4,4,4,4,4,4,
//         5,5,5,5,5,5,5,5,
//         6,6,6,6,6,6,6,6,
//         7,7,7,7,7,7,7,7
//     };

//     int output[NN];
//     idct(input, output);

//     // Print the output array
//     for (int i = 0; i < N; i++) {
//         for (int j = 0; j < N; j++) {
//             printf("%d ", output[i*N + j]);
//         }
//         printf("\n");
//     }

//     return EXIT_SUCCESS;
// }




// #include <stdlib.h>
// #include <stdbool.h>
// #include <stdio.h>
// #include <math.h>

// #define PI 3.14159265358979323846
// #define N 8
// #define NN N*N

// double C(int x) {
//     return (x == 0) ? 1 / sqrt(2) : 1;
// }

// void idct(int input[NN], int output[NN]) {
//     double cos_values[N][N][N][N];
//     for (int x = 0; x < N; x++) {
//         for (int y = 0; y < N; y++) { 
//             for (int u = 0; u < N; u++) {
//                 for (int v = 0; v < N; v++) {
//                     cos_values[x][y][u][v] = cos(((2 * x + 1) * u * PI) / (2 * N)) * cos(((2 * y + 1) * v * PI) / (2 * N));
//                 }
//             }
//         }
//     }

//     for (int x = 0; x < N; x++) {
//         for (int y = 0; y < N; y++) { 
//             double sum = 0.0;

//             for (int u = 0; u < N; u++) {
//                 for (int v = 0; v < N; v++) {
//                     double cu = C(u);
//                     double cv = C(v);
//                     int dct_uv = input[u*N + v];
//                     sum += cu * cv * dct_uv * cos_values[x][y][u][v];
//                 }
//             }

//             output[x*N + y] = round(sum * 0.25);
//         }
//     }
// }

// bool verbose = 1;

// int main() {
//     int input[NN] = {
//         0,0,0,0,0,0,0,0,
//         1,1,1,1,1,1,1,1,
//         2,2,2,2,2,2,2,2,
//         3,3,3,3,3,3,3,3,
//         4,4,4,4,4,4,4,4,
//         5,5,5,5,5,5,5,5,
//         6,6,6,6,6,6,6,6,
//         7,7,7,7,7,7,7,7
//     };

//     int output[NN];
//     idct(input, output);

//     // Print the output array
    
//     for (int i = 0; i < N; i++) {
//         for (int j = 0; j < N; j++) {
//             printf("%d ", output[i*N + j]);
//         }
//         printf("\n");
//     }

//     return EXIT_SUCCESS;
// }


// #include <stdlib.h>
// #include <stdbool.h>
// #include <stdio.h>
// #include <math.h>

// #define PI 3.14159265358979323846
// #define N 8
// #define NN N*N

// double C[N];
// double cos_values[N][N][N][N];

// void initialize() {
//     for (int i = 0; i < N; i++) {
//         C[i] = (i == 0) ? 1 / sqrt(2) : 1;
//     }

//     for (int x = 0; x < N; x++) {
//         for (int y = 0; y < N; y++) {
//             for (int u = 0; u < N; u++) {
//                 for (int v = 0; v < N; v++) {
//                     cos_values[x][y][u][v] = cos(((2 * x + 1) * u * PI) / (2 * N)) * cos(((2 * y + 1) * v * PI) / (2 * N));
//                 }
//             }
//         }
//     }
// }

// void idct(int input[NN], int output[NN]) {
//     for (int x = 0; x < N; x++) {
//         for (int y = 0; y < N; y++) { 
//             double sum = 0.0;

//             for (int u = 0; u < N; u++) {
//                 double cu = C[u];
//                 for (int v = 0; v < N; v++) {
//                     double cv = C[v];
//                     int dct_uv = input[u*N + v];
//                     sum += cu * cv * dct_uv * cos_values[x][y][u][v];
//                 }
//             }

//             output[x*N + y] = round(sum * 0.25);
//         }
//     }
// }

// bool verbose = 1;

// int main() {
//     int input[NN] = {
//         0,0,0,0,0,0,0,0,
//         1,1,1,1,1,1,1,1,
//         2,2,2,2,2,2,2,2,
//         3,3,3,3,3,3,3,3,
//         4,4,4,4,4,4,4,4,
//         5,5,5,5,5,5,5,5,
//         6,6,6,6,6,6,6,6,
//         7,7,7,7,7,7,7,7
//     };

//     int output[NN];

//     initialize();
//     idct(input, output);

//     // Print the output array
//     for (int i = 0; i < N; i++) {
//         for (int j = 0; j < N; j++) {
//             printf("%d ", output[i*N + j]);
//         }
//         printf("\n");
//     }

//     return EXIT_SUCCESS;
// }



#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <IDCT.h>

#define PI 3.14159265358979323846
// #define N 8
// #define NN N*N

double C[N];
double cos_values[N][N][N][N];

void initialize() {
    FILE *file;

    for (int i = 0; i < N; i++) {
        C[i] = (i == 0) ? 1 / sqrt(2) : 1;
    }
    file = fopen("C.dat", "wb");
    fwrite(C, sizeof(double), N, file);
    fclose(file);

    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            for (int u = 0; u < N; u++) {
                for (int v = 0; v < N; v++) {
                    cos_values[x][y][u][v] = cos(((2 * x + 1) * u * PI) / (2 * N)) * cos(((2 * y + 1) * v * PI) / (2 * N));
                }
            }
        }
    }
    file = fopen("cos_values.dat", "wb");
    fwrite(cos_values, sizeof(double), N*N*N*N, file);
    fclose(file);
}

void load() {
    FILE *file;

    file = fopen("C.dat", "rb");
    fread(C, sizeof(double), N, file);
    fclose(file);

    file = fopen("cos_values.dat", "rb");
    fread(cos_values, sizeof(double), N*N*N*N, file);
    fclose(file);
}

void idct(int input[NN], int output[NN]) {
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) { 
            double sum = 0.0;

            for (int u = 0; u < N; u++) {
                double cu = C[u];
                for (int v = 0; v < N; v++) {
                    double cv = C[v];
                    int dct_uv = input[u*N + v];
                    sum += cu * cv * dct_uv * cos_values[x][y][u][v];
                }
            }

            output[x * N + y] = round(sum * 0.25);
        }
    }
}

