#include "InstanceMeshManager.h"
#include "InstanceMeshComponent.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"

const unsigned int InstanceMeshManager::mMatrixElemNum = 16;
const size_t InstanceMeshManager::mMatrix4Size = sizeof(float) * 16;
//const unsigned int InstanceMeshManager::mMaxInstance = 1000;

InstanceMeshManager::InstanceMeshManager()
	: mInstanceTypeNum(static_cast<int>(InstanceType::InstanceTypeNum))
{
	mInstanceShader = new Shader();
	mInstanceShader->Load("Shaders/InstanceMesh.vert","Shaders/InstanceMesh.frag");
}

InstanceMeshManager::~InstanceMeshManager()
{
}

void InstanceMeshManager::SetInstanceMesh(Mesh* mesh, InstanceType type,unsigned int mAmount)
{
	mInstances[type].mMaxInstance = mAmount;

	// メッシュVAOを取得
	mInstances[type].mVAO = mesh->GetVertexArray()->GetVAO();

	// インデックス数の取得
	mInstances[type].mIndexBufferNum = mesh->GetVertexArray()->GetNumIndices();

	// バッファ行列を作成
	mInstances[type].mBufferMatrices = new float[mInstances[type].mMaxInstance * mMatrixElemNum];

	mInstances[type].mMesh = mesh;

	// バッファオブジェクト紐づけ
	glGenBuffers(1, &mInstances[type].mInstanceVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mInstances[type].mInstanceVAO);

	// 全てのインスタンスオブジェクトのデータメモリをGPUに割り当て
	glBufferData(GL_ARRAY_BUFFER, mInstances[type].mMaxInstance * mMatrix4Size, mInstances[type].mBufferMatrices, GL_STATIC_DRAW);

	// メッシュVAOをinstanceVAOに登録
	glBindVertexArray(mInstances[type].mVAO);

	// 頂点アトリビュートに行列をセット
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, mMatrix4Size, (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, mMatrix4Size, (void*)(1 * sizeof(float) * 4));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, mMatrix4Size, (void*)(2 * sizeof(float) * 4));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, mMatrix4Size, (void*)(3 * sizeof(float) * 4));

	// 引数１：頂点配列のインデックスを指定
	// 引数２：スロットでの汎用属性の更新分で渡されるインスタンスの数を指定する
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glBindVertexArray(0);
}

// 行列バッファを準備する
void InstanceMeshManager::PreparationBufferMatrices()
{
	for (InstanceType type = InstanceType::Begin; type != InstanceType::End; ++type)
	{
		if (mInstances[type].mInstanceMeshComp.size())
		{
			int num = 0;
			for (auto iter = mInstances[type].mInstanceMeshComp.begin(); iter != mInstances[type].mInstanceMeshComp.end(); iter++)
			{
				// 入ってきたワールド行列を取得
				Matrix4 mat = (*iter)->GetOwner()->GetWorldTransform();

				dst = mInstances[type].mBufferMatrices;

				// 行列の行と列を転置する
				mat.Transpose();
				memcpy(&(mInstances[type].mBufferMatrices[num * mMatrixElemNum]), mat.GetAsFloatPtr(), mMatrix4Size);
				num++;
			}
		}
	}

	for (InstanceType type = InstanceType::Begin; type != InstanceType::End; ++type)
	{
		if (mInstances[type].mInstanceMeshComp.size())
		{
			// インスタンスのMatrixデータを更新する
			glBindBuffer(GL_ARRAY_BUFFER, mInstances[type].mInstanceVAO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, mInstances[type].mInstanceMeshComp.size() * mMatrix4Size, mInstances[type].mBufferMatrices);
		}
	}
	GLErrorHandle("PreparationBufferMatrices");
}

void InstanceMeshManager::SetShader()
{

}

void InstanceMeshManager::Draw()
{
	mInstanceShader->SetActive();
	mInstanceShader->SetMatrixUniform("uViewProj", RENDERER->GetViewMatrix() * RENDERER->GetProjectionMatrix());
	mInstanceShader->SetIntUniform("uTexture", 0);

	// 種類ごとのループ
	for (InstanceType type=InstanceType::Begin;type!=InstanceType::End;++type)
	{
		// インスタンス種に一つもインスタンスがない場合

		if (!mInstances[type].mInstanceMeshComp.size())
		{
			continue;
		}

		// テクスチャのセット
		Texture* t = mInstances[type].mMesh->GetTexture(0);

		if (t)
		{
			glActiveTexture(GL_TEXTURE0);
			t->SetActive();
		}
		// インスタンシング描画
		glBindVertexArray(mInstances[type].mMesh->GetVertexArray()->GetVAO());
		glDrawElementsInstanced(GL_TRIANGLES, mInstances[type].mIndexBufferNum, GL_UNSIGNED_INT, 0, static_cast<GLsizei>(mInstances[type].mInstanceMeshComp.size()));
		glBindVertexArray(0);
	}
}

void InstanceMeshManager::Entry(InstanceMeshComponent* instaneMeshComp, InstanceType type)
{
	// 最大インスタンス数を超えたらリターン
	if (mInstances[type].mInstanceMeshComp.size() > mInstances[type].mMaxInstance)
	{
		printf("インスタンスの最大数を超えました\n");
		return;
	}

	mInstances[type].mInstanceMeshComp.emplace_back(instaneMeshComp);
}

void InstanceMeshManager::Remove(InstanceMeshComponent* instanceMeshComp,InstanceType type)
{
	auto iter = std::find(mInstances[type].mInstanceMeshComp.begin(), mInstances[type].mInstanceMeshComp.end(), instanceMeshComp);
	if (iter == mInstances[type].mInstanceMeshComp.end())
	{
		printf("エラーです\n");
		return;
	}

	// 末尾に移動して削除
	std::iter_swap(iter, mInstances[type].mInstanceMeshComp.end() - 1);

	mInstances[type].mInstanceMeshComp.pop_back();
}

