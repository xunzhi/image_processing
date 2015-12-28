// Image Processing Exercise
#include <stdlib.h>

void resizeImage(Image*InputImage,Image*OutputImage){
    int outputH = 0, outputW = 0;
    for (outputH=0;outputH<OutputImage->height;outputH++){
            (OutputImage->height-1) * (InputImage->height-1);
                (OutputImage->width-1) * (InputImage->width-1);
}

// Usage: input_image(raw/jpg) output_image(raw/jpg) inHeight inWidth
// When input_image is jpg, inHeight, inWidth and Channels are only references
    int Channels = 3; // Default: color image;
    if(argc >= 1+6){
        InputHeight = atoi(argv[3]);
        InputWidth = atoi(argv[4]);
        OutputHeight = atoi(argv[5]);
        OutputWidth = atoi(argv[6]);
    }
    initImage(&InputImage, &InputHeight, &InputWidth, &Channels, argv[1]);
    Image OutputImage;
    initImage(&OutputImage, &OutputHeight, &OutputWidth, &Channels);

    resizeImage(&InputImage,&OutputImage);
