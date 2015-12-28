// Image Processing Exercise// Author: Xunzhi Wang// Email: wxzpursue@gmail.com// Resizing via Bilinear Interpolation#include <stdio.h>#include <iostream>
#include <stdlib.h>#include <vector>#include "util.h"
using namespace std;
void resizeImage(Image*InputImage,Image*OutputImage){
    int outputH = 0, outputW = 0;    int channel = 0;    unsigned char output = 0;
    for (outputH=0;outputH<OutputImage->height;outputH++){        // Mapping output height to input        double mappedH = outputH*1.0 /
            (OutputImage->height-1) * (InputImage->height-1);        for (outputW=0;outputW<OutputImage->width;outputW++){            // Mapping output width to input            double mappedW = outputW*1.0 /
                (OutputImage->width-1) * (InputImage->width-1);            // Calculate value by bilinear interpolation            for (channel=0;channel<OutputImage->channels;channel++){                output = readPixel(InputImage,mappedH,mappedW,channel,1);                writePixel(OutputImage,output,outputH,outputW,channel);            }        }    }
}

// Usage: input_image(raw/jpg) output_image(raw/jpg) inHeight inWidth//        outHeight outWidth [Channels = 3]
// When input_image is jpg, inHeight, inWidth and Channels are only references// Function: resizing a color imageint main(int argc, char *argv[]){    // Default parameters
    int Channels = 3; // Default: color image;	int InputHeight = 512, InputWidth = 512;	int OutputHeight = 650, OutputWidth = 650;	// Check for proper syntax	if (argc < 1+2){		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;		cout << argv[0] << "input_image.raw output_image.raw inHeight inWidth\                outHeight outWidth [Channels = 3]" << endl;		return 0;	}
    if(argc >= 1+6){
        InputHeight = atoi(argv[3]);
        InputWidth = atoi(argv[4]);
        OutputHeight = atoi(argv[5]);
        OutputWidth = atoi(argv[6]);
    }	if(argc >= 1+7)		Channels = atoi(argv[7]);	Image InputImage;
    initImage(&InputImage, &InputHeight, &InputWidth, &Channels, argv[1]);
    Image OutputImage;
    initImage(&OutputImage, &OutputHeight, &OutputWidth, &Channels);

    resizeImage(&InputImage,&OutputImage);	image2File(&OutputImage,argv[2]);
	return 0;}