#include "PlayerStateRunForward.h"
#include "PlayerBase.h"
#include "Input.h"
#include "SkeletalMeshComponent.h"

const float PlayerStateRunForward::mRoot = 1.41421356237f;

PlayerStateRunForward::PlayerStateRunForward(PlayerBehaviorComponent* owner)
	: PlayerStateBase(owner)
{
	mStateType = PlayerStateEnum::RunForward;
}

PlayerStateRunForward::~PlayerStateRunForward()
{
}

PlayerStateEnum PlayerStateRunForward::Update(float deltaTime)
{
	// �v���C���[���S�t���O
	const bool isDead = mOwner->GetHitPoint() <= 0;
	if (isDead)
	{
		return PlayerStateEnum::Die;
	}

	// �R���g���[�����͂��ꂽ��
	Vector2 stickL = INPUT_INSTANCE.GetLStick();
	const bool isContollerInputOff = !INPUT_INSTANCE.IsLStickMove();

	//�����L�[�����͂��ꂽ��
	const bool isIdle = INPUT_INSTANCE.IsKeyOff(KEY_UP) &
		INPUT_INSTANCE.IsKeyOff(KEY_RIGHT) &
		INPUT_INSTANCE.IsKeyOff(KEY_DOWN) &
		INPUT_INSTANCE.IsKeyOff(KEY_LEFT) &
		isContollerInputOff;
	if (isIdle)
	{
		return PlayerStateEnum::Idle;
	}

	const bool isRun = INPUT_INSTANCE.IsKeyPressed(KEY_L);
	if (!isRun)
	{
		return PlayerStateEnum::Forward;
	}

	// ���ςɂ���ăA�j���[�V�����J��
	const bool isFoward = mDot < 0.0f;
	const bool isBackward = mDot > 0.0f;
	const bool isLeft = 0.0f;
	const bool isRight = -0.0f;


	// �ړ�����
	MoveCalc(deltaTime);

	return PlayerStateEnum::RunForward;
}

// RUN��Ԃւ̐؂�ւ�����
void PlayerStateRunForward::OnEnter()
{
	// �A�j���[�V�������Đ�
	SkeletalMeshComponent* meshcomp = mOwner->GetSkeletalMeshComp();
	meshcomp->PlayAnimation(mOwner->GetAnim(PlayerState::PLAYER_STATE_RUN_FORWARD));
}

void PlayerStateRunForward::OnExit()
{
}

// �ړ�����
void PlayerStateRunForward::MoveCalc(float deltaTime)
{
	//�L��������
	const float speed = 500.0f;
	float charaSpeed = mOwner->GetSpeed(); //  �L�����̌��݂̃X�s�[�h

	// �J��������݂��O�i�������擾
	Vector3 TargetPos = GAMEINSTANCE.GetViewTarget();
	Vector3 ViewPos = GAMEINSTANCE.GetViewPos();
	Vector3 forwardVec = TargetPos - ViewPos;
	forwardVec.z = 0.0; // ���������𖳎�

	// �J�����O���x�N�g���ƉE�����x�N�g���Z�o
	forwardVec = Vector3::Normalize(forwardVec);
	Vector3 rightVec = Vector3::Cross(Vector3::UnitZ, forwardVec);

	// �E�����x�N�g������J������]�p���Z�o
	float forwardAngle = 0.0f;
	float angleSign;

	Vector3 tmpVec;
	forwardAngle = acosf(Vector3::Dot(Vector3::UnitX, rightVec));

	// �E��]������]���H
	tmpVec = Vector3::Cross(Vector3::UnitX, rightVec);
	angleSign = (tmpVec.z > 0.0) ? 1.0f : -1.0f;
	forwardAngle *= angleSign;

	Vector3 charaForwardVec = mOwner->GetForward(); // �L�����̑O�i�x�N�g��

	// �L�����N�^�[�ړ��̓��̓L�[�擾
	Vector3 DirVecL(0.0f, 0.0f, 0.0f);
	Vector3 DirVecR(0.0f, 0.0f, 0.0f);

	//if (INPUT_INSTANCE.IsKeyPressed(KEY_UP))
	//{
	//	DirVec += forwardVec;
	//}

	//if (INPUT_INSTANCE.IsKeyPressed(KEY_DOWN))
	//{
	//	DirVec -= forwardVec;
	//}

	//if (INPUT_INSTANCE.IsKeyPressed(KEY_RIGHT))
	//{
	//	DirVec += rightVec;
	//}

	//if (INPUT_INSTANCE.IsKeyPressed(KEY_LEFT))
	//{
	//	DirVec -= rightVec;
	//}

	// �X�e�B�b�N���ǂݍ���
	Vector2 stickL = INPUT_INSTANCE.GetLStick();
	Vector2 stickR = INPUT_INSTANCE.GetRStick();

	Matrix3 rotL = Matrix3::CreateRotation(forwardAngle);
	stickL = Vector2::Transform(stickL, rotL);
	DirVecL.x += stickL.x;
	DirVecL.y += stickL.y;

	Matrix3 rotR = Matrix3::CreateRotation(forwardAngle);
	stickR = Vector2::Transform(stickR, rotR);
	DirVecR.x += stickR.x;
	DirVecR.y += stickR.y;

	// ���X�e�B�b�N�̏���
	if (DirVecL.LengthSq() > 0.5f)
	{
		// �ړ�����
		Vector3 position = mOwner->GetPosition();

		// ���݂̃X�s�[�h��ۑ�
		charaSpeed = speed * deltaTime;
		position += DirVecL * charaSpeed;

		// �L�����̈ʒu�E�X�s�[�h�E�ϊ��s��̍Čv�Z�̕K�v���Z�b�g
		mOwner->SetPosition(position);
		mOwner->SetSpeed(charaSpeed);

		//printf("x : %f , y : %f , z : %f\n", DirVecL.x, DirVecL.y, DirVecL.z);
	}

	// �E�X�e�B�b�N�̏���
	if (DirVecR.LengthSq() > 0.5f)
	{
		// ���͂��ꂽ�����Ɍ����Ċ��炩�ɉ�]
		charaForwardVec=Vector3::Lerp(mOwner->GetForward(),DirVecR,0.2f);
		charaForwardVec.Normalize();
		mOwner->RotateToNewForward(charaForwardVec);
	}

	// �L�����N�^�[�̑O���ƍ�������������̓��ς����
	mDot = Vector3::Dot(charaForwardVec, DirVecL);

	mOwner->SetComputeWorldTransform();
}

