#pragma once

//<GenerateArea1Start>
//
//<GenerateArea1End>

class ShipInfo
{
protected:
	UINT64	m_ModifyFlag;
	
//<GenerateArea2Start>	   
	OBJECT_ID		m_ShipID;
	SHIP_TYPE		m_ShipType;;
	CEasyString		m_Name;
	POS				m_Pos;
	UINT			m_HP;
	UINT			m_Atk;
	
//<GenerateArea2End>
	
public:
//<GenerateArea3Start>
	
	enum SHIP_INFO_MEMBER_IDS
	{
		SST_SI_SHIP_ID = 101,
		SST_SI_SHIP_TYPE = 106,
		SST_SI_NAME = 102,
		SST_SI_POS = 103,
		SST_SI_HP = 104,
		SST_SI_ATK = 105,
		
	};
	
	
	
	enum SHIP_INFO_MODIFY_FLAGS:UINT64
	{
		MF_SHIP_ID = (((UINT64)1) << 0),
		MF_SHIP_TYPE = (((UINT64)1) << 1),
		MF_NAME = (((UINT64)1) << 2),
		MF_POS = (((UINT64)1) << 3),
		MF_HP = (((UINT64)1) << 4),
		MF_ATK = (((UINT64)1) << 5),
		
	};
	
	
//<GenerateArea3End>

public:
//<GenerateArea4Start>
	ShipInfo();
	ShipInfo(const ShipInfo& Object)
	{
		*this=Object;
	}
	virtual ~ShipInfo();
	virtual void Clear();
	
	void SetModifyFlag(UINT64 RemoveFlags,UINT64 AddFlags);
	 
	void SetShipID(OBJECT_ID Value);
	void SetShipType(SHIP_TYPE Value);
	void SetName(const CEasyString& Value);
	void SetPos(const POS& Value);
	void SetHP(UINT Value);
	void SetAtk(UINT Value);
	
		   
	OBJECT_ID GetShipID() const;
	SHIP_TYPE GetShipType() const;
	 CEasyString& GetName() ;
	const CEasyString& GetName() const;
	 POS& GetPos() ;
	const POS& GetPos() const;
	UINT GetHP() const;
	UINT GetAtk() const;
	

	
	virtual void GetModifyFlag(DATA_OBJECT_MODIFY_FLAGS& ModifyFlags,int GetType,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	virtual bool IsModified(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	virtual void ClearModifyFlag(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags);
	virtual bool MakeUpdatePacket(CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	virtual bool MakePacket(CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	virtual void ParsePacket(const CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags);
	virtual void CloneFrom(const ShipInfo& DataObject,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags);
	virtual UINT GetSmartStructSize(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	ShipInfo& operator=(const ShipInfo& DataObject);
	
	
	
//<GenerateArea4End>

};


//<GenerateArea5Start>

inline void ShipInfo::SetModifyFlag(UINT64 RemoveFlags,UINT64 AddFlags)
{
	m_ModifyFlag&=~RemoveFlags;
	m_ModifyFlag|=AddFlags;
}

inline void ShipInfo::SetShipID(OBJECT_ID Value)
{
	m_ShipID=Value;
	m_ModifyFlag|=MF_SHIP_ID;
}
inline void ShipInfo::SetShipType(SHIP_TYPE Value)
{
	if(m_ShipType!=Value)
	{
		m_ShipType=Value;
		m_ModifyFlag|=MF_SHIP_TYPE;
	}
}
inline void ShipInfo::SetName(const CEasyString& Value)
{
	m_Name=Value;
	m_ModifyFlag|=MF_NAME;
}
inline void ShipInfo::SetPos(const POS& Value)
{
	m_Pos=Value;
	m_ModifyFlag|=MF_POS;
}
inline void ShipInfo::SetHP(UINT Value)
{
	if(m_HP!=Value)
	{
		m_HP=Value;
		m_ModifyFlag|=MF_HP;
	}
}
inline void ShipInfo::SetAtk(UINT Value)
{
	if(m_Atk!=Value)
	{
		m_Atk=Value;
		m_ModifyFlag|=MF_ATK;
	}
}

		   
inline OBJECT_ID ShipInfo::GetShipID() const
{
	return m_ShipID;
}
inline SHIP_TYPE ShipInfo::GetShipType() const
{
	return m_ShipType;
}
inline  CEasyString& ShipInfo::GetName() 
{
	return m_Name;
}
inline const CEasyString& ShipInfo::GetName() const
{
	return m_Name;
}
inline  POS& ShipInfo::GetPos() 
{
	return m_Pos;
}
inline const POS& ShipInfo::GetPos() const
{
	return m_Pos;
}
inline UINT ShipInfo::GetHP() const
{
	return m_HP;
}
inline UINT ShipInfo::GetAtk() const
{
	return m_Atk;
}


//<GenerateArea5End>

//<GenerateArea6Start>

//<GenerateArea6End>