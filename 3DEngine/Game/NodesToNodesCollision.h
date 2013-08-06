#ifndef __NodesToNodesCollision__
#define __NodesToNodesCollision__

#include "../Config.h"
#include "../Events/EventSender.h"
#include "../Events/Event.h"

#include "../Node.h"

class CollisionNode {
public:
	NEWDEL_DECL;
	CollisionNode(Node * pNode)
	{
		this->pNode = pNode;
		this->pExtra1 = NULL;
		this->pExtra2 = NULL;
		this->pExtra3 = NULL;
		mEnable = true;
	}

	CollisionNode(Node * pNode, void * pExtra1)
	{
		this->pNode = pNode;
		this->pExtra1 = pExtra1;
		this->pExtra2 = NULL;
		this->pExtra3 = NULL;
		mEnable = true;
	}

	CollisionNode(Node * pNode, void * pExtra1, void * pExtra2)
	{
		this->pNode = pNode;
		this->pExtra1 = pExtra1;
		this->pExtra2 = pExtra2;
		this->pExtra3 = NULL;
		mEnable = true;
	}

	CollisionNode(Node * pNode, void * pExtra1, void * pExtra2, void * pExtra3)
	{
		this->pNode = pNode;
		this->pExtra1 = pExtra1;
		this->pExtra2 = pExtra2;
		this->pExtra3 = pExtra3;
		mEnable = true;
	}

	inline void Enable()
	{
		mEnable = true;
	}

	inline void Disable()
	{
		mEnable = false;
	}


	inline bool IsEnabled()
	{
		return mEnable;
	}

	bool mEnable;
	Node * pNode;
	void * pExtra1;
	void * pExtra2;
	void * pExtra3;
};


class CollisionEvent : public Event {
public:
	NEWDEL_DECL;
	CollisionEvent(EventSender * pSender, String type) : Event(pSender, type)
	{
//		pNode = NULL;
		minDist = 0.0f;
		pExtra1 = NULL;
		pExtra2 = NULL;
		pExtra3 = NULL;
	}

	virtual ~CollisionEvent()
	{
		if (pCollisionNode)
			delete pCollisionNode;
	}

	CollisionNode * pCollisionNode;
	//Node * pNode;
	float minDist;
	void * pExtra1;
	void * pExtra2;
	void * pExtra3;
};

class INodesCollision : public EventSender {
public:
	NEWDEL_DECL;
	INodesCollision( String type );
	virtual ~INodesCollision();

	virtual void OnTick() = 0;
};
class NodesToNodesCollision : public INodesCollision {
public:
	NEWDEL_DECL;
	NodesToNodesCollision( String type );
	virtual ~NodesToNodesCollision();

	void AddCollisionNode(CollisionNode * pCollisionNode);
	void AddNodeEvent(CollisionEvent * pNodeEvent);

	virtual void OnTick();

protected: 
	int mNumNodes;
	int mNumEventNodes;

	CollisionNode * mCollisionNodes[MAX_COLLISION_OBJ_NUM];
	CollisionEvent * mNodeEvents[MAX_COLLISION_OBJ_NUM];
};

#endif //__NodesToNodesCollision__
