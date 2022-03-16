#pragma once

// 毎フレームにかかった時間を計測し上限を設けるクラス
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

	const float mSetMaxFps;			//フレームレートの最高値
	const float mOneFrameTickCount;	//１フレームにかける時間
	int mFpsCount;					//フレームのカウンター
	float mFrameStartTickTime;		//FPSの計算を始めた時の時間
	float mFps;						//計算したフレームレート
	float mBeforetickCount;			//前のフレームのかかった時間
	float mDeltaTime;				//現在のフレームのかかった時間
};

