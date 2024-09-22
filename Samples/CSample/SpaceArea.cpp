#include "pch.h"


START_DOS_MSG_MAP(CSpaceArea)
DOS_MSG_MAP(CSpaceAreaMsgHandler)
END_DOS_MSG_MAP(CSpaceArea)

CSpaceArea::CSpaceArea()
{
	INIT_DOS_MSG_MAP;
	m_pOperator = NULL;
	m_ShipCreateIndex = 0;
}
CSpaceArea::~CSpaceArea()
{

}

bool CSpaceArea::Initialize(IDistributedObjectOperator* pOperator)
{
	m_pOperator = pOperator;
	LogDebug("SpaceArea created");

	m_Ships.Create(128, 128, 32);

	m_CreateTimer.SetTimeOut(5000);
	return true;
}
void CSpaceArea::Destory()
{

}
UINT CSpaceArea::AddUseRef()
{
	return CNameObject::AddUseRef();
}
void CSpaceArea::Release()
{
	CNameObject::Release();
}

int CSpaceArea::Update(int ProcessPacketLimit)
{
	int ProcessCount = 0;
	if (m_CreateTimer.IsTimeOut())
	{
		m_CreateTimer.SaveTime();
		if (m_ShipyardList.GetCount() > 0 && m_Ships.GetObjectCount() < 10)
		{
			int Index = GetRand(0, (int)m_ShipyardList.GetCount() - 1);
			OBJECT_ID ShipyardID = m_ShipyardList[Index];
			LogDebug("现有%u,向(%d/%d)0x%llX请求建船", m_Ships.GetObjectCount(), Index, m_ShipyardList.GetCount(), ShipyardID.ID);

			CShipyardMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), ShipyardID);
			SHIP_TYPE Type = (SHIP_TYPE)GetRand((int)SHIP_TYPE::SMALL, (int)SHIP_TYPE::LARGE);
			CEasyString ShipName;
			ShipName.Format("Ship%d", m_ShipCreateIndex++);
			MsgCaller.CreateShip(Type, ShipName);
		}
	}
	return ProcessCount;
}

void CSpaceArea::OnConcernedObjectLost(OBJECT_ID ObjectID)
{
	//目标消失，移除目标ID
	if (m_ShipyardList.RemoveAll(ObjectID) > 0)
		LogDebug("船厂0x%llX已移除", ObjectID.ID);
}

int CSpaceArea::RegisterShip(ShipInfo RegisterShipInfo)
{
	CSpaceAreaAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), m_CurMsgSenderID);
	RegisterShipInfo.SetShipID(m_CurMsgSenderID);
	m_Ships.Insert(RegisterShipInfo.GetShipID(), RegisterShipInfo);
	MsgCaller.RegisterShipAck(MSR_SUCEED);
	LogDebug("ship 0x%llX registered", RegisterShipInfo.GetShipID().ID);

	return COMMON_RESULT_SUCCEED;
}
int CSpaceArea::UnregisterShip()
{
	CSpaceAreaAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), m_CurMsgSenderID);
	if (m_Ships.Delete(m_CurMsgSenderID))
		LogDebug("ship 0x%llX unregistered", m_CurMsgSenderID.ID);
	MsgCaller.UnregisterShipAck(MSR_SUCEED);

	return COMMON_RESULT_SUCCEED;
}
int CSpaceArea::FindShip()
{
	LogDebug("0x%llX请求查找目标", m_CurMsgSenderID.ID);
	CSpaceAreaAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), m_CurMsgSenderID);
	OBJECT_ID ShipID;
	if (m_Ships.GetObjectCount() >= 2)
	{
		int Index = GetRand((UINT)0, m_Ships.GetObjectCount() - 2);
		for (ShipInfo Ship : m_Ships)
		{
			if (Ship.GetShipID() != m_CurMsgSenderID)
			{
				if (Index <= 0)
				{
					ShipID = Ship.GetShipID();
					break;
				}
				else
				{
					Index--;
				}
			}
		}
	}
	if (ShipID.ID)
	{
		MsgCaller.FindShipAck(MSR_SUCEED, ShipID);
	}
	else
	{
		MsgCaller.FindShipAck(MSR_NOT_EXIST, ShipID);
	}
	return COMMON_RESULT_SUCCEED;
}
int CSpaceArea::RegisterShipyard()
{
	if (m_ShipyardList.FindIndex(m_CurMsgSenderID) >= 0)
	{
		LogDebug("船厂0x%llX已经注册过了", m_CurMsgSenderID.ID);
	}
	else
	{
		LogDebug("收到船厂0x%llX的注册", m_CurMsgSenderID.ID);
		m_ShipyardList.Add(m_CurMsgSenderID);
		m_pOperator->AddConcernedObject(m_CurMsgSenderID, true);
	}
	return COMMON_RESULT_SUCCEED;
}