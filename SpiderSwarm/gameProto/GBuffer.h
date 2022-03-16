#pragma once
#include <vector>

class Texture;

class GBuffer
{
public:
	// Gバッファに格納されるデータの種類
	enum Type
	{
		DIFFUSE=0,
		NORMAL,
		WORLD_POS,
		GBUFFER_TEXTURE_NUM
	};

	GBuffer();
	~GBuffer();

	// Gバッファの生成と破棄
	bool Create(int width,int height);
	void Destroy();

	// 指定するテクスチャを取得
	Texture* GetTexture(Type type);

	// フレームバッファオブジェクトのIDを取得
	unsigned int GetBufferID() const { return mBufferID; }

	// 全てのGバッファテクスチャを読み込み用に設定
	void SetTextureActive();

private:
	std::vector<Texture*> mTextures;	// Gバッファに割り当てられたテクスチャ
	unsigned int mBufferID;				// フレームバッファオブジェクトID
};

