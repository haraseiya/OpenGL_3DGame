#pragma once

#define SCORE_MANAGER ScoreManager::GetInstance();

#include <vector>

class ScoreManager
{
public:
	static ScoreManager* GetInstance() { return mScoreManager; }

	void Initialize();
	void AddScore(int score);
	void ScoreUpdate();
	int GetSumScore() { return mSumScore; }
	int GetHighScore();

	static void CreateInstance();
	static void RemoveInstance();

private:
	ScoreManager();
	~ScoreManager(){}

	static ScoreManager* mScoreManager;
	int mSumScore;
	int mHighScore;

	std::vector<int> mHighScores;
};

