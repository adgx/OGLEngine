#pragma once
#include "utils/math.h"
#include "texture.h"

namespace SpaceEngine
{
    struct PBRMaterial
    {
        float roughness = 0.f;
        bool isMetal = false;
        Vector3 color = Vector3(0.f, 0.f, 0.f);
        Texture* pAlbedo = nullptr;
        Texture* pRoughness = nullptr;
        Texture* pMetallic = nullptr;
        Texture* pNormalMap = nullptr;
        Texture* pAO = nullptr;
        Texture* pEmissive = nullptr;
    };

    enum TEXTURE_TYPE 
    {
        BASE = 0,      // Base color / diffuse / albedo
        SPECULAR = 1,
        NORMAL = 2,
        METALNESS = 3,
        EMISSIVE = 4,
        NORMAL_CAMERA = 5,
        EMISSION_COLOR = 6,
        ROUGHNESS = 7,
        AMBIENT_OCCLUSION = 8,
        CLEARCOAT = 9,
        CLEARCOAT_ROUGHNESS = 10,
        CLEARCOAT_NORMAL = 11,
        COUNT = 12
    };

    class Material 
    {

        public:

            std::string name;

            Vector4 AmbientColor = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
            Vector4 DiffuseColor = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
            Vector4 SpecularColor = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
            Vector4 BaseColor = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
            Vector4 EmissiveColor = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
            Vector4 MetallicRoughnessNormalOcclusion = Vector4(1.0f);
            Vector4 ClearCoatTransmissionThickness = Vector4(1.0f);

            PBRMaterial PBRmaterial;

            Texture* pTextures[TEXTURE_TYPE::COUNT] = { 0 };

            float transparencyFactor = 1.0f;
            float alphaTest = 0.0f;
            uint32_t flags = 0;

            ~Material()
            {
                for (Texture* pTex : pTextures) {
                    delete pTex;
                }
            }
    };
}