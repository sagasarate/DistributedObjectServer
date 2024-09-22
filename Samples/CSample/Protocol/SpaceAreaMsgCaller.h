#pragma once


class CSpaceAreaMsgCaller :
	public ISpaceArea
{
protected:
	IDistributedObjectOperator *	m_pOperator;
	OBJECT_ID						m_SenderID;
	CEasyArray<OBJECT_ID>			m_TargetIDs;
public:
	CSpaceAreaMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID TargetID);
	CSpaceAreaMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,const OBJECT_ID * pTargetIDs,ID_LIST_COUNT_TYPE TargetIDCount);
	~CSpaceAreaMsgCaller(void);
	
	
	
	/*************************************************
	函数名:	RegisterShip
	用途:	
	参数:
		RegisterShipInfo				
	返回值:无特别意义
	*************************************************/
	virtual int RegisterShip(ShipInfo RegisterShipInfo );
		
	
	
	/*************************************************
	函数名:	UnregisterShip
	用途:	
	参数:
		
	返回值:无特别意义
	*************************************************/
	virtual int UnregisterShip();
		
	
	
	/*************************************************
	函数名:	FindShip
	用途:	
	参数:
		
	返回值:无特别意义
	*************************************************/
	virtual int FindShip();
		
	
	
	/*************************************************
	函数名:	RegisterShipyard
	用途:	
	参数:
		
	返回值:无特别意义
	*************************************************/
	virtual int RegisterShipyard();
		
	
	static bool PackMsgRegisterShip(CSmartStruct& Packet,ShipInfo RegisterShipInfo );
	static bool PackMsgUnregisterShip(CSmartStruct& Packet);
	static bool PackMsgFindShip(CSmartStruct& Packet);
	static bool PackMsgRegisterShipyard(CSmartStruct& Packet);
	
};
