using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using DOSSystem;
using DOSSample;


namespace DOSSample
{
	
	public class CShipyardAckMsgHandler
	{
		IShipyardAck	m_pMsgHandler;
		public void InitMsgMap(IShipyardAck pMsgHandler,Dictionary<uint, DistributedObject.MsgHandler> MsgMap)
		{
			m_pMsgHandler = pMsgHandler;
			MsgMap.Add(DistributedObject.MakeMsgID((uint)MODULE_IDS.MODULE_ID_DOSSAMPLE, (uint)DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SHIPYARD, (uint)SHIPYARD_METHODS.METHOD_CREATE_SHIP, true), HandleMsgCreateShipAck);
			
		}

		int HandleMsgCreateShipAck(CSmartStruct Packet)
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
				case (ushort)SHIPYARD_ACK_CREATE_SHIP_ACK_MEMBER_IDS.SST_CREATE_SHIP_ACK_RESULT:
					{
						Result=PacketMember;
					}
					break;
				
				}
			}
				
		
			return m_pMsgHandler.CreateShipAck(Result);
		}
		
	};
};