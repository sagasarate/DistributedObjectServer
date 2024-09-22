using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using DOSSystem;
using DOSSample;


namespace DOSSample
{
	
	public class CShipyardMsgHandler
	{
		IShipyard	m_pMsgHandler;
		public void InitMsgMap(IShipyard pMsgHandler,Dictionary<uint, DistributedObject.MsgHandler> MsgMap)
		{
			m_pMsgHandler = pMsgHandler;
			MsgMap.Add(DistributedObject.MakeMsgID((uint)MODULE_IDS.MODULE_ID_DOSSAMPLE, (uint)DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SHIPYARD, (uint)SHIPYARD_METHODS.METHOD_CREATE_SHIP, false), HandleMsgCreateShip);
			
		}

		int HandleMsgCreateShip(CSmartStruct Packet)
		{
			SHIP_TYPE		ShipType;
			string			ShipName=string.Empty;
			
			
			ShipType=0;
			ShipName=string.Empty;
			
		
			
			int Pos=Packet.GetFirstMemberPosition();
			while(Pos>0)
			{
				ushort MemberID=0;
				CSmartValue PacketMember=Packet.GetNextMember(ref Pos,ref MemberID);
				switch(MemberID)
				{
				case (ushort)SHIPYARD_CREATE_SHIP_MEMBER_IDS.SST_CREATE_SHIP_SHIP_TYPE:
					{
						ShipType=(SHIP_TYPE)((uint)PacketMember);
					}
					break;
				case (ushort)SHIPYARD_CREATE_SHIP_MEMBER_IDS.SST_CREATE_SHIP_SHIP_NAME:
					{
						ShipName=PacketMember;
					}
					break;
				
				}
			}
				
		
			return m_pMsgHandler.CreateShip(ShipType,ShipName);
		}
		
	};
};