#ifndef IMAGE_READER_H
#define IMAGE_READER_H

#include "../Core/StandardIncludes.h"

typedef struct
{
    short Width;
    short Height;
    short BitsPerPixel;
    short DataOffset;
} ImageInfo;

#include "../Graphics/TGAReader.h"
#include "../Graphics/PNGReader.h"

class Asset;

class ImageReader
{
public:
    ImageReader();
    virtual ~ImageReader();

    // Auto-detect Image extension
    Asset* LoadImage(const string& _file, ImageInfo* _imageInfo);
    
    // Cache for ImageInfo to avoid reprocessing
    static map<string, ImageInfo> s_imageInfoCache;

private:
    bool IsPNG(const unsigned char* _data, int _size);
    bool IsTGA(const unsigned char* _data, int _size);
};

#endif // IMAGE_READER_H