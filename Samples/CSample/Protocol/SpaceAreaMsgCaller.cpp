#include "../pch.h"


CSpaceAreaMsgCaller::CSpaceAreaMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID TargetID)
{
	m_pOperator=pOperator;
	m_SenderID=SenderID;
	m_TargetIDs.Add(TargetID);
}

CSpaceAreaMsgCaller::CSpaceAreaMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,const OBJECT_ID * pTargetIDs,ID_LIST_COUNT_TYPE TargetIDCount)
{
	m_pOperator=pOperator;
	m_SenderID=SenderID;
	m_TargetIDs.Resize(TargetIDCount);
	for(UINT i=0;i<TargetIDCount;i++)
	{
		m_TargetIDs[i]=pTargetIDs[i];
	}	
}

CSpaceAreaMsgCaller::~CSpaceAreaMsgCaller(void)
{
}


int CSpaceAreaMsgCaller::RegisterShip(ShipInfo RegisterShipInfo )
{
	if(m_pOperator==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT _PackSize=CSmartStruct::GetEmptyStructSize();
	_PackSize += CSmartStruct::GetStructMemberSize(RegisterShipInfo.GetSmartStructSize(DOMF_SHIP_INFO_FOR_REGISTER));
	
	

	WORD MsgFlag=0;
		
	CDOSMessagePacket * pMsgPacket=m_pOperator->NewMessagePacket(_PackSize,(UINT)m_TargetIDs.GetCount());
	if(pMsgPacket==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsgPacket->GetMessage().SetMsgID(MAKE_MSG_ID(MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACE_SPACE_AREA,ISpaceArea::METHOD_REGISTER_SHIP,false));
	pMsgPacket->GetMessage().SetSenderID(m_SenderID);
	pMsgPacket->GetMessage().SetDataLength(_PackSize);
	pMsgPacket->GetMessage().SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsgPacket->GetMessage().GetEmptyDataPacket();
	
	if(PackMsgRegisterShip(Packet,RegisterShipInfo))
	{	
		pMsgPacket->SetTargetIDs((ID_LIST_COUNT_TYPE)m_TargetIDs.GetCount(),m_TargetIDs.GetBuffer());
		if(m_pOperator->SendMessagePacket(pMsgPacket))
		{
			m_pOperator->ReleaseMessagePacket(pMsgPacket);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pOperator->ReleaseMessagePacket(pMsgPacket);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pOperator->ReleaseMessagePacket(pMsgPacket);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CSpaceAreaMsgCaller::UnregisterShip()
{
	if(m_pOperator==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT _PackSize=CSmartStruct::GetEmptyStructSize();
	

	WORD MsgFlag=0;
		
	CDOSMessagePacket * pMsgPacket=m_pOperator->NewMessagePacket(_PackSize,(UINT)m_TargetIDs.GetCount());
	if(pMsgPacket==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsgPacket->GetMessage().SetMsgID(MAKE_MSG_ID(MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACE_SPACE_AREA,ISpaceArea::METHOD_UNREGISTER_SHIP,false));
	pMsgPacket->GetMessage().SetSenderID(m_SenderID);
	pMsgPacket->GetMessage().SetDataLength(_PackSize);
	pMsgPacket->GetMessage().SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsgPacket->GetMessage().GetEmptyDataPacket();
	
	if(PackMsgUnregisterShip(Packet))
	{	
		pMsgPacket->SetTargetIDs((ID_LIST_COUNT_TYPE)m_TargetIDs.GetCount(),m_TargetIDs.GetBuffer());
		if(m_pOperator->SendMessagePacket(pMsgPacket))
		{
			m_pOperator->ReleaseMessagePacket(pMsgPacket);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pOperator->ReleaseMessagePacket(pMsgPacket);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pOperator->ReleaseMessagePacket(pMsgPacket);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CSpaceAreaMsgCaller::FindShip()
{
	if(m_pOperator==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT _PackSize=CSmartStruct::GetEmptyStructSize();
	

	WORD MsgFlag=0;
		
	CDOSMessagePacket * pMsgPacket=m_pOperator->NewMessagePacket(_PackSize,(UINT)m_TargetIDs.GetCount());
	if(pMsgPacket==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsgPacket->GetMessage().SetMsgID(MAKE_MSG_ID(MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACE_SPACE_AREA,ISpaceArea::METHOD_FIND_SHIP,false));
	pMsgPacket->GetMessage().SetSenderID(m_SenderID);
	pMsgPacket->GetMessage().SetDataLength(_PackSize);
	pMsgPacket->GetMessage().SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsgPacket->GetMessage().GetEmptyDataPacket();
	
	if(PackMsgFindShip(Packet))
	{	
		pMsgPacket->SetTargetIDs((ID_LIST_COUNT_TYPE)m_TargetIDs.GetCount(),m_TargetIDs.GetBuffer());
		if(m_pOperator->SendMessagePacket(pMsgPacket))
		{
			m_pOperator->ReleaseMessagePacket(pMsgPacket);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pOperator->ReleaseMessagePacket(pMsgPacket);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pOperator->ReleaseMessagePacket(pMsgPacket);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CSpaceAreaMsgCaller::RegisterShipyard()
{
	if(m_pOperator==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT _PackSize=CSmartStruct::GetEmptyStructSize();
	

	WORD MsgFlag=0;
		
	CDOSMessagePacket * pMsgPacket=m_pOperator->NewMessagePacket(_PackSize,(UINT)m_TargetIDs.GetCount());
	if(pMsgPacket==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsgPacket->GetMessage().SetMsgID(MAKE_MSG_ID(MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACE_SPACE_AREA,ISpaceArea::METHOD_REGISTER_SHIPYARD,false));
	pMsgPacket->GetMessage().SetSenderID(m_SenderID);
	pMsgPacket->GetMessage().SetDataLength(_PackSize);
	pMsgPacket->GetMessage().SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsgPacket->GetMessage().GetEmptyDataPacket();
	
	if(PackMsgRegisterShipyard(Packet))
	{	
		pMsgPacket->SetTargetIDs((ID_LIST_COUNT_TYPE)m_TargetIDs.GetCount(),m_TargetIDs.GetBuffer());
		if(m_pOperator->SendMessagePacket(pMsgPacket))
		{
			m_pOperator->ReleaseMessagePacket(pMsgPacket);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pOperator->ReleaseMessagePacket(pMsgPacket);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pOperator->ReleaseMessagePacket(pMsgPacket);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}


bool CSpaceAreaMsgCaller::PackMsgRegisterShip(CSmartStruct& Packet,ShipInfo RegisterShipInfo )
{
	UINT FailCount=0;

	{
		CSmartStruct SubPacket=Packet.PrepareSubStruct();
		if(!RegisterShipInfo.MakePacket(SubPacket,DOMF_SHIP_INFO_FOR_REGISTER))
			FailCount++;
		if(!Packet.FinishMember(SST_REGISTER_SHIP_REGISTER_SHIP_INFO,SubPacket.GetDataLen()))
			FailCount++;
	}
	
	

	return FailCount==0;
}

bool CSpaceAreaMsgCaller::PackMsgUnregisterShip(CSmartStruct& Packet)
{
	UINT FailCount=0;

	

	return FailCount==0;
}

bool CSpaceAreaMsgCaller::PackMsgFindShip(CSmartStruct& Packet)
{
	UINT FailCount=0;

	

	return FailCount==0;
}

bool CSpaceAreaMsgCaller::PackMsgRegisterShipyard(CSmartStruct& Packet)
{
	UINT FailCount=0;

	

	return FailCount==0;
}


