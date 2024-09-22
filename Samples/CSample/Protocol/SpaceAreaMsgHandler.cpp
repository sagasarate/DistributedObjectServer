#include "../pch.h"

CSpaceAreaMsgHandler::CSpaceAreaMsgHandler(void)
{
}

CSpaceAreaMsgHandler::~CSpaceAreaMsgHandler(void)
{
}

void CSpaceAreaMsgHandler::InitMsgMap(CHashMap<MSG_ID_TYPE,DOS_MSG_HANDLE_INFO>& MsgMap)
{
	DOS_MSG_HANDLE_INFO MsgHandleInfo;
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(DOS_MSG_HANDLE_FN)&CSpaceAreaMsgHandler::HandleMsgRegisterShip;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACE_SPACE_AREA,ISpaceArea::METHOD_REGISTER_SHIP,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(DOS_MSG_HANDLE_FN)&CSpaceAreaMsgHandler::HandleMsgUnregisterShip;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACE_SPACE_AREA,ISpaceArea::METHOD_UNREGISTER_SHIP,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(DOS_MSG_HANDLE_FN)&CSpaceAreaMsgHandler::HandleMsgFindShip;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACE_SPACE_AREA,ISpaceArea::METHOD_FIND_SHIP,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(DOS_MSG_HANDLE_FN)&CSpaceAreaMsgHandler::HandleMsgRegisterShipyard;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACE_SPACE_AREA,ISpaceArea::METHOD_REGISTER_SHIPYARD,false),MsgHandleInfo);
	
}

int CSpaceAreaMsgHandler::HandleMsgRegisterShip(const CSmartStruct& Packet)
{
	ShipInfo	RegisterShipInfo;
	
	
	RegisterShipInfo.Clear();
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_REGISTER_SHIP_REGISTER_SHIP_INFO:
			{
				RegisterShipInfo.ParsePacket(Value,DOMF_SHIP_INFO_FOR_REGISTER);
			}
			break;
		
		}
	}
		

	return RegisterShip(RegisterShipInfo);
}
int CSpaceAreaMsgHandler::HandleMsgUnregisterShip(const CSmartStruct& Packet)
{
	
	
	

	return UnregisterShip();
}
int CSpaceAreaMsgHandler::HandleMsgFindShip(const CSmartStruct& Packet)
{
	
	
	

	return FindShip();
}
int CSpaceAreaMsgHandler::HandleMsgRegisterShipyard(const CSmartStruct& Packet)
{
	
	
	

	return RegisterShipyard();
}
