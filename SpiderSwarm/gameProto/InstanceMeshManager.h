#pragma once

#include "Math.h"
#include "InstanceType.h"
#include <vector>
#include <unordered_map>

class Mesh;
class Shader;
class Texture;
class GameObject;
class InstanceMeshComponent;
class InstanceBase;

class InstanceMeshManager
{
public:
	InstanceMeshManager();
	~InstanceMeshManager();
	
	// インスタンスメッシュをセット
	void SetInstanceMesh(Mesh* mesh,InstanceType type,unsigned int mAmount);

	// 行列バッファを準備
	void PreparationBufferMatrices();

	// シェーダーのセット
	void SetShader();

	void Draw();

	void Entry(InstanceMeshComponent* instaneMeshComp,InstanceType type);
	void Remove(InstanceMeshComponent* instanceMeshComp,InstanceType type);

private:
	// インスタンス群
	struct Instance
	{
		std::vector<InstanceMeshComponent*> mInstanceMeshComp;
		Mesh* mMesh;						// メッシュ
		Texture* mTexture;					// テクスチャ

		unsigned int mVAO;					// VAO	
		unsigned int mIndexBufferNum;	
		unsigned int mInstanceVAO;
		unsigned int mMaxInstance;
		float* mBufferMatrices;
	};
	Shader* mInstanceShader;
	std::unordered_map<InstanceType,Instance> mInstances;

	//unsigned int mVAO;							// 頂点配列
	//unsigned int mIndexNum;						// インデックスの総数
	////unsigned int mInstanceVAO;					// インスタンスVAO

	static const unsigned int mMatrixElemNum;	// マトリックスの要素数
	//unsigned int mMaxInstance;					// 描画するインスタンスの総数
	static const size_t mMatrix4Size;			// 行列の大きさ
	const int mInstanceTypeNum;					// インスタンスの種類数
	float* dst;
	//float* mBufferMatrices;						// バッファー行列の先頭ポインタ
	//unsigned int mBuffer;						// バッファー
	//Texture* mTexture;							// テクスチャ
	////Shader* mInstanceShader;					// インスタンス用のシェーダー
	////Mesh* mMesh;								// メッシュ
};

