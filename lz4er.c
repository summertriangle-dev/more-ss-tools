/*
 * lz4er.c - quick ss lz4 tool
 * Copyright (c) 2015 The Holy Constituency of the Summer Triangle.
 * All rights reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include "lz4.h"

typedef union {
    unsigned char b[4];
    uint32_t n;
} byte_addressable_uint32;

int main (int argc, char const *argv[]) {
    int fd = open(argv[1], O_RDONLY);
    assert(fd >= 0);

    byte_addressable_uint32 unpacked_size;    
    byte_addressable_uint32 fsiz;
    
    lseek(fd, 4, SEEK_SET);
    read(fd, unpacked_size.b, 4);
    read(fd, fsiz.b, 4);
    
    unsigned char *buf = malloc(fsiz.n);
    unsigned char *out = malloc(unpacked_size.n);
    
    assert(buf && out);
    lseek(fd, 4, SEEK_CUR);
    assert(read(fd, buf, fsiz.n) == fsiz.n);
    
    int ret = LZ4_decompress_safe((const char *)buf, (char *)out, fsiz.n, unpacked_size.n);
    assert(ret > 0);
    
    write(STDOUT_FILENO, out, ret);
    return 0;
}
