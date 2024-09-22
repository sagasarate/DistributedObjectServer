using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using DOSSystem;
using DOSSample;


namespace DOSSample
{
	public class CSpaceAreaMsgCaller :
		ISpaceArea
	{
		DistributedObjectOperator						m_Operator;		
		OBJECT_ID										m_SenderID;
		OBJECT_ID[]										m_TargetIDList;
		
		public CSpaceAreaMsgCaller(DistributedObjectOperator Operator, OBJECT_ID SenderID, OBJECT_ID TargetID)
		{
			m_Operator = Operator;
			m_SenderID = SenderID;
			m_TargetIDList = new OBJECT_ID[1]{TargetID};
		}
		public CSpaceAreaMsgCaller(DistributedObjectOperator Operator, OBJECT_ID SenderID, OBJECT_ID[] TargetIDList)
		{
			m_Operator = Operator;
			m_SenderID = SenderID;
			m_TargetIDList = TargetIDList;
		}
		
		
		public int RegisterShip(ShipInfo RegisterShipInfo )
		{
			if(m_Operator == null)
			{
				return (int)COMMON_RESULT_CODE.COMMON_RESULT_FAILED;
			}
		
			uint _PackSize = CSmartStruct.GetEmptyStructSize();
			_PackSize += CSmartStruct.GetStructMemberSize(((ShipInfo)RegisterShipInfo).GetSmartStructSize(DATA_OBJECT_MODIFY_FLAGS.DOMF_SHIP_INFO_FOR_REGISTER));
			
			;
		
			ushort MsgFlag = 0;
				
			byte[] MsgBuffer=new byte[_PackSize];
			
			
		    uint MsgID = DistributedObject.MakeMsgID((uint)MODULE_IDS.MODULE_ID_DOSSAMPLE,(uint)DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA,(uint)SPACE_AREA_METHODS.METHOD_REGISTER_SHIP,false);		
		
			
			CSmartStruct Packet = new CSmartStruct(MsgBuffer,0,_PackSize,true);
			
			if(PackMsgRegisterShip(Packet,RegisterShipInfo))
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
		
		public int UnregisterShip()
		{
			if(m_Operator == null)
			{
				return (int)COMMON_RESULT_CODE.COMMON_RESULT_FAILED;
			}
		
			uint _PackSize = CSmartStruct.GetEmptyStructSize();
			;
		
			ushort MsgFlag = 0;
				
			byte[] MsgBuffer=new byte[_PackSize];
			
			
		    uint MsgID = DistributedObject.MakeMsgID((uint)MODULE_IDS.MODULE_ID_DOSSAMPLE,(uint)DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA,(uint)SPACE_AREA_METHODS.METHOD_UNREGISTER_SHIP,false);		
		
			
			CSmartStruct Packet = new CSmartStruct(MsgBuffer,0,_PackSize,true);
			
			if(PackMsgUnregisterShip(Packet))
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
		
		public int FindShip()
		{
			if(m_Operator == null)
			{
				return (int)COMMON_RESULT_CODE.COMMON_RESULT_FAILED;
			}
		
			uint _PackSize = CSmartStruct.GetEmptyStructSize();
			;
		
			ushort MsgFlag = 0;
				
			byte[] MsgBuffer=new byte[_PackSize];
			
			
		    uint MsgID = DistributedObject.MakeMsgID((uint)MODULE_IDS.MODULE_ID_DOSSAMPLE,(uint)DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA,(uint)SPACE_AREA_METHODS.METHOD_FIND_SHIP,false);		
		
			
			CSmartStruct Packet = new CSmartStruct(MsgBuffer,0,_PackSize,true);
			
			if(PackMsgFindShip(Packet))
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
		
		public int RegisterShipyard()
		{
			if(m_Operator == null)
			{
				return (int)COMMON_RESULT_CODE.COMMON_RESULT_FAILED;
			}
		
			uint _PackSize = CSmartStruct.GetEmptyStructSize();
			;
		
			ushort MsgFlag = 0;
				
			byte[] MsgBuffer=new byte[_PackSize];
			
			
		    uint MsgID = DistributedObject.MakeMsgID((uint)MODULE_IDS.MODULE_ID_DOSSAMPLE,(uint)DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA,(uint)SPACE_AREA_METHODS.METHOD_REGISTER_SHIPYARD,false);		
		
			
			CSmartStruct Packet = new CSmartStruct(MsgBuffer,0,_PackSize,true);
			
			if(PackMsgRegisterShipyard(Packet))
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
		
		
		public bool PackMsgRegisterShip(CSmartStruct Packet,ShipInfo RegisterShipInfo )
		{
			uint FailCount=0;
		
			{
				CSmartStruct SubPacket=Packet.PrepareSubStruct();
				if(!RegisterShipInfo.MakePacket(SubPacket,DATA_OBJECT_MODIFY_FLAGS.DOMF_SHIP_INFO_FOR_REGISTER))
					FailCount++;
				Packet.FinishMember((ushort)SPACE_AREA_REGISTER_SHIP_MEMBER_IDS.SST_REGISTER_SHIP_REGISTER_SHIP_INFO,SubPacket.GetDataLen());
			}
			
			
		
			return (FailCount==0);
		}
		
		public bool PackMsgUnregisterShip(CSmartStruct Packet)
		{
			uint FailCount=0;
		
			
		
			return (FailCount==0);
		}
		
		public bool PackMsgFindShip(CSmartStruct Packet)
		{
			uint FailCount=0;
		
			
		
			return (FailCount==0);
		}
		
		public bool PackMsgRegisterShipyard(CSmartStruct Packet)
		{
			uint FailCount=0;
		
			
		
			return (FailCount==0);
		}
		
		
	};
};
