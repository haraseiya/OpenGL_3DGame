#pragma once
#include "MeshComponent.h"
#include "MatrixPalette.h"

class GameObject;
class Shader;
class Skeleton;
class Animation;

class SkeletalMeshComponent : public MeshComponent
{
public:
	SkeletalMeshComponent(GameObject* owner);
	~SkeletalMeshComponent();

	// ���b�V���R���|�[�l���g�̕`��
	void Draw(Shader* shader) override;
	void Update(float deltaTime) override;

	// �X�P���g���̃Z�b�g
	void SetSkeleton(const Skeleton* sk) { mSkeleton = sk; }

	// �A�j���[�V�����̍Đ��B�A�j���[�V�����̎c�蒷����Ԃ�
	float PlayAnimation(const Animation* anim, float playRate = 0.5f);

	// ���ݍĐ����̃A�j���[�V�����͍Đ������H true : �Đ��� false : �Đ��I��
	bool IsPlaying();

	// ���ݍĐ����̃A�j���[�V����
	const Animation* GetNowPlayingAnimation() { return mAnimation; }

	// ���݂̃A�j���[�V�����̍Đ����Ԃ��擾
	float GetPlayTime() { return mAnimTime; }

	int GetBoneIndexFromName(const char* boneName) const;
	void GetMatrixFromBoneIndex(Matrix4& boneWorldMatrix, int boneIndex)const;

	void SetHitColor(Vector3 color) { mHitColor = color; }

protected:
	void ComputeMatrixPalette();    // �s��p���b�g�̌v�Z

	MatrixPalette mPalette;			// �s��p���b�g
	const Skeleton* mSkeleton;      // �X�P���g��
	const Animation* mAnimation;    // �A�j���[�V����
	float mAnimPlayRate;            // �A�j���[�V�����̍Đ����x
	float mAnimTime;                // �A�j���[�V��������
	Vector3 mHitColor;				// ��e�F
};
