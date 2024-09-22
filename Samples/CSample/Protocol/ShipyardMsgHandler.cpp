#include "../pch.h"

CShipyardMsgHandler::CShipyardMsgHandler(void)
{
}

CShipyardMsgHandler::~CShipyardMsgHandler(void)
{
}

void CShipyardMsgHandler::InitMsgMap(CHashMap<MSG_ID_TYPE,DOS_MSG_HANDLE_INFO>& MsgMap)
{
	DOS_MSG_HANDLE_INFO MsgHandleInfo;
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(DOS_MSG_HANDLE_FN)&CShipyardMsgHandler::HandleMsgCreateShip;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACE_SHIPYARD,IShipyard::METHOD_CREATE_SHIP,false),MsgHandleInfo);
	
}

int CShipyardMsgHandler::HandleMsgCreateShip(const CSmartStruct& Packet)
{
	SHIP_TYPE		ShipType;;
	CEasyString		ShipName;
	
	
	ShipType=(SHIP_TYPE)0;
	ShipName.Clear();
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_CREATE_SHIP_SHIP_TYPE:
			{
				ShipType=(SHIP_TYPE)((UINT)Value);
			}
			break;
		case SST_CREATE_SHIP_SHIP_NAME:
			{
				Value.GetString(ShipName);
			}
			break;
		
		}
	}
		

	return CreateShip(ShipType,ShipName);
}
