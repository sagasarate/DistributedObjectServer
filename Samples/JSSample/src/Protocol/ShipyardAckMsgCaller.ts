import { CSmartValue, CSmartArray, CSmartStruct } from "../DOSSystem/SmartStruct";
import { IDistributedObjectOperator, COMMON_RESULT_CODE, DOSMakeMsgID, OBJECT_ID } from "../DOSSystem/DOSSystem";
import { MODULE_IDS, DOSSAMPLE_INTERFACES } from "./DOSSampleSystemEnums";
import { IShipyardAck } from "./ShipyardAckInterface";
import { SHIPYARD_METHODS } from "./ShipyardInterface";
import { SHIPYARD_ACK_CREATE_SHIP_ACK_MEMBER_IDS } from "./ShipyardAckInterface"


export class CShipyardAckMsgCaller implements IShipyardAck	
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
		
		
		
	public CreateShipAck(Result: number ): number
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
		
	    MsgPacket.MsgID = DOSMakeMsgID(MODULE_IDS.MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SHIPYARD,SHIPYARD_METHODS.METHOD_CREATE_SHIP,true);		
	
		
		let Packet = MsgPacket.EmptyDataPacket;
		
		if(this.PackMsgCreateShipAck(Packet,Result))
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
	
	
	public PackMsgCreateShipAck(Packet: CSmartStruct,Result: number ): boolean
	{
		let FailCount=0;
	
		{
			if(!Packet.AddMemberShort(SHIPYARD_ACK_CREATE_SHIP_ACK_MEMBER_IDS.SST_CREATE_SHIP_ACK_RESULT,Result)) FailCount++;
		}
		
	
		return (FailCount==0);
	}
	
	
};
