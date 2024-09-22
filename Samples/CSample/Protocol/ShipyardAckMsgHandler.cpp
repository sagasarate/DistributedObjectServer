#include "../pch.h"

CShipyardAckMsgHandler::CShipyardAckMsgHandler(void)
{
}

CShipyardAckMsgHandler::~CShipyardAckMsgHandler(void)
{
}

void CShipyardAckMsgHandler::InitMsgMap(CHashMap<MSG_ID_TYPE,DOS_MSG_HANDLE_INFO>& MsgMap)
{
	DOS_MSG_HANDLE_INFO MsgHandleInfo;
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(DOS_MSG_HANDLE_FN)&CShipyardAckMsgHandler::HandleMsgCreateShipAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACE_SHIPYARD,IShipyard::METHOD_CREATE_SHIP,true),MsgHandleInfo);
	
}

int CShipyardAckMsgHandler::HandleMsgCreateShipAck(const CSmartStruct& Packet)
{
	short		Result;
	
	
	Result=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_CREATE_SHIP_ACK_RESULT:
			{
				Result=Value;
			}
			break;
		
		}
	}
		

	return CreateShipAck(Result);
}
