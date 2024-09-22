#pragma once

class CShipyardMsgHandler:public IShipyard
{

public:
	CShipyardMsgHandler(void);
	~CShipyardMsgHandler(void);

	void InitMsgMap(CHashMap<MSG_ID_TYPE,DOS_MSG_HANDLE_INFO>& MsgMap);	

	int HandleMsgCreateShip(const CSmartStruct& Packet);
	

};

