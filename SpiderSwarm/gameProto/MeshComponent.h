// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Component.h"
#include <cstddef>

class GameObject;
class Shader;
class Mesh;

class MeshComponent : public Component
{
public:
	MeshComponent(GameObject* owner, bool isSkeletal = false);      // メッシュコンポーネントコンストラクタ。（要：親アクター）
	~MeshComponent();

	virtual void Draw(Shader* shader);								// メッシュコンポーネントの描画
	virtual void SetMesh(Mesh* mesh) { mMesh = mesh; }				// メッシュコンポーネントで使用するメッシュのセット
	void SetTextureIndex(size_t index) { mTextureIndex = index; }   // テクスチャのセット

	void SetVisible(bool visible) { mVisible = visible; }             // 表示フラグのセット
	bool GetVisible() const { return mVisible; }                      // 表示フラグのゲット

	bool GetIsSkeletal() const { return mIsSkeletal; }                // スケルタルモデルかどうかの取得
protected:
	virtual void SetTextureToShader(Shader* shader);

	Mesh* mMesh;           // メッシュオブジェクトの取得
	size_t mTextureIndex;  // テクスチャーのインデックス
	bool mVisible;         // 表示フラグ
	bool mIsSkeletal;      // スケルタルモデルかどうかのフラグ
};