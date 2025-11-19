#ifndef PNG_READER_H
#define PNG_READER_H

#include "StandardIncludes.h"
#include "ImageReader.h"

class Asset;

class PNGReader
{
public:
    // Constructors/ Destructors
    PNGReader();
    virtual ~PNGReader();

    void ProcessAsset(Asset* _rawPNG, ImageInfo* _imageInfo);
    Asset* LoadPNGFromFile(string _file, ImageInfo* _imageInfo);

private:
    Asset* m_data;
};

#endif // PNG_READER_H
