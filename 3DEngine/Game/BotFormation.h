#ifndef __BotFormation__
#define __BotFormation__

#include "../Config.h"

#define MAX_GROUP_SIZE 32

class BotNode; 
class PlayerNode; 

struct BotFormationBoxParam {
	float width;
	float depth;
	float distance;

};

struct BotFormationCircleParam {
	float radius;
	float distance;
};


struct BotFormationLineParam {
	float length;
	float distance;
};

class BotFormation {

public:
	NEWDEL_DECL
	enum BotFormationMethod {
		box_mode,
		horizontal_line_mode,
		vertical_line_mode,
		diagonal_line_mode,
		circle_mode,
	};

	BotFormation();
	virtual ~BotFormation();
	
	void OnTick();

	bool AddBotNode(BotNode * pBotNode);
	void RemoveBotNode(BotNode * pBotNode);
	void RemoveBotNode(int index);
	void SetFormationMethod(BotFormationMethod method);

	void SetPlayerNode( PlayerNode * playerNode);
	PlayerNode * GetPlayerNode( PlayerNode * playerNode)
	{
		return mPlayerNode;
	}

	inline bool IsGroupFull()
	{
		return (mNumBotNodes == mGroupSize);
	}


	inline int GetNumberBotNodes()
	{
		return mNumBotNodes;
	}

	void SetGroupSize(int size);
	void SetBoxParams(float width, float depth, float distance);
	void SetCircleParams(float radius, float distance);
	void SetLineParams(float length, float distance);

	void BoxMode();
	void CircleMode();
	void DiagonalLineMode();
	void HorizontalLineMode();
	void VerticalLineMode();
	void LineMode(float addAngle);

protected:
	int mNumBotNodes;
	float mPlayerMoveAngle;
	BotNode * mBotNodes[MAX_GROUP_SIZE];
	PlayerNode * mPlayerNode;
	PVRTVec3 mLastPlayerWorldPos;

	BotFormationMethod mMethod;

	BotFormationBoxParam		mBoxParams;
	BotFormationCircleParam		mCircleParams;
	BotFormationLineParam		mLineParams;

	PVRTVec3 mTargetPoints[MAX_GROUP_SIZE];
	PVRTVec3 mDistVec[MAX_GROUP_SIZE];
	float mRandomFactor[MAX_GROUP_SIZE];
	int mGroupSize;

	
};

#endif //__BotFormation__
