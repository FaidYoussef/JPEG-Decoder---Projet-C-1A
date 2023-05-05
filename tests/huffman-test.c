#include <stdlib.h>
#include <stdbool.h>

#include <huffman.h>

bool verbose = 1;

int main() {
    unsigned char *bitstream1 = malloc(15 * sizeof(unsigned char));
    bitstream1 = "111100001010101";
    unsigned char *expected_output1 = malloc(8 * sizeof(unsigned char));
    expected_output1[0] = 0xb;
    expected_output1[1] = 0xa;
    expected_output1[2] = 0xd;
    expected_output1[3] = 0xe;
    expected_output1[4] = 0xe;
    expected_output1[5] = 0xe;
    expected_output1[6] = 0xd;
    expected_output1[7] = 0x0;

    unsigned char *bitstream2 = malloc(15 * sizeof(unsigned char));
    bitstream2 = "111100001010100";
    unsigned char *expected_output2 = malloc(8 * sizeof(unsigned char));
    expected_output2[0] = 0xb;
    expected_output2[1] = 0xa;
    expected_output2[2] = 0xd;
    expected_output2[3] = 0xe;
    expected_output2[4] = 0xe;
    expected_output2[5] = 0xe;
    expected_output2[6] = 0xe;
    expected_output2[7] = 0x0;

    test_decode_bitstream(bitstream1, expected_output1);
    test_decode_bitstream(bitstream2, expected_output1);
    test_decode_bitstream(bitstream2, expected_output2);
    
    return EXIT_SUCCESS;
}