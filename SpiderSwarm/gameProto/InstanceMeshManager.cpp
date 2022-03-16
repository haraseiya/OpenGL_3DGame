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

	// ���b�V��VAO���擾
	mInstances[type].mVAO = mesh->GetVertexArray()->GetVAO();

	// �C���f�b�N�X���̎擾
	mInstances[type].mIndexBufferNum = mesh->GetVertexArray()->GetNumIndices();

	// �o�b�t�@�s����쐬
	mInstances[type].mBufferMatrices = new float[mInstances[type].mMaxInstance * mMatrixElemNum];

	mInstances[type].mMesh = mesh;

	// �o�b�t�@�I�u�W�F�N�g�R�Â�
	glGenBuffers(1, &mInstances[type].mInstanceVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mInstances[type].mInstanceVAO);

	// �S�ẴC���X�^���X�I�u�W�F�N�g�̃f�[�^��������GPU�Ɋ��蓖��
	glBufferData(GL_ARRAY_BUFFER, mInstances[type].mMaxInstance * mMatrix4Size, mInstances[type].mBufferMatrices, GL_STATIC_DRAW);

	// ���b�V��VAO��instanceVAO�ɓo�^
	glBindVertexArray(mInstances[type].mVAO);

	// ���_�A�g���r���[�g�ɍs����Z�b�g
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, mMatrix4Size, (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, mMatrix4Size, (void*)(1 * sizeof(float) * 4));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, mMatrix4Size, (void*)(2 * sizeof(float) * 4));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, mMatrix4Size, (void*)(3 * sizeof(float) * 4));

	// �����P�F���_�z��̃C���f�b�N�X���w��
	// �����Q�F�X���b�g�ł̔ėp�����̍X�V���œn�����C���X�^���X�̐����w�肷��
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glBindVertexArray(0);
}

// �s��o�b�t�@����������
void InstanceMeshManager::PreparationBufferMatrices()
{
	for (InstanceType type = InstanceType::Begin; type != InstanceType::End; ++type)
	{
		if (mInstances[type].mInstanceMeshComp.size())
		{
			int num = 0;
			for (auto iter = mInstances[type].mInstanceMeshComp.begin(); iter != mInstances[type].mInstanceMeshComp.end(); iter++)
			{
				// �����Ă������[���h�s����擾
				Matrix4 mat = (*iter)->GetOwner()->GetWorldTransform();

				dst = mInstances[type].mBufferMatrices;

				// �s��̍s�Ɨ��]�u����
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
			// �C���X�^���X��Matrix�f�[�^���X�V����
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

	// ��ނ��Ƃ̃��[�v
	for (InstanceType type=InstanceType::Begin;type!=InstanceType::End;++type)
	{
		// �C���X�^���X��Ɉ���C���X�^���X���Ȃ��ꍇ

		if (!mInstances[type].mInstanceMeshComp.size())
		{
			continue;
		}

		// �e�N�X�`���̃Z�b�g
		Texture* t = mInstances[type].mMesh->GetTexture(0);

		if (t)
		{
			glActiveTexture(GL_TEXTURE0);
			t->SetActive();
		}
		// �C���X�^���V���O�`��
		glBindVertexArray(mInstances[type].mMesh->GetVertexArray()->GetVAO());
		glDrawElementsInstanced(GL_TRIANGLES, mInstances[type].mIndexBufferNum, GL_UNSIGNED_INT, 0, static_cast<GLsizei>(mInstances[type].mInstanceMeshComp.size()));
		glBindVertexArray(0);
	}
}

void InstanceMeshManager::Entry(InstanceMeshComponent* instaneMeshComp, InstanceType type)
{
	// �ő�C���X�^���X���𒴂����烊�^�[��
	if (mInstances[type].mInstanceMeshComp.size() > mInstances[type].mMaxInstance)
	{
		printf("�C���X�^���X�̍ő吔�𒴂��܂���\n");
		return;
	}

	mInstances[type].mInstanceMeshComp.emplace_back(instaneMeshComp);
}

void InstanceMeshManager::Remove(InstanceMeshComponent* instanceMeshComp,InstanceType type)
{
	auto iter = std::find(mInstances[type].mInstanceMeshComp.begin(), mInstances[type].mInstanceMeshComp.end(), instanceMeshComp);
	if (iter == mInstances[type].mInstanceMeshComp.end())
	{
		printf("�G���[�ł�\n");
		return;
	}

	// �����Ɉړ����č폜
	std::iter_swap(iter, mInstances[type].mInstanceMeshComp.end() - 1);

	mInstances[type].mInstanceMeshComp.pop_back();
}

