/* dump_loop.c : CP/M-80 (z88dk)
   Repeatedly dump 16 bytes from address 0xFE00 forever.
*/

#include <stdio.h>
#include <cpm.h>

#define BASE_ADDR 0xFE00

static void dump16(const unsigned char *p, unsigned int addr) {
    unsigned int i;

    printf("%04X: ", addr);
    for (i = 0; i < 16; ++i)
        printf("%02X ", p[i]);

    printf("  ");
    for (i = 0; i < 16; ++i) {
        unsigned char c = p[i];
        putchar((c >= 32 && c <= 126) ? c : '.');
    }
    printf("\r\n");
}

int main(void) {
    const unsigned char *p = (const unsigned char *)BASE_ADDR;

    printf("Dumping 16 bytes repeatedly from 0x%04X ...\r\n", BASE_ADDR);

    while (1) {
        dump16(p, BASE_ADDR);
        /* 小さな遅延を入れると端末が読める速度になります */
        for (volatile unsigned long i = 0; i < 50000UL; i++) ;
    }

    return 0;
}
