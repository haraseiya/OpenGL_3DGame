#include "game.h"
#include "Input.h"
#include "renderer.h"
#include "SceneBase.h"
#include "GameObject.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "Animation.h"
#include "Skeleton.h"
#include "CameraActor.h"
#include "PhysicsWorld.h"
#include "InstanceMeshManager.h"
#include "FPSCounter.h"
#include "UIManager.h"
#include "ScoreManager.h"
#include "Font.h"
#include "GameObjectManager.h"

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <typeinfo>
#include <iostream>

Game::Game()
	: mRenderer(nullptr)
	, mNowScene(nullptr)
	, mIsRunning(true)
	, mIsPauseMode(false)
	, mActiveCamera(nullptr)
	, mPhysicsWorld(nullptr)
	, mFont(nullptr)
	, mGameObjectManager(nullptr)
	, mAudioManager(nullptr)
	, mPlayerActor(nullptr)
	, mEnemyActor(nullptr)
	, mFPSCounter(nullptr)
	, mTmpScene(nullptr)
	, mTicksCount(0)
	, mDeltaTime(0.0f)
{
}

Game::~Game()
{
}

void Game::Input()
{
	// �I���C�x���g�̃L���b�`
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		default:
			// �p�b�h�C�x���g�擾
			INPUT_INSTANCE.GetControllerInstance()->ReceiveEvent(event);
		}
	}

	// �L�[�{�[�h���͍X�V
	INPUT_INSTANCE.Update();

	// ESC�������ꂽ��I��
	if (INPUT_INSTANCE.IsKeyPullup(SDL_SCANCODE_ESCAPE))
	{
		mIsRunning = false;
	}

	// �A�N�^�[�f�o�b�O
	if (INPUT_INSTANCE.IsKeyPullup(SDL_SCANCODE_F12))
	{
	/*	ShowActor();*/
	}

	// �����_�����O���\�[�X�\��
	if (INPUT_INSTANCE.IsKeyPullup(SDL_SCANCODE_F11))
	{
		GetRenderer()->ShowResource();
	}

	// �|�[�Y���[�h�ڍs�^����
	if (INPUT_INSTANCE.IsKeyPullup(SDL_SCANCODE_BACKSPACE))
	{
		////�|�[�Y���[�h�ؑ�
		//mIsPauseMode = !mIsPauseMode;
		//GameObject::State changeState;
		//if (mIsPauseMode)
		//{
		//	changeState = GameObject::STATE_PAUSED; //�|�[�Y
		//}
		//else
		//{
		//	changeState = GameObject::STATE_ACTIVE; //�A�N�e�B�u
		//}

		////�S�ẴX�e�[�g��ύX����
		//for (auto tag = Tag::BEGIN; tag != Tag::END; ++tag)
		//{
		//	for (auto itr : mActors[tag])
		//	{
		//		if (itr->GetState() != GameObject::STATE_DEAD)
		//		{
		//			itr->SetState(changeState);
		//		}
		//	}
		//}
	}
}

void Game::Draw()
{
	if (!mNowScene)
	{
		return;
	}
	mNowScene->draw();
}

int Game::Update()
{
	// 16�~���b(= 60�t���[��/�b)�ɂȂ�悤�ɁA�O�̃t���[�������16�~���b�ȏ�o�߂���܂ő҂�
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// �O�̃t���[�����猻�ݎ����܂ł̌o�ߎ��ԎZ�o(�b�P�ʁj
	mDeltaTime = (SDL_GetTicks() - mTicksCount + 16) / 1000.0f;

	//���̃t���[���̊J�n���ԕۑ��i���̃t���[���܂ł̌o�ߎ��Ԃ��͂��邽�߁j
	mTicksCount = SDL_GetTicks();

	// �t���[�����Ԃ����܂�ɂ��o�߂��Ă���ꍇ��0.05 �� 20fps�Œ�
	if (mDeltaTime > 0.10f)
	{
		mDeltaTime = 0.10f;
	}

	if (!mNowScene)
	{
		return 0;
	}

	// �A�N�^�[�̏���
	GameObjectManager::GetInstance()->Update(mDeltaTime);
	//ActorUpdate();

	RENDERER->GetInstanceMeshManager()->PreparationBufferMatrices();

	//�����蔻��
	mPhysicsWorld->Collision();

	SceneBase* tmpScene;

	if (mActiveCamera != nullptr)
	{
		// �J�����s��v�Z
		mActiveCamera->Update(mDeltaTime);

		// �A�N�e�B�u�J�����̃r���[�s��������Ă���
		mViewMatrix = mActiveCamera->GetViewMatrix();
		GetRenderer()->SetViewMatrix(mViewMatrix);
	}
	//�V�[�������s
	tmpScene = mNowScene->update();

	if (tmpScene != mNowScene)
	{
		delete mNowScene;

		//�V�K�V�[�������̃V�[���ɍ����ւ�
		mNowScene = tmpScene;
		return 1;
	}
	return 0;
}

void Game::Run()
{
	// �����_���[������������Ă��Ȃ������珉����
	if (!mRenderer)
	{
		Initialize(1920, 1080);
	}
	// �J�n�V�[������`����Ă��Ȃ�������I��
	if (!mNowScene)
	{
		printf("�J�n�V�[�����ݒ肳��Ă��܂���");
		return;
	}

	// �Q�[�����[�v
	while (mIsRunning)
	{
		Input();
		if (Update())
		{
			continue; // �V�[���؂�ւ����̂ݕ`�揈���������Ȃ�����
		}
		Draw();
	}
}

void Game::Shutdown()
{
	GameObjectManager::GetInstance()->Finalize();
	GameObjectManager::Destroy();

	// �}�l�[�W���[�̔j��
	UIManager::DeleteInstance();
	ScoreManager::RemoveInstance();

	// �t�H���g�̏I������
	mFont->Finalize();
}

bool Game::Initialize(int screenWidth, int screenHeight, bool fullScreen)
{
	// SDL������
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
	{
		printf("SDL���������s: %s\n", SDL_GetError());
		return false;
	}

	// �����_���[�̍쐬
	mRenderer = new Renderer;
	if (!mRenderer->Initialize(screenWidth, screenHeight, fullScreen))
	{
		printf("Renderer�̏������Ɏ��s");
		delete mRenderer;
		return false;
	}

	// �T�E���h�̏�����
	if (!Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG))
	{
		delete mRenderer;
		return false;
	}
	printf("SDLMixer����������\n");

	// SDLMixer API�������@44100:�����̎��g�� 2:�X�e���I 4096:�����o�b�t�@�T�C�Y
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
	{
		delete mRenderer;
		Mix_CloseAudio();
		Mix_Quit();
		return false;
	}

	int decordNum = Mix_GetNumMusicDecoders();
	for (int i = 0; i < decordNum; ++i)
	{
		printf("MusicDecorder %d : %s\n", i, Mix_GetMusicDecoder(i));
	}

	int chunkNum = Mix_GetNumChunkDecoders();
	for (int i = 0; i < chunkNum; ++i)
	{
		printf("SoundDecorder %d : %s\n", i, Mix_GetChunkDecoder(i));
	}
	printf("SDLMixerAPI����������\n");

	mFPSCounter = new FPSCounter(60);
	// �����蔻��V�X�e���쐬
	mPhysicsWorld = new PhysicsWorld;

	// �R���g���[��������
	INPUT_INSTANCE.ControllerInit();

	mTicksCount = SDL_GetTicks();

	// �}�l�[�W���[����
	UIManager::CreateInstance();
	ScoreManager::CreateInstance();
	GameObjectManager::CreateInstance();

	// �t�H���g�̏�����
	mFont->Initialize();

	return true;
}

SDL_Renderer* Game::GetSDLRenderer()
{
	return mRenderer->GetSDLRenderer();
}

//void Game::AddActor(GameObject* actor)
//{
//	// ��������y���f�B���O�A�N�^�[�ɒǉ�
//	mPendingGameObjects.emplace_back(actor);
//}
//
//void Game::RemoveActor(GameObject* actor)
//{
//	//�y���f�B���O�A�N�^�[���ɂ���H
//	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
//	if (iter != mPendingActors.end())
//	{
//		//�y���f�B���O�A�N�^�[�̍Ō���Ƀf�[�^���ړ����āA�f�[�^������
//		std::iter_swap(iter, mPendingActors.end() - 1);
//		mPendingActors.pop_back();
//		return;
//	}
//
//	// �A�N�e�B�u�A�N�^�[���ɂ���H
//	auto tag = actor->GetTag();
//	iter = std::find(mActors[tag].begin(), mActors[tag].end(), actor);
//	if (iter != mActors[tag].end())
//	{
//		//�A�N�e�B�u�A�N�^�[�̍Ō���Ƀf�[�^���ړ����āA�f�[�^����
//		std::iter_swap(iter, mActors[tag].end() - 1);
//		mActors[tag].pop_back();
//	}
//}

void Game::SetCameraActor(CameraActor* camera)
{
	printf("SetCamera [%p]\n", camera);
	mActiveCamera = camera;
}

void Game::InActiveCamera(CameraActor* inActiveCam)
{
	if (inActiveCam == mActiveCamera)
	{
		printf("Camera is inActive. change to default view.\n");
		mActiveCamera = nullptr;
	}

	mViewMatrix =
		Matrix4::CreateLookAt(Vector3(0, 0, 100),
			Vector3(0, 0, 0),
			Vector3(0, 0, 1));
}

void Game::SetPlayerActor(GameObject* player)
{
	mPlayerActor = player;
}

//GameObject* Game::GetPlayerActor()
//{
//	return GetFirstActor(Tag::PLAYER);
//}
//
//GameObject* Game::GetEnemyActor()
//{
//	return GetFirstActor(Tag::ENEMY);
//}

const Matrix4& Game::GetViewMatrix()
{
	return mViewMatrix;
}

const Vector3& Game::GetViewTarget()
{
	if (mActiveCamera == nullptr)
	{
		printf("Camera is inActive. return IllegalVec\n");
	}
	return mActiveCamera->GetViewTarget();
}

const Vector3& Game::GetViewPos()
{
	if (mActiveCamera == nullptr)
	{
		printf("Camera is inActive. return IllegalVec\n");
	}
	return mActiveCamera->GetViewPos();
}

//std::vector<class GameObject*> const& Game::GetActors(Tag type)
//{
//	return mActors[type];
//}

//GameObject* Game::GetFirstActor(Tag type)
//{
//	// �A�N�e�B�u���X�g���`�F�b�N
//	if (mActors[type].size() != 0)
//	{
//		return mActors[type][0];
//	}
//
//	// �y���f�B���O�ɂ��Ȃ���������
//	for (auto iter = mPendingActors.begin(); iter != mPendingActors.end(); ++iter)
//	{
//		if ((*iter)->GetTag() == type)
//		{
//			return *iter;
//		}
//	}
//
//	return nullptr;
//}

//bool Game::IsExistActorType(Tag type)
//{
//	return mActors[type].size() != 0;
//}

//GameObject* Game::FindActorFromID(int searchActorID)
//{
//	// �A�N�e�B�u���X�g�����猟��
//	for (Tag t = Tag::BEGIN; t != Tag::END; ++t)
//	{
//		for (auto item : mActors[t])
//		{
//			if (item->GetID() == searchActorID)
//			{
//				return item;
//			}
//		}
//	}
//	// �y���f�B���O�ɂ��Ȃ���������
//	for (auto iter = mPendingActors.begin(); iter != mPendingActors.end(); ++iter)
//	{
//		if ((*iter)->GetID() == searchActorID)
//		{
//			return *iter;
//		}
//	}
//	return nullptr;
//}

