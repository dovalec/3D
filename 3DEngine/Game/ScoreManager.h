#ifndef __ScoreManager__
#define __ScoreManager__

#include "../Config.h"
#include "../GlobalHeap.h"

class ScoreManager {
public:
	NEWDEL_DECL;

    ScoreManager();
	virtual ~ScoreManager();
    
	inline static ScoreManager * GetScoreManager()
	{
		return GlobalHeap::GetScoreManager();
	}

	void SetMaxLife(int life);

	inline int GetLife()
	{
		return mLife;
	}
	inline int GetMaxLife()
	{
		return mMaxLife;
	}


	inline int GetScore()
	{
		return mScore;
	}
	void IncScore();
	void DecScore();
	void IncScore(int num);
	void DecScore(int num);

	void IncLife();
	void DecLife();
	void IncLife(int num);
	void DecLife(int num);

protected:
	int mScore;
	int mLife;
	int mMaxLife;
	
};

#endif //__ScoreManager__
