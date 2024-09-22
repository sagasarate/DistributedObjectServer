using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using DOSSystem;
using DOSSample;


namespace DOSSample
{
	
	public class CSpaceAreaAckMsgHandler
	{
		ISpaceAreaAck	m_pMsgHandler;
		public void InitMsgMap(ISpaceAreaAck pMsgHandler,Dictionary<uint, DistributedObject.MsgHandler> MsgMap)
		{
			m_pMsgHandler = pMsgHandler;
			MsgMap.Add(DistributedObject.MakeMsgID((uint)MODULE_IDS.MODULE_ID_DOSSAMPLE, (uint)DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA, (uint)SPACE_AREA_METHODS.METHOD_REGISTER_SHIP, true), HandleMsgRegisterShipAck);
			MsgMap.Add(DistributedObject.MakeMsgID((uint)MODULE_IDS.MODULE_ID_DOSSAMPLE, (uint)DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA, (uint)SPACE_AREA_METHODS.METHOD_UNREGISTER_SHIP, true), HandleMsgUnregisterShipAck);
			MsgMap.Add(DistributedObject.MakeMsgID((uint)MODULE_IDS.MODULE_ID_DOSSAMPLE, (uint)DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA, (uint)SPACE_AREA_METHODS.METHOD_FIND_SHIP, true), HandleMsgFindShipAck);
			MsgMap.Add(DistributedObject.MakeMsgID((uint)MODULE_IDS.MODULE_ID_DOSSAMPLE, (uint)DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA, (uint)SPACE_AREA_METHODS.METHOD_REGISTER_SHIPYARD, true), HandleMsgRegisterShipyardAck);
			
		}

		int HandleMsgRegisterShipAck(CSmartStruct Packet)
		{
			short		Result;
			
			
			Result=0;
			
		
			
			int Pos=Packet.GetFirstMemberPosition();
			while(Pos>0)
			{
				ushort MemberID=0;
				CSmartValue PacketMember=Packet.GetNextMember(ref Pos,ref MemberID);
				switch(MemberID)
				{
				case (ushort)SPACE_AREA_ACK_REGISTER_SHIP_ACK_MEMBER_IDS.SST_REGISTER_SHIP_ACK_RESULT:
					{
						Result=PacketMember;
					}
					break;
				
				}
			}
				
		
			return m_pMsgHandler.RegisterShipAck(Result);
		}
		int HandleMsgUnregisterShipAck(CSmartStruct Packet)
		{
			short		Result;
			
			
			Result=0;
			
		
			
			int Pos=Packet.GetFirstMemberPosition();
			while(Pos>0)
			{
				ushort MemberID=0;
				CSmartValue PacketMember=Packet.GetNextMember(ref Pos,ref MemberID);
				switch(MemberID)
				{
				case (ushort)SPACE_AREA_ACK_UNREGISTER_SHIP_ACK_MEMBER_IDS.SST_UNREGISTER_SHIP_ACK_RESULT:
					{
						Result=PacketMember;
					}
					break;
				
				}
			}
				
		
			return m_pMsgHandler.UnregisterShipAck(Result);
		}
		int HandleMsgFindShipAck(CSmartStruct Packet)
		{
			short			Result;
			OBJECT_ID		ShipID;
			
			
			Result=0;
			ShipID.ID=0;
			
		
			
			int Pos=Packet.GetFirstMemberPosition();
			while(Pos>0)
			{
				ushort MemberID=0;
				CSmartValue PacketMember=Packet.GetNextMember(ref Pos,ref MemberID);
				switch(MemberID)
				{
				case (ushort)SPACE_AREA_ACK_FIND_SHIP_ACK_MEMBER_IDS.SST_FIND_SHIP_ACK_RESULT:
					{
						Result=PacketMember;
					}
					break;
				case (ushort)SPACE_AREA_ACK_FIND_SHIP_ACK_MEMBER_IDS.SST_FIND_SHIP_ACK_SHIP_ID:
					{
						ShipID.ID=PacketMember;
					}
					break;
				
				}
			}
				
		
			return m_pMsgHandler.FindShipAck(Result,ShipID);
		}
		int HandleMsgRegisterShipyardAck(CSmartStruct Packet)
		{
			short		Result;
			
			
			Result=0;
			
		
			
			int Pos=Packet.GetFirstMemberPosition();
			while(Pos>0)
			{
				ushort MemberID=0;
				CSmartValue PacketMember=Packet.GetNextMember(ref Pos,ref MemberID);
				switch(MemberID)
				{
				case (ushort)SPACE_AREA_ACK_REGISTER_SHIPYARD_ACK_MEMBER_IDS.SST_REGISTER_SHIPYARD_ACK_RESULT:
					{
						Result=PacketMember;
					}
					break;
				
				}
			}
				
		
			return m_pMsgHandler.RegisterShipyardAck(Result);
		}
		
	};
};