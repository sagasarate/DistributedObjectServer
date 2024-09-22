using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using DOSSystem;
using DOSSample;


namespace DOSSample
{
	public class CSpaceAreaAckMsgCaller :
		ISpaceAreaAck
	{
		DistributedObjectOperator						m_Operator;		
		OBJECT_ID										m_SenderID;
		OBJECT_ID[]										m_TargetIDList;
		
		public CSpaceAreaAckMsgCaller(DistributedObjectOperator Operator, OBJECT_ID SenderID, OBJECT_ID TargetID)
		{
			m_Operator = Operator;
			m_SenderID = SenderID;
			m_TargetIDList = new OBJECT_ID[1]{TargetID};
		}
		public CSpaceAreaAckMsgCaller(DistributedObjectOperator Operator, OBJECT_ID SenderID, OBJECT_ID[] TargetIDList)
		{
			m_Operator = Operator;
			m_SenderID = SenderID;
			m_TargetIDList = TargetIDList;
		}
		
		
		public int RegisterShipAck(short Result )
		{
			if(m_Operator == null)
			{
				return (int)COMMON_RESULT_CODE.COMMON_RESULT_FAILED;
			}
		
			uint _PackSize = CSmartStruct.GetEmptyStructSize();
			_PackSize += CSmartStruct.GetFixMemberSize(sizeof(short));
			
			;
		
			ushort MsgFlag = 0;
				
			byte[] MsgBuffer=new byte[_PackSize];
			
			
		    uint MsgID = DistributedObject.MakeMsgID((uint)MODULE_IDS.MODULE_ID_DOSSAMPLE,(uint)DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA,(uint)SPACE_AREA_METHODS.METHOD_REGISTER_SHIP,true);		
		
			
			CSmartStruct Packet = new CSmartStruct(MsgBuffer,0,_PackSize,true);
			
			if(PackMsgRegisterShipAck(Packet,Result))
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
		
		public int UnregisterShipAck(short Result )
		{
			if(m_Operator == null)
			{
				return (int)COMMON_RESULT_CODE.COMMON_RESULT_FAILED;
			}
		
			uint _PackSize = CSmartStruct.GetEmptyStructSize();
			_PackSize += CSmartStruct.GetFixMemberSize(sizeof(short));
			
			;
		
			ushort MsgFlag = 0;
				
			byte[] MsgBuffer=new byte[_PackSize];
			
			
		    uint MsgID = DistributedObject.MakeMsgID((uint)MODULE_IDS.MODULE_ID_DOSSAMPLE,(uint)DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA,(uint)SPACE_AREA_METHODS.METHOD_UNREGISTER_SHIP,true);		
		
			
			CSmartStruct Packet = new CSmartStruct(MsgBuffer,0,_PackSize,true);
			
			if(PackMsgUnregisterShipAck(Packet,Result))
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
		
		public int FindShipAck(short Result ,OBJECT_ID ShipID )
		{
			if(m_Operator == null)
			{
				return (int)COMMON_RESULT_CODE.COMMON_RESULT_FAILED;
			}
		
			uint _PackSize = CSmartStruct.GetEmptyStructSize();
			_PackSize += CSmartStruct.GetFixMemberSize(sizeof(short));
			
			_PackSize += CSmartStruct.GetFixMemberSize((uint)Marshal.SizeOf(typeof(OBJECT_ID)));
			
			;
		
			ushort MsgFlag = 0;
				
			byte[] MsgBuffer=new byte[_PackSize];
			
			
		    uint MsgID = DistributedObject.MakeMsgID((uint)MODULE_IDS.MODULE_ID_DOSSAMPLE,(uint)DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA,(uint)SPACE_AREA_METHODS.METHOD_FIND_SHIP,true);		
		
			
			CSmartStruct Packet = new CSmartStruct(MsgBuffer,0,_PackSize,true);
			
			if(PackMsgFindShipAck(Packet,Result,ShipID))
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
		
		public int RegisterShipyardAck(short Result )
		{
			if(m_Operator == null)
			{
				return (int)COMMON_RESULT_CODE.COMMON_RESULT_FAILED;
			}
		
			uint _PackSize = CSmartStruct.GetEmptyStructSize();
			_PackSize += CSmartStruct.GetFixMemberSize(sizeof(short));
			
			;
		
			ushort MsgFlag = 0;
				
			byte[] MsgBuffer=new byte[_PackSize];
			
			
		    uint MsgID = DistributedObject.MakeMsgID((uint)MODULE_IDS.MODULE_ID_DOSSAMPLE,(uint)DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA,(uint)SPACE_AREA_METHODS.METHOD_REGISTER_SHIPYARD,true);		
		
			
			CSmartStruct Packet = new CSmartStruct(MsgBuffer,0,_PackSize,true);
			
			if(PackMsgRegisterShipyardAck(Packet,Result))
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
		
		
		public bool PackMsgRegisterShipAck(CSmartStruct Packet,short Result )
		{
			uint FailCount=0;
		
			{
				if(!Packet.AddMember((ushort)SPACE_AREA_ACK_REGISTER_SHIP_ACK_MEMBER_IDS.SST_REGISTER_SHIP_ACK_RESULT,Result)) FailCount++;
			}
			
			
		
			return (FailCount==0);
		}
		
		public bool PackMsgUnregisterShipAck(CSmartStruct Packet,short Result )
		{
			uint FailCount=0;
		
			{
				if(!Packet.AddMember((ushort)SPACE_AREA_ACK_UNREGISTER_SHIP_ACK_MEMBER_IDS.SST_UNREGISTER_SHIP_ACK_RESULT,Result)) FailCount++;
			}
			
			
		
			return (FailCount==0);
		}
		
		public bool PackMsgFindShipAck(CSmartStruct Packet,short Result ,OBJECT_ID ShipID )
		{
			uint FailCount=0;
		
			{
				if(!Packet.AddMember((ushort)SPACE_AREA_ACK_FIND_SHIP_ACK_MEMBER_IDS.SST_FIND_SHIP_ACK_RESULT,Result)) FailCount++;
			}
			
			{
				if(!Packet.AddMember((ushort)SPACE_AREA_ACK_FIND_SHIP_ACK_MEMBER_IDS.SST_FIND_SHIP_ACK_SHIP_ID,ShipID.ID)) FailCount++;
			}
			
			
		
			return (FailCount==0);
		}
		
		public bool PackMsgRegisterShipyardAck(CSmartStruct Packet,short Result )
		{
			uint FailCount=0;
		
			{
				if(!Packet.AddMember((ushort)SPACE_AREA_ACK_REGISTER_SHIPYARD_ACK_MEMBER_IDS.SST_REGISTER_SHIPYARD_ACK_RESULT,Result)) FailCount++;
			}
			
			
		
			return (FailCount==0);
		}
		
		
	};
};
