#ifndef _IDCT_H_
#define _IDCT_H_

#include <math.h>
#include <stdio.h>

#include <extract.h>
#include <utils.h>

#define PI 3.14159265358979323846
#define N 8
#define NN N*N


// //**********************************************************************************************************
// // IDCT naive

// // Fonction C() dans la formule de l'IDCT
// double C(int16_t x);

// // IDCT naive
// int8_t IDCT_function_naive(int16_t **input);


// //**********************************************************************************************************
// // IDCT légèrement optimisée : on pré-calcule les coefficients de la fonction C() dans la formule

// // Fonction qui calcule les valeurs de C_cos_values et les enregistre dans un fichier
// int8_t initialize_C_cos_values();

// // Fonction qui vérifie si le fichier C_cos_values.dat existe et le crée si besoin
// // Puis on charge les valeurs de C_cos_values en mémoire
// int8_t load_C_cos_values();

// // Inverse Discrete Cosine Transform function
// int8_t IDCT_function_optimized_v1(int16_t **input);


//**********************************************************************************************************
// FAST IDCT

// Inverse du module papillon de loeffler pour récupérer I0
float papillon_inv_I0(float O0, float O1);

// Inverse du module papillon de loeffler pour récupérer I1
float papillon_inv_I1(float O0, float O1);

// Inverse du module rotation de loeffler pour récupérer I0
float rotation_inv_I0(float O0, float O1, float k, uint8_t num_coef);

// Inverse du module rotation de loeffler pour récupérer I1
float rotation_inv_I0(float O0, float O1, float k, uint8_t num_coef);

// Fast Inverse Discrete Cosine Transform function using Loeffler algorithm
int8_t fast_IDCT_function(int16_t **input);

//**********************************************************************************************************
int8_t IDCT(struct JPEG * jpeg);

#endif
