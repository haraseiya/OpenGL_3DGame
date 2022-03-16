#include "Game.h"
#include "CameraActor.h"

CameraActor::CameraActor(GameObject* targetActor)
	: mTargetActor(targetActor)
	, mCameraSceneState(CameraSceneState::CAMERA_SCENE_TITLE)
	, mPosition(Vector3::Zero)
	, mViewTarget(Vector3::Zero)
	, mViewVector(Vector3::Zero)
{	
	//�f�t�H���g�J�����l�Z�b�g
	mViewMatrix = Matrix4::CreateLookAt(Vector3(-100, 0, 100),Vector3(0, 0, 0),Vector3(0, 0, 1));

	// �Q�[���V�X�e�����ɂ��ꂪ�A�N�e�B�u�J�����ł��邱�Ƃ�ʒm
	SetActive();
}

CameraActor::CameraActor(std::vector<GameObject*> targets)
{
	mTargetActors = targets;

}

CameraActor::~CameraActor()
{
	// �Q�[���V�X�e�����ɃA�N�e�B�u�J���������Ȃ��Ȃ������Ƃ�ʒm
	// �J�����A�N�^�[���̂�Actor�Ńf�X�g���N�g�����
	GAMEINSTANCE.InActiveCamera(this);
}

// �I�[�o�[���C�h���Ďg��Ȃ��ꍇ
void CameraActor::UpdateActor(float deltaTime)
{
}

void CameraActor::Update(float deltaTime)
{
	// �A�N�^�[�X�V����
	UpdateActor(deltaTime);

	// �J���������x�N�g���A�J�����s��쐬
	mViewVector = mViewTarget - mPosition;
	mViewMatrix = Matrix4::CreateLookAt(mPosition, mViewTarget, Vector3(0, 0, 1));

	// �V�[�����̃J��������
	switch (mCameraSceneState)
	{
	// �^�C�g���V�[��
	case CameraSceneState::CAMERA_SCENE_TITLE:

		break;

	// �Q�[���V�[��
	case CameraSceneState::CAMERA_SCENE_GAME:

		break;

	// ���U���g
	case CameraSceneState::CAMERA_SCENE_RESULT:

		break;
	}
}

//�Q�[���V�X�e�����ɂ��ꂪ�A�N�e�B�u�J�����ł��邱�Ƃ��蓮�Œʒm����
void CameraActor::SetActive()
{
	//�Q�[���V�X�e�����ɂ��ꂪ�A�N�e�B�u�J�����ł��邱�Ƃ�ʒm
	GAMEINSTANCE.SetCameraActor(this);
}

// �ʒu����������
void CameraActor::Init(const Vector3& cameraPos, const Vector3& targetPos, const Vector3& upVec)
{
	mPosition = cameraPos;
	mViewTarget = targetPos;
	mViewVector = targetPos - cameraPos;

	mViewMatrix = Matrix4::CreateLookAt(cameraPos, targetPos, upVec);
}

