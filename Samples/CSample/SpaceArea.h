#pragma once
class CSpaceArea :
	public IDistributedObject,
	public CNameObject,
	public CSpaceAreaMsgHandler
{
protected:
	DECLARE_DOS_MSG_MAP(CSpaceArea)

	IDistributedObjectOperator*			m_pOperator;

	CEasyArray<OBJECT_ID>				m_ShipyardList;
	CStaticMap<OBJECT_ID, ShipInfo>		m_Ships;
	int									m_ShipCreateIndex;
	CEasyTimer							m_CreateTimer;
public:
	CSpaceArea();
	~CSpaceArea();

	virtual bool Initialize(IDistributedObjectOperator* pOperator) override;
	virtual void Destory() override;
	virtual UINT AddUseRef() override;
	virtual void Release() override;

	virtual int Update(int ProcessPacketLimit) override;
	virtual void OnConcernedObjectLost(OBJECT_ID ObjectID) override;

protected:
	virtual int RegisterShip(ShipInfo RegisterShipInfo) override;
	virtual int UnregisterShip() override;
	virtual int FindShip() override;
	virtual int RegisterShipyard() override;
};

