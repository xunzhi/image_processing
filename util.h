// Image Processing Exercise
// Author: Xunzhi Wang
// Email: wxzpursue@gmail.com
// Utilization header used in common

#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <vector>
using namespace std;
// Settings
#define DEBUG 0

#define PI 3.14

// Image structure
struct Image{
    // Use vector for variable length matrix
    vector<vector<vector<unsigned char> > > matrix;
    int height;
    int width;
    int channels; // RGB:3; gray:1
};

struct MatDouble{
    vector<vector<vector<double> > > matrix;    int height;
    int width;
    int channels;
};

// Initialize image by 0s or file
void initImage(Image*image, int *height, int *width, int *channel,
               char* filename=NULL);
// Write image to files
void image2File(Image*image, char* filename);

// Read and write pixel: different versions

// Read image according to position, channel and boundary mirroring method
// Boundary mirroring method:
// 0: default and read outer pixels as 0; usually not care too much
// 1: boundary mirroring: row "-1" would copy row "1", etc.
//    More than one height/width away from boundary would be considered as error
// 2: background mirroring: view 0,0 as background and read outer ones as that
unsigned char readPixel(Image*image, int inHeight, int inWidth, int channel=0,
                        int mirror=0);
// Read using bilinear interpolation. This function would call the above one.
unsigned char readPixel(Image*image, double mappedH, double mappedW,
                        int channel=0, int mirror=0);
// Write value to given position
void writePixel(Image*image, unsigned char value, int outHeight, int outWidth,
                int channel=0);


#endif // _UTIL_H_
