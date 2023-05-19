CC = gcc


# -O0 désactive les optimisations à la compilation
# C'est utile pour débugger, par contre en "production"
# on active au moins les optimisations de niveau 2 (-O2).
# -O3 active les optimisations de niveau 3
CFLAGS = -std=c99 -Wall -Wextra -g -O3 -I../include

# -maxvx et -mavx2 utilisent respectivement les instructions AVX et AVX2 du processeur (loop vectorization, ...)
# -fopt-info-vec-optimized permet d'afficher les optimisations vectorielles
# Il faut cependant vérifier que le processeur supporte ces instructions

# On récupère l'environnement (Linux, Mac OS X, Windows, ...)
UNAME := $(shell uname)

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

LDFLAGS = -lm

TESTS = IQ-test \
	IZZ-test \
	huffman-test \
	IDCT-test \
	ppm-test \
	ycbcr2rgb-test

SRC = $(TESTS:=.c)
OBJ = $(TESTS:=.o)

all: $(TESTS)
# 	make -C ../

# huffman-test: huffman-test.o ../obj/huffman.o 
# 	$(CC) $(LDFLAGS) $^ -o $@
# tot-test: idct-test.o ../obj/idct.o 
# 	$(CC) $(LDFLAGS) $^ -o $@


%-test: %.o ../obj/%.o 
	$(CC) $^ -o $@  $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

IDCT-test: IDCT-test.o ../obj/IDCT.o
	$(CC) $^ -o $@ $(LDFLAGS)

IQ-test: IQ-test.o ../obj/IQ.o
	$(CC) $^ -o $@ $(LDFLAGS)

IZZ-test: IZZ-test.o ../obj/IZZ.o
	$(CC) $^ -o $@ $(LDFLAGS)

ppm-test: ppm-test.o ../obj/ppm.o
	$(CC) $^ -o $@ $(LDFLAGS)

# .PHONY: clean
.PHONY: all

clean:
	rm -rf *.o *~ $(TESTS)