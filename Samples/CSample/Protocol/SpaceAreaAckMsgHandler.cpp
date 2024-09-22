#include "../pch.h"

CSpaceAreaAckMsgHandler::CSpaceAreaAckMsgHandler(void)
{
}

CSpaceAreaAckMsgHandler::~CSpaceAreaAckMsgHandler(void)
{
}

void CSpaceAreaAckMsgHandler::InitMsgMap(CHashMap<MSG_ID_TYPE,DOS_MSG_HANDLE_INFO>& MsgMap)
{
	DOS_MSG_HANDLE_INFO MsgHandleInfo;
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(DOS_MSG_HANDLE_FN)&CSpaceAreaAckMsgHandler::HandleMsgRegisterShipAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACE_SPACE_AREA,ISpaceArea::METHOD_REGISTER_SHIP,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(DOS_MSG_HANDLE_FN)&CSpaceAreaAckMsgHandler::HandleMsgUnregisterShipAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACE_SPACE_AREA,ISpaceArea::METHOD_UNREGISTER_SHIP,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(DOS_MSG_HANDLE_FN)&CSpaceAreaAckMsgHandler::HandleMsgFindShipAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACE_SPACE_AREA,ISpaceArea::METHOD_FIND_SHIP,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(DOS_MSG_HANDLE_FN)&CSpaceAreaAckMsgHandler::HandleMsgRegisterShipyardAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACE_SPACE_AREA,ISpaceArea::METHOD_REGISTER_SHIPYARD,true),MsgHandleInfo);
	
}

int CSpaceAreaAckMsgHandler::HandleMsgRegisterShipAck(const CSmartStruct& Packet)
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
		case SST_REGISTER_SHIP_ACK_RESULT:
			{
				Result=Value;
			}
			break;
		
		}
	}
		

	return RegisterShipAck(Result);
}
int CSpaceAreaAckMsgHandler::HandleMsgUnregisterShipAck(const CSmartStruct& Packet)
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
		case SST_UNREGISTER_SHIP_ACK_RESULT:
			{
				Result=Value;
			}
			break;
		
		}
	}
		

	return UnregisterShipAck(Result);
}
int CSpaceAreaAckMsgHandler::HandleMsgFindShipAck(const CSmartStruct& Packet)
{
	short			Result;
	OBJECT_ID		ShipID;
	
	
	Result=0;
	ShipID.ID=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_FIND_SHIP_ACK_RESULT:
			{
				Result=Value;
			}
			break;
		case SST_FIND_SHIP_ACK_SHIP_ID:
			{
				ShipID.ID=Value;
			}
			break;
		
		}
	}
		

	return FindShipAck(Result,ShipID);
}
int CSpaceAreaAckMsgHandler::HandleMsgRegisterShipyardAck(const CSmartStruct& Packet)
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
		case SST_REGISTER_SHIPYARD_ACK_RESULT:
			{
				Result=Value;
			}
			break;
		
		}
	}
		

	return RegisterShipyardAck(Result);
}
