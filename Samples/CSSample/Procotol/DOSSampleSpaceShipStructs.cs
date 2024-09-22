using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Xml;
using System.Text;
using Newtonsoft.Json.Linq;
#if ENABLE_XLS_PROCESS
using System.Windows.Forms;
using Spire.Xls;
using DataConvertor;
#endif
using DOSSystem;
using DOSSample;


namespace DOSSample
{
	//
	public class POS
	{
		public int		x;
		public int		y;
		
		
		public enum POS_MEMBER_IDS
		{
			SST_TEST_STRUCT_X = 1,
			SST_TEST_STRUCT_Y = 2,
			
		};
		
		public class POS_MEMBER_IDS_UTILS
		{
			public const int ENUM_COUNT = 2;
		};
		
		public POS()
		{
			Clear();
		}
		public void Clear()
		{
			x=0;
			y=0;
			
		}
		public bool MakePacket(CSmartStruct Packet)
		{
			uint FailCount=0;
	
			{
				if(!Packet.AddMember((ushort)POS_MEMBER_IDS.SST_TEST_STRUCT_X,x)) FailCount++;
			}
			
			{
				if(!Packet.AddMember((ushort)POS_MEMBER_IDS.SST_TEST_STRUCT_Y,y)) FailCount++;
			}
			
			
	
			return FailCount==0;
		}
		public bool ParsePacket(CSmartStruct Packet)
		{
			
			int Pos=Packet.GetFirstMemberPosition();
			while(Pos>0)
			{
				ushort MemberID=0;
				CSmartValue PacketMember=Packet.GetNextMember(ref Pos,ref MemberID);
				switch(MemberID)
				{
				case (ushort)POS_MEMBER_IDS.SST_TEST_STRUCT_X:
					{
						x=PacketMember;
					}
					break;
				case (ushort)POS_MEMBER_IDS.SST_TEST_STRUCT_Y:
					{
						y=PacketMember;
					}
					break;
				
				}
			}
			
			return true;
		}
		public uint GetSmartStructSize()
		{
			uint _PackSize = 0;
			_PackSize += CSmartStruct.GetFixMemberSize(sizeof(int));
			
			_PackSize += CSmartStruct.GetFixMemberSize(sizeof(int));
			
			
			return _PackSize;
		}
		public void CloneFrom(POS Value)
		{
			{
				x=Value.x;
			}
			{
				y=Value.y;
			}
			
		}
		public POS Clone()
		{
			POS CloneObject=new POS();
			CloneObject.CloneFrom(this);
			return CloneObject;
		}
		
		
		
	};
	
	
	
	
};