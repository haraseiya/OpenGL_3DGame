#include "ScoreManager.h"
#include <algorithm>

ScoreManager* ScoreManager::mScoreManager = nullptr;

ScoreManager::ScoreManager()
	: mSumScore(0)
	, mHighScore(0)
{
	//for (int i = 0; i < 5; i++)
	//{
	//	mHighScores[i] = 0;
	//}
}

void ScoreManager::Initialize()
{
	mSumScore = 0;
}

void ScoreManager::AddScore(int score)
{
	mSumScore += score;
}

void ScoreManager::ScoreUpdate()
{
	mHighScores.push_back(mHighScore);
	std::sort(mHighScores.begin(), mHighScores.end());
}

int ScoreManager::GetHighScore()
{
	// 合計スコアがハイスコアを超えたら
	const bool isUpdateHightScore = mSumScore < mHighScore;
	if (isUpdateHightScore)
	{
		mHighScore = mSumScore;
	}
	return mHighScore;
}

void ScoreManager::CreateInstance()
{
	if (mScoreManager == nullptr)
	{
		mScoreManager = new ScoreManager();
	}
}

void ScoreManager::RemoveInstance()
{
	if (mScoreManager != nullptr)
	{
		delete mScoreManager;
		mScoreManager = nullptr;
	}
}

