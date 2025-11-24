#pragma once

#include <string>

#include <glad/gl.h>

namespace SpaceEngine
{
    class Texture
    {
        public:
            Texture(GLenum textureTarget, const std::string& fileName);
            Texture(GLenum textureTarget);
            bool load(bool isSRGB = false);
            bool load(const std::string& fileName, bool isSRGB = false);
            void load(uint32_t bufferSize, void* pImageData, bool isSRGB);
            void loadRaw(int width, int height, int BPP, const unsigned char* pImageData, bool isSRGB);
            void loadF32(int width, int height, const unsigned char* pImageData);
            void bind(GLenum textureUnit);

            void getImageSize(int& imageWidth, int& imageHeight)
            {
                imageWidth = this->imageWidth;
                imageHeight = this->imageHeight;
            }

            GLuint getTexture() const { return textureObj;}
            GLuint getBindlessHandle() const { return bindlessHandle;}

            private:
                void loadInternal(const void* pImageData, bool IsSRGB);
                void bindInternal(GLenum textureUnit);
                
                std::string fileName;
                GLuint64 bindlessHandle = -1;
                int imageWidth = 0;
                int imageHeight = 0;
                int imageBPP = 0;
                GLenum textureTarget = 0;
                GLenum textureObj = 0;
    }; 
}