#pragma once

// ���t���[���ɂ����������Ԃ��v���������݂���N���X
class FPSCounter
{
public:
	FPSCounter(const float maxFps);
	~FPSCounter();

	void Update();
	float GetDeltaTime() { return mDeltaTime; }

	float GetFPS() { return mFps; }

private:
	void Wait();

	const float mSetMaxFps;			//�t���[�����[�g�̍ō��l
	const float mOneFrameTickCount;	//�P�t���[���ɂ����鎞��
	int mFpsCount;					//�t���[���̃J�E���^�[
	float mFrameStartTickTime;		//FPS�̌v�Z���n�߂����̎���
	float mFps;						//�v�Z�����t���[�����[�g
	float mBeforetickCount;			//�O�̃t���[���̂�����������
	float mDeltaTime;				//���݂̃t���[���̂�����������
};

