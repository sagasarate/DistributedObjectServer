#include "../pch.h"


CShipMsgCaller::CShipMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID TargetID)
{
	m_pOperator=pOperator;
	m_SenderID=SenderID;
	m_TargetIDs.Add(TargetID);
}

CShipMsgCaller::CShipMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,const OBJECT_ID * pTargetIDs,ID_LIST_COUNT_TYPE TargetIDCount)
{
	m_pOperator=pOperator;
	m_SenderID=SenderID;
	m_TargetIDs.Resize(TargetIDCount);
	for(UINT i=0;i<TargetIDCount;i++)
	{
		m_TargetIDs[i]=pTargetIDs[i];
	}	
}

CShipMsgCaller::~CShipMsgCaller(void)
{
}


int CShipMsgCaller::Attack(int Damage )
{
	if(m_pOperator==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT _PackSize=CSmartStruct::GetEmptyStructSize();
	_PackSize += CSmartStruct::GetFixMemberSize(sizeof(int));
	
	

	WORD MsgFlag=0;
		
	CDOSMessagePacket * pMsgPacket=m_pOperator->NewMessagePacket(_PackSize,(UINT)m_TargetIDs.GetCount());
	if(pMsgPacket==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsgPacket->GetMessage().SetMsgID(MAKE_MSG_ID(MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACE_SHIP,IShip::METHOD_ATTACK,false));
	pMsgPacket->GetMessage().SetSenderID(m_SenderID);
	pMsgPacket->GetMessage().SetDataLength(_PackSize);
	pMsgPacket->GetMessage().SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsgPacket->GetMessage().GetEmptyDataPacket();
	
	if(PackMsgAttack(Packet,Damage))
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


bool CShipMsgCaller::PackMsgAttack(CSmartStruct& Packet,int Damage )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_ATTACK_DAMAGE,Damage),FailCount);
	}
	
	

	return FailCount==0;
}


