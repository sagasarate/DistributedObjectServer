#pragma once

class CSpaceAreaAckMsgHandler:public ISpaceAreaAck
{

public:
	CSpaceAreaAckMsgHandler(void);
	~CSpaceAreaAckMsgHandler(void);

	void InitMsgMap(CHashMap<MSG_ID_TYPE,DOS_MSG_HANDLE_INFO>& MsgMap);	

	int HandleMsgRegisterShipAck(const CSmartStruct& Packet);
	int HandleMsgUnregisterShipAck(const CSmartStruct& Packet);
	int HandleMsgFindShipAck(const CSmartStruct& Packet);
	int HandleMsgRegisterShipyardAck(const CSmartStruct& Packet);
	

};

