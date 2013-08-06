#ifndef __NetShell__
#define __NetShell__

#include "Config.h"

class BotNode;
class Node;
class PlayerNode;

#define MAX_NET_BOT 10


class NetShell {

public:	

	NEWDEL_DECL
    NetShell();
    
	inline static NetShell * GetNetShell()
	{
		return GlobalSys::GetNetShell();	
	}

	void Destroy()
	{

	}

    
   void GetPlayerData( float & tx, float & ty, float & tz, float & ro, int & state1, int & state2);
   void SetBotData(int index, float tickCount, float tx, float ty, float tz, float ro, int state1, int state2);
   void SetPlayerNode(PlayerNode * pPlayerNode);    
   void SetBotNode(BotNode * pNode, int index);    
    
protected:

    ///////////// player and bot data //////////////
    
    PlayerNode * mPlayerNode;
    BotNode * mBotNodes[MAX_NET_BOT];
    
    ///////////// player and bot  data //////////////


};

#endif //__NetShell__
