#include "Game.h"
#include "CameraActor.h"

CameraActor::CameraActor(GameObject* targetActor)
	: mTargetActor(targetActor)
	, mCameraSceneState(CameraSceneState::CAMERA_SCENE_TITLE)
	, mPosition(Vector3::Zero)
	, mViewTarget(Vector3::Zero)
	, mViewVector(Vector3::Zero)
{	
	//デフォルトカメラ値セット
	mViewMatrix = Matrix4::CreateLookAt(Vector3(-100, 0, 100),Vector3(0, 0, 0),Vector3(0, 0, 1));

	// ゲームシステム側にこれがアクティブカメラであることを通知
	SetActive();
}

CameraActor::CameraActor(std::vector<GameObject*> targets)
{
	mTargetActors = targets;

}

CameraActor::~CameraActor()
{
	// ゲームシステム側にアクティブカメラがいなくなったことを通知
	// カメラアクター自体はActorでデストラクトされる
	GAMEINSTANCE.InActiveCamera(this);
}

// オーバーライドして使わない場合
void CameraActor::UpdateActor(float deltaTime)
{
}

void CameraActor::Update(float deltaTime)
{
	// アクター更新処理
	UpdateActor(deltaTime);

	// カメラ視線ベクトル、カメラ行列作成
	mViewVector = mViewTarget - mPosition;
	mViewMatrix = Matrix4::CreateLookAt(mPosition, mViewTarget, Vector3(0, 0, 1));

	// シーン毎のカメラ挙動
	switch (mCameraSceneState)
	{
	// タイトルシーン
	case CameraSceneState::CAMERA_SCENE_TITLE:

		break;

	// ゲームシーン
	case CameraSceneState::CAMERA_SCENE_GAME:

		break;

	// リザルト
	case CameraSceneState::CAMERA_SCENE_RESULT:

		break;
	}
}

//ゲームシステム側にこれがアクティブカメラであることを手動で通知する
void CameraActor::SetActive()
{
	//ゲームシステム側にこれがアクティブカメラであることを通知
	GAMEINSTANCE.SetCameraActor(this);
}

// 位置方向初期化
void CameraActor::Init(const Vector3& cameraPos, const Vector3& targetPos, const Vector3& upVec)
{
	mPosition = cameraPos;
	mViewTarget = targetPos;
	mViewVector = targetPos - cameraPos;

	mViewMatrix = Matrix4::CreateLookAt(cameraPos, targetPos, upVec);
}

