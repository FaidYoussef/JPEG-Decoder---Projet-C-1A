CC = gcc
LD = gcc

# -O0 désactive les optimisations à la compilation
# -O3 active les optimisations de niveau 3
# -fopt-info-vec-optimized permet d'afficher les optimisations vectorielles
# -maxvx active les optimisations AVX (loop vectorization, ...)
# C'est utile pour débugger, par contre en "production"
# on active au moins les optimisations de niveau 2 (-O2).
# -lm on lie la bibliothèque mathématique (sqrt, cos, etc.)
CFLAGS = -Wall -Wextra -std=c99 -Iinclude -O3 -mavx -mavx2 -g -lm
LDFLAGS =

# Par défaut, on compile tous les fichiers source (.c) qui se trouvent dans le
# répertoire src/
SRC_FILES=$(wildcard src/*.c)

# Par défaut, la compilation de src/toto.c génère le fichier objet obj/toto.o
OBJ_FILES=$(patsubst src/%.c,obj/%.o,$(SRC_FILES))

all: jpeg2ppm

jpeg2ppm: $(OBJ_FILES)
	$(LD) $(OBJ_FILES) $(LDFLAGS) -o $@ $(CFLAGS)

obj/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

tests: $(OBJ_FILES)
	make -C tests/

test-huffman: obj/huffman.o
	make -C tests/ huffman-test 

test-IDCT: obj/IDCT.o
	make -C tests/ IDCT-test 

test-IQ: obj/IQ.o
	make -C tests/ IQ-test

test-IZZ: obj/IZZ.o
	make -C tests/ IZZ-test

test-ppm: obj/ppm.o
	make -C tests/ ppm-test 

test-ycbcr2rgb: obj/ycbcr2rgb.o
	make -C tests/ ycbcr2rgb-test 

.PHONY: clean

clean:
	rm -rf jpeg2ppm tests/huffman-test tests/IDCT-test tests/ppm tests/IQ-test tests/IZZ-test tests/ycbcr2rgb $(OBJ_FILES)
