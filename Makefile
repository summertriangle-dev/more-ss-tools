ahff2png:
	${CXX} ${CXXFLAGS} -c rg_etc1.cpp rg_etc1wrap.cpp
	${CC} ${CFLAGS} -o ahff2png ahff2png.c lodepng.c rg_etc1.o rg_etc1wrap.o -lz

lz4er:
	make -C ./lz4/lib
	${CC} ${CFLAGS} -o lz4er lz4er.c -I./lz4/lib -L./lz4/lib ./lz4/lib/liblz4.a
