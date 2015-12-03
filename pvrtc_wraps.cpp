#include "pvrtc/PVRTDecompress.h"

typedef unsigned char byte;

extern "C" {
    void copy_pvrtc4_rgba(byte *raw, byte *output, int width, int height) {
        PVRTDecompressPVRTC(raw, 0, width, height, output);
    }
}
