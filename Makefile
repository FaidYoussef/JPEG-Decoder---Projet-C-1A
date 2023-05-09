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
	$(LD) $(OBJ_FILES) $(LDFLAGS) -o $@ $(CFLAGS)

obj/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

tests: $(OBJ_FILES)
	make -C tests/

test-huffman: obj/huffman.o
	make -C tests/ huffman-test 

test-IDCT: obj/IDCT.o
	make -C tests/ IDCT-test 

test-quant_zigzag: obj/quant_zigzag.o
	make -C tests/ quant_zigzag-test

test-RLE_decode: obj/RLE_decode.o
	make -C tests/ RLE_decode-test 

.PHONY: clean

clean:
	rm -rf jpeg2ppm tests/huffman-test tests/IDCT-test tests/quant_zigzag-test tests/RLE_decode $(OBJ_FILES)
