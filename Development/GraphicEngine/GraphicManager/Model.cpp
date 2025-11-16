#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/gtx/matrix_decompose.hpp>
#include <filesystem>

Model::Model(const char* file, Material material):m_file(file), m_material(material) {

    Assimp::Importer importer;
    importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
    const aiScene* scene = importer.ReadFile(m_file, 
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_GenNormals |
        aiProcess_GlobalScale |
        aiProcess_JoinIdenticalVertices |
        aiProcess_CalcTangentSpace |
        aiProcess_ImproveCacheLocality|
        aiProcess_SortByPType
    );
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;
        return;
    }

    ProcessNode(scene->mRootNode, scene);
}

void Model::Draw(Shader& shader, camera::Camera& cam) {

    for (size_t n{}; n < m_meshes.size(); n++) {
        auto& mesh = m_meshes[n];
        mesh.Draw(shader, cam, m_matricesMeshes[n]);
    }

}

void Model::ProcessNode(aiNode* node, const aiScene* scene, glm::mat4 parentTransform)
{
    // Compute global transform for this node
    glm::mat4 nodeTransform = aiMat4ToGlm(node->mTransformation);
    glm::mat4 globalTransform = parentTransform * nodeTransform;

    //reserve memory
    m_translationsMeshes.reserve(node->mNumMeshes);
    m_rotationsMeshes.reserve(node->mNumMeshes);
    m_scalesMeshes.reserve(node->mNumMeshes);
    m_matricesMeshes.reserve(node->mNumMeshes);
    m_meshes.reserve(node->mNumMeshes);

    // Process all meshes of this node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        unsigned int meshIndex = node->mMeshes[i];
        aiMesh* mesh = scene->mMeshes[meshIndex];

        if (mesh->mPrimitiveTypes & aiPrimitiveType_POINT)
        {
            continue;   // Skip this mesh
        }

        // Store global matrix for the mesh
        m_matricesMeshes.push_back(globalTransform);

        // Decompose to translation, rotation, scale
        glm::vec3 translation;
        glm::quat rotation;
        glm::vec3 scale;
        glm::vec3 skew;           // ignored
        glm::vec4 perspective;    // ignored
        glm::decompose(globalTransform, scale, rotation, translation, skew, perspective);

        m_translationsMeshes.push_back(translation);
        m_rotationsMeshes.push_back(rotation);
        m_scalesMeshes.push_back(scale);

        //load mesh
        
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Vertex> vertices = LoadVertex(mesh);
        std::vector<GLuint> indices = LoadIndices(mesh);
        std::vector<Texture> textures = LoadTexture(material);
        m_meshes.emplace_back(vertices, indices, textures);
    }

    // Recursively process children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene, globalTransform);
    }
}

glm::mat4 Model::aiMat4ToGlm(const aiMatrix4x4& from)
{
    glm::mat4 to;
    to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
    to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
    to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
    to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
    return to;
}


std::vector<Vertex> Model::LoadVertex(const aiMesh* mesh)
{
    std::vector<Vertex> vecVertex;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex{};

        // Position
        vertex.position = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        );

        // Normals
        if (mesh->HasNormals()) {
            vertex.normal = glm::vec3(
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z
            );
        }

        // Texture Coordinates
        if (mesh->mTextureCoords[0]) {
            vertex.texUV = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        }
        else {
            vertex.texUV = glm::vec2(0.0f, 0.0f);
        }

        vecVertex.push_back(std::move(vertex));
    }


    return vecVertex;
}

std::vector<GLuint> Model::LoadIndices(const aiMesh* mesh)
{
    std::vector<GLuint> vecIndices;

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            vecIndices.push_back(face.mIndices[j]);
        }
    }

    return vecIndices;
}

std::vector<Texture> Model::LoadTexture(const aiMaterial* material)
{
    std::vector<Texture> textures;

    // Diffuse maps
    if (!m_material.diffuseDirectory.empty()) {
        auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, DIFFUSE, m_material.diffuseDirectory);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    }

    // Specular maps
    if (!m_material.specularDirectory.empty()) {

        auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, SPECULAR, m_material.specularDirectory);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    // Normal maps
    if (!m_material.normalDirectory.empty()) {
        auto normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, NORMAL, m_material.normalDirectory);
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    }






    return textures;
}

std::vector<Texture> Model::loadMaterialTextures(const aiMaterial* mat, aiTextureType type, TextureType textureType, const std::string& directory)
{
    std::vector<Texture> textures;

    unsigned int count = mat->GetTextureCount(type);
    for (unsigned int i = 0; i < count; i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        Texture texture(directory.c_str(), textureType, nextTextureSlot++);
        textures.push_back(texture);
    }

    return textures;
}
