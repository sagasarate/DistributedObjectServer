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

START_DOS_MSG_MAP(CShip)
DOS_MSG_MAP(CShipMsgHandler)
DOS_MSG_MAP(CSpaceAreaAckMsgHandler)
END_DOS_MSG_MAP(CShip)

CShip::CShip()
{
	INIT_DOS_MSG_MAP;
	m_pOperator = NULL;
	m_pParent = NULL;
	m_AttackTimerID = 0;
}
CShip::~CShip()
{

}

bool CShip::Init(CShipyard* pParent, SHIP_TYPE Type, LPCTSTR Name, OBJECT_ID SpaceAreaID)
{
	m_pParent = pParent;
	m_SpaceAreaID = SpaceAreaID;
	m_ShipInfo.SetShipType(Type);
	m_ShipInfo.SetName(Name);
	m_AttackTimerID = 0;
	m_TargetID.ID = 0;
	return true;
}

bool CShip::Initialize(IDistributedObjectOperator* pOperator)
{
	m_pOperator = pOperator;
	m_ShipInfo.SetShipID(m_pOperator->GetObjectID());
	LogDebug("已创建:Type=%d Name=%s SpaceAreaID=0x%llX", m_ShipInfo.GetShipType(), (LPCTSTR)m_ShipInfo.GetName(), m_SpaceAreaID.ID);

	switch (m_ShipInfo.GetShipType())
	{
	case SHIP_TYPE::SMALL:
		m_ShipInfo.SetHP(100);
		m_ShipInfo.SetAtk(10);
		break;
	case SHIP_TYPE::MEDIUM:
		m_ShipInfo.SetHP(200);
		m_ShipInfo.SetAtk(20);
		break;
	case SHIP_TYPE::LARGE:
		m_ShipInfo.SetHP(300);
		m_ShipInfo.SetAtk(10);
		break;
	}

	CSpaceAreaMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), m_SpaceAreaID);
	MsgCaller.RegisterShip(m_ShipInfo);

	return true;
}
void CShip::Destory()
{
	if (m_AttackTimerID != 0)
	{
		m_pOperator->DeleteTimer(m_AttackTimerID);
		m_AttackTimerID = 0;
	}
	CSpaceAreaMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), m_SpaceAreaID);
	MsgCaller.UnregisterShip();
}
UINT CShip::AddUseRef()
{
	return CNameObject::AddUseRef();
}
void CShip::Release()
{
	CNameObject::Release();
}

void CShip::OnConcernedObjectLost(OBJECT_ID ObjectID)
{
	//目标消失，置空目标ID
	LogDebug("对象0x%llX已消失", ObjectID.ID);
	if (ObjectID == m_TargetID)
		m_TargetID.ID = 0;
}
void CShip::OnTimer(UINT ID, UINT64 Param, bool IsRepeat)
{
	if (m_TargetID.ID != 0)
	{
		//有目标，发起攻击
		CShipMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), m_TargetID);
		MsgCaller.Attack((int)m_ShipInfo.GetAtk());
		LogDebug("向0x%llX发起攻击%u", m_TargetID.ID, m_ShipInfo.GetAtk());
	}
	else
	{
		//没有目标，寻找一个目标
		CSpaceAreaMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), m_SpaceAreaID);
		MsgCaller.FindShip();
	}
}

int CShip::Attack(int Damage)
{
	//受到攻击
	LogDebug("受到0x%llX的攻击%u", m_CurMsgSenderID.ID, Damage);
	if ((int)m_ShipInfo.GetHP() > Damage)
	{
		m_ShipInfo.SetHP((UINT)(m_ShipInfo.GetHP() - Damage));
		LogDebug("HP剩余%u", m_ShipInfo.GetHP());
	}
	else
	{
		//HP归零，销毁自身
		LogDebug("HP归零，销毁");
		m_ShipInfo.SetHP(0);
		m_pOperator->Release();
	}
	return COMMON_RESULT_SUCCEED;
}

int CShip::RegisterShipAck(short Result)
{
	if (Result == MSR_SUCEED)
	{
		//注册成功，创建行动定时器
		m_AttackTimerID = m_pOperator->AddTimer(3000, 0, true);
	}
	else
	{
		m_pOperator->Release();
	}
	return COMMON_RESULT_SUCCEED;
}
int CShip::FindShipAck(short Result, OBJECT_ID ShipID)
{
	if (Result == MSR_SUCEED)
	{
		//找到目标，设置目标ID
		LogDebug("找到目标：0x%llX", ShipID.ID);
		m_TargetID = ShipID;
		m_pOperator->AddConcernedObject(m_TargetID, false);
	}
	return COMMON_RESULT_SUCCEED;
}