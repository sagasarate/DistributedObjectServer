#pragma once


class CShipyardAckMsgCaller :
	public IShipyardAck
{
protected:
	IDistributedObjectOperator *	m_pOperator;
	OBJECT_ID						m_SenderID;
	CEasyArray<OBJECT_ID>			m_TargetIDs;
public:
	CShipyardAckMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID TargetID);
	CShipyardAckMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,const OBJECT_ID * pTargetIDs,ID_LIST_COUNT_TYPE TargetIDCount);
	~CShipyardAckMsgCaller(void);
	
	
	
	/*************************************************
	函数名:	CreateShipAck
	用途:	
	参数:
		Result				
	返回值:无特别意义
	*************************************************/
	virtual int CreateShipAck(short Result );
		
	
	static bool PackMsgCreateShipAck(CSmartStruct& Packet,short Result );
	
};
