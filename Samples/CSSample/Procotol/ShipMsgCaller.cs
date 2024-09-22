using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using DOSSystem;
using DOSSample;


namespace DOSSample
{
	public class CShipMsgCaller :
		IShip
	{
		DistributedObjectOperator						m_Operator;		
		OBJECT_ID										m_SenderID;
		OBJECT_ID[]										m_TargetIDList;
		
		public CShipMsgCaller(DistributedObjectOperator Operator, OBJECT_ID SenderID, OBJECT_ID TargetID)
		{
			m_Operator = Operator;
			m_SenderID = SenderID;
			m_TargetIDList = new OBJECT_ID[1]{TargetID};
		}
		public CShipMsgCaller(DistributedObjectOperator Operator, OBJECT_ID SenderID, OBJECT_ID[] TargetIDList)
		{
			m_Operator = Operator;
			m_SenderID = SenderID;
			m_TargetIDList = TargetIDList;
		}
		
		
		public int Attack(int Damage )
		{
			if(m_Operator == null)
			{
				return (int)COMMON_RESULT_CODE.COMMON_RESULT_FAILED;
			}
		
			uint _PackSize = CSmartStruct.GetEmptyStructSize();
			_PackSize += CSmartStruct.GetFixMemberSize(sizeof(int));
			
			;
		
			ushort MsgFlag = 0;
				
			byte[] MsgBuffer=new byte[_PackSize];
			
			
		    uint MsgID = DistributedObject.MakeMsgID((uint)MODULE_IDS.MODULE_ID_DOSSAMPLE,(uint)DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SHIP,(uint)SHIP_METHODS.METHOD_ATTACK,false);		
		
			
			CSmartStruct Packet = new CSmartStruct(MsgBuffer,0,_PackSize,true);
			
			if(PackMsgAttack(Packet,Damage))
			{	
				if(m_Operator.SendMessageMulti(m_TargetIDList, true, m_SenderID, MsgID, MsgFlag, MsgBuffer, 0, MsgBuffer.Length))
				{
					return (int)COMMON_RESULT_CODE.COMMON_RESULT_SUCCEED;
				}
				else
				{
					return (int)COMMON_RESULT_CODE.COMMON_RESULT_MSG_SEND_ERROR;
				}
			}
			else
			{
				return (int)COMMON_RESULT_CODE.COMMON_RESULT_MSG_PACK_ERROR;
			}
			
		}
		
		
		public bool PackMsgAttack(CSmartStruct Packet,int Damage )
		{
			uint FailCount=0;
		
			{
				if(!Packet.AddMember((ushort)SHIP_ATTACK_MEMBER_IDS.SST_ATTACK_DAMAGE,Damage)) FailCount++;
			}
			
			
		
			return (FailCount==0);
		}
		
		
	};
};
