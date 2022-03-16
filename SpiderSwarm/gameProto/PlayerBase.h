#pragma once

#include "GameObject.h"
#include "BoxCollider.h"


// プレイヤークラス状態enum 状態に変更があったらstatePlayerNameも変更
enum class PlayerStateEnum : unsigned char
{
	Invalid,    // 無効なステート
	Spawn,      // 出現
	Idle,       // 待機状態
	Walk,       // 歩く
	Turn,       // ターン
	Forward,	// 前進
	Right,		// 右移動
	Left,		// 左移動
	Backward,	// 後退
	RunForward, // 前走り
	RunRight,	// 右走り
	RunLeft,	// 左走り
	RunBackward,// 後ろ走り
	LookAround, // 見回す
	Attack1,    // 攻撃１
	Attack2,    // 攻撃２
	Attack3,    // 攻撃３
	GetDamage,  // ダメージ受けた
	Roar,       // 吠える
	Stun,       // 気絶
	Die,        // 死亡
	Hold,		// 銃を構える
	Victory,	// 勝利ポーズ
	StateNum	// 状態の総数
};

// シーンごとのプレイヤー
enum class PlayerSceneState
{
	PLAYER_TITLESCENE = 0,	// タイトルシーン
	PLAYER_SELECTSCENE,		// セレクトシーン
	PLAYER_GAMESCENE,		// ゲームシーン
	PLAYER_RESULT,			// リザルトシーン
};

// プレイヤーの状態
enum class PlayerState
{
	PLAYER_STATE_IDLE = 0,		// 待機
	PLAYER_STATE_HOLD,			// 構える
	PLAYER_STATE_FORWARD,		// 前進
	PLAYER_STATE_RIGHT,			// 右移動
	PLAYER_STATE_LEFT,			// 左移動
	PLAYER_STATE_BACKWARD,		// 後退
	PLAYER_STATE_RUN_FORWARD,	// 走る
	PLAYER_STATE_RUN_RIGHT,		// 右走り
	PLAYER_STATE_RUN_LEFT,		// 左走り
	PLAYER_STATE_RUN_BACKWARD,	// 後ろ走り
	PLAYER_STATE_DIE,			// 死亡
	PLAYER_STATE_VICTORY,		// 勝利ポーズ
	PLAYER_STATE_NUM,			// 総アニメーション数
};

class SkeletalMeshComponent;
class Animation;
class BoxCollider;
class EffectComponent;
class PlayerStateBase;
class Mesh;
class PlayerBehaviorComponent;
class AttachMeshComponent;
class InvincibleComponent;

class PlayerBase : public GameObject
{
public:
	PlayerBase();
	~PlayerBase();

	virtual void UpdateActor(float deltaTime) = 0;

	SkeletalMeshComponent* GetSkeletalMeshComp();
	const Animation* GetAnim(PlayerState state);

	void OnCollisionEnter(ColliderComponent* own, ColliderComponent* other);

	// キャラ情報読み込み関連
	virtual void LoadModel() = 0;
	virtual void LoadSkeleton() = 0;
	virtual void LoadAnimation() = 0;

	// ふるまいを追加
	virtual void BehaviorResister() = 0;

	// 当たり判定を追加
	virtual void SetCollider() = 0;

	// ステート名を取得するヘルパー関数
	const char* GetPlayerStateEnumName(PlayerStateEnum state);

	// HPのゲッターセッター
	int GetHitPoint() { return mHitPoint; }
	void SetHitPoint(int hp) { mHitPoint = hp; }

	bool GetDeadAnimFlag() { return mIsDeadAnimFinish; }
	void SetDeadAnimFlag(bool isDeadAnimFinish) { mIsDeadAnimFinish = isDeadAnimFinish; }

	void SetPlayerSceneState(PlayerSceneState state) { mPlayerSceneState = state; }

	void SetForwardVec(Vector3& v);

	const int& GetLevel() { return mLevel; }

	void SetVisivle(bool isVisible);

protected:
	AABB mPlayerBox;
	Mesh* mMesh;

	SkeletalMeshComponent* mMeshComp;
	std::vector<const class Animation*> mAnimTypes;
	BoxCollider* mHitBox;
	BoxCollider* mHitTrigger;
	BoxCollider* mHitGroundBox;
	BoxCollider* mHitHeadBox;
	BoxCollider* mAttackBox;	

	EffectComponent* mEffectComp;
	AttachMeshComponent* mAttachMeshComp;
	InvincibleComponent* mInvincibleComp;

	PlayerState mNowState;        // 現在のステート
	PlayerState mNextState;       // 次のステート
	std::vector<PlayerStateBase*> mStatePools;      // ステートクラスプール
	PlayerSceneState mPlayerSceneState;				// プレイヤーのシーン毎の状態

	PlayerBehaviorComponent* mPlayerBehavior;	// プレイヤーのふるまい

	Vector3 mVelocityVec;
	Vector3 mForwardVec;  // 前進方向ベクトル  
			
	static const float mAnimationSpeed;
	static const float mInterval;
	static const float mSpecialShotInterval;
	static const float mInvisibleInterval;	
	static const float mInvincibleTime;		// 無敵時間
	static const float mBulletSpeed;		// 弾速
	static const float mBulletScale;

	float mShootTimer;
	float mSpecialShotTimer;
	float mInvincibleTimer;					// 無敵時間を計測

	bool mAimMode;

	int mHitPoint;
	bool mIsDeadAnimFinish;

	int mLevel;							// レベル
	int mExperience;					// 経験値
	static const int mRequireExperience;	// 必要経験値

	SceneBase::SceneState mSceneState;
};

