#include "../pch.h"



ShipInfo::ShipInfo()
{
	Clear();
}
ShipInfo::~ShipInfo()
{
}

void ShipInfo::Clear()
{
//<GenerateArea1Start>
	m_ModifyFlag=0;
	   
	m_ShipID.ID=0;
	m_ShipType=(SHIP_TYPE)0;
	m_Name.Clear();
	m_Pos.Clear();
	m_HP=0;
	m_Atk=0;
	
//<GenerateArea1End>

}

void ShipInfo::GetModifyFlag(DATA_OBJECT_MODIFY_FLAGS& ModifyFlags,int GetType,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const
{
//<GenerateArea2Start>
	switch(GetType)
	{
	case GMFT_COPY:
		ModifyFlags[DATA_OBJECT_FLAG_SHIP_INFO]=m_ModifyFlag&MemberFlags[DATA_OBJECT_FLAG_SHIP_INFO];
		break;
	case GMFT_AND:
		ModifyFlags[DATA_OBJECT_FLAG_SHIP_INFO]&=m_ModifyFlag&MemberFlags[DATA_OBJECT_FLAG_SHIP_INFO];
		break;
	case GMFT_OR:
		ModifyFlags[DATA_OBJECT_FLAG_SHIP_INFO]|=m_ModifyFlag&MemberFlags[DATA_OBJECT_FLAG_SHIP_INFO];
		break;
	}
	
//<GenerateArea2End>
}

bool ShipInfo::IsModified(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const
{
	bool IsModified=false;
//<GenerateArea3Start>	
	
	IsModified=IsModified||(m_ModifyFlag&MemberFlags[DATA_OBJECT_FLAG_SHIP_INFO])!=0;
	
	IsModified=IsModified
		;
	
//<GenerateArea3End>
	return IsModified;

}

void ShipInfo::ClearModifyFlag(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags)
{
//<GenerateArea4Start>
	m_ModifyFlag&=~MemberFlags[DATA_OBJECT_FLAG_SHIP_INFO];
	
//<GenerateArea4End>
}

bool ShipInfo::MakeUpdatePacket(CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const
{
//<GenerateArea5Start>
	DATA_OBJECT_MODIFY_FLAGS ModifyFlags;
	ZeroMemory(&ModifyFlags,sizeof(ModifyFlags));
	GetModifyFlag(ModifyFlags,GMFT_OR,MemberFlags);	
//<GenerateArea5End>
	return MakePacket(Packet,ModifyFlags);
}

bool ShipInfo::MakePacket(CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const
{
//<GenerateArea6Start>
	UINT FailCount=0;
	
	UINT64 Flag=MemberFlags[DATA_OBJECT_FLAG_SHIP_INFO];

	if(Flag&MF_SHIP_ID)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SI_SHIP_ID,m_ShipID.ID),FailCount);
	}
	
	if(Flag&MF_SHIP_TYPE)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SI_SHIP_TYPE,(UINT)m_ShipType),FailCount);
	}
	
	if(Flag&MF_NAME)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SI_NAME,m_Name),FailCount);
	}
	
	if(Flag&MF_POS)
	{
		CSmartStruct SubPacket=Packet.PrepareSubStruct();
		if(!m_Pos.MakePacket(SubPacket))
			FailCount++;
		if(!Packet.FinishMember(SST_SI_POS,SubPacket.GetDataLen()))
			FailCount++;
	}
	
	if(Flag&MF_HP)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SI_HP,m_HP),FailCount);
	}
	
	if(Flag&MF_ATK)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SI_ATK,m_Atk),FailCount);
	}
	
	
//<GenerateArea6End>
	return FailCount==0;

}

void ShipInfo::ParsePacket(const CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags)
{
//<GenerateArea7Start>
	
	UINT64 Flag=MemberFlags[DATA_OBJECT_FLAG_SHIP_INFO];
	UINT64 UpdateFlag=0;
	
	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_SI_SHIP_ID:
			if(Flag&MF_SHIP_ID)
			{
				m_ShipID.ID=Value;
				UpdateFlag |= MF_SHIP_ID;
			}
			break;
		case SST_SI_SHIP_TYPE:
			if(Flag&MF_SHIP_TYPE)
			{
				m_ShipType=(SHIP_TYPE)((UINT)Value);
				UpdateFlag |= MF_SHIP_TYPE;
			}
			break;
		case SST_SI_NAME:
			if(Flag&MF_NAME)
			{
				Value.GetString(m_Name);
				UpdateFlag |= MF_NAME;
			}
			break;
		case SST_SI_POS:
			if(Flag&MF_POS)
			{
				m_Pos.ParsePacket(Value);
				
				UpdateFlag |= MF_POS;
			}
			break;
		case SST_SI_HP:
			if(Flag&MF_HP)
			{
				m_HP=Value;
				UpdateFlag |= MF_HP;
			}
			break;
		case SST_SI_ATK:
			if(Flag&MF_ATK)
			{
				m_Atk=Value;
				UpdateFlag |= MF_ATK;
			}
			break;
		
		}
	}
	
//<GenerateArea7End>
	
	m_ModifyFlag|=UpdateFlag;	

}

void ShipInfo::CloneFrom(const ShipInfo& DataObject,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags)
{
//<GenerateArea8Start>
	
	UINT64 Flag=MemberFlags[DATA_OBJECT_FLAG_SHIP_INFO];
	UINT64 UpdateFlag=0;
	
	if(Flag&MF_SHIP_ID)
	{
		m_ShipID=DataObject.m_ShipID;
		UpdateFlag|=MF_SHIP_ID;
	}
	if(Flag&MF_SHIP_TYPE)
	{
		m_ShipType=DataObject.m_ShipType;
		UpdateFlag|=MF_SHIP_TYPE;
	}
	if(Flag&MF_NAME)
	{
		m_Name=DataObject.m_Name;
		UpdateFlag|=MF_NAME;
	}
	if(Flag&MF_POS)
	{
		m_Pos=DataObject.m_Pos;
		UpdateFlag|=MF_POS;
	}
	if(Flag&MF_HP)
	{
		m_HP=DataObject.m_HP;
		UpdateFlag|=MF_HP;
	}
	if(Flag&MF_ATK)
	{
		m_Atk=DataObject.m_Atk;
		UpdateFlag|=MF_ATK;
	}
	
	
//<GenerateArea8End>

	m_ModifyFlag|=UpdateFlag;
}

UINT ShipInfo::GetSmartStructSize(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const
{
//<GenerateArea9Start>
	UINT64 Flag=MemberFlags[DATA_OBJECT_FLAG_SHIP_INFO];
	UINT _PackSize=0;
	if(Flag&MF_SHIP_ID)
	{
		_PackSize += CSmartStruct::GetFixMemberSize(sizeof(OBJECT_ID));
	}
	if(Flag&MF_SHIP_TYPE)
	{
		_PackSize += CSmartStruct::GetFixMemberSize(sizeof(UINT));
	}
	if(Flag&MF_NAME)
	{
		_PackSize += CSmartStruct::GetStringMemberSize(m_Name);
	}
	if(Flag&MF_POS)
	{
		_PackSize += CSmartStruct::GetStructMemberSize(m_Pos.GetSmartStructSize());
	}
	if(Flag&MF_HP)
	{
		_PackSize += CSmartStruct::GetFixMemberSize(sizeof(UINT));
	}
	if(Flag&MF_ATK)
	{
		_PackSize += CSmartStruct::GetFixMemberSize(sizeof(UINT));
	}
	
		
//<GenerateArea9End>	
	return _PackSize;

}

ShipInfo& ShipInfo::operator=(const ShipInfo& DataObject)
{
//<GenerateArea10Start>
	CloneFrom(DataObject,DOMF_SHIP_INFO_FULL);
//<GenerateArea10End>	
	return *this;
}

//<GenerateArea11Start>

//<GenerateArea11End>

//<GenerateArea12Start>

//<GenerateArea12End>

