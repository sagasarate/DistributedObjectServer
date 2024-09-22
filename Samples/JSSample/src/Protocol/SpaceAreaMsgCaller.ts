import { CSmartValue, CSmartArray, CSmartStruct } from "../DOSSystem/SmartStruct";
import { IDistributedObjectOperator, COMMON_RESULT_CODE, DOSMakeMsgID, OBJECT_ID } from "../DOSSystem/DOSSystem";
import { MODULE_IDS, DOSSAMPLE_INTERFACES } from "./DOSSampleSystemEnums";
import { ISpaceArea } from "./SpaceAreaInterface";
import { SPACE_AREA_METHODS } from "./SpaceAreaInterface";
import { ShipInfo } from "./ShipInfo"
import { DOMF_SHIP_INFO_FOR_REGISTER } from "./DataObjectModifyFlags"
import { SPACE_AREA_REGISTER_SHIP_MEMBER_IDS,SPACE_AREA_UNREGISTER_SHIP_MEMBER_IDS,SPACE_AREA_FIND_SHIP_MEMBER_IDS,SPACE_AREA_REGISTER_SHIPYARD_MEMBER_IDS } from "./SpaceAreaInterface"


export class CSpaceAreaMsgCaller implements ISpaceArea	
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
		
		
		
	public RegisterShip(RegisterShipInfo: ShipInfo ): number
	{
		if((this.m_Operator == null) || (this.m_Operator == undefined))
		{
			return COMMON_RESULT_CODE.COMMON_RESULT_FAILED;
		}
	
		let _PackSize = CSmartStruct.GetEmptyStructSize();
		_PackSize += CSmartStruct.GetStructMemberSize(RegisterShipInfo.GetSmartStructSize(DOMF_SHIP_INFO_FOR_REGISTER));
		
		
	
		let MsgPacket = this.m_Operator.NewMessagePacket(_PackSize, this.m_TargetID.length);	
		MsgPacket.SenderID = this.m_SenderID;
		let MsgFlag=0;
		MsgPacket.MsgFlag = MsgFlag;
		
	    MsgPacket.MsgID = DOSMakeMsgID(MODULE_IDS.MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA,SPACE_AREA_METHODS.METHOD_REGISTER_SHIP,false);		
	
		
		let Packet = MsgPacket.EmptyDataPacket;
		
		if(this.PackMsgRegisterShip(Packet,RegisterShipInfo))
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
	
	public UnregisterShip(): number
	{
		if((this.m_Operator == null) || (this.m_Operator == undefined))
		{
			return COMMON_RESULT_CODE.COMMON_RESULT_FAILED;
		}
	
		let _PackSize = CSmartStruct.GetEmptyStructSize();
		
	
		let MsgPacket = this.m_Operator.NewMessagePacket(_PackSize, this.m_TargetID.length);	
		MsgPacket.SenderID = this.m_SenderID;
		let MsgFlag=0;
		MsgPacket.MsgFlag = MsgFlag;
		
	    MsgPacket.MsgID = DOSMakeMsgID(MODULE_IDS.MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA,SPACE_AREA_METHODS.METHOD_UNREGISTER_SHIP,false);		
	
		
		let Packet = MsgPacket.EmptyDataPacket;
		
		if(this.PackMsgUnregisterShip(Packet))
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
	
	public FindShip(): number
	{
		if((this.m_Operator == null) || (this.m_Operator == undefined))
		{
			return COMMON_RESULT_CODE.COMMON_RESULT_FAILED;
		}
	
		let _PackSize = CSmartStruct.GetEmptyStructSize();
		
	
		let MsgPacket = this.m_Operator.NewMessagePacket(_PackSize, this.m_TargetID.length);	
		MsgPacket.SenderID = this.m_SenderID;
		let MsgFlag=0;
		MsgPacket.MsgFlag = MsgFlag;
		
	    MsgPacket.MsgID = DOSMakeMsgID(MODULE_IDS.MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA,SPACE_AREA_METHODS.METHOD_FIND_SHIP,false);		
	
		
		let Packet = MsgPacket.EmptyDataPacket;
		
		if(this.PackMsgFindShip(Packet))
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
	
	public RegisterShipyard(): number
	{
		if((this.m_Operator == null) || (this.m_Operator == undefined))
		{
			return COMMON_RESULT_CODE.COMMON_RESULT_FAILED;
		}
	
		let _PackSize = CSmartStruct.GetEmptyStructSize();
		
	
		let MsgPacket = this.m_Operator.NewMessagePacket(_PackSize, this.m_TargetID.length);	
		MsgPacket.SenderID = this.m_SenderID;
		let MsgFlag=0;
		MsgPacket.MsgFlag = MsgFlag;
		
	    MsgPacket.MsgID = DOSMakeMsgID(MODULE_IDS.MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA,SPACE_AREA_METHODS.METHOD_REGISTER_SHIPYARD,false);		
	
		
		let Packet = MsgPacket.EmptyDataPacket;
		
		if(this.PackMsgRegisterShipyard(Packet))
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
	
	
	public PackMsgRegisterShip(Packet: CSmartStruct,RegisterShipInfo: ShipInfo ): boolean
	{
		let FailCount=0;
	
		{
			let SubPacket=Packet.PrepareSubStruct();
			if(!RegisterShipInfo.MakePacket(SubPacket,DOMF_SHIP_INFO_FOR_REGISTER))
				FailCount++;
			Packet.FinishMember(SPACE_AREA_REGISTER_SHIP_MEMBER_IDS.SST_REGISTER_SHIP_REGISTER_SHIP_INFO,SubPacket.GetDataLen());
		}
		
	
		return (FailCount==0);
	}
	
	public PackMsgUnregisterShip(Packet: CSmartStruct): boolean
	{
		let FailCount=0;
	
		
	
		return (FailCount==0);
	}
	
	public PackMsgFindShip(Packet: CSmartStruct): boolean
	{
		let FailCount=0;
	
		
	
		return (FailCount==0);
	}
	
	public PackMsgRegisterShipyard(Packet: CSmartStruct): boolean
	{
		let FailCount=0;
	
		
	
		return (FailCount==0);
	}
	
	
};
