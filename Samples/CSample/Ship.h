#pragma once

class CShipyard;

class CShip:
	public IDistributedObject,
	public CNameObject,
	public CShipMsgHandler,
	public CSpaceAreaAckMsgHandler
{
protected:
	DECLARE_DOS_MSG_MAP(CShip)

	IDistributedObjectOperator* m_pOperator;
	CShipyard*					m_pParent;
	OBJECT_ID					m_SpaceAreaID;
	OBJECT_ID					m_TargetID;
	UINT						m_AttackTimerID;

	ShipInfo					m_ShipInfo;
public:
	CShip();
	~CShip();

	bool Init(CShipyard* pParent, SHIP_TYPE Type, LPCTSTR Name, OBJECT_ID SpaceAreaID);

	virtual bool Initialize(IDistributedObjectOperator* pOperator) override;
	virtual void Destory() override;
	virtual UINT AddUseRef() override;
	virtual void Release() override;

	virtual void OnConcernedObjectLost(OBJECT_ID ObjectID) override;
	virtual void OnTimer(UINT ID, UINT64 Param, bool IsRepeat) override;

protected:
	void LogLocal(const char* szFunction, const char* Format, ...);
	void LogDebugLocal(const char* szFunction, const char* Format, ...);

protected:
	virtual int Attack(int Damage) override;
protected:
	virtual int RegisterShipAck(short Result) override;
	virtual int FindShipAck(short Result, OBJECT_ID ShipID) override;
};

inline void CShip::LogLocal(const char* szFunction, const char* Format, ...)
{
	va_list vl;

	char Tag[256];
	sprintf_s(Tag, 256, "%s(0x%llX)", szFunction, m_pOperator->GetObjectID().ID);

	va_start(vl, Format);
	CLogManager::GetInstance()->PrintLogVL(g_LogChannel, ILogPrinter::LOG_LEVEL_NORMAL, Tag, Format, vl);
	va_end(vl);
}

inline void CShip::LogDebugLocal(const char* szFunction, const char* Format, ...)
{
	va_list vl;

	char Tag[256];
	sprintf_s(Tag, 256, "%s(0x%llX)", szFunction, m_pOperator->GetObjectID().ID);

	va_start(vl, Format);
	CLogManager::GetInstance()->PrintLogVL(g_LogChannel, ILogPrinter::LOG_LEVEL_DEBUG, Tag, Format, vl);
	va_end(vl);
}