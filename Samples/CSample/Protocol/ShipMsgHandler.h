#pragma once

class CShipMsgHandler:public IShip
{

public:
	CShipMsgHandler(void);
	~CShipMsgHandler(void);

	void InitMsgMap(CHashMap<MSG_ID_TYPE,DOS_MSG_HANDLE_INFO>& MsgMap);	

	int HandleMsgAttack(const CSmartStruct& Packet);
	

};

