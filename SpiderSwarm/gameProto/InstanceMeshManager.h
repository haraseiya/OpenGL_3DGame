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
	
	// �C���X�^���X���b�V�����Z�b�g
	void SetInstanceMesh(Mesh* mesh,InstanceType type,unsigned int mAmount);

	// �s��o�b�t�@������
	void PreparationBufferMatrices();

	// �V�F�[�_�[�̃Z�b�g
	void SetShader();

	void Draw();

	void Entry(InstanceMeshComponent* instaneMeshComp,InstanceType type);
	void Remove(InstanceMeshComponent* instanceMeshComp,InstanceType type);

private:
	// �C���X�^���X�Q
	struct Instance
	{
		std::vector<InstanceMeshComponent*> mInstanceMeshComp;
		Mesh* mMesh;						// ���b�V��
		Texture* mTexture;					// �e�N�X�`��

		unsigned int mVAO;					// VAO	
		unsigned int mIndexBufferNum;	
		unsigned int mInstanceVAO;
		unsigned int mMaxInstance;
		float* mBufferMatrices;
	};
	Shader* mInstanceShader;
	std::unordered_map<InstanceType,Instance> mInstances;

	//unsigned int mVAO;							// ���_�z��
	//unsigned int mIndexNum;						// �C���f�b�N�X�̑���
	////unsigned int mInstanceVAO;					// �C���X�^���XVAO

	static const unsigned int mMatrixElemNum;	// �}�g���b�N�X�̗v�f��
	//unsigned int mMaxInstance;					// �`�悷��C���X�^���X�̑���
	static const size_t mMatrix4Size;			// �s��̑傫��
	const int mInstanceTypeNum;					// �C���X�^���X�̎�ސ�
	float* dst;
	//float* mBufferMatrices;						// �o�b�t�@�[�s��̐擪�|�C���^
	//unsigned int mBuffer;						// �o�b�t�@�[
	//Texture* mTexture;							// �e�N�X�`��
	////Shader* mInstanceShader;					// �C���X�^���X�p�̃V�F�[�_�[
	////Mesh* mMesh;								// ���b�V��
};

