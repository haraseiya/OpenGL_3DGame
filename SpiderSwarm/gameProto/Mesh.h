// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "Renderer.h"
#include "Collision.h"
#include "rapidjson/rapidjson.h"

class Renderer;
class Texture;
class VertexArray;

class Mesh
{
public:
	Mesh();
	~Mesh();

	// メッシュのロード・アンロード
	bool Load(const std::string& fileName,Renderer* renderer); 
	void Unload();

	// メッシュの頂点配列の取得
	VertexArray* GetVertexArray() { return mVertexArray; }

	// 指定されたインデックスからテクスチャの取得
	Texture* GetTexture(int index);

	int GetTextureID(TextureStage stage);

	// シェーダー名の取得
	const std::string& GetShaderName() const { return mShaderName; }     

	// コリジョンボックスを取得（オブジェクト空間）
	const AABB& GetCollisionBox() const { return mBox; }

	// バウンディングスフィアの半径を取得
	float GetRadius() const { return mRadius; }  

	// スペキュラ強度を取得
	float GetSpecPower() const { return mSpecPower; }

	// ルミナンス強度を取得
	float GetLuminace() const { return mLuminance; }

private:
	AABB mBox;							// AABBコリジョン
	std::vector<Texture*> mTextures;    // メッシュのテクスチャ
	VertexArray* mVertexArray;          // メッシュの頂点配列

	std::string mShaderName;	// メッシュ指定のシェーダー

	float mRadius;	            // バウンディングスフィアの半径
	float mSpecPower;	        // 表面のスペキュラー値
	float mLuminance;           // 自己発光強度(HDR)

	std::unordered_map<TextureStage, int> mStageDefTexture;  // テクスチャステージに割り当てられたテクスチャ

};