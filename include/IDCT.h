#ifndef _IDCT_H_
#define _IDCT_H_

#include <math.h>
#include <stdio.h>

#include <extract.h>
#include <utils.h>


// Fonction qui calcule les valeurs de C_cos_values et les enregistre dans un fichier
void initialize_C_cos_values();

// Fonction qui vérifie si le fichier C_cos_values.dat existe et le crée si besoin
// Puis on charge les valeurs de C_cos_values en mémoire
int8_t load_C_cos_values();

// Inverse Discrete Cosine Transform function
int8_t IDCT_function(int16_t **mcu);

int8_t IDCT(struct JPEG * jpeg);

#endif
