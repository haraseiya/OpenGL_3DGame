#include "PlayerStateForward.h"
#include "PlayerBase.h"
#include "Input.h"
#include "SkeletalMeshComponent.h"
#include "PlayerBullet.h"

const float PlayerStateForward::mRoot = 1.41421356237f;
const float PlayerStateForward::mInterval = 0.1f;

PlayerStateForward::PlayerStateForward(PlayerBehaviorComponent* owner)
	: PlayerStateBase(owner)
	, mBullet(nullptr)
	, mDot(0.0f)
	, mShootTimer(0.0f)
{
	mStateType = PlayerStateEnum::Forward;
}

PlayerStateForward::~PlayerStateForward()
{
}

PlayerStateEnum PlayerStateForward::Update(float deltaTime)
{
	// プレイヤー死亡フラグ
	const bool isDead = mOwner->GetHitPoint() <= 0;
	if (isDead)
	{
		return PlayerStateEnum::Die;
	}

	// Lボタンが押されたら走る
	const bool isRun = INPUT_INSTANCE.IsKeyPressed(KEY_L);
	if (isRun)
	{
		return PlayerStateEnum::RunForward;
	}

	// コントローラ入力されたか
	Vector2 stickL = INPUT_INSTANCE.GetLStick();
	const bool isContollerInputOff = !INPUT_INSTANCE.IsLStickMove();

	//方向キーが入力されたか
	const bool IsIdle = INPUT_INSTANCE.IsKeyOff(KEY_UP) &
		INPUT_INSTANCE.IsKeyOff(KEY_RIGHT) &
		INPUT_INSTANCE.IsKeyOff(KEY_DOWN) &
		INPUT_INSTANCE.IsKeyOff(KEY_LEFT) &
		isContollerInputOff;

	// 待機状態の場合
	if (IsIdle)
	{
		return PlayerStateEnum::Idle;
	}

	// 移動処理
	MoveCalc(deltaTime);
	Shot(deltaTime);
	return PlayerStateEnum::Forward;
}

// RUN状態への切り替え処理
void PlayerStateForward::OnEnter()
{
	// アニメーションを再生
	SkeletalMeshComponent* meshcomp = mOwner->GetSkeletalMeshComp();
	meshcomp->PlayAnimation(mOwner->GetAnim(PlayerState::PLAYER_STATE_FORWARD));
}

void PlayerStateForward::OnExit()
{
}

void PlayerStateForward::Shot(float deltaTime)
{
	// 弾が撃てるなら
	mShootTimer += deltaTime;
	const bool isShoot = INPUT_INSTANCE.IsKeyPressed(KEY_R) && mShootTimer > mInterval;
	if (isShoot)
	{
		mShootTimer = 0.0f;

		if (mOwner->GetLevel() == 1)
		{
			mBullet = new PlayerBullet(mOwner, mOwner->GetPosition(), mOwner->GetForward(), 1.0f, 1000.0f);
		}
		else if (mOwner->GetLevel() == 2)
		{
			Vector3 upperRight = Vector3(1.0f, 0.3f, 0.0f);
			Vector3 upperLeft = Vector3(1.0f, -0.3f, 0.0f);
			upperRight.Normalize();
			upperLeft.Normalize();
			mBullet = new PlayerBullet(mOwner, mOwner->GetPosition(), mOwner->GetForward(), 1.0f, 1000.0f);
			mBullet = new PlayerBullet(mOwner, mOwner->GetPosition(), mOwner->GetDirectionFromForward(upperRight), 1.0f, 1000.0f);
			mBullet = new PlayerBullet(mOwner, mOwner->GetPosition(), mOwner->GetDirectionFromForward(upperLeft), 1.0f, 1000.0f);
		}
		else if (mOwner->GetLevel() >= 3)
		{
			Vector3 upperRight1 = Vector3(1.0f, 0.3f, 0.0f);
			Vector3 upperLeft1 = Vector3(1.0f, -0.3f, 0.0f);
			Vector3 upperRight2 = Vector3(1.0f, 0.5f, 0.0f);
			Vector3 upperLeft2 = Vector3(1.0f, -0.5f, 0.0f);
			upperRight1.Normalize();
			upperLeft1.Normalize();
			upperRight2.Normalize();
			upperLeft2.Normalize();

			mBullet = new PlayerBullet(mOwner, mOwner->GetPosition(), mOwner->GetForward(), 1.0f, 1000.0f);
			mBullet = new PlayerBullet(mOwner, mOwner->GetPosition(), mOwner->GetDirectionFromForward(upperRight1), 1.0f, 1000.0f);
			mBullet = new PlayerBullet(mOwner, mOwner->GetPosition(), mOwner->GetDirectionFromForward(upperRight2), 1.0f, 1000.0f);
			mBullet = new PlayerBullet(mOwner, mOwner->GetPosition(), mOwner->GetDirectionFromForward(upperLeft1), 1.0f, 1000.0f);
			mBullet = new PlayerBullet(mOwner, mOwner->GetPosition(), mOwner->GetDirectionFromForward(upperLeft2), 1.0f, 1000.0f);
		}
	}
}

// 移動処理
void PlayerStateForward::MoveCalc(float deltaTime)
{
	//キャラ入力
	const float speed = 350.0f;
	float charaSpeed = mOwner->GetSpeed();

	// カメラからみた前進方向を取得
	Vector3 TargetPos = GAMEINSTANCE.GetViewTarget();
	Vector3 ViewPos = GAMEINSTANCE.GetViewPos();
	Vector3 forwardVec = TargetPos - ViewPos;
	forwardVec.z = 0.0; // 高さ方向を無視

	// カメラ前方ベクトルと右方向ベクトル算出
	forwardVec = Vector3::Normalize(forwardVec);
	Vector3 rightVec = Vector3::Cross(Vector3::UnitZ, forwardVec);

	// 右方向ベクトルからカメラ回転角を算出
	float forwardAngle = 0.0f;
	float angleSign;

	Vector3 tmpVec;
	forwardAngle = acosf(Vector3::Dot(Vector3::UnitX, rightVec));

	// 右回転か左回転か？
	tmpVec = Vector3::Cross(Vector3::UnitX, rightVec);
	angleSign = (tmpVec.z > 0.0) ? 1.0f : -1.0f;
	forwardAngle *= angleSign;

	Vector3 charaForwardVec = mOwner->GetForward(); // キャラの前進ベクトル

	// キャラクター移動の入力キー取得
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

	// スティック情報読み込み
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

	// 左スティックの処理
	if (DirVecL.LengthSq() > 0.5f)
	{
		// 移動処理
		Vector3 position = mOwner->GetPosition();

		// 現在のスピードを保存
		charaSpeed = speed * deltaTime;
		position += DirVecL * charaSpeed;

		// キャラの位置・スピード・変換行列の再計算の必要をセット
		mOwner->SetPosition(position);
		mOwner->SetSpeed(charaSpeed);

		//printf("x : %f , y : %f , z : %f\n", DirVecL.x, DirVecL.y, DirVecL.z);
	}

	// 右スティックの処理
	if (DirVecR.LengthSq() > 0.5f)
	{
		// 入力された方向に向けて滑らかに回転
		charaForwardVec = Vector3::Lerp(mOwner->GetForward(), DirVecR, 0.2f);
		charaForwardVec.Normalize();
		mOwner->RotateToNewForward(charaForwardVec);
	}

	// キャラクターの前方と今から向く方向の内積を取る
	mDot = Vector3::Dot(charaForwardVec, DirVecR);

	mOwner->SetComputeWorldTransform();
}

