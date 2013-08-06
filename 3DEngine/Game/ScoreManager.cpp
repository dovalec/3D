#include "ScoreManager.h"


NEWDEL_IMPL(ScoreManager)
ScoreManager::ScoreManager()
{
	mScore = 0;
	mLife = 100;
	mMaxLife = 100;
}

ScoreManager::~ScoreManager()
{
	
}

void ScoreManager::SetMaxLife(int life)
{
	mLife = mMaxLife = life;
}

void ScoreManager::IncScore()
{
	mScore++;
}
void ScoreManager::DecScore()
{
	mScore--;
	if (mScore<0)
		mScore = 0;
}
void ScoreManager::IncScore(int num)
{
	mScore += num;
}
void ScoreManager::DecScore(int num)
{
	mScore -= num;
	if (mScore<0)
		mScore = 0;
}

void ScoreManager::IncLife()
{
	mLife++;
	if (mLife > mMaxLife)
		mLife = mMaxLife;
}

void ScoreManager::IncLife(int num)
{
	mLife += num;
	if (mLife > mMaxLife)
		mLife = mMaxLife;
}

void ScoreManager::DecLife()
{
	mLife--;

	if (mLife<0)
		mLife = 0;

}


void ScoreManager::DecLife(int num)
{
	if (!mLife)
		return;

	mLife -= num;

	if (mLife<0)
		mLife = 0;
}
