#pragma once



//
struct POS
{
	int		x;
	int		y;
	
	
	enum POS_MEMBER_IDS
	{
		SST_TEST_STRUCT_X = 1,
		SST_TEST_STRUCT_Y = 2,
		
	};
	
	
	POS()
	{
		Clear();
	}
	POS(const POS& Value)
	{
		*this=Value;
	}
	void Clear()
	{
		x=0;
		y=0;
		
	}
	bool MakePacket(CSmartStruct& Packet) const
	{
		UINT FailCount=0;

		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_TEST_STRUCT_X,x),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_TEST_STRUCT_Y,y),FailCount);
		}
		
		

		return FailCount==0;
	}
	bool ParsePacket(const CSmartStruct& Packet)
	{
		
		void * Pos=Packet.GetFirstMemberPosition();
		while(Pos)
		{
			WORD MemberID;
			CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
			switch(MemberID)
			{
			case SST_TEST_STRUCT_X:
				{
					x=Value;
				}
				break;
			case SST_TEST_STRUCT_Y:
				{
					y=Value;
				}
				break;
			
			}
		}
		
		return true;
	}
	UINT GetSmartStructSize() const
	{
		UINT _PackSize = 0;
		_PackSize += CSmartStruct::GetFixMemberSize(sizeof(int));
		
		_PackSize += CSmartStruct::GetFixMemberSize(sizeof(int));
		
		
		return _PackSize;
	}
	void CloneFrom(const POS& Value)
	{
		{
			x=Value.x;
		}
		{
			y=Value.y;
		}
		
	}
	POS& operator=(const POS& Value)
	{
		CloneFrom(Value);
		return *this;
	}
	
	
	
	
	
	

};




