#pragma once


class CShipMsgCaller :
	public IShip
{
protected:
	IDistributedObjectOperator *	m_pOperator;
	OBJECT_ID						m_SenderID;
	CEasyArray<OBJECT_ID>			m_TargetIDs;
public:
	CShipMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID TargetID);
	CShipMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,const OBJECT_ID * pTargetIDs,ID_LIST_COUNT_TYPE TargetIDCount);
	~CShipMsgCaller(void);
	
	
	
	/*************************************************
	函数名:	Attack
	用途:	
	参数:
		Damage				
	返回值:无特别意义
	*************************************************/
	virtual int Attack(int Damage );
		
	
	static bool PackMsgAttack(CSmartStruct& Packet,int Damage );
	
};
