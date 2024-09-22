using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using DOSSystem;
using DOSSample;


namespace DOSSample
{
	
	public class CSpaceAreaMsgHandler
	{
		ISpaceArea	m_pMsgHandler;
		public void InitMsgMap(ISpaceArea pMsgHandler,Dictionary<uint, DistributedObject.MsgHandler> MsgMap)
		{
			m_pMsgHandler = pMsgHandler;
			MsgMap.Add(DistributedObject.MakeMsgID((uint)MODULE_IDS.MODULE_ID_DOSSAMPLE, (uint)DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA, (uint)SPACE_AREA_METHODS.METHOD_REGISTER_SHIP, false), HandleMsgRegisterShip);
			MsgMap.Add(DistributedObject.MakeMsgID((uint)MODULE_IDS.MODULE_ID_DOSSAMPLE, (uint)DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA, (uint)SPACE_AREA_METHODS.METHOD_UNREGISTER_SHIP, false), HandleMsgUnregisterShip);
			MsgMap.Add(DistributedObject.MakeMsgID((uint)MODULE_IDS.MODULE_ID_DOSSAMPLE, (uint)DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA, (uint)SPACE_AREA_METHODS.METHOD_FIND_SHIP, false), HandleMsgFindShip);
			MsgMap.Add(DistributedObject.MakeMsgID((uint)MODULE_IDS.MODULE_ID_DOSSAMPLE, (uint)DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA, (uint)SPACE_AREA_METHODS.METHOD_REGISTER_SHIPYARD, false), HandleMsgRegisterShipyard);
			
		}

		int HandleMsgRegisterShip(CSmartStruct Packet)
		{
			ShipInfo	RegisterShipInfo=new ShipInfo();
			
			
			RegisterShipInfo.Clear();
			
		
			
			int Pos=Packet.GetFirstMemberPosition();
			while(Pos>0)
			{
				ushort MemberID=0;
				CSmartValue PacketMember=Packet.GetNextMember(ref Pos,ref MemberID);
				switch(MemberID)
				{
				case (ushort)SPACE_AREA_REGISTER_SHIP_MEMBER_IDS.SST_REGISTER_SHIP_REGISTER_SHIP_INFO:
					{
						RegisterShipInfo.ParsePacket(PacketMember,DATA_OBJECT_MODIFY_FLAGS.DOMF_SHIP_INFO_FOR_REGISTER);
					}
					break;
				
				}
			}
				
		
			return m_pMsgHandler.RegisterShip(RegisterShipInfo);
		}
		int HandleMsgUnregisterShip(CSmartStruct Packet)
		{
			
			
			
		
			return m_pMsgHandler.UnregisterShip();
		}
		int HandleMsgFindShip(CSmartStruct Packet)
		{
			
			
			
		
			return m_pMsgHandler.FindShip();
		}
		int HandleMsgRegisterShipyard(CSmartStruct Packet)
		{
			
			
			
		
			return m_pMsgHandler.RegisterShipyard();
		}
		
	};
};