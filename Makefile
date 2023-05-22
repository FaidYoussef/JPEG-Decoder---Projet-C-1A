CC = gcc
LD = gcc


# -O0 désactive les optimisations à la compilation
# C'est utile pour débugger, par contre en "production"
# on active au moins les optimisations de niveau 2 (-O2).
# -O3 active les optimisations de niveau 3
CFLAGS = -Wall -Wextra -std=c99 -Iinclude -O3 -g

# -maxvx et -mavx2 permettent d'utiliser respectivement les instructions AVX et AVX2 du processeur (loop vectorization, ...)
# -fopt-info-vec-optimized permet d'afficher les optimisations vectorielles
# Il faut cependant vérifier que le processeur supporte ces instructions

# On récupère l'environnement (Linux, Mac OS X, Windows, ...)
UNAME := $(shell uname)

# On vérifie que le processeur supporte les instructions AVX et AVX2 et on les active si c'est le cas
ifeq ($(UNAME), Linux)
check_avx := $(shell echo | gcc -dM -E - -mavx 2>/dev/null | grep -c "AVX")
check_avx2 := $(shell echo | gcc -dM -E - -mavx2 2>/dev/null | grep -c "AVX2")
else ifeq ($(UNAME), Darwin)
check_avx := $(shell echo | gcc -dM -E - -mavx 2>/dev/null | grep -c "AVX")
check_avx2 := $(shell echo | gcc -dM -E - -mavx2 2>/dev/null | grep -c "AVX2")
else ifeq ($(UNAME), Windows_NT)
check_avx := $(shell echo | gcc -dM -E - -mavx 2>NUL | find /C "AVX")
check_avx2 := $(shell echo | gcc -dM -E - -mavx2 2>NUL | find /C "AVX2")
else
$(error Unsupported operating system: $(UNAME))
endif

ifeq ($(check_avx), 1)
CFLAGS += -mavx
endif

ifeq ($(check_avx2), 1)
CFLAGS += -mavx2
endif

# -lm on lie la bibliothèque mathématique (sqrt, cos, etc.)
# Note : ce flag DOIT se trouver en fin de ligne !!!
CFLAGS += -lm
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

test-extract: obj/extract.o
	make -C tests/ extract-test 

test-IDCT: obj/IDCT.o
	make -C tests/ IDCT-test 

test-IQ: obj/IQ.o
	make -C tests/ IQ-test

test-IZZ: obj/IZZ.o
	make -C tests/ IZZ-test

test-ycbcr2rgb: obj/ycbcr2rgb.o
	make -C tests/ ycbcr2rgb-test 

.PHONY: clean

clean:
	rm -rf jpeg2ppm tests/IDCT-test tests/IQ-test tests/IZZ-test tests/ppm tests/ycbcr2rgb $(OBJ_FILES)
	make -C tests/ clean
