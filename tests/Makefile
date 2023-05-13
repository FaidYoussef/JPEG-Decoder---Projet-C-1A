CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -g -O0 -mavx -I../include
LDFLAGS = -lm

TESTS = huffman-test \
	IDCT-test \
	ppm-test \
	quant_zigzag-test\
	ycbcr2rgb-test

SRC = $(TESTS:=.c)
OBJ = $(TESTS:=.o)

all: $(TESTS)
	make -C ../

# huffman-test: huffman-test.o ../obj/huffman.o 
# 	$(CC) $(LDFLAGS) $^ -o $@
# tot-test: idct-test.o ../obj/idct.o 
# 	$(CC) $(LDFLAGS) $^ -o $@


%-test: %-test.o ../obj/%.o 
	$(CC) $^ -o $@  $(LDFLAGS)

%-test.o: %-test.c
	$(CC) $(CFLAGS) -c $^

.PHONY: clean

clean:
	rm -rf *.o *~ $(TESTS)