ahff2png:
	${CC} ${CFLAGS} -o ahff2png ahff2png.c lodepng.c -lz

lz4er:
	make -C ./lz4/lib
	${CC} ${CFLAGS} -o lz4er lz4er.c -I./lz4/lib -L./lz4/lib ./lz4/lib/liblz4.a
