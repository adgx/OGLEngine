#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad/gl.h>
#include "utils/math.h"
#include "material.h"

#define INVALID_MATERIAL 0xFFFFFFFF


namespace SpaceEngine
{
    class IRenderCallbacks
    {
    public:
        virtual void drawStartCB(uint32_t DrawIndex) {}
        virtual void controlSpecularExponent(bool IsEnabled) {}
        virtual void setMaterial(const Material& material) {}
        virtual void disableDiffuseTexture() {}
    };

    class Mesh
    {
        public:
            Mesh() = default;
            ~Mesh();
            bool loadMesh(const std::string& fileName);
        private:
            void clear();
            bool initFromScene(const aiScene* pScene, const std::string& fileName);
            void countVerticesAndIndices(const aiScene* pScene, unsigned int& numVertices, unsigned int& numIndices);
            void reserveSpace(unsigned int numVertices, unsigned int numIndices);
            void initAllMeshes(const aiScene* pScene);
            void initSingleMesh(uint32_t meshIndex, const aiMesh* paiMesh);
            bool initMaterials(const aiScene* pScene, const std::string& fileName);
            void loadTextures(const std::string& dir, const aiMaterial* pMaterial, int index);
            void loadDiffuseTexture(const std::string& dir, const aiMaterial* pMaterial, int materialIndex);
            void loadRoughnessTexture(const std::string& dir, const aiMaterial* pMaterial, int materialIndex);
            void loadRoughnessTextureEmbedded(const aiTexture* paiTexture, int materialIndex);
            void loadRoughnessTextureFromFile(const std::string& dir, const aiString& Path, int materialIndex);
            void loadDiffuseTextureEmbedded(const aiTexture* paiTexture, int materialIndex);
            void loadDiffuseTextureFromFile(const std::string& dir, const aiString& Path, int materialIndex);
            void loadSpecularTexture(const std::string& dir, const aiMaterial* pMaterial, int materialIndex);
            void loadSpecularTextureEmbedded(const aiTexture* paiTexture, int materialIndex);
            void loadSpecularTextureFromFile(const std::string& dir, const aiString& Path, int materialIndex);
            void loadAlbedoTexture(const std::string& dir, const aiMaterial* pMaterial, int materialIndex);
            void loadAlbedoTextureEmbedded(const aiTexture* paiTexture, int materialIndex);
            void loadAlbedoTextureFromFile(const std::string& dir, const aiString& path, int materialIndex);
            void loadMetalnessTexture(const std::string& dir, const aiMaterial* pMaterial, int materialIndex);
            void loadMetalnessTextureEmbedded(const aiTexture* paiTexture, int materialIndex);
            void loadMetalnessTextureFromFile(const std::string& dir, const aiString& path, int materialIndex);

            
            void loadColors(const aiMaterial* pMaterial, int index);
            void populateBuffers();
            void render(IRenderCallbacks* pRenderCallbacks);
            void render(unsigned int drawIndex, unsigned int primID);
            void render(unsigned int numInstannes, const Matrix4* WVPMats, const Matrix4* worldMats);
            void setupRenderMaterialsPhong(unsigned int meshIndex, unsigned int materialIndex, IRenderCallbacks* pRenderCallbacks);
            void setupRenderMaterialsPBR();
            const Material& getMaterial();
            void getLeadingVertex(uint32_t drawIndex, uint32_t primID, Vector3& vertex);


            struct Vertex 
            {
                Vector3 position;
                Vector2 texCoords;
                Vector3 normal;
            };

            enum BUFFER_TYPE
            {
                INDEX_BUFFER = 0,
                VERTEX_BUFFER = 1,
                WVP_MAT_BUFFER = 2,
                WORLD_MAT_BUFFER = 3,
                NUM_BUFFER = 4,
            };

            struct MeshEntry 
            {
                MeshEntry()
                {
                    numIndices = 0;
                    baseVertex = 0;
                    baseIndex = 0;
                    materialIndex = INVALID_MATERIAL;
                }
            
                uint32_t numIndices;
                uint32_t baseVertex;
                uint32_t baseIndex;
                uint32_t materialIndex;
            };

            GLuint VAO = 0;
            GLuint buffers[NUM_BUFFER] = {0};
            const aiScene* pScene;
            Assimp::Importer importer;
            //transform
            Matrix4 globalInverseTransform;
            std::vector<Material> materials;
            std::vector<MeshEntry> meshes;
            std::vector<uint32_t> indices;
            std::vector<Vertex> vertices;
            bool isPBR = false;

    };
    

};