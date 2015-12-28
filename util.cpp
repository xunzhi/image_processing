// Image Processing Exercise
// Author: Xunzhi Wang
// Email: wxzpursue@gmail.com
// Utilization source used in common

#include "util.h"
// See util.h for default parameters
#include <iostream>#include <stdlib.h>
#include <string.h>

#include "jpge.h"
#include "jpgd.h"

#define RAW 1
#define JPG 2
int getFileExtension(char* filename){
    if(filename==NULL)
        return 0;
    char* dot = NULL;
    for(dot=filename+strlen(filename);dot>filename;dot--)
        if(*dot=='.')
            break;
    if(strncmp(dot,".raw",4)==0)
        return RAW;
    if(strncmp(dot,".jpg",4)==0 || strncmp(dot,".jpeg",5)==0)
        return JPG;
    return 0;
}

// Initialize image
void initImage(Image*image, int* height, int* width, int* channels,
               char* filename){
    if(image==NULL){
        cerr<<"Invalid image!"<<endl;
        exit(1);
    }
    if(height==NULL||width==NULL||channels==NULL){
        cerr<<"Invalid size pointers!"<<endl;
        exit(1);
    }

    unsigned char * pImage=NULL;
    // If file is jpg, read the size information from file
    if(getFileExtension(filename)==JPG){
        pImage =
            jpgd::decompress_jpeg_image_from_file(filename,width,height,channels,3);
        cout<<"Original height,width:"<<*height<<","<<*width<<endl;
    }

    if(*height==0 || *width==0 || *channels==0){
        cerr<<"Invalid size!"<<endl;
        exit(1);
    }
    // Size information is known
    image->height = *height;
    image->width = *width;
    image->channels = *channels;
    image->matrix.resize(image->height);    int inputH = 0, inputW = 0;    int channel = 0;    for(inputH=0;inputH<image->height;inputH++){        image->matrix[inputH].resize(image->width);        for(inputW=0;inputW<image->width;inputW++){            image->matrix[inputH][inputW].resize(image->channels);
            for(channel=0;channel<image->channels;channel++)
                image->matrix[inputH][inputW][channel] = 0;
        }    }

    FILE* file=NULL;
    if(filename!=NULL){
        // More advanced method can be considered
        unsigned char tempImage[image->height][image->width][image->channels];        if(pImage!=NULL){
            memcpy(tempImage,pImage,sizeof(tempImage));
        } else {
            if (!(file=fopen(filename,"rb"))) {                cout << "Cannot open file: " << filename << endl;                exit(1);            }
            fread(tempImage, sizeof(unsigned char),                  (image->height)*(image->width)*(image->channels), file);        }
        for(inputH=0;inputH<image->height;inputH++)            for(inputW=0;inputW<image->width;inputW++)                for(channel=0;channel<image->channels;channel++)                    image->matrix[inputH][inputW][channel]=                        tempImage[inputH][inputW][channel];
        if(pImage!=NULL)
            free(pImage);
        if(file!=NULL)
            fclose(file);
    }
}

void image2File(Image*image, char* filename){
    if(image==NULL){
        cerr<<"Invalid image!"<<endl;
        exit(1);
    }
    if(filename==NULL){
        cerr<<"Invalid filename!"<<endl;
        exit(1);
    }

    int height = image->height;
    int width = image->width;
    int channels = image->channels;
    int inputH=0, inputW=0;
    int channel=0;
    unsigned char tempImage[height][width][channels];
    for(inputH=0;inputH<height;inputH++)            for(inputW=0;inputW<width;inputW++)                for(channel=0;channel<channels;channel++)                    tempImage[inputH][inputW][channel]=                        image->matrix[inputH][inputW][channel];


    cout<<"Writing to "<<filename<<endl;
    if(getFileExtension(filename)==RAW){
        FILE* file;
        if (!(file=fopen(filename,"wb"))) {            cout << "Cannot open file: " << filename << endl;            exit(1);        }
        fwrite(tempImage, sizeof(unsigned char),               height*width*channel, file);
        fclose(file);
    } else if(getFileExtension(filename)==JPG){
        jpge::params params;
        params.m_quality = 90;//1~100, default 85
        int subsampling = (channels==1)?(jpge::Y_ONLY):(jpge::H2V2);
        params.m_subsampling = static_cast<jpge::subsampling_t>(subsampling);
        //params.m_two_pass_flag = (optimize_huffman_tables != 0);

        if (!jpge::compress_image_to_jpeg_file(filename,
             width, height, channels, (const unsigned char*)tempImage, params)){
            cerr<<"jpg encoding error!"<<endl;
            exit(1);
        }
    }
}

// Read image according to position, channel and boundary mirroring method
// Boundary mirroring method:
// 0: default and read outer pixels as 0; usually not care too much
// 1: boundary mirroring: row "-1" would copy row "1", etc.
//    More than one height/width away from boundary would be considered as error
// 2: background mirroring: view 0,0 as background and read outer ones as that
unsigned char readPixel(Image*image, int inHeight, int inWidth, int channel,
                        int mirror){
    if(image==NULL){
        cerr<<"Invalid image!"<<endl;
        exit(1);
    }
    if(channel<0||channel>=image->channels){
        cerr<<"Wrong channel:"<<channel<<endl;
        exit(1);
    }
    int outHeight = 0, outWidth = 0;
    switch(mirror){
        case 0:
            if(inHeight < 0 || inHeight >= image->height ||
                inWidth < 0 || inWidth >= image->width )
                return 0;
            return image->matrix[inHeight][inWidth][channel];
        case 1:
            // Height boundary mirror
            if(inHeight < 0)
                outHeight = -inHeight;
            else if (inHeight >= image->height)
                outHeight = (image->height-1) * 2 - inHeight;
            else
                outHeight = inHeight;
            // Width boundary mirror
            if(inWidth < 0)
                outWidth = - inWidth;
            else if (inWidth >= image->width)
                outWidth = (image->width-1) * 2 - inWidth;
            else
                outWidth = inWidth;
            if(outHeight < 0 || outHeight > image->height ||
               outWidth < 0 || outWidth > image->width)
                cerr<<"Abnormal reading index! "<<inHeight<<","<<inWidth
                    <<"->"<<outHeight<<","<<outWidth<<endl;
            return image->matrix[outHeight][outWidth][channel];
        case 2:
            if(inHeight < 0 || inHeight >= image->height ||
                inWidth < 0 || inWidth >= image->width )
                return image->matrix[0][0][channel];
            return image->matrix[inHeight][inWidth][channel];
        default:
            cerr<<"Mirror method not supported!"<<endl;
            exit(1);
    }
}

// Read using bilinear interpolation. This function would call the above one.
unsigned char readPixel(Image*image, double mappedH, double mappedW,
                        int channel, int mirror){
    // The closest upper height and left width in input image
    int inputH = (int)mappedH;
    int inputW = (int)mappedW;
    // Calculate value by bilinear interpolation
    double outputValue =
        (inputH+1 - mappedH) *
        (   (inputW+1 - mappedW)
          * readPixel(image,inputH,inputW,channel,mirror)
          + (mappedW - inputW)
          * readPixel(image,inputH,inputW+1,channel,mirror) )
      + (mappedH - inputH) *
        (   (inputW+1 - mappedW)
          * readPixel(image,inputH+1,inputW,channel,mirror)
          + (mappedW - inputW)
          * readPixel(image,inputH+1,inputW+1,channel,mirror) )
      ;
    return outputValue;
}

// Write value to given position
void writePixel(Image*image, unsigned char value, int outHeight, int outWidth,
                int channel){
    if(image==NULL){
        cerr<<"Invalid image!"<<endl;
        exit(1);
    }
    if(channel<0||channel>=image->channels){
        cerr<<"Wrong channel:"<<channel<<endl;
        exit(1);
    }
    if(outHeight < 0 || outHeight >= image->height ||
           outWidth < 0 || outWidth >= image->width){
        cerr<<"Abnormal writing index! "<<outHeight<<","<<outWidth<<endl;
        exit(1);
    }
    image->matrix[outHeight][outWidth][channel] = value;
}
