#include "../pch.h"

CShipMsgHandler::CShipMsgHandler(void)
{
}

CShipMsgHandler::~CShipMsgHandler(void)
{
}

void CShipMsgHandler::InitMsgMap(CHashMap<MSG_ID_TYPE,DOS_MSG_HANDLE_INFO>& MsgMap)
{
	DOS_MSG_HANDLE_INFO MsgHandleInfo;
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(DOS_MSG_HANDLE_FN)&CShipMsgHandler::HandleMsgAttack;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACE_SHIP,IShip::METHOD_ATTACK,false),MsgHandleInfo);
	
}

int CShipMsgHandler::HandleMsgAttack(const CSmartStruct& Packet)
{
	int		Damage;
	
	
	Damage=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_ATTACK_DAMAGE:
			{
				Damage=Value;
			}
			break;
		
		}
	}
		

	return Attack(Damage);
}
