#include "../Graphics/ImageReader.h"
#include "../Resources/AssetController.h"

// Initialize static cache
// Stores image dimensions (width, height, etc.) for each loaded file
map<string, ImageInfo> ImageReader::s_imageInfoCache;

ImageReader::ImageReader() 
{
}

ImageReader::~ImageReader() 
{
}

bool ImageReader::IsPNG(const unsigned char* _data, int _size)
{
    if (_size < 4) return false;
    // PNG signature: 89 50 4E 47
    return (_data[0] == 0x89 &&
        _data[1] == 0x50 &&
        _data[2] == 0x4E &&
        _data[3] == 0x47);
}

bool ImageReader::IsTGA(const unsigned char* _data, int size)
{
    if (size < 18) return false;

    const TGAHeader* hdr = (const TGAHeader*)_data;

    // Valid TGA: uncompressed RGB == 2
    return (hdr->DataTypeCode == 2 &&
        (hdr->BitsPerPixel == 24 || hdr->BitsPerPixel == 32));
}

Asset* ImageReader::LoadImage(const string& _file, ImageInfo* _imageInfo)
{
    // Get the asset from AssetController (may be cached from previous load)
    Asset* raw = AssetController::Instance().GetAsset(_file);

    // Check if we've already loaded this image before
    // If yes, use the cached ImageInfo instead of processing again
    if (s_imageInfoCache.count(_file) > 0)
    {
        // File was already processed, just copy the cached info
        *_imageInfo = s_imageInfoCache[_file];
        return raw;
    }

    // New File
    unsigned char* data = raw->GetData();
    int size = raw->GetDataSize();

    // Check file extension
    string low = _file;
    std::transform(low.begin(), low.end(), low.begin(), ::tolower);

    if (low.find(".png") != string::npos || IsPNG(data, size))
    {
        PNGReader png;
        png.ProcessAsset(raw, _imageInfo);  // Convert PNG to RGBA pixels
        s_imageInfoCache[_file] = *_imageInfo;  // Save ImageInfo for next time
        return raw;
    }

    if (low.find(".tga") != string::npos || IsTGA(data, size))
    {
        TGAReader tga;
        tga.ProcessAsset(raw, _imageInfo);  // Process TGA
        s_imageInfoCache[_file] = *_imageInfo;  // Save ImageInfo for next time
        return raw;
    }

    // Fallback detection by signature
    if (IsPNG(data, size))
    {
        PNGReader png;
        png.ProcessAsset(raw, _imageInfo);
        s_imageInfoCache[_file] = *_imageInfo;
        return raw;
    }

    // Default fallback TGA
    TGAReader tga;
    tga.ProcessAsset(raw, _imageInfo);
    s_imageInfoCache[_file] = *_imageInfo;
    return raw;
}