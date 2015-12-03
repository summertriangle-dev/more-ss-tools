ahff2png: ahff2png.c pixel.c wraps.o rg_etc1wrap.o pvrtc_wraps.o
	make -C pvrtc PVRTDecompress.o
	${CC} ${CFLAGS} -o ahff2png ahff2png.c lodepng.c rg_etc1wrap.o rg_etc1.o pvrtc_wraps.o pvrtc/PVRTDecompress.o -lz

rg_etc1wrap.o:
	${CXX} ${CXXFLAGS} -c rg_etc1wrap.cpp rg_etc1.cpp

pvrtc_wraps.o:
	${CXX} ${CXXFLAGS} -c pvrtc_wraps.cpp

lz4er:
	make -C ./lz4/lib
	${CC} ${CFLAGS} -o lz4er lz4er.c -I./lz4/lib -L./lz4/lib ./lz4/lib/liblz4.a
