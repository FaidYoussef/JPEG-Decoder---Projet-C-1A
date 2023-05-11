#include <stdlib.h>
#include <stdbool.h>

#include <huffman.h>

bool verbose = 1;

int main() {
    int decimal_value = 0x30;
    int hexadecimal_value = dec2hex(decimal_value);
    printf("Decimal value: %d, Hexadecimal value: %x\n", decimal_value, hexadecimal_value);


    unsigned char *bitstream1 = malloc(15*4 * sizeof(unsigned char));
    check_memory_allocation((void *) bitstream1);
    bitstream1 = (unsigned char*) "111100001010101111100001010101111100001010101111100001010101";

    unsigned char *expected_output1 = malloc(8*4 * sizeof(unsigned char));
    check_memory_allocation((void *) expected_output1);
    expected_output1[0] = 0xb;
    expected_output1[1] = 0xa;
    expected_output1[2] = 0xd;
    expected_output1[3] = 0xe;
    expected_output1[4] = 0xe;
    expected_output1[5] = 0xe;
    expected_output1[6] = 0xd;
    expected_output1[7] = 0x0;
    //
    expected_output1[8] = 0xb;
    expected_output1[9] = 0xa;
    expected_output1[10] = 0xd;
    expected_output1[11] = 0xe;
    expected_output1[12] = 0xe;
    expected_output1[13] = 0xe;
    expected_output1[14] = 0xd;
    expected_output1[15] = 0x0;
    expected_output1[16] = 0xb;
    expected_output1[17] = 0xa;
    expected_output1[18] = 0xd;
    expected_output1[19] = 0xe;
    expected_output1[20] = 0xe;
    expected_output1[21] = 0xe;
    expected_output1[22] = 0xd;
    expected_output1[23] = 0x0;
    expected_output1[24] = 0xb;
    expected_output1[25] = 0xa;
    expected_output1[26] = 0xd;
    expected_output1[27] = 0xe;
    expected_output1[28] = 0xe;
    expected_output1[29] = 0xe;
    expected_output1[30] = 0xd;
    expected_output1[31] = 0x0;

    unsigned char *bitstream2 = malloc(15 * sizeof(unsigned char));
    check_memory_allocation((void *) bitstream2);
    bitstream2 = (unsigned char*) "111100001010100";

    unsigned char *expected_output2 = malloc(8 * sizeof(unsigned char));
    check_memory_allocation((void *) expected_output2);
    expected_output2[0] = 0xb;
    expected_output2[1] = 0xa;
    expected_output2[2] = 0xd;
    expected_output2[3] = 0xe;
    expected_output2[4] = 0xe;
    expected_output2[5] = 0xe;
    expected_output2[6] = 0xe;
    expected_output2[7] = 0x0;

    for (int i=0; i<8; i++){
        printf("%x", expected_output1[i]);
    }
    test_decode_bitstream(bitstream1, expected_output1);
    test_decode_bitstream(bitstream2, expected_output1);
    test_decode_bitstream(bitstream2, expected_output2);

    // On libère la mémoire
    free(bitstream1);
    // free(expected_output1);
    free(bitstream2);
    // free(expected_output2);
    
    return EXIT_SUCCESS;
}