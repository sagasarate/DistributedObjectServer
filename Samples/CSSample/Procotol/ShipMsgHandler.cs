using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using DOSSystem;
using DOSSample;


namespace DOSSample
{
	
	public class CShipMsgHandler
	{
		IShip	m_pMsgHandler;
		public void InitMsgMap(IShip pMsgHandler,Dictionary<uint, DistributedObject.MsgHandler> MsgMap)
		{
			m_pMsgHandler = pMsgHandler;
			MsgMap.Add(DistributedObject.MakeMsgID((uint)MODULE_IDS.MODULE_ID_DOSSAMPLE, (uint)DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SHIP, (uint)SHIP_METHODS.METHOD_ATTACK, false), HandleMsgAttack);
			
		}

		int HandleMsgAttack(CSmartStruct Packet)
		{
			int		Damage;
			
			
			Damage=0;
			
		
			
			int Pos=Packet.GetFirstMemberPosition();
			while(Pos>0)
			{
				ushort MemberID=0;
				CSmartValue PacketMember=Packet.GetNextMember(ref Pos,ref MemberID);
				switch(MemberID)
				{
				case (ushort)SHIP_ATTACK_MEMBER_IDS.SST_ATTACK_DAMAGE:
					{
						Damage=PacketMember;
					}
					break;
				
				}
			}
				
		
			return m_pMsgHandler.Attack(Damage);
		}
		
	};
};