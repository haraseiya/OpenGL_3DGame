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

	// メッシュコンポーネントの描画
	void Draw(Shader* shader) override;
	void Update(float deltaTime) override;

	// スケルトンのセット
	void SetSkeleton(const Skeleton* sk) { mSkeleton = sk; }

	// アニメーションの再生。アニメーションの残り長さを返す
	float PlayAnimation(const Animation* anim, float playRate = 0.5f);

	// 現在再生中のアニメーションは再生中か？ true : 再生中 false : 再生終了
	bool IsPlaying();

	// 現在再生中のアニメーション
	const Animation* GetNowPlayingAnimation() { return mAnimation; }

	// 現在のアニメーションの再生時間を取得
	float GetPlayTime() { return mAnimTime; }

	int GetBoneIndexFromName(const char* boneName) const;
	void GetMatrixFromBoneIndex(Matrix4& boneWorldMatrix, int boneIndex)const;

	void SetHitColor(Vector3 color) { mHitColor = color; }

protected:
	void ComputeMatrixPalette();    // 行列パレットの計算

	MatrixPalette mPalette;			// 行列パレット
	const Skeleton* mSkeleton;      // スケルトン
	const Animation* mAnimation;    // アニメーション
	float mAnimPlayRate;            // アニメーションの再生速度
	float mAnimTime;                // アニメーション時間
	Vector3 mHitColor;				// 被弾色
};
