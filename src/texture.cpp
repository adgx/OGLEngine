#include "texture.h"
#include "utils/stb_image.h"
#include "log.h"

namespace SpaceEngine
{
    Texture::Texture(GLenum textureTarget, const std::string& FileName)
    {
        this->textureTarget = textureTarget;
        this->fileName      = fileName;
    }

    Texture::Texture(GLenum textureTarget)
    {
        this->textureTarget = textureTarget;
    }
    bool Texture::load(bool isSRGB = false)
    {
        unsigned char* pImageData = NULL;
        stbi_set_flip_vertically_on_load(1);
        pImageData = stbi_load(fileName.c_str(), &imageWidth, &imageHeight, &imageBPP, 0);

        if (!pImageData) {
            SPACE_ENGINE_ERROR("Can't load texture from '{}' - {}", fileName.c_str(), stbi_failure_reason());
            return false;
        }

        SPACE_ENGINE_INFO("Loaded texture '{}' width {}, height {}, bpp {}", fileName.c_str(), imageWidth, imageHeight, imageBPP);
        loadInternal(pImageData, isSRGB);
        stbi_image_free(pImageData);
        
        return true;
    }

    bool Texture::load(const std::string& fileName, bool isSRGB = false)
    {
        this->fileName = fileName;

        if (!load(isSRGB)) {
            return false;
        }

        return true;
    }

    void Texture::load(uint32_t bufferSize, void* pImageData, bool isSRGB)
    {
        void* pImageData = stbi_load_from_memory((const stbi_uc*)pImageData, bufferSize, &imageWidth, &imageHeight, &imageBPP, 0);
        loadInternal(pImageData, isSRGB);
        stbi_image_free(pImageData);
    }

    void Texture::loadRaw(int width, int height, int BPP, const unsigned char* pImageData, bool isSRGB)
    {
        imageWidth = width;
        imageHeight = height;
        imageBPP = BPP;

        loadInternal(pImageData, isSRGB);
    }

    void Texture::loadInternal(const void* pImageData, bool isSRGB)
    {
        glGenTextures(1, &textureObj);
        glBindTexture(textureTarget, textureObj);

        GLenum internalFormat = GL_NONE;

        if (textureTarget == GL_TEXTURE_2D) {
            switch (imageBPP) {
            case 1: {
                glTexImage2D(textureTarget, 0, GL_RED, imageWidth, imageHeight, 0, GL_RED, GL_UNSIGNED_BYTE, pImageData);
                GLint SwizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_RED };
                glTexParameteriv(textureTarget, GL_TEXTURE_SWIZZLE_RGBA, SwizzleMask);
            }
                    break;

            case 2:
                glTexImage2D(textureTarget, 0, GL_RG, imageWidth, imageHeight, 0, GL_RG, GL_UNSIGNED_BYTE, pImageData);
                break;

            case 3:
                internalFormat = isSRGB ? GL_SRGB8 : GL_RGB8;
                glTexImage2D(textureTarget, 0, internalFormat, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pImageData);
                break;

            case 4:
                internalFormat = isSRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;
                glTexImage2D(textureTarget, 0, internalFormat, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImageData);
                break;

            default:
            }
        } else {
            SPACE_ENGINE_FATAL("Support for texture target {} is not implemented\n", textureTarget);
            exit(1);
        }

        glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(textureTarget, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glGenerateMipmap(textureTarget);

        glBindTexture(textureTarget, 0);
    }

    void Texture::loadF32(int width, int height, const unsigned char* pImageData)
    {
        imageWidth = width;
        imageHeight = height;

        glCreateTextures(textureTarget, 1, &textureObj);
        glTextureStorage2D(textureObj, 1, GL_R32F, imageWidth, imageHeight);
        glTextureSubImage2D(textureObj, 0, 0, 0, imageWidth, imageHeight, GL_RED, GL_FLOAT, pImageData);

        glTextureParameteri(textureObj, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(textureObj, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameterf(textureObj, GL_TEXTURE_BASE_LEVEL, 0);
        glTextureParameteri(textureObj, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(textureObj, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    void Texture::bind(GLenum textureUnit)
    {
        bindInternal(textureUnit);
    }

    void Texture::bindInternal(GLenum textureUnit)
    {
        glActiveTexture(textureUnit);
        glBindTexture(textureTarget, textureObj);
    }

}