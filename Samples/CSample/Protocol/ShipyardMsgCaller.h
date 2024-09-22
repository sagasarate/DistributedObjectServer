#pragma once


class CShipyardMsgCaller :
	public IShipyard
{
protected:
	IDistributedObjectOperator *	m_pOperator;
	OBJECT_ID						m_SenderID;
	CEasyArray<OBJECT_ID>			m_TargetIDs;
public:
	CShipyardMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID TargetID);
	CShipyardMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,const OBJECT_ID * pTargetIDs,ID_LIST_COUNT_TYPE TargetIDCount);
	~CShipyardMsgCaller(void);
	
	
	
	/*************************************************
	函数名:	CreateShip
	用途:	
	参数:
		ShipType				
		ShipName				
	返回值:无特别意义
	*************************************************/
	virtual int CreateShip(SHIP_TYPE ShipType ,CEasyString ShipName );
		
	
	static bool PackMsgCreateShip(CSmartStruct& Packet,SHIP_TYPE ShipType ,CEasyString ShipName );
	
};
