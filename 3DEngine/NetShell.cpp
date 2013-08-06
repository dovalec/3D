#include "NetShell.h"

#include "Game/BotNode.h"
#include "Node.h"
#include "Game/PlayerNode.h"

NEWDEL_IMPL(NetShell)
NetShell::NetShell()
{
	mPlayerNode = NULL;
    for (int n=0 ; n< MAX_NET_BOT ; n++)
    {
        mBotNodes[n] = NULL;
    }
 
}


void NetShell::GetPlayerData( float & tx, float & ty, float & tz, float & ro,  int & state1, int & state2)
{ 
    if (mPlayerNode == NULL)
        return;
    
    Node * playerRoot = mPlayerNode->GetNode();
    if (playerRoot == NULL)
        return;
    
    playerRoot->GetTranslation(tx, ty, tz);
    PVRTQUATERNION playerQuat  = playerRoot->GetRotation();
    
    PVRTVECTOR3 axis={0,1,0};
    PVRTMatrixQuaternionToAxisAngle(playerQuat,axis, ro);
    
    state1 = 0;
    state2 = 0;
}

void NetShell::SetBotData( int index, float tickCount, float tx, float ty, float tz, float ro, int state1, int state2)
{
    if (index >= MAX_NET_BOT)
        STOP;
    
    if (mBotNodes[index] == NULL)
        return;
    
    mBotNodes[index]->SetOnlineMode(true);
    mBotNodes[index]->SetNetTickCount(tickCount);
    mBotNodes[index]->SetNetTranslation(tx, ty, tz);
    mBotNodes[index]->SetNetRotation(ro);
    
}

void NetShell::SetPlayerNode(PlayerNode * pPlayerNode)
{
    mPlayerNode = pPlayerNode;
}

void NetShell::SetBotNode(BotNode * pNode, int index)
{
    if (index >= MAX_NET_BOT)
        STOP;
    
    mBotNodes[index] = pNode;
}

