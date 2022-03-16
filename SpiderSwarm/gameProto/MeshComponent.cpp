// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "MeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "GameObject.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include <typeinfo>

// メッシュコンポーネント　ownerとスキンメッシュかの情報入れる
MeshComponent::MeshComponent(GameObject* owner, bool isSkeletal)
	:Component(owner)
	, mMesh(nullptr)
	, mTextureIndex(0)
	, mVisible(true)
	, mIsSkeletal(isSkeletal)
{
	GAMEINSTANCE.GetRenderer()->AddMeshComponent(this);
	printf("Create : MeshComponent [%5d] owner->( 0x%p )\n", GetID(), owner);
}

MeshComponent::~MeshComponent()
{
	printf("Remove : MeshComponent [%5d] owner->( 0x%p )\n", GetID(), mOwner);
	GAMEINSTANCE.GetRenderer()->RemoveMeshComponent(this);
}

void MeshComponent::Draw(Shader* shader)
{
	if (!mMesh) return;

	// ワールド変換をセット
	shader->SetMatrixUniform("uWorldTransform",mOwner->GetWorldTransform());

	// スペキュラ強度セット
	shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());

	// ルミナンス強度セット
	shader->SetFloatUniform("uLuminance", mMesh->GetLuminace());

	// メッシュに定義されているテクスチャをセット
	SetTextureToShader(shader);

	// 頂点配列をアクティブに
	VertexArray* va = mMesh->GetVertexArray();
	va->SetActive();

	// 描画する
	glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}

void MeshComponent::SetTextureToShader(Shader* shader)
{
	// メッシュテクスチャセット
	int texID, stageCount = 0;

	// ディフューズ
	texID = mMesh->GetTextureID(TextureStage::DiffuseMap); 
	{
		glActiveTexture(GL_TEXTURE0 + stageCount);
		glBindTexture(GL_TEXTURE_2D, texID);
		shader->SetIntUniform("uDiffuseMap", stageCount);
		stageCount++;
	}

	// 法線マップ
	texID = mMesh->GetTextureID(TextureStage::NormalMap); 
	{
		glActiveTexture(GL_TEXTURE0 + stageCount);
		glBindTexture(GL_TEXTURE_2D, texID);
		shader->SetIntUniform("uNormalMap", stageCount);
		stageCount++;
	}

	// スペキュラーマップ
	texID = mMesh->GetTextureID(TextureStage::SpecularMap); 
	{
		glActiveTexture(GL_TEXTURE0 + stageCount);
		glBindTexture(GL_TEXTURE_2D, texID);
		shader->SetIntUniform("uSpecularMap", stageCount);
		stageCount++;
	}

	// 自己放射マップ
	texID = mMesh->GetTextureID(TextureStage::EmissiveMap); 
	{
		glActiveTexture(GL_TEXTURE0 + stageCount);
		glBindTexture(GL_TEXTURE_2D, texID);
		shader->SetIntUniform("uEmissiveMap", stageCount);
		stageCount++;
	}
}
