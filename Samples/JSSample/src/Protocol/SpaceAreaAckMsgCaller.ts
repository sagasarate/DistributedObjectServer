import { CSmartValue, CSmartArray, CSmartStruct } from "../DOSSystem/SmartStruct";
import { IDistributedObjectOperator, COMMON_RESULT_CODE, DOSMakeMsgID, OBJECT_ID } from "../DOSSystem/DOSSystem";
import { MODULE_IDS, DOSSAMPLE_INTERFACES } from "./DOSSampleSystemEnums";
import { ISpaceAreaAck } from "./SpaceAreaAckInterface";
import { SPACE_AREA_METHODS } from "./SpaceAreaInterface";
import { DOMF_SHIP_INFO_FULL } from "./DataObjectModifyFlags"
import { SPACE_AREA_ACK_REGISTER_SHIP_ACK_MEMBER_IDS,SPACE_AREA_ACK_UNREGISTER_SHIP_ACK_MEMBER_IDS,SPACE_AREA_ACK_FIND_SHIP_ACK_MEMBER_IDS,SPACE_AREA_ACK_REGISTER_SHIPYARD_ACK_MEMBER_IDS } from "./SpaceAreaAckInterface"


export class CSpaceAreaAckMsgCaller implements ISpaceAreaAck	
{
	m_Operator: IDistributedObjectOperator;
	m_SenderID: OBJECT_ID;
	m_TargetID: Array<OBJECT_ID>;
		
	constructor(Operator: IDistributedObjectOperator, SenderID: OBJECT_ID, TargetID: OBJECT_ID | Array<OBJECT_ID>)
	{
		this.m_Operator = Operator;
		this.m_SenderID = SenderID;
		if (TargetID instanceof Array)
			this.m_TargetID = TargetID;
		else
			this.m_TargetID = [TargetID];
	}
		
		
		
	public RegisterShipAck(Result: number ): number
	{
		if((this.m_Operator == null) || (this.m_Operator == undefined))
		{
			return COMMON_RESULT_CODE.COMMON_RESULT_FAILED;
		}
	
		let _PackSize = CSmartStruct.GetEmptyStructSize();
		_PackSize += CSmartStruct.GetFixMemberSize(2);
		
		
	
		let MsgPacket = this.m_Operator.NewMessagePacket(_PackSize, this.m_TargetID.length);	
		MsgPacket.SenderID = this.m_SenderID;
		let MsgFlag=0;
		MsgPacket.MsgFlag = MsgFlag;
		
	    MsgPacket.MsgID = DOSMakeMsgID(MODULE_IDS.MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA,SPACE_AREA_METHODS.METHOD_REGISTER_SHIP,true);		
	
		
		let Packet = MsgPacket.EmptyDataPacket;
		
		if(this.PackMsgRegisterShipAck(Packet,Result))
		{
			MsgPacket.DataLength = Packet.GetDataLen();
			MsgPacket.TargetIDs = this.m_TargetID;
			if(this.m_Operator.SendMessagePacket(MsgPacket))
			{
				return COMMON_RESULT_CODE.COMMON_RESULT_SUCCEED;
			}
			else
			{
				return COMMON_RESULT_CODE.COMMON_RESULT_MSG_SEND_ERROR;
			}
		}
		else
		{
			return COMMON_RESULT_CODE.COMMON_RESULT_MSG_PACK_ERROR;
		}
		
	}
	
	public UnregisterShipAck(Result: number ): number
	{
		if((this.m_Operator == null) || (this.m_Operator == undefined))
		{
			return COMMON_RESULT_CODE.COMMON_RESULT_FAILED;
		}
	
		let _PackSize = CSmartStruct.GetEmptyStructSize();
		_PackSize += CSmartStruct.GetFixMemberSize(2);
		
		
	
		let MsgPacket = this.m_Operator.NewMessagePacket(_PackSize, this.m_TargetID.length);	
		MsgPacket.SenderID = this.m_SenderID;
		let MsgFlag=0;
		MsgPacket.MsgFlag = MsgFlag;
		
	    MsgPacket.MsgID = DOSMakeMsgID(MODULE_IDS.MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA,SPACE_AREA_METHODS.METHOD_UNREGISTER_SHIP,true);		
	
		
		let Packet = MsgPacket.EmptyDataPacket;
		
		if(this.PackMsgUnregisterShipAck(Packet,Result))
		{
			MsgPacket.DataLength = Packet.GetDataLen();
			MsgPacket.TargetIDs = this.m_TargetID;
			if(this.m_Operator.SendMessagePacket(MsgPacket))
			{
				return COMMON_RESULT_CODE.COMMON_RESULT_SUCCEED;
			}
			else
			{
				return COMMON_RESULT_CODE.COMMON_RESULT_MSG_SEND_ERROR;
			}
		}
		else
		{
			return COMMON_RESULT_CODE.COMMON_RESULT_MSG_PACK_ERROR;
		}
		
	}
	
	public FindShipAck(Result: number ,ShipID: OBJECT_ID ): number
	{
		if((this.m_Operator == null) || (this.m_Operator == undefined))
		{
			return COMMON_RESULT_CODE.COMMON_RESULT_FAILED;
		}
	
		let _PackSize = CSmartStruct.GetEmptyStructSize();
		_PackSize += CSmartStruct.GetFixMemberSize(2);
		
		_PackSize += CSmartStruct.GetFixMemberSize(8);
		
		
	
		let MsgPacket = this.m_Operator.NewMessagePacket(_PackSize, this.m_TargetID.length);	
		MsgPacket.SenderID = this.m_SenderID;
		let MsgFlag=0;
		MsgPacket.MsgFlag = MsgFlag;
		
	    MsgPacket.MsgID = DOSMakeMsgID(MODULE_IDS.MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA,SPACE_AREA_METHODS.METHOD_FIND_SHIP,true);		
	
		
		let Packet = MsgPacket.EmptyDataPacket;
		
		if(this.PackMsgFindShipAck(Packet,Result,ShipID))
		{
			MsgPacket.DataLength = Packet.GetDataLen();
			MsgPacket.TargetIDs = this.m_TargetID;
			if(this.m_Operator.SendMessagePacket(MsgPacket))
			{
				return COMMON_RESULT_CODE.COMMON_RESULT_SUCCEED;
			}
			else
			{
				return COMMON_RESULT_CODE.COMMON_RESULT_MSG_SEND_ERROR;
			}
		}
		else
		{
			return COMMON_RESULT_CODE.COMMON_RESULT_MSG_PACK_ERROR;
		}
		
	}
	
	public RegisterShipyardAck(Result: number ): number
	{
		if((this.m_Operator == null) || (this.m_Operator == undefined))
		{
			return COMMON_RESULT_CODE.COMMON_RESULT_FAILED;
		}
	
		let _PackSize = CSmartStruct.GetEmptyStructSize();
		_PackSize += CSmartStruct.GetFixMemberSize(2);
		
		
	
		let MsgPacket = this.m_Operator.NewMessagePacket(_PackSize, this.m_TargetID.length);	
		MsgPacket.SenderID = this.m_SenderID;
		let MsgFlag=0;
		MsgPacket.MsgFlag = MsgFlag;
		
	    MsgPacket.MsgID = DOSMakeMsgID(MODULE_IDS.MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA,SPACE_AREA_METHODS.METHOD_REGISTER_SHIPYARD,true);		
	
		
		let Packet = MsgPacket.EmptyDataPacket;
		
		if(this.PackMsgRegisterShipyardAck(Packet,Result))
		{
			MsgPacket.DataLength = Packet.GetDataLen();
			MsgPacket.TargetIDs = this.m_TargetID;
			if(this.m_Operator.SendMessagePacket(MsgPacket))
			{
				return COMMON_RESULT_CODE.COMMON_RESULT_SUCCEED;
			}
			else
			{
				return COMMON_RESULT_CODE.COMMON_RESULT_MSG_SEND_ERROR;
			}
		}
		else
		{
			return COMMON_RESULT_CODE.COMMON_RESULT_MSG_PACK_ERROR;
		}
		
	}
	
	
	public PackMsgRegisterShipAck(Packet: CSmartStruct,Result: number ): boolean
	{
		let FailCount=0;
	
		{
			if(!Packet.AddMemberShort(SPACE_AREA_ACK_REGISTER_SHIP_ACK_MEMBER_IDS.SST_REGISTER_SHIP_ACK_RESULT,Result)) FailCount++;
		}
		
	
		return (FailCount==0);
	}
	
	public PackMsgUnregisterShipAck(Packet: CSmartStruct,Result: number ): boolean
	{
		let FailCount=0;
	
		{
			if(!Packet.AddMemberShort(SPACE_AREA_ACK_UNREGISTER_SHIP_ACK_MEMBER_IDS.SST_UNREGISTER_SHIP_ACK_RESULT,Result)) FailCount++;
		}
		
	
		return (FailCount==0);
	}
	
	public PackMsgFindShipAck(Packet: CSmartStruct,Result: number ,ShipID: OBJECT_ID ): boolean
	{
		let FailCount=0;
	
		{
			if(!Packet.AddMemberShort(SPACE_AREA_ACK_FIND_SHIP_ACK_MEMBER_IDS.SST_FIND_SHIP_ACK_RESULT,Result)) FailCount++;
		}
		{
			if(!Packet.AddMemberLong(SPACE_AREA_ACK_FIND_SHIP_ACK_MEMBER_IDS.SST_FIND_SHIP_ACK_SHIP_ID,ShipID.ID)) FailCount++;
		}
		
	
		return (FailCount==0);
	}
	
	public PackMsgRegisterShipyardAck(Packet: CSmartStruct,Result: number ): boolean
	{
		let FailCount=0;
	
		{
			if(!Packet.AddMemberShort(SPACE_AREA_ACK_REGISTER_SHIPYARD_ACK_MEMBER_IDS.SST_REGISTER_SHIPYARD_ACK_RESULT,Result)) FailCount++;
		}
		
	
		return (FailCount==0);
	}
	
	
};
