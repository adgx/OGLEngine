#include "mesh.h"
#include "log.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | \
     aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)

#define POSITION_LOCATION  0
#define TEX_COORD_LOCATION 1
#define NORMAL_LOCATION    2

#define COLOR_TEXTURE_UNIT              GL_TEXTURE0
#define COLOR_TEXTURE_UNIT_INDEX        0
#define SHADOW_TEXTURE_UNIT             GL_TEXTURE1
#define SHADOW_TEXTURE_UNIT_INDEX       1
#define NORMAL_TEXTURE_UNIT             GL_TEXTURE2
#define NORMAL_TEXTURE_UNIT_INDEX       2
#define RANDOM_TEXTURE_UNIT             GL_TEXTURE3
#define RANDOM_TEXTURE_UNIT_INDEX       3
#define DISPLACEMENT_TEXTURE_UNIT       GL_TEXTURE4
#define DISPLACEMENT_TEXTURE_UNIT_INDEX 4
#define ALBEDO_TEXTURE_UNIT             GL_TEXTURE5
#define ALBEDO_TEXTURE_UNIT_INDEX       5          
#define ROUGHNESS_TEXTURE_UNIT          GL_TEXTURE6
#define ROUGHNESS_TEXTURE_UNIT_INDEX    6
#define MOTION_TEXTURE_UNIT             GL_TEXTURE7
#define MOTION_TEXTURE_UNIT_INDEX       7
#define SPECULAR_EXPONENT_UNIT             GL_TEXTURE8
#define SPECULAR_EXPONENT_UNIT_INDEX       8
#define CASCACDE_SHADOW_TEXTURE_UNIT0               SHADOW_TEXTURE_UNIT
#define CASCACDE_SHADOW_TEXTURE_UNIT0_INDEX         SHADOW_TEXTURE_UNIT_INDEX
#define CASCACDE_SHADOW_TEXTURE_UNIT1               GL_TEXTURE9
#define CASCACDE_SHADOW_TEXTURE_UNIT1_INDEX         9
#define CASCACDE_SHADOW_TEXTURE_UNIT2               GL_TEXTURE10
#define CASCACDE_SHADOW_TEXTURE_UNIT2_INDEX         10
#define SHADOW_CUBE_MAP_TEXTURE_UNIT                GL_TEXTURE11
#define SHADOW_CUBE_MAP_TEXTURE_UNIT_INDEX          11
#define SHADOW_MAP_RANDOM_OFFSET_TEXTURE_UNIT       GL_TEXTURE12
#define SHADOW_MAP_RANDOM_OFFSET_TEXTURE_UNIT_INDEX 12
#define METALLIC_TEXTURE_UNIT                       GL_TEXTURE14
#define METALLIC_TEXTURE_UNIT_INDEX                 14
#define HEIGHT_TEXTURE_UNIT                         GL_TEXTURE15
#define HEIGHT_TEXTURE_UNIT_INDEX                   15

namespace SpaceEngine
{
    std::string getFullPath(const std::string& dir, const aiString& path)
    {
        std::string p(path.data);

        if (p == "C:\\\\") {
            p = "";
        }
        else if (p.substr(0, 2) == ".\\") {
            p = p.substr(2, p.size() - 2);
        }

        std::string fullPath = dir + "/" + p;

        return fullPath;
    }

    void Mesh::clear()
    {
        if(buffers[0] != 0)
        {
            glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(buffers), buffers);
        }

        if(VAO != 0)
        {
            glDeleteVertexArrays(1, &VAO);
            VAO = 0;
        }
    }

    std::string getDirFromFilename(const std::string& fileName)
    {
        // Extract the directory part from the file name
        std::string::size_type slashIndex;

    #ifdef _WIN64
        slashIndex = fileName.find_last_of("\\");

        if (slashIndex == -1) {
            slashIndex = fileName.find_last_of("/");
        }
    #else
        SlashIndex = Filename.find_last_of("/");
    #endif

        std::string dir;

        if (slashIndex == std::string::npos) {
            dir = ".";
        }
        else if (slashIndex == 0) {
            dir = "/";
        }
        else {
            dir = fileName.substr(0, slashIndex);
        }

        return dir;
    }

    bool Mesh::loadMesh(const std::string& fileName)
    {
        clear();

        //VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(buffers), buffers);

        bool ret = false;

        pScene = importer.ReadFile(fileName.c_str(), ASSIMP_LOAD_FLAGS);

        if(pScene)
        {
            globalInverseTransform = aiMat4_2_Mat4(pScene->mRootNode->mTransformation);
            globalInverseTransform = inverse(globalInverseTransform);
            ret = initFromScene(pScene, fileName);
        }

    }

    bool Mesh::initFromScene(const aiScene* pScene, const std::string& fileName)
    {
        meshes.resize(pScene->mNumMeshes);
        materials.resize(pScene->mNumMaterials);

        unsigned int NumVertices = 0;
        unsigned int numIndices = 0;

        countVerticesAndIndices(pScene, NumVertices, numIndices);

        reserveSpace(NumVertices, numIndices);

        initAllMeshes(pScene);

        if (!initMaterials(pScene, fileName)) {
            return false;
        }

        populateBuffers();

        return glGetError() == GL_NO_ERROR;
    }

    void Mesh::countVerticesAndIndices(const aiScene* pScene, unsigned int& numVertices, unsigned int& numIndices)
    {
        for (unsigned int i = 0 ; i < meshes.size() ; i++) {
            meshes[i].materialIndex = pScene->mMeshes[i]->mMaterialIndex;
            meshes[i].numIndices = pScene->mMeshes[i]->mNumFaces * 3;
            meshes[i].baseVertex = numVertices;
            meshes[i].baseIndex = numIndices;

            numVertices += pScene->mMeshes[i]->mNumVertices;
            numIndices  += meshes[i].numIndices;
        }
    }

    void Mesh::reserveSpace(unsigned int numVertices, unsigned int numIndices)
    {
        vertices.reserve(numVertices);
        indices.reserve(numIndices);
    }


    void Mesh::initAllMeshes(const aiScene* pScene)
    {
        for (unsigned int i = 0 ; i < meshes.size() ; i++) 
        {
            const aiMesh* paiMesh = pScene->mMeshes[i];
            initSingleMesh(i, paiMesh);
        }
    }


    void Mesh::initSingleMesh(uint32_t meshIndex, const aiMesh* paiMesh)
    {
        const aiVector3D zero3D(0.0f, 0.0f, 0.0f);

        // SPACE_ENGINE_INFO("Mesh {}", meshIndex);
        // Populate the vertex attribute vectors
        Vertex v;

        for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
            const aiVector3D& pPos = paiMesh->mVertices[i];

            v.position = Vector3(pPos.x, pPos.y, pPos.z);

            if (paiMesh->mNormals) {
                const aiVector3D& pNormal = paiMesh->mNormals[i];
                v.normal = Vector3(pNormal.x, pNormal.y, pNormal.z);
            } else {
                aiVector3D Normal(0.0f, 1.0f, 0.0f);
                v.normal = Vector3(Normal.x, Normal.y, Normal.z);
            }

            const aiVector3D& pTexCoord = paiMesh->HasTextureCoords(0) ? paiMesh->mTextureCoords[0][i] : zero3D;
            v.texCoords = Vector2(pTexCoord.x, pTexCoord.y);

            vertices.push_back(v);
        }

        // Populate the index buffer
        for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
            const aiFace& face = paiMesh->mFaces[i];
            indices.push_back(face.mIndices[0]);
            indices.push_back(face.mIndices[1]);
            indices.push_back(face.mIndices[2]);
        }
    }

    bool Mesh::initMaterials(const aiScene* pScene, const std::string& fileName)
    {
        std::string dir = getDirFromFilename(fileName);

        SPACE_ENGINE_DEBUG("Num materials: {}", pScene->mNumMaterials);

        // Initialize the materials
        for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++) {
            const aiMaterial* pMaterial = pScene->mMaterials[i];

            loadTextures(dir, pMaterial, i);
            loadColors(pMaterial, i);
        }

        return true;
    }


    void Mesh::loadTextures(const std::string& dir, const aiMaterial* pMaterial, int index)
    {
        loadDiffuseTexture(dir, pMaterial, index);
        loadSpecularTexture(dir, pMaterial, index);

        // PBR
        loadAlbedoTexture(dir, pMaterial, index);
        loadMetalnessTexture(dir, pMaterial, index);
        loadRoughnessTexture(dir, pMaterial, index);
    }


    void Mesh::loadDiffuseTexture(const std::string& dir, const aiMaterial* pMaterial, int materialIndex)
    {
        materials[materialIndex].pTextures[TEXTURE_TYPE::BASE] = NULL;

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                const aiTexture* paiTexture = pScene->GetEmbeddedTexture(path.C_Str());

                if (paiTexture) {
                    loadDiffuseTextureEmbedded(paiTexture, materialIndex);
                } else {
                    loadDiffuseTextureFromFile(dir, path, materialIndex);
                }
            }
        }
    }


    void Mesh::loadDiffuseTextureEmbedded(const aiTexture* paiTexture, int materialIndex)
    {
        SPACE_ENGINE_DEBUG("Embeddeded diffuse texture type '{}'\n", paiTexture->achFormatHint);
        materials[materialIndex].pTextures[TEXTURE_TYPE::BASE] = new Texture(GL_TEXTURE_2D);
        int buffer_size = paiTexture->mWidth;
        bool IsSRGB = true;
        materials[materialIndex].pTextures[TEXTURE_TYPE::BASE]->load(buffer_size, paiTexture->pcData, IsSRGB);
    }


    void Mesh::loadDiffuseTextureFromFile(const std::string& dir, const aiString& path, int materialIndex)
    {
        std::string fullPath = getFullPath(dir, path);

        materials[materialIndex].pTextures[TEXTURE_TYPE::BASE] = new Texture(GL_TEXTURE_2D, fullPath.c_str());

        bool IsSRGB = true;

        if (!materials[materialIndex].pTextures[TEXTURE_TYPE::BASE]->load(IsSRGB)) {
            SPACE_ENGINE_ERROR("Error loading diffuse texture '{}'", fullPath.c_str());
            exit(0);
        }
        else {
            SPACE_ENGINE_INFO("Loaded diffuse texture '{}' at index {}", fullPath.c_str(), materialIndex);
        }
    }


    void Mesh::loadSpecularTexture(const std::string& dir, const aiMaterial* pMaterial, int materialIndex)
    {
        materials[materialIndex].pTextures[TEXTURE_TYPE::SPECULAR] = NULL;

        if (pMaterial->GetTextureCount(aiTextureType_SHININESS) > 0) {
            aiString path;

            if (pMaterial->GetTexture(aiTextureType_SHININESS, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                const aiTexture* paiTexture = pScene->GetEmbeddedTexture(path.C_Str());

                if (paiTexture) {
                    loadSpecularTextureEmbedded(paiTexture, materialIndex);
                } else {
                    loadSpecularTextureFromFile(dir, path, materialIndex);
                }
            }
        }
    }


    void Mesh::loadSpecularTextureEmbedded(const aiTexture* paiTexture, int materialIndex)
    {
        SPACE_ENGINE_DEBUG("Embeddeded specular texture type '{}'", paiTexture->achFormatHint);
        materials[materialIndex].pTextures[TEXTURE_TYPE::SPECULAR] = new Texture(GL_TEXTURE_2D);
        int buffer_size = paiTexture->mWidth;
        bool IsSRGB = false;
        materials[materialIndex].pTextures[TEXTURE_TYPE::SPECULAR]->load(buffer_size, paiTexture->pcData, IsSRGB);
    }


    void Mesh::loadSpecularTextureFromFile(const std::string& dir, const aiString& path, int materialIndex)
    {
        std::string fullPath = getFullPath(dir, path);

        materials[materialIndex].pTextures[TEXTURE_TYPE::SPECULAR] = new Texture(GL_TEXTURE_2D, fullPath.c_str());

        bool IsSRGB = false;

        if (!materials[materialIndex].pTextures[TEXTURE_TYPE::SPECULAR]->load(IsSRGB)) {
            SPACE_ENGINE_ERROR("Error loading specular texture '{}'", fullPath.c_str());
            exit(0);
        }
        else {
            SPACE_ENGINE_INFO("Loaded specular texture '{}'", fullPath.c_str());
        }
    }


    void Mesh::loadAlbedoTexture(const std::string& dir, const aiMaterial* pMaterial, int materialIndex)
    {
        materials[materialIndex].PBRmaterial.pAlbedo = NULL;

        if (pMaterial->GetTextureCount(aiTextureType_BASE_COLOR) > 0) {
            aiString path;

            if (pMaterial->GetTexture(aiTextureType_BASE_COLOR, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                const aiTexture* paiTexture = pScene->GetEmbeddedTexture(path.C_Str());

                if (paiTexture) {
                    loadAlbedoTextureEmbedded(paiTexture, materialIndex);
                } else {
                    loadAlbedoTextureFromFile(dir, path, materialIndex);
                }
            }
        }
    }


    void Mesh::loadAlbedoTextureEmbedded(const aiTexture* paiTexture, int materialIndex)
    {
        SPACE_ENGINE_DEBUG("Embeddeded albedo texture type '{}'", paiTexture->achFormatHint);
        materials[materialIndex].PBRmaterial.pAlbedo = new Texture(GL_TEXTURE_2D);
        int buffer_size = paiTexture->mWidth;
        bool IsSRGB = true;
        materials[materialIndex].PBRmaterial.pAlbedo->load(buffer_size, paiTexture->pcData, IsSRGB);
    }


    void Mesh::loadAlbedoTextureFromFile(const std::string& dir, const aiString& path, int materialIndex)
    {
        std::string FullPath = getFullPath(dir, path);

        materials[materialIndex].PBRmaterial.pAlbedo = new Texture(GL_TEXTURE_2D, FullPath.c_str());

        bool IsSRGB = true;

        if (!materials[materialIndex].PBRmaterial.pAlbedo->load(IsSRGB)) {
            SPACE_ENGINE_ERROR("Error loading albedo texture '{}'", FullPath.c_str());
            exit(0);
        }
        else {
            SPACE_ENGINE_INFO("Loaded albedo texture '{}'", FullPath.c_str());
        }
    }


    void Mesh::loadMetalnessTexture(const std::string& dir, const aiMaterial* pMaterial, int materialIndex)
    {
        materials[materialIndex].PBRmaterial.pMetallic = NULL;

        int NumTextures = pMaterial->GetTextureCount(aiTextureType_METALNESS);

        if (NumTextures > 0) {
            SPACE_ENGINE_DEBUG("Num metalness textures {}", NumTextures);

            aiString path;

            if (pMaterial->GetTexture(aiTextureType_METALNESS, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                const aiTexture* paiTexture = pScene->GetEmbeddedTexture(path.C_Str());

                if (paiTexture) {
                    loadMetalnessTextureEmbedded(paiTexture, materialIndex);
                }
                else {
                    loadMetalnessTextureFromFile(dir, path, materialIndex);
                }
            }
        }
    }


    void Mesh::loadMetalnessTextureEmbedded(const aiTexture* paiTexture, int materialIndex)
    {
        SPACE_ENGINE_DEBUG("Embeddeded metalness texture type '{}'", paiTexture->achFormatHint);
        materials[materialIndex].PBRmaterial.pMetallic = new Texture(GL_TEXTURE_2D);
        int buffer_size = paiTexture->mWidth;
        bool IsSRGB = false;
        materials[materialIndex].PBRmaterial.pMetallic->load(buffer_size, paiTexture->pcData, IsSRGB);
    }


    void Mesh::loadMetalnessTextureFromFile(const std::string& dir, const aiString& path, int materialIndex)
    {
        std::string FullPath = getFullPath(dir, path);

        materials[materialIndex].PBRmaterial.pMetallic = new Texture(GL_TEXTURE_2D, FullPath.c_str());

        bool IsSRGB = false;

        if (!materials[materialIndex].PBRmaterial.pMetallic->load(IsSRGB)) {
            SPACE_ENGINE_ERROR("Error loading metalness texture '{}'", FullPath.c_str());
            exit(0);
        }
        else {
            SPACE_ENGINE_INFO("Loaded metalness texture '{}'", FullPath.c_str());
        }
    }


    void Mesh::loadRoughnessTexture(const std::string& dir, const aiMaterial* pMaterial, int materialIndex)
    {
        materials[materialIndex].PBRmaterial.pRoughness = NULL;

        int NumTextures = pMaterial->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS);

        if (NumTextures > 0) {
            SPACE_ENGINE_DEBUG("Num roughness textures {}", NumTextures);

            aiString path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                const aiTexture* paiTexture = pScene->GetEmbeddedTexture(path.C_Str());

                if (paiTexture) {
                    loadRoughnessTextureEmbedded(paiTexture, materialIndex);
                }
                else {
                    loadRoughnessTextureFromFile(dir, path, materialIndex);
                }
            }
        }
    }


    void Mesh::loadRoughnessTextureEmbedded(const aiTexture* paiTexture, int materialIndex)
    {
        SPACE_ENGINE_DEBUG("Embeddeded roughness texture type '{}'", paiTexture->achFormatHint);
        materials[materialIndex].PBRmaterial.pRoughness = new Texture(GL_TEXTURE_2D);
        int buffer_size = paiTexture->mWidth;
        bool IsSRGB = false;
        materials[materialIndex].PBRmaterial.pRoughness->load(buffer_size, paiTexture->pcData, IsSRGB);
    }


    void Mesh::loadRoughnessTextureFromFile(const std::string& dir, const aiString& path, int materialIndex)
    {
        std::string FullPath = getFullPath(dir, path);

        materials[materialIndex].PBRmaterial.pRoughness = new Texture(GL_TEXTURE_2D, FullPath.c_str());
        bool IsSRGB = false;

        if (!materials[materialIndex].PBRmaterial.pRoughness->load(IsSRGB)) {
            SPACE_ENGINE_ERROR("Error loading roughness texture '{}'", FullPath.c_str());
            exit(0);
        }
        else {
            SPACE_ENGINE_INFO("Loaded roughness texture '{}'", FullPath.c_str());
        }
    }


    void Mesh::loadColors(const aiMaterial* pMaterial, int index)
    {
        aiColor4D AmbientColor(0.0f, 0.0f, 0.0f, 0.0f);
        Vector4 AllOnes(1.0f);

        int ShadingModel = 0;
        if (pMaterial->Get(AI_MATKEY_SHADING_MODEL, ShadingModel) == AI_SUCCESS) {
            SPACE_ENGINE_DEBUG("Shading model {}", ShadingModel);
        }

        if (pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, AmbientColor) == AI_SUCCESS) {
            SPACE_ENGINE_INFO("Loaded ambient color [{} {} {}]", AmbientColor.r, AmbientColor.g, AmbientColor.b);
            materials[index].AmbientColor.r = AmbientColor.r;
            materials[index].AmbientColor.g = AmbientColor.g;
            materials[index].AmbientColor.b = AmbientColor.b;
        } else {
            materials[index].AmbientColor = AllOnes;
        }

        aiColor3D DiffuseColor(0.0f, 0.0f, 0.0f);

        if (pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, DiffuseColor) == AI_SUCCESS) {
            SPACE_ENGINE_INFO("Loaded diffuse color [{} {} {}]", DiffuseColor.r, DiffuseColor.g, DiffuseColor.b);
            materials[index].DiffuseColor.r = DiffuseColor.r;
            materials[index].DiffuseColor.g = DiffuseColor.g;
            materials[index].DiffuseColor.b = DiffuseColor.b;
        }

        aiColor3D SpecularColor(0.0f, 0.0f, 0.0f);

        if (pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, SpecularColor) == AI_SUCCESS) {
            SPACE_ENGINE_INFO("Loaded specular color [{} {} {}]\n", SpecularColor.r, SpecularColor.g, SpecularColor.b);
            materials[index].SpecularColor.r = SpecularColor.r;
            materials[index].SpecularColor.g = SpecularColor.g;
            materials[index].SpecularColor.b = SpecularColor.b;
        }
    }


    void Mesh::populateBuffers()
    {
        populateBuffersNonDSA();
    }


    void Mesh::populateBuffersNonDSA()
    {
        glBindBuffer(GL_ARRAY_BUFFER, buffers[VERTEX_BUFFER]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDEX_BUFFER]);

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

        size_t NumFloats = 0;

        glEnableVertexAttribArray(POSITION_LOCATION);
        glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(NumFloats * sizeof(float)));
        NumFloats += 3;

        glEnableVertexAttribArray(TEX_COORD_LOCATION);
        glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(NumFloats * sizeof(float)));
        NumFloats += 2;

        glEnableVertexAttribArray(NORMAL_LOCATION);
        glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(NumFloats * sizeof(float)));
    }


    // Introduced in youtube tutorial #18
    void Mesh::render(IRenderCallbacks* pRenderCallbacks)
    {
        if (isPBR) {
            setupRenderMaterialsPBR();
        }

        glBindVertexArray(VAO);

        for (unsigned int meshIndex = 0 ; meshIndex < meshes.size() ; meshIndex++) {
            unsigned int materialIndex = meshes[meshIndex].materialIndex;
            assert(materialIndex < materials.size());

            if (!isPBR) {
                setupRenderMaterialsPhong(meshIndex, materialIndex, pRenderCallbacks);
            }

            glDrawElementsBaseVertex(GL_TRIANGLES,
                                     meshes[meshIndex].numIndices,
                                     GL_UNSIGNED_INT,
                                     (void*)(sizeof(unsigned int) * meshes[meshIndex].baseIndex),
                                     meshes[meshIndex].baseVertex);
        }

        // Make sure the VAO is not changed from the outside
        glBindVertexArray(0);
    }


    void Mesh::setupRenderMaterialsPhong(unsigned int meshIndex, unsigned int materialIndex, IRenderCallbacks* pRenderCallbacks)
    {
        if (materials[materialIndex].pTextures[TEXTURE_TYPE::BASE]) {
            materials[materialIndex].pTextures[TEXTURE_TYPE::BASE]->bind(COLOR_TEXTURE_UNIT);
        }

        if (materials[materialIndex].pTextures[TEXTURE_TYPE::SPECULAR]) {
            materials[materialIndex].pTextures[TEXTURE_TYPE::SPECULAR]->bind(SPECULAR_EXPONENT_UNIT);

            if (pRenderCallbacks) {
                pRenderCallbacks->controlSpecularExponent(true);
            }
        } else {
            if (pRenderCallbacks) {
                pRenderCallbacks->controlSpecularExponent(false);
            }
        }

        if (pRenderCallbacks) {
            if (materials[materialIndex].pTextures[TEXTURE_TYPE::BASE]) {
                pRenderCallbacks->drawStartCB(meshIndex);
                pRenderCallbacks->setMaterial(materials[materialIndex]);
            }  else {
                pRenderCallbacks->disableDiffuseTexture();
            }
        }
    }


    void Mesh::setupRenderMaterialsPBR()
    {
        int PBRMaterialIndex = 0;

        if (materials[PBRMaterialIndex].PBRmaterial.pAlbedo) {
            materials[PBRMaterialIndex].PBRmaterial.pAlbedo->bind(ALBEDO_TEXTURE_UNIT);
        }

        if (materials[PBRMaterialIndex].PBRmaterial.pRoughness) {
            materials[PBRMaterialIndex].PBRmaterial.pRoughness->bind(ROUGHNESS_TEXTURE_UNIT);
        }

        if (materials[PBRMaterialIndex].PBRmaterial.pMetallic) {
            materials[PBRMaterialIndex].PBRmaterial.pMetallic->bind(METALLIC_TEXTURE_UNIT);
        }

        if (materials[PBRMaterialIndex].PBRmaterial.pNormalMap) {
            materials[PBRMaterialIndex].PBRmaterial.pNormalMap->bind(NORMAL_TEXTURE_UNIT);
        }

    }


    void Mesh::render(unsigned int drawIndex, unsigned int primID)
    {
        glBindVertexArray(VAO);

        unsigned int materialIndex = meshes[drawIndex].materialIndex;
        assert(materialIndex < materials.size());

        if (materials[materialIndex].pTextures[TEXTURE_TYPE::BASE]) {
            materials[materialIndex].pTextures[TEXTURE_TYPE::BASE]->bind(COLOR_TEXTURE_UNIT);
        }

        if (materials[materialIndex].pTextures[TEXTURE_TYPE::SPECULAR]) {
            materials[materialIndex].pTextures[TEXTURE_TYPE::SPECULAR]->bind(SPECULAR_EXPONENT_UNIT);
        }

        glDrawElementsBaseVertex(GL_TRIANGLES,
                                 3,
                                 GL_UNSIGNED_INT,
                                 (void*)(sizeof(unsigned int) * (meshes[drawIndex].baseIndex + primID * 3)),
                                 meshes[drawIndex].baseVertex);

        // Make sure the VAO is not changed from the outside
        glBindVertexArray(0);
    }



    // Used only by instancing
    void Mesh::render(unsigned int numInstannes, const Matrix4* WVPMats, const Matrix4* worldMats)
    {
        glBindBuffer(GL_ARRAY_BUFFER, buffers[WVP_MAT_BUFFER]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Matrix4) * numInstannes, WVPMats, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, buffers[WORLD_MAT_BUFFER]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Matrix4) * numInstannes, worldMats, GL_DYNAMIC_DRAW);

        glBindVertexArray(VAO);

        for (unsigned int i = 0 ; i < meshes.size() ; i++) {
            const unsigned int materialIndex = meshes[i].materialIndex;

            assert(materialIndex < materials.size());

            if (materials[materialIndex].pTextures[TEXTURE_TYPE::BASE]) {
                materials[materialIndex].pTextures[TEXTURE_TYPE::BASE]->bind(COLOR_TEXTURE_UNIT);
            }

            if (materials[materialIndex].pTextures[TEXTURE_TYPE::SPECULAR]) {
                materials[materialIndex].pTextures[TEXTURE_TYPE::SPECULAR]->bind(SPECULAR_EXPONENT_UNIT);
            }

            glDrawElementsInstancedBaseVertex(GL_TRIANGLES,
                                              meshes[i].numIndices,
                                              GL_UNSIGNED_INT,
                                              (void*)(sizeof(unsigned int) * meshes[i].baseIndex),
                                              numInstannes,
                                              meshes[i].baseVertex);
        }

        // Make sure the VAO is not changed from the outside
        glBindVertexArray(0);
    }


    const Material& Mesh::getMaterial()
    {
        for (unsigned int i = 0 ; i < materials.size() ; i++) {
            if (materials[i].AmbientColor != Vector4(0.0f)) {
                return materials[i];
            }
        }

        if (materials.size() == 0) {
            SPACE_ENGINE_ERROR("No materials");
            exit(0);
        }

        return materials[0];
    }


    void Mesh::getLeadingVertex(uint32_t drawIndex, uint32_t primID, Vector3& vertex)
    {
        uint32_t meshIndex = drawIndex; // Each mesh is rendered in its own draw call

        assert(meshIndex < pScene->mNumMeshes);
        const aiMesh* paiMesh = pScene->mMeshes[meshIndex];

        assert(primID < paiMesh->mNumFaces);
        const aiFace& Face = paiMesh->mFaces[primID];

        uint32_t LeadingIndex = Face.mIndices[0];

        assert(LeadingIndex < paiMesh->mNumVertices);
        const aiVector3D& Pos = paiMesh->mVertices[LeadingIndex];
        vertex.x = Pos.x;
        vertex.y = Pos.y;
        vertex.z = Pos.z;
    }
}