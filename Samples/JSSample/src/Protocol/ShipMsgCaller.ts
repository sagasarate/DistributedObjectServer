import { CSmartValue, CSmartArray, CSmartStruct } from "../DOSSystem/SmartStruct";
import { IDistributedObjectOperator, COMMON_RESULT_CODE, DOSMakeMsgID, OBJECT_ID } from "../DOSSystem/DOSSystem";
import { MODULE_IDS, DOSSAMPLE_INTERFACES } from "./DOSSampleSystemEnums";
import { IShip } from "./ShipInterface";
import { SHIP_METHODS } from "./ShipInterface";
import { SHIP_ATTACK_MEMBER_IDS } from "./ShipInterface"


export class CShipMsgCaller implements IShip	
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
		
		
		
	public Attack(Damage: number ): number
	{
		if((this.m_Operator == null) || (this.m_Operator == undefined))
		{
			return COMMON_RESULT_CODE.COMMON_RESULT_FAILED;
		}
	
		let _PackSize = CSmartStruct.GetEmptyStructSize();
		_PackSize += CSmartStruct.GetFixMemberSize(4);
		
		
	
		let MsgPacket = this.m_Operator.NewMessagePacket(_PackSize, this.m_TargetID.length);	
		MsgPacket.SenderID = this.m_SenderID;
		let MsgFlag=0;
		MsgPacket.MsgFlag = MsgFlag;
		
	    MsgPacket.MsgID = DOSMakeMsgID(MODULE_IDS.MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SHIP,SHIP_METHODS.METHOD_ATTACK,false);		
	
		
		let Packet = MsgPacket.EmptyDataPacket;
		
		if(this.PackMsgAttack(Packet,Damage))
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
	
	
	public PackMsgAttack(Packet: CSmartStruct,Damage: number ): boolean
	{
		let FailCount=0;
	
		{
			if(!Packet.AddMemberInt(SHIP_ATTACK_MEMBER_IDS.SST_ATTACK_DAMAGE,Damage)) FailCount++;
		}
		
	
		return (FailCount==0);
	}
	
	
};
