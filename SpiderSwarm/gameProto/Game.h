#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_types.h>
#include "math.h"
#include "Tag.h"

class GameObjectManager;
class GameObject;
class SceneBase;
class Renderer;
class CameraActor;
class PhysicsWorld;
class AudioManager;
class FPSCounter;
class UIScreen;
class Font;

// �Q�[���̏��
enum class GameState
{
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_RESULT,
	SCENE_GAMECLEAR,
	SCENE_GAMEOVER,
	SCENE_MOVIE
};

class Game
{
private:
	Game();				// �V���O���g��

	void Input();		// ���͏���
	void Draw();        // �`��
	int  Update();      // �t���[���X�V����

	// �}�l�[�W���[�֘A
	AudioManager* mAudioManager;			// �I�[�f�B�I�}�l�[�W���[
	GameObjectManager* mGameObjectManager;	// �Q�[���I�u�W�F�N�g�}�l�[�W���[

	Renderer* mRenderer;			// �����_�����O�G���W��
	SceneBase* mNowScene;           // �x�[�X�V�[���N���X
	SceneBase* mTmpScene;			// �V�[������ւ��p
	CameraActor* mActiveCamera;     // �V�X�e�����`��Ɏg���J���� 
	PhysicsWorld* mPhysicsWorld;    // �����蔻��V�X�e��

	bool  mIsRunning;               // �Q�[�����[�v�񂷂��H
	bool  mIsPauseMode;             // �|�[�Y���[�h

	float    mDeltaTime;	// 1�t���[���̌o�ߎ��ԁi�b�P�ʁj
	Uint32   mTicksCount;	// �o�ߎ��ԁi�~���b�P�ʁj
	Matrix4  mViewMatrix;	// �r���[�s��

	GameObject* mPlayerActor;	// �v���C���[�A�N�^�[
	GameObject* mEnemyActor;	// �G�A�N�^�[
	FPSCounter* mFPSCounter;

public:
	~Game();
	static Game& GetInstance()
	{
		static Game instance;
		return instance;
	}

	void Run();                                                                    // �Q�[���{�̎��s
	void Shutdown();                                                               // �I����
	bool Initialize(int screenWidth, int screenHeight, bool fullScreen = false);   // ��ʏ������ݒ�

	void setFirstScene(SceneBase* pScene) { mNowScene = pScene; }                  // �J�n�V�[���̃Z�b�g
	void setShutDown() { mIsRunning = false; }                                     // �V���b�g�_�E��

	Renderer* GetRenderer() { return mRenderer; }									// �����_���[�V�X�e���̎擾
	SDL_Renderer* GetSDLRenderer();                                                // 2D��SDL�����_���[�̎擾

	//void AddActor(GameObject* actor);                                             // �A�N�^�[�̒ǉ�
	//void RemoveActor(GameObject* actor);                                          // �A�N�^�[�̍폜

	void SetCameraActor(CameraActor* camera);      // �J�����A�N�^�[���V�X�e���ɓo�^
	void InActiveCamera(CameraActor* inActiveCam); // �J�����̓o�^������

	void SetPlayerActor(GameObject* player);              // �v���C���[�A�N�^�[�̓o�^

	Font* mFont;

	const Matrix4& GetViewMatrix();                                                      // ���݃Z�b�g����Ă���J�����̃r���[�s���Ԃ�
	const Vector3& GetViewTarget();
	const Vector3& GetViewPos();
	const float    GetDeltaTime() { return mDeltaTime; }
	PhysicsWorld* const GetPhysics() { return mPhysicsWorld; }

	AudioManager* const GetAudio() { return mAudioManager; }

	//std::vector<class GameObject*>const& GetActors(Tag type);
	//class GameObject* GetFirstActor(Tag type);

	//bool IsExistActorType(Tag type);                    // ���̃A�N�^�[�^�C�v�͑��݂��邩�H
	//GameObject* FindActorFromID(int searchActorID); // �A�N�^�[ID����A�N�^�[�ւ̃|�C���^����������
};

#define GAMEINSTANCE	Game::GetInstance()
#define RENDERER		Game::GetInstance().GetRenderer()
#define AUDIO			Game::GetInstance().GetAudio()

