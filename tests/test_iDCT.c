#include <IDCT.h>

int main() {
    verbose = 1;
    
    double input[N][N] = {
        // Mettez ici les coefficients DCT de la matrice 8x8
		{0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1},
		{2,2,2,2,2,2,2,2},
		{3,3,3,3,3,3,3,3},
		{4,4,4,4,4,4,4,4},
		{5,5,5,5,5,5,5,5},
		{6,6,6,6,6,6,6,6},
		{7,7,7,7,7,7,7,7}
    };

    double output[N][N];
    idct(input, output);

    // Affiche la matrice de sortie
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%f ", output[i][j]);
        }
        printf("\n");
    }

    return 0;
}