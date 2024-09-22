#pragma once

class CShipAckMsgHandler:public IShipAck
{

public:
	CShipAckMsgHandler(void);
	~CShipAckMsgHandler(void);

	void InitMsgMap(CHashMap<MSG_ID_TYPE,DOS_MSG_HANDLE_INFO>& MsgMap);	

	

};

