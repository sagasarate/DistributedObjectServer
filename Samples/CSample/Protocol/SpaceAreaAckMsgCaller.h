#pragma once


class CSpaceAreaAckMsgCaller :
	public ISpaceAreaAck
{
protected:
	IDistributedObjectOperator *	m_pOperator;
	OBJECT_ID						m_SenderID;
	CEasyArray<OBJECT_ID>			m_TargetIDs;
public:
	CSpaceAreaAckMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID TargetID);
	CSpaceAreaAckMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,const OBJECT_ID * pTargetIDs,ID_LIST_COUNT_TYPE TargetIDCount);
	~CSpaceAreaAckMsgCaller(void);
	
	
	
	/*************************************************
	函数名:	RegisterShipAck
	用途:	
	参数:
		Result				
	返回值:无特别意义
	*************************************************/
	virtual int RegisterShipAck(short Result );
		
	
	
	/*************************************************
	函数名:	UnregisterShipAck
	用途:	
	参数:
		Result				
	返回值:无特别意义
	*************************************************/
	virtual int UnregisterShipAck(short Result );
		
	
	
	/*************************************************
	函数名:	FindShipAck
	用途:	
	参数:
		Result				
		ShipID				
	返回值:无特别意义
	*************************************************/
	virtual int FindShipAck(short Result ,OBJECT_ID ShipID );
		
	
	
	/*************************************************
	函数名:	RegisterShipyardAck
	用途:	
	参数:
		Result				
	返回值:无特别意义
	*************************************************/
	virtual int RegisterShipyardAck(short Result );
		
	
	static bool PackMsgRegisterShipAck(CSmartStruct& Packet,short Result );
	static bool PackMsgUnregisterShipAck(CSmartStruct& Packet,short Result );
	static bool PackMsgFindShipAck(CSmartStruct& Packet,short Result ,OBJECT_ID ShipID );
	static bool PackMsgRegisterShipyardAck(CSmartStruct& Packet,short Result );
	
};
