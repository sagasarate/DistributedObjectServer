#pragma once

class CShipyardAckMsgHandler:public IShipyardAck
{

public:
	CShipyardAckMsgHandler(void);
	~CShipyardAckMsgHandler(void);

	void InitMsgMap(CHashMap<MSG_ID_TYPE,DOS_MSG_HANDLE_INFO>& MsgMap);	

	int HandleMsgCreateShipAck(const CSmartStruct& Packet);
	

};

