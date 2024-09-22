#pragma once
class CShipyard :
	public IDistributedObject,
	public CNameObject,
	public CShipyardMsgHandler
{
protected:
	DECLARE_DOS_MSG_MAP(CShipyard)

	IDistributedObjectOperator* m_pOperator;
	OBJECT_ID					m_SpaceAreaID;
	UINT						m_ObjectFindTimerID;

	CIDStorage<CShip>			m_ShipList;
public:
	CShipyard();
	~CShipyard();

	virtual bool Initialize(IDistributedObjectOperator* pOperator) override;
	virtual void Destory() override;
	virtual UINT AddUseRef() override;
	virtual void Release() override;

	virtual void OnObjectReport(OBJECT_ID ObjectID, const void* pObjectInfoData, UINT DataSize) override;
	virtual void OnTimer(UINT ID, UINT64 Param, bool IsRepeat) override;

	bool DeleteShip(CShip* pShip);

protected:
	void LogLocal(const char* szFunction, const char* Format, ...);
	void LogDebugLocal(const char* szFunction, const char* Format, ...);

protected:
	virtual int CreateShip(SHIP_TYPE ShipType, CEasyString ShipName) override;
};

inline void CShipyard::LogLocal(const char* szFunction, const char* Format, ...)
{
	va_list vl;

	char Tag[256];
	sprintf_s(Tag, 256, "%s(0x%llX)", szFunction, m_pOperator->GetObjectID().ID);

	va_start(vl, Format);
	CLogManager::GetInstance()->PrintLogVL(g_LogChannel, ILogPrinter::LOG_LEVEL_NORMAL, Tag, Format, vl);
	va_end(vl);
}

inline void CShipyard::LogDebugLocal(const char* szFunction, const char* Format, ...)
{
	va_list vl;

	char Tag[256];
	sprintf_s(Tag, 256, "%s(0x%llX)", szFunction, m_pOperator->GetObjectID().ID);

	va_start(vl, Format);
	CLogManager::GetInstance()->PrintLogVL(g_LogChannel, ILogPrinter::LOG_LEVEL_DEBUG, Tag, Format, vl);
	va_end(vl);
}