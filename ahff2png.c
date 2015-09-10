/*
 * ahff2png.c - disunityed file tool
 * Copyright (c) 2015 The Holy Constituency of the Summer Triangle.
 * All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>

#include "lodepng.h"

typedef union {
    unsigned char b[4];
    uint32_t n;
} byte_addressable_uint32;
typedef unsigned char byte;

// the Ad-Hoc File Format is intentionally very simple so i don't have 
// to write so much java
// packed should have no effect, this is just for safety reasons
typedef union {
    byte b[16];
    struct __attribute__((packed)) {
        uint32_t width;
        uint32_t height;
        uint32_t datsize;
        uint32_t pixel_format;
    } s;
} ahff_header_t;

enum /* pixel_format_t */ {
    A8 = 1,
    RGB24 = 3,
    RGBA32 = 4,
    RGB565 = 7,
    RGBA4444 = 13,
};

#define READ_FULLY(fd, buf, size) do { \
    size_t _eval_one = (size); \
    size_t nread = read(fd, buf, _eval_one); \
    assert(nread == _eval_one || !"READ_FULLY did not read fully."); \
} while(0)

#include "pixel.c"
    
void flip_image_sideways(byte *buf, uint32_t width, uint32_t height) {
    byte *work = malloc(width * 4);
    byte *worp = work;
    
    for (int row = 0; row < height; ++row) {
        byte *crow = buf + (row * width * 4);
        worp = work;
        
        for (size_t i = (width - 1) * 4; i > 0; i -= 4) {
            memcpy(worp, crow + i, 4);
            worp += 4;
        }
        
        memcpy(crow, work, width * 4);
    }
    
    free(work);
}

void flip_image_upside_down(byte *buf, uint32_t width, uint32_t height) {
    byte *work = malloc(width * 4);
    
    for (int row = 0, target_row = height - 1; row < (height / 2); ++row, --target_row) {
        memcpy(work, buf + (target_row * width * 4), width * 4);
        memcpy(buf + (target_row * width * 4), buf + (row * width * 4), width * 4);
        memcpy(buf + (row * width * 4), work, width * 4);
    }
    
    free(work);
}

int main (int argc, char const *argv[]) {
    int fd = open(argv[1], O_RDONLY);
    assert(fd >= 0);
    
    ahff_header_t info = { 0 };
    READ_FULLY(fd, info.b, sizeof(ahff_header_t));
    
    unsigned char *buf = malloc(info.s.datsize);
    unsigned char *out = calloc(info.s.width * info.s.height, 4);
    assert(buf && out);
    
    READ_FULLY(fd, buf, info.s.datsize);
    
    int len = strlen(argv[1]);
    char *cp = strdup(argv[1]);
    char *fn;
    if (strcmp(cp + len - 5, ".ahff") == 0) {
        memcpy(cp + len - 5, ".png\0", 5);
        fn = cp;
    } else {
        fn = malloc(len + 5);
        snprintf(fn, len + 5, "%s.png", cp);
        free(cp);
    }
    printf("[>] %s\n", fn);
    
    switch (info.s.pixel_format) {
        case A8:
            copy_1bpp_alpha(buf, info.s.datsize, out);
            break;
        case RGB24:
            copy_3bpp_rgb(buf, info.s.datsize, out);
            break;
        case RGB565:
            copy_2bpp_rgb565(buf, info.s.datsize, out);
            break;
        case RGBA4444:
            copy_2bpp_rgba4444(buf, info.s.datsize, out);
            break;
        case RGBA32:
            memcpy(out, buf, info.s.datsize);
            break;
        default:
            fprintf(stderr, "unknown pixel format %d\n", info.s.pixel_format);
            goto end;
    }
    
    // flip_image_sideways(out, info.width, info.height);
    flip_image_upside_down(out, info.s.width, info.s.height);
    lodepng_encode32_file(fn, out, info.s.width, info.s.height);

  end:
    free(fn);
    free(buf);
    free(out);
    return 0;
}
