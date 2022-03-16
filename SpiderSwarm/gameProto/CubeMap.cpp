#include "CubeMap.h"
#include "GL/glew.h"
#include "Shader.h"
#include "Texture.h"
#include "Math.h"
#include "Game.h"
#include "Renderer.h"
#include "CameraActor.h"
#include "Game.h"

#define STB_IMAGE_IMPLEMENTATION
#include <SDL/SDL_image.h>
#include <stb_image.h>
#include <iostream>

CubeMap::CubeMap()
{
    // キューブマップ用のシェーダー
    mSkyBoxShader = new Shader();
    mSkyBoxShader->Load("Shaders/SkyBox.vert", "Shaders/SkyBox.frag");

    // スカイボックス用の頂点配列
    float skyboxVertices[] = { 
        // 位置        
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // スカイボックス作成
    glGenVertexArrays(1, &mSkyBoxVAO);
    glGenBuffers(1, &mSkyBoxVBO);
    glBindVertexArray(mSkyBoxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mSkyBoxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    mFaces.push_back("assets/SkyBox/right.jpg");
    mFaces.push_back("assets/SkyBox/left.jpg");
    mFaces.push_back("assets/SkyBox/top.jpg");
    mFaces.push_back("assets/SkyBox/bottom.jpg");
    mFaces.push_back("assets/SkyBox/front.jpg");
    mFaces.push_back("assets/SkyBox/back.jpg");

    mCubeMapTexture = LoadCubeMap(mFaces);

    mSkyBoxShader->SetActive();
    mSkyBoxShader->SetIntUniform("skybox", 0);
}

CubeMap::~CubeMap()
{
    glDeleteVertexArrays(1, &mCubeMapVAO);
    glDeleteVertexArrays(1, &mSkyBoxVAO);
    glDeleteBuffers(1, &mCubeMapVBO);
    glDeleteBuffers(1, &mSkyBoxVAO);
}

void CubeMap::Draw()
{
    // スカイボックス描画
    Matrix4 view = RENDERER->GetViewMatrix();
    Matrix4 projection = RENDERER->GetProjectionMatrix();
    glDepthFunc(GL_LEQUAL);
    mSkyBoxShader->SetActive();
    mSkyBoxShader->SetMatrixUniform("view", view);
    mSkyBoxShader->SetMatrixUniform("projection", projection);
    glBindVertexArray(mSkyBoxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mCubeMapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

// キューブマップをロード
unsigned int CubeMap::LoadCubeMap(std::vector<std::string> faces)
{
    // テクスチャを生成
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;

    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
