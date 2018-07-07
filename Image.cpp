#include "Image.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

static void error(const std::string &filename, const std::string &errtext) {
    cerr << "Error reading image: " << filename << ": " << errtext << '\n';
    exit(1);
}

Image::Image(int xres, int yres)
        : xres(xres), yres(yres) {
    data = new Pixel *[yres];
    Pixel *p = new Pixel[xres * yres];
    for (int i = 0; i < yres; i++)
        data[i] = p + i * xres;
}

Image::~Image() {
    delete[] data[0];
    delete[] data;
}

void Image::write(const std::string &filename) const {
    unsigned char file[14] = {
        'B','M', // magic
        0,0,0,0, // size in bytes
        0,0, // app data
        0,0, // app data
        40 + 14,0,0,0 // start of data offset
    };
    unsigned char info[40] = {
        40,0,0,0, // info hd size
        0,0,0,0, // width
        0,0,0,0, // heigth
        1,0, // number color planes
        24,0, // bits per pixel
        0,0,0,0, // compression is none
        0,0,0,0, // image bits size
        0x13,0x0B,0,0, // horz resoluition in pixel / m
        0x13,0x0B,0,0, // vert resolutions (0x03C3 = 96 dpi, 0x0B13 = 72 dpi)
        0,0,0,0, // #colors in pallete
        0,0,0,0, // #important colors
    };

    int w = xres;
    int h = yres;

    int padSize = (4 - (w * 3) % 4) % 4;
    int sizeData = w * h * 3 + h * padSize;
    int sizeAll = sizeData + sizeof(file) + sizeof(info);

    file[2] = (unsigned char)(sizeAll);
    file[3] = (unsigned char)(sizeAll >> 8);
    file[4] = (unsigned char)(sizeAll >> 16);
    file[5] = (unsigned char)(sizeAll >> 24);

    info[4] = (unsigned char)(w);
    info[5] = (unsigned char)(w >> 8);
    info[6] = (unsigned char)(w >> 16);
    info[7] = (unsigned char)(w >> 24);

    info[8] = (unsigned char)(h);
    info[9] = (unsigned char)(h >> 8);
    info[10] = (unsigned char)(h >> 16);
    info[11] = (unsigned char)(h >> 24);

    info[20] = (unsigned char)(sizeData);
    info[21] = (unsigned char)(sizeData >> 8);
    info[22] = (unsigned char)(sizeData >> 16);
    info[23] = (unsigned char)(sizeData >> 24);

    std::ofstream stream(filename.c_str(), std::ios::out | std::ios::binary);

    stream.write((char*)file, sizeof(file));
    stream.write((char*)info, sizeof(info));

    unsigned char pad[3] = { 0,0,0 };

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            char* p = (char*)&data[y][x];
            stream.write(p, 3);
        }
        stream.write((char*)pad, padSize);
    }
}


/*
void Image::write(const std::string &filename) const {
    std::ofstream out(filename.c_str(), std::ios::out | std::ios::binary);
    out << "P6 " << xres << " " << yres << " 255\n";
    for (int i = yres - 1; i >= 0; i--)
        out.write(reinterpret_cast<char *>(data[i]), sizeof(Pixel) * xres);
}
*/
