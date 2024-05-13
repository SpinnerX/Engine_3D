#pragma once
#include <Engine3D/Core/core.h>
#include <string>

namespace Engine3D{

    /**
     * @class Image
     * @note Used for loading and reading/writing our images
    */
    class Image{
    public:
        Image(const std::string& filename);
        Image(int w, int h);

        ~Image();

        bool IsLoaded() const;

        void* GetImageData();

        int GetWidth() const;
        int GetHeight()const;

        int GetPixelChannels();

        void SetVerticalOnLoad(bool enabled);

    private:
        int width, height, pixelChannels;
        unsigned char* data = nullptr;
        bool isImageLoadedSuccessfully = false;
    };
};