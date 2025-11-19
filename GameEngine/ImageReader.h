#ifndef IMAGE_READER_H
#define IMAGE_READER_H

#include "StandardIncludes.h"

typedef struct
{
    short Width;
    short Height;
    short BitsPerPixel;
    short DataOffset;
} ImageInfo;

#include "TGAReader.h"
#include "PNGReader.h"

class Asset;

class ImageReader
{
public:
    ImageReader();
    virtual ~ImageReader();

    // Auto-detect Image extension
    Asset* LoadImage(const string& _file, ImageInfo* _imageInfo);

private:
    bool IsPNG(const unsigned char* _data, int _size);
    bool IsTGA(const unsigned char* _data, int _size);
};

#endif // IMAGE_READER_H