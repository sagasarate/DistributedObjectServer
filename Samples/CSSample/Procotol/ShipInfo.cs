using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using Newtonsoft.Json.Linq;
using DOSSystem;
using DOSSample;

namespace DOSSample
{
	//<GenerateArea1Start>
	//
	//<GenerateArea1End>

	public class ShipInfo

	{

		protected  ulong	m_ModifyFlag=0;
	
	//<GenerateArea2Start>	   
		protected OBJECT_ID		m_ShipID;
		protected SHIP_TYPE		m_ShipType;
		protected string		m_Name=string.Empty;
		protected POS			m_Pos=new POS();
		protected uint			m_HP;
		protected uint			m_Atk;
		
	//<GenerateArea2End>
	

	//<GenerateArea3Start>
		
		public enum SHIP_INFO_MEMBER_IDS
		{
			SST_SI_SHIP_ID = 101,
			SST_SI_SHIP_TYPE = 106,
			SST_SI_NAME = 102,
			SST_SI_POS = 103,
			SST_SI_HP = 104,
			SST_SI_ATK = 105,
			
		};
		
		public class SHIP_INFO_MEMBER_IDS_UTILS
		{
			public const int ENUM_COUNT = 6;
		};
		
		
		public enum SHIP_INFO_MODIFY_FLAGS : long
		{
			MF_SHIP_ID = (((long)1) << 0),
			MF_SHIP_TYPE = (((long)1) << 1),
			MF_NAME = (((long)1) << 2),
			MF_POS = (((long)1) << 3),
			MF_HP = (((long)1) << 4),
			MF_ATK = (((long)1) << 5),
			
		};
		
		public class SHIP_INFO_MODIFY_FLAGS_UTILS
		{
			public const int ENUM_COUNT = 6;
		};
	//<GenerateArea3End>


		public ShipInfo()
		{
			Clear();
		}
	
		public virtual void Clear()
		{
	//<GenerateArea4Start>
			m_ModifyFlag=0;
	   
			m_ShipID.ID=0;
			m_ShipType=0;
			m_Name=string.Empty;
			m_Pos.Clear();
			m_HP=0;
			m_Atk=0;
			
	//<GenerateArea4End>

		}
	 
	//<GenerateArea5Start>

		public  void SetModifyFlag(ulong RemoveFlags,ulong AddFlags)
		{
			m_ModifyFlag&=~RemoveFlags;
			m_ModifyFlag|=AddFlags;
		}


		public void SetShipID(OBJECT_ID Value)
		{
			m_ShipID=Value;
			m_ModifyFlag|=(ulong)SHIP_INFO_MODIFY_FLAGS.MF_SHIP_ID;
		}
		public void SetShipType(SHIP_TYPE Value)
		{
			m_ShipType=Value;
			m_ModifyFlag|=(ulong)SHIP_INFO_MODIFY_FLAGS.MF_SHIP_TYPE;
		}
		public void SetName(string Value)
		{
			m_Name=Value;
			m_ModifyFlag|=(ulong)SHIP_INFO_MODIFY_FLAGS.MF_NAME;
		}
		public void SetPos(POS Value)
		{
			m_Pos=(POS)Value.Clone();
			m_ModifyFlag|=(ulong)SHIP_INFO_MODIFY_FLAGS.MF_POS;
		}
		public void SetHP(uint Value)
		{
			m_HP=Value;
			m_ModifyFlag|=(ulong)SHIP_INFO_MODIFY_FLAGS.MF_HP;
		}
		public void SetAtk(uint Value)
		{
			m_Atk=Value;
			m_ModifyFlag|=(ulong)SHIP_INFO_MODIFY_FLAGS.MF_ATK;
		}
		
			   
		public OBJECT_ID GetShipID()
		{
			return m_ShipID;
		}
		public SHIP_TYPE GetShipType()
		{
			return m_ShipType;
		}
		public string GetName()
		{
			return m_Name;
		}
		public POS GetPos()
		{
			return m_Pos;
		}
		public uint GetHP()
		{
			return m_HP;
		}
		public uint GetAtk()
		{
			return m_Atk;
		}
		

	//<GenerateArea5End>

		public virtual void GetModifyFlag(ulong[] ModifyFlags,int GetType,ulong[] MemberFlags)
		{
	//<GenerateArea6Start>
			switch(GetType)
			{
			case (int)GET_MODIFY_FLAG_TYPE.GMFT_COPY:
				ModifyFlags[(int)DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_SHIP_INFO]=m_ModifyFlag&MemberFlags[(int)DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_SHIP_INFO];
				break;
			case (int)GET_MODIFY_FLAG_TYPE.GMFT_AND:
				ModifyFlags[(int)DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_SHIP_INFO]&=m_ModifyFlag&MemberFlags[(int)DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_SHIP_INFO];
				break;
			case (int)GET_MODIFY_FLAG_TYPE.GMFT_OR:
				ModifyFlags[(int)DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_SHIP_INFO]|=m_ModifyFlag&MemberFlags[(int)DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_SHIP_INFO];
				break;
			}
			
	//<GenerateArea6End>
		}
	
		public virtual bool IsModified(ulong[] MemberFlags)
		{
			bool IsModified=false;
	//<GenerateArea7Start>
		
			IsModified=IsModified||(m_ModifyFlag&MemberFlags[(int)DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_SHIP_INFO])!=0;
			IsModified=IsModified
				;
	//<GenerateArea7End>
			return IsModified;

		}
	
		public virtual void ClearModifyFlag(ulong[] MemberFlags)
		{
	//<GenerateArea8Start>
			m_ModifyFlag&=~MemberFlags[(int)DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_SHIP_INFO];
			
	//<GenerateArea8End>
		}

		public virtual bool MakeUpdatePacket(CSmartStruct Packet,ulong[] MemberFlags)
		{
	//<GenerateArea9Start>
			ulong[] ModifyFlags=new ulong[(int)DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_MAX];		
			GetModifyFlag(ModifyFlags,(int)GET_MODIFY_FLAG_TYPE.GMFT_OR,MemberFlags);	
	//<GenerateArea9End>
			return MakePacket(Packet,ModifyFlags);
		}
	
		public virtual bool MakePacket(CSmartStruct Packet,ulong[] MemberFlags)
		{
	//<GenerateArea10Start>
			uint FailCount=0;
		
			ulong Flag=MemberFlags[(int)DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_SHIP_INFO];

			if((Flag&(ulong)SHIP_INFO_MODIFY_FLAGS.MF_SHIP_ID)!=0)
			{
				if(!Packet.AddMember((ushort)SHIP_INFO_MEMBER_IDS.SST_SI_SHIP_ID,m_ShipID.ID)) FailCount++;
			}
			
			if((Flag&(ulong)SHIP_INFO_MODIFY_FLAGS.MF_SHIP_TYPE)!=0)
			{
				if(!Packet.AddMember((ushort)SHIP_INFO_MEMBER_IDS.SST_SI_SHIP_TYPE,(uint)m_ShipType)) FailCount++;
				
			}
			
			if((Flag&(ulong)SHIP_INFO_MODIFY_FLAGS.MF_NAME)!=0)
			{
				if(!Packet.AddMember((ushort)SHIP_INFO_MEMBER_IDS.SST_SI_NAME,m_Name)) FailCount++;
			}
			
			if((Flag&(ulong)SHIP_INFO_MODIFY_FLAGS.MF_POS)!=0)
			{
				CSmartStruct SubPacket=Packet.PrepareSubStruct();
				if(!m_Pos.MakePacket(SubPacket))
					FailCount++;
				Packet.FinishMember((ushort)SHIP_INFO_MEMBER_IDS.SST_SI_POS,SubPacket.GetDataLen());
			}
			
			if((Flag&(ulong)SHIP_INFO_MODIFY_FLAGS.MF_HP)!=0)
			{
				if(!Packet.AddMember((ushort)SHIP_INFO_MEMBER_IDS.SST_SI_HP,m_HP)) FailCount++;
			}
			
			if((Flag&(ulong)SHIP_INFO_MODIFY_FLAGS.MF_ATK)!=0)
			{
				if(!Packet.AddMember((ushort)SHIP_INFO_MEMBER_IDS.SST_SI_ATK,m_Atk)) FailCount++;
			}
			
			
	//<GenerateArea10End>
			return FailCount==0;
		}
		
		public virtual void ParsePacket(CSmartStruct Packet,ulong[] MemberFlags)
		{
	//<GenerateArea11Start>
		
			ulong Flag=MemberFlags[(int)DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_SHIP_INFO];
			ulong UpdateFlag=0;
		
			
			int Pos=Packet.GetFirstMemberPosition();
			while(Pos>0)
			{
				ushort MemberID=0;
				CSmartValue PacketMember=Packet.GetNextMember(ref Pos,ref MemberID);
				switch(MemberID)
				{
				case (ushort)SHIP_INFO_MEMBER_IDS.SST_SI_SHIP_ID:
					if((Flag&(ulong)SHIP_INFO_MODIFY_FLAGS.MF_SHIP_ID)!=0)
					{
						m_ShipID.ID=PacketMember;
						UpdateFlag|=(ulong)SHIP_INFO_MODIFY_FLAGS.MF_SHIP_ID;
					}
					break;
				case (ushort)SHIP_INFO_MEMBER_IDS.SST_SI_SHIP_TYPE:
					if((Flag&(ulong)SHIP_INFO_MODIFY_FLAGS.MF_SHIP_TYPE)!=0)
					{
						m_ShipType=(SHIP_TYPE)((uint)PacketMember);
						UpdateFlag|=(ulong)SHIP_INFO_MODIFY_FLAGS.MF_SHIP_TYPE;
					}
					break;
				case (ushort)SHIP_INFO_MEMBER_IDS.SST_SI_NAME:
					if((Flag&(ulong)SHIP_INFO_MODIFY_FLAGS.MF_NAME)!=0)
					{
						m_Name=PacketMember;
						UpdateFlag|=(ulong)SHIP_INFO_MODIFY_FLAGS.MF_NAME;
					}
					break;
				case (ushort)SHIP_INFO_MEMBER_IDS.SST_SI_POS:
					if((Flag&(ulong)SHIP_INFO_MODIFY_FLAGS.MF_POS)!=0)
					{
						m_Pos.ParsePacket(PacketMember);
						
						UpdateFlag|=(ulong)SHIP_INFO_MODIFY_FLAGS.MF_POS;
					}
					break;
				case (ushort)SHIP_INFO_MEMBER_IDS.SST_SI_HP:
					if((Flag&(ulong)SHIP_INFO_MODIFY_FLAGS.MF_HP)!=0)
					{
						m_HP=PacketMember;
						UpdateFlag|=(ulong)SHIP_INFO_MODIFY_FLAGS.MF_HP;
					}
					break;
				case (ushort)SHIP_INFO_MEMBER_IDS.SST_SI_ATK:
					if((Flag&(ulong)SHIP_INFO_MODIFY_FLAGS.MF_ATK)!=0)
					{
						m_Atk=PacketMember;
						UpdateFlag|=(ulong)SHIP_INFO_MODIFY_FLAGS.MF_ATK;
					}
					break;
				
				}
			}
			
	//<GenerateArea11End>
			m_ModifyFlag|=UpdateFlag;	
		}
	
		public void CloneFrom(ShipInfo DataObject,ulong[] MemberFlags)
		{
		//<GenerateArea12Start>
		
			ulong Flag=MemberFlags[(int)DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_SHIP_INFO];
			ulong UpdateFlag=0;
		
			if((Flag&(ulong)SHIP_INFO_MODIFY_FLAGS.MF_SHIP_ID)!=0)
			{
				m_ShipID=DataObject.m_ShipID;
				UpdateFlag|=(ulong)SHIP_INFO_MODIFY_FLAGS.MF_SHIP_ID;
			}
			if((Flag&(ulong)SHIP_INFO_MODIFY_FLAGS.MF_SHIP_TYPE)!=0)
			{
				m_ShipType=DataObject.m_ShipType;
				UpdateFlag|=(ulong)SHIP_INFO_MODIFY_FLAGS.MF_SHIP_TYPE;
			}
			if((Flag&(ulong)SHIP_INFO_MODIFY_FLAGS.MF_NAME)!=0)
			{
				m_Name=DataObject.m_Name;
				UpdateFlag|=(ulong)SHIP_INFO_MODIFY_FLAGS.MF_NAME;
			}
			if((Flag&(ulong)SHIP_INFO_MODIFY_FLAGS.MF_POS)!=0)
			{
				m_Pos=DataObject.m_Pos.Clone();
				UpdateFlag|=(ulong)SHIP_INFO_MODIFY_FLAGS.MF_POS;
			}
			if((Flag&(ulong)SHIP_INFO_MODIFY_FLAGS.MF_HP)!=0)
			{
				m_HP=DataObject.m_HP;
				UpdateFlag|=(ulong)SHIP_INFO_MODIFY_FLAGS.MF_HP;
			}
			if((Flag&(ulong)SHIP_INFO_MODIFY_FLAGS.MF_ATK)!=0)
			{
				m_Atk=DataObject.m_Atk;
				UpdateFlag|=(ulong)SHIP_INFO_MODIFY_FLAGS.MF_ATK;
			}
			
		
		//<GenerateArea12End>

			m_ModifyFlag|=UpdateFlag;
		}
		public virtual uint GetSmartStructSize(ulong[] MemberFlags)
		{
	//<GenerateArea13Start>
			uint _PackSize = 0;
			ulong Flag = MemberFlags[(int)DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_SHIP_INFO];
			if((Flag&(ulong)SHIP_INFO_MODIFY_FLAGS.MF_SHIP_ID)!=0)
			{
				_PackSize += CSmartStruct.GetFixMemberSize((uint)Marshal.SizeOf(typeof(OBJECT_ID)));
			}
			if((Flag&(ulong)SHIP_INFO_MODIFY_FLAGS.MF_SHIP_TYPE)!=0)
			{
				_PackSize += CSmartStruct.GetFixMemberSize(sizeof(int));
			}
			if((Flag&(ulong)SHIP_INFO_MODIFY_FLAGS.MF_NAME)!=0)
			{
				_PackSize += CSmartStruct.GetStringMemberSize(m_Name);
			}
			if((Flag&(ulong)SHIP_INFO_MODIFY_FLAGS.MF_POS)!=0)
			{
				_PackSize += CSmartStruct.GetStructMemberSize(((POS)m_Pos).GetSmartStructSize());
			}
			if((Flag&(ulong)SHIP_INFO_MODIFY_FLAGS.MF_HP)!=0)
			{
				_PackSize += CSmartStruct.GetFixMemberSize(sizeof(uint));
			}
			if((Flag&(ulong)SHIP_INFO_MODIFY_FLAGS.MF_ATK)!=0)
			{
				_PackSize += CSmartStruct.GetFixMemberSize(sizeof(uint));
			}
			
			
	//<GenerateArea13End>	
			return _PackSize;
		}
		public virtual object Clone()
		{
	//<GenerateArea14Start>
			ShipInfo DataObject=new ShipInfo();
			DataObject.CloneFrom(this,DATA_OBJECT_MODIFY_FLAGS.DOMF_SHIP_INFO_FULL);
			return DataObject;
	//<GenerateArea14End>	
		}
	//<GenerateArea15Start>
		
	//<GenerateArea15End>
	};

	
};