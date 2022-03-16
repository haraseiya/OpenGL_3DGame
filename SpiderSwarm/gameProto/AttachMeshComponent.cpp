#include "AttachMeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Shader.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "Texture.h"

#include <typeinfo>

AttachMeshComponent::AttachMeshComponent(GameObject* owner, SkeletalMeshComponent* skMesh, const char* AttachBoneName)
	: MeshComponent(owner, false)
	, mAttachBoneIndex(0)
	, mAttachSkeletalMesh(skMesh)
{
	// アタッチメッシュコンポーネントをレンダークラスに追加
	GAMEINSTANCE.GetRenderer()->AddAttachMeshComponent(this);
	mAttachBoneIndex = skMesh->GetBoneIndexFromName(AttachBoneName);
	printf("Create : AttachMeshComponent [%d]\n", mID);
}

AttachMeshComponent::~AttachMeshComponent()
{
	// アタッチメッシュコンポーネントをレンダークラスから削除
	printf("Remove : AttachMeshComponent [%d]\n", mID);
	GAMEINSTANCE.GetRenderer()->RemoveAttachMeshComponent(this);
}

void AttachMeshComponent::SetOffsetPosition(Vector3& offset)
{
	// 初期位置をセット
	mOffsetPos = Matrix4::CreateTranslation(offset);
}

void AttachMeshComponent::SetOffsetRotation(Vector3& rotation)
{
	mOffsetRotation = Matrix4::CreateRotationY(rotation.y)
		* Matrix4::CreateRotationX(rotation.x)
		* Matrix4::CreateRotationZ(rotation.z);
}

void AttachMeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		Matrix4 finalMat, animationMat;
		mAttachSkeletalMesh->GetMatrixFromBoneIndex(animationMat, mAttachBoneIndex);
		finalMat = mOffsetRotation * mOffsetPos * animationMat;

		// アタッチ位置算出＆更新
		Vector3 pos(0, 0, 0);
		mComputeAttachPos = Vector3::Transform(pos, finalMat);
		mComputeTransMatrix = finalMat;

		// シェーダーにマトリックスをセット
		shader->SetMatrixUniform("uWorldTransform", finalMat);

		// スペキュラ強度セット
		shader->SetFloatUniform("uSpecPower", 100);

		// アクティブテクスチャセット
		Texture* t = mMesh->GetTexture(static_cast<int>(mTextureIndex));
		if (t)
		{
			t->SetActive();
		}

		SetTextureToShader(shader);

		// 頂点配列をアクティブに
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();

		// 描画する
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}
