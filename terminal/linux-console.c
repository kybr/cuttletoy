// dump_console_font.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/kd.h>

/*
 * struct console_font_op is defined in <linux/kd.h>:
 *
 *   struct console_font_op {
 *       unsigned int op;        // one of KD_FONT_OP_GET, SET, etc.
 *       unsigned int flags;     // must be zero for GET
 *       unsigned int width;     // max width in pixels on input; actual width on return
 *       unsigned int height;    // glyph height in pixels
 *       unsigned int charcount; // max number of glyphs on input; actual on return
 *       void        *data;      // pointer to buffer for glyph bits
 *   };
 *
 * On KD_FONT_OP_GET, the kernel will fill width, height, charcount, and write
 * charcount * bytes_per_glyph bytes into your buffer.  bytes_per_glyph ==
 * ((width+7)/8) * height.
 */

int main(void) {
    int fd = open("/dev/tty2", O_RDONLY);
    if (fd < 0) {
        perror("open /dev/tty0");
        return 1;
    }

    struct console_font_op cf;
    memset(&cf, 0, sizeof(cf));

    cf.op        = KD_FONT_OP_GET;
    cf.flags     = 0;
    cf.width     = 32;    // maximum glyph width we expect
    cf.height    = 32;    // maximum glyph height we expect
    cf.charcount = 512;   // maximum number of glyphs

    // allocate a big enough blob: max_chars * max_bytes_per_glyph
    size_t bytes_per_row   = (cf.width + 7) / 8;
    size_t max_bytes_per_glyph = bytes_per_row * cf.height;
    size_t bufsize = cf.charcount * max_bytes_per_glyph;
    cf.data = malloc(bufsize);
    if (!cf.data) {
        perror("malloc");
        close(fd);
        return 1;
    }

    if (ioctl(fd, KDFONTOP, &cf) < 0) {
        perror("ioctl KD_FONT_OP_GET");
        free(cf.data);
        close(fd);
        return 1;
    }

    // now cf.width, cf.height and cf.charcount are the actual values
    bytes_per_row = (cf.width + 7) / 8;
    size_t bytes_per_glyph = bytes_per_row * cf.height;

    printf("Console font: %u glyphs × %u×%u px\n\n",
           cf.charcount, cf.width, cf.height);

    for (unsigned int ch = 0; ch < cf.charcount; ch++) {
        unsigned char *glyph = (unsigned char*)cf.data + ch * bytes_per_glyph;
        printf("Glyph %#04x:\n", ch);
        for (unsigned int y = 0; y < cf.height; y++) {
            // each row may span multiple bytes
            for (unsigned int byte = 0; byte < bytes_per_row; byte++) {
                unsigned char row = glyph[y * bytes_per_row + byte];
                for (int bit = 7; bit >= 0; bit--) {
                    // only print up to cf.width pixels
                    unsigned int x = byte*8 + (7 - bit);
                    if (x >= cf.width) break;
                    putchar((row & (1 << bit)) ? '#' : ' ');
                }
            }
            putchar('\n');
        }
        putchar('\n');
    }

    free(cf.data);
    close(fd);
    return 0;
}

