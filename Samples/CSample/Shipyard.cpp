#include "pch.h"

#undef  Log
#undef  LogDebug

#ifdef WIN32
#define Log(_Format, ...)	LogLocal(__FUNCTION__, _Format, ##__VA_ARGS__)
#define LogDebug(_Format, ...)	LogDebugLocal(__FUNCTION__, _Format, ##__VA_ARGS__)
#else
#define Log(_Format, ...)	LogLocal(__PRETTY_FUNCTION__, _Format, ##__VA_ARGS__)
#define LogDebug(_Format, ...)	LogDebugLocal(__PRETTY_FUNCTION__, _Format, ##__VA_ARGS__)
#endif

START_DOS_MSG_MAP(CShipyard)
DOS_MSG_MAP(CShipyardMsgHandler)
END_DOS_MSG_MAP(CShipyard)



CShipyard::CShipyard()
{
	INIT_DOS_MSG_MAP;
	m_pOperator = NULL;
	m_ObjectFindTimerID = 0;
}
CShipyard::~CShipyard()
{

}

bool CShipyard::Initialize(IDistributedObjectOperator* pOperator)
{
	m_pOperator = pOperator;
	LogDebug("造船厂已创建");

	m_ShipList.Create(128, 128, 32);
	m_ObjectFindTimerID = m_pOperator->AddTimer(5000, 0, true);

	return true;
}
void CShipyard::Destory()
{

}
UINT CShipyard::AddUseRef()
{
	return CNameObject::AddUseRef();
}
void CShipyard::Release()
{
	CNameObject::Release();
}

bool CShipyard::DeleteShip(CShip* pShip)
{
	return m_ShipList.DeleteObject(pShip->GetID());
}

void CShipyard::OnObjectReport(OBJECT_ID ObjectID, const void* pObjectInfoData, UINT DataSize)
{
	m_SpaceAreaID = ObjectID;
	LogDebug("已找到空间区域0x%llX", m_SpaceAreaID.ID);
	m_pOperator->DeleteTimer(m_ObjectFindTimerID);
	CSpaceAreaMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), m_SpaceAreaID);
	MsgCaller.RegisterShipyard();
	LogDebug("已向空间区域0x%llX注册船厂", m_SpaceAreaID.ID);

}
void CShipyard::OnTimer(UINT ID, UINT64 Param, bool IsRepeat)
{
	LogDebug("正在查找空间区域");
	m_pOperator->FindObject(OBJECT_TYPE_SPACE_AREA, false);
}

int CShipyard::CreateShip(SHIP_TYPE ShipType, CEasyString ShipName)
{
	LogDebug("收到建船请求：Type = %d,Name = %s", ShipType, (LPCTSTR)ShipName);
	CShipyardAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), m_CurMsgSenderID);
	if (m_SpaceAreaID.ID)
	{
		CShip* pShip = m_ShipList.NewObject();
		if(pShip)
		{
			if(pShip->Init(this, ShipType, ShipName, m_SpaceAreaID))
			{
				DOS_OBJ_REGISTER_INFO Info;
				Info.ObjectID.ObjectTypeID = OBJECT_TYPE_SHIP;
				Info.szObjectTypeName = "Ship";
				Info.ObjectGroupIndex = m_pOperator->GetGroupIndex();
				Info.Flag = DOS_OBJECT_REGISTER_FLAG_SEND_DESTORY_NOTIFY;
				Info.pObject = pShip;
				m_pOperator->RegisterObject(Info);
				MsgCaller.CreateShipAck(MSR_SUCEED);
			}
			else
			{
				Log("Ship初始化失败");
				m_ShipList.DeleteObject(pShip->GetID());
				MsgCaller.CreateShipAck(MSR_FAILED);
			}
		}
		else
		{
			Log("对象池已满%u/%u", m_ShipList.GetObjectCount(), m_ShipList.GetBufferSize());
			MsgCaller.CreateShipAck(MSR_FAILED);
		}
	}
	else
	{
		MsgCaller.CreateShipAck(MSR_FAILED);
	}

	return COMMON_RESULT_SUCCEED;
}