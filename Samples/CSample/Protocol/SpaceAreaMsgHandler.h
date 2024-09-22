#pragma once

class CSpaceAreaMsgHandler:public ISpaceArea
{

public:
	CSpaceAreaMsgHandler(void);
	~CSpaceAreaMsgHandler(void);

	void InitMsgMap(CHashMap<MSG_ID_TYPE,DOS_MSG_HANDLE_INFO>& MsgMap);	

	int HandleMsgRegisterShip(const CSmartStruct& Packet);
	int HandleMsgUnregisterShip(const CSmartStruct& Packet);
	int HandleMsgFindShip(const CSmartStruct& Packet);
	int HandleMsgRegisterShipyard(const CSmartStruct& Packet);
	

};

