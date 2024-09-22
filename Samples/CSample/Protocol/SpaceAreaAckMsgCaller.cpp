#include "../pch.h"


CSpaceAreaAckMsgCaller::CSpaceAreaAckMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID TargetID)
{
	m_pOperator=pOperator;
	m_SenderID=SenderID;
	m_TargetIDs.Add(TargetID);
}

CSpaceAreaAckMsgCaller::CSpaceAreaAckMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,const OBJECT_ID * pTargetIDs,ID_LIST_COUNT_TYPE TargetIDCount)
{
	m_pOperator=pOperator;
	m_SenderID=SenderID;
	m_TargetIDs.Resize(TargetIDCount);
	for(UINT i=0;i<TargetIDCount;i++)
	{
		m_TargetIDs[i]=pTargetIDs[i];
	}	
}

CSpaceAreaAckMsgCaller::~CSpaceAreaAckMsgCaller(void)
{
}


int CSpaceAreaAckMsgCaller::RegisterShipAck(short Result )
{
	if(m_pOperator==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT _PackSize=CSmartStruct::GetEmptyStructSize();
	_PackSize += CSmartStruct::GetFixMemberSize(sizeof(short));
	
	

	WORD MsgFlag=0;
		
	CDOSMessagePacket * pMsgPacket=m_pOperator->NewMessagePacket(_PackSize,(UINT)m_TargetIDs.GetCount());
	if(pMsgPacket==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsgPacket->GetMessage().SetMsgID(MAKE_MSG_ID(MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACE_SPACE_AREA,ISpaceArea::METHOD_REGISTER_SHIP,true));
	pMsgPacket->GetMessage().SetSenderID(m_SenderID);
	pMsgPacket->GetMessage().SetDataLength(_PackSize);
	pMsgPacket->GetMessage().SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsgPacket->GetMessage().GetEmptyDataPacket();
	
	if(PackMsgRegisterShipAck(Packet,Result))
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

int CSpaceAreaAckMsgCaller::UnregisterShipAck(short Result )
{
	if(m_pOperator==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT _PackSize=CSmartStruct::GetEmptyStructSize();
	_PackSize += CSmartStruct::GetFixMemberSize(sizeof(short));
	
	

	WORD MsgFlag=0;
		
	CDOSMessagePacket * pMsgPacket=m_pOperator->NewMessagePacket(_PackSize,(UINT)m_TargetIDs.GetCount());
	if(pMsgPacket==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsgPacket->GetMessage().SetMsgID(MAKE_MSG_ID(MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACE_SPACE_AREA,ISpaceArea::METHOD_UNREGISTER_SHIP,true));
	pMsgPacket->GetMessage().SetSenderID(m_SenderID);
	pMsgPacket->GetMessage().SetDataLength(_PackSize);
	pMsgPacket->GetMessage().SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsgPacket->GetMessage().GetEmptyDataPacket();
	
	if(PackMsgUnregisterShipAck(Packet,Result))
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

int CSpaceAreaAckMsgCaller::FindShipAck(short Result ,OBJECT_ID ShipID )
{
	if(m_pOperator==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT _PackSize=CSmartStruct::GetEmptyStructSize();
	_PackSize += CSmartStruct::GetFixMemberSize(sizeof(short));
	
	_PackSize += CSmartStruct::GetFixMemberSize(sizeof(OBJECT_ID));
	
	

	WORD MsgFlag=0;
		
	CDOSMessagePacket * pMsgPacket=m_pOperator->NewMessagePacket(_PackSize,(UINT)m_TargetIDs.GetCount());
	if(pMsgPacket==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsgPacket->GetMessage().SetMsgID(MAKE_MSG_ID(MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACE_SPACE_AREA,ISpaceArea::METHOD_FIND_SHIP,true));
	pMsgPacket->GetMessage().SetSenderID(m_SenderID);
	pMsgPacket->GetMessage().SetDataLength(_PackSize);
	pMsgPacket->GetMessage().SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsgPacket->GetMessage().GetEmptyDataPacket();
	
	if(PackMsgFindShipAck(Packet,Result,ShipID))
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

int CSpaceAreaAckMsgCaller::RegisterShipyardAck(short Result )
{
	if(m_pOperator==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT _PackSize=CSmartStruct::GetEmptyStructSize();
	_PackSize += CSmartStruct::GetFixMemberSize(sizeof(short));
	
	

	WORD MsgFlag=0;
		
	CDOSMessagePacket * pMsgPacket=m_pOperator->NewMessagePacket(_PackSize,(UINT)m_TargetIDs.GetCount());
	if(pMsgPacket==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsgPacket->GetMessage().SetMsgID(MAKE_MSG_ID(MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACE_SPACE_AREA,ISpaceArea::METHOD_REGISTER_SHIPYARD,true));
	pMsgPacket->GetMessage().SetSenderID(m_SenderID);
	pMsgPacket->GetMessage().SetDataLength(_PackSize);
	pMsgPacket->GetMessage().SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsgPacket->GetMessage().GetEmptyDataPacket();
	
	if(PackMsgRegisterShipyardAck(Packet,Result))
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


bool CSpaceAreaAckMsgCaller::PackMsgRegisterShipAck(CSmartStruct& Packet,short Result )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_REGISTER_SHIP_ACK_RESULT,Result),FailCount);
	}
	
	

	return FailCount==0;
}

bool CSpaceAreaAckMsgCaller::PackMsgUnregisterShipAck(CSmartStruct& Packet,short Result )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_UNREGISTER_SHIP_ACK_RESULT,Result),FailCount);
	}
	
	

	return FailCount==0;
}

bool CSpaceAreaAckMsgCaller::PackMsgFindShipAck(CSmartStruct& Packet,short Result ,OBJECT_ID ShipID )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FIND_SHIP_ACK_RESULT,Result),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FIND_SHIP_ACK_SHIP_ID,ShipID.ID),FailCount);
	}
	
	

	return FailCount==0;
}

bool CSpaceAreaAckMsgCaller::PackMsgRegisterShipyardAck(CSmartStruct& Packet,short Result )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_REGISTER_SHIPYARD_ACK_RESULT,Result),FailCount);
	}
	
	

	return FailCount==0;
}


