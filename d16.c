/* dump16.c : CP/M-80 (z88dk) - press any key to dump 16 bytes from 0x0100 */
#include <stdio.h>
#include <cpm.h>

/* BDOS function numbers (CP/M 2.2) */
#define CPM_CONIN   1   /* Console input, waits for a character */
#define CPM_CONST   11  /* Console status, 0x00 = no key, 0xFF = key ready */

#define BASE_ADDR   0xFE00  /* example: start of the TPA (COM load address) */

static void dump16(const unsigned char *p, unsigned int addr) {
    unsigned int i;
    /* address */
    printf("%04X: ", addr);

    /* hex bytes */
    for (i = 0; i < 16; ++i) {
        printf("%02X ", p[i]);
    }

    /* two spaces between hex and ascii for readability */
    printf(" ");

    /* ASCII (printable -> char, else '.') */
    for (i = 0; i < 16; ++i) {
        unsigned char c = p[i];
        if (c >= 32 && c <= 126) putchar(c);
        else putchar('.');
    }

    printf("\r\n");
}

int main(void) {
    const unsigned char *p = (const unsigned char *)BASE_ADDR;

    printf("\r\nPress any key to dump 16 bytes from 0x%04X (ESC to exit)\r\n", BASE_ADDR);

    for (;;) {
        int ch = bdos(CPM_CONIN, 0);   /* read the key (and echo by BIOS usually) */
        if (ch == 27) {                /* ESC */
            printf("\r\nBye.\r\n");
            break;
        }
        dump16(p, BASE_ADDR);
        printf("Press any key again to re-dump (ESC to exit)\r\n");
        /* (Optional) do nothing / could add a tiny delay if you like */
    }
    return 0;
}
