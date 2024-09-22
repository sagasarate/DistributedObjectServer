import { CSmartValue, CSmartArray, CSmartStruct } from "../DOSSystem/SmartStruct";
import { IDistributedObjectOperator, COMMON_RESULT_CODE, DOSMakeMsgID, OBJECT_ID } from "../DOSSystem/DOSSystem";
import { MODULE_IDS, DOSSAMPLE_INTERFACES } from "./DOSSampleSystemEnums";
import { IShipyard } from "./ShipyardInterface";
import { SHIPYARD_METHODS } from "./ShipyardInterface";
import { SHIP_TYPE } from "./DOSSampleSpaceShipEnums"
import { SHIPYARD_CREATE_SHIP_MEMBER_IDS } from "./ShipyardInterface"


export class CShipyardMsgCaller implements IShipyard	
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
		
		
		
	public CreateShip(ShipType: SHIP_TYPE ,ShipName: string ): number
	{
		if((this.m_Operator == null) || (this.m_Operator == undefined))
		{
			return COMMON_RESULT_CODE.COMMON_RESULT_FAILED;
		}
	
		let _PackSize = CSmartStruct.GetEmptyStructSize();
		_PackSize += CSmartStruct.GetFixMemberSize(4);
		
		_PackSize += CSmartStruct.GetStringMemberSize(ShipName);
		
		
	
		let MsgPacket = this.m_Operator.NewMessagePacket(_PackSize, this.m_TargetID.length);	
		MsgPacket.SenderID = this.m_SenderID;
		let MsgFlag=0;
		MsgPacket.MsgFlag = MsgFlag;
		
	    MsgPacket.MsgID = DOSMakeMsgID(MODULE_IDS.MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SHIPYARD,SHIPYARD_METHODS.METHOD_CREATE_SHIP,false);		
	
		
		let Packet = MsgPacket.EmptyDataPacket;
		
		if(this.PackMsgCreateShip(Packet,ShipType,ShipName))
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
	
	
	public PackMsgCreateShip(Packet: CSmartStruct,ShipType: SHIP_TYPE ,ShipName: string ): boolean
	{
		let FailCount=0;
	
		{
			if(!Packet.AddMemberInt(SHIPYARD_CREATE_SHIP_MEMBER_IDS.SST_CREATE_SHIP_SHIP_TYPE,ShipType)) FailCount++;
		}
		{
			if(!Packet.AddMemberString(SHIPYARD_CREATE_SHIP_MEMBER_IDS.SST_CREATE_SHIP_SHIP_NAME,ShipName)) FailCount++;
		}
		
	
		return (FailCount==0);
	}
	
	
};
