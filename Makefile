CC = gcc
LD = gcc

# -O0 désactive les optimisations à la compilation
# -O3 active les optimisations de niveau 3
# -maxvx active les optimisations AVX (loop vectorization, ...)
# C'est utile pour débugger, par contre en "production"
# on active au moins les optimisations de niveau 2 (-O2).
# -lm on lie la bibliothèque mathématique (sqrt, cos, etc.)
CFLAGS = -Wall -Wextra -std=c99 -Iinclude -O0 -mavx -g -lm
LDFLAGS =

# Par défaut, on compile tous les fichiers source (.c) qui se trouvent dans le
# répertoire src/
SRC_FILES=$(wildcard src/*.c)

# Par défaut, la compilation de src/toto.c génère le fichier objet obj/toto.o
OBJ_FILES=$(patsubst src/%.c,obj/%.o,$(SRC_FILES))

all: jpeg2ppm

jpeg2ppm: $(OBJ_FILES) 
	$(LD) $(OBJ_FILES) $(LDFLAGS) -o $@

obj/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -rf jpeg2ppm $(OBJ_FILES)
