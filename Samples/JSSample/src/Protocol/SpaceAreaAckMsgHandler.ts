import { CSmartValue, CSmartArray, CSmartStruct } from "../DOSSystem/SmartStruct";
import { DOSMakeMsgID, DOS_MSG_MAP, OBJECT_ID } from "../DOSSystem/DOSSystem";
import { MODULE_IDS, DOSSAMPLE_INTERFACES } from "./DOSSampleSystemEnums";
import { ISpaceAreaAck } from "./SpaceAreaAckInterface";
import { SPACE_AREA_METHODS } from "./SpaceAreaInterface";
import { DOMF_SHIP_INFO_FULL } from "./DataObjectModifyFlags"
import { SPACE_AREA_ACK_REGISTER_SHIP_ACK_MEMBER_IDS,SPACE_AREA_ACK_UNREGISTER_SHIP_ACK_MEMBER_IDS,SPACE_AREA_ACK_FIND_SHIP_ACK_MEMBER_IDS,SPACE_AREA_ACK_REGISTER_SHIPYARD_ACK_MEMBER_IDS } from "./SpaceAreaAckInterface"


export class CSpaceAreaAckMsgHandler
{
	protected m_pMsgHandler: ISpaceAreaAck;
	public InitMsgMap(pMsgHandler: ISpaceAreaAck,MsgMap: DOS_MSG_MAP): void
	{
		this.m_pMsgHandler=pMsgHandler;
		MsgMap.set(DOSMakeMsgID(MODULE_IDS.MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA,SPACE_AREA_METHODS.METHOD_REGISTER_SHIP,true),{ThisPtr: this, HandlerFN: this.HandleMsgRegisterShipAck});
		MsgMap.set(DOSMakeMsgID(MODULE_IDS.MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA,SPACE_AREA_METHODS.METHOD_UNREGISTER_SHIP,true),{ThisPtr: this, HandlerFN: this.HandleMsgUnregisterShipAck});
		MsgMap.set(DOSMakeMsgID(MODULE_IDS.MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA,SPACE_AREA_METHODS.METHOD_FIND_SHIP,true),{ThisPtr: this, HandlerFN: this.HandleMsgFindShipAck});
		MsgMap.set(DOSMakeMsgID(MODULE_IDS.MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA,SPACE_AREA_METHODS.METHOD_REGISTER_SHIPYARD,true),{ThisPtr: this, HandlerFN: this.HandleMsgRegisterShipyardAck});
		
	}

	HandleMsgRegisterShipAck(Packet: CSmartStruct): number
	{
		let Result: number;
		
		
		Result=0;
		
	
		
		let Pos=Packet.GetFirstMemberPosition();
		while(Pos.Pos>0)
		{
			let PacketMember=Packet.GetNextMember(Pos);
			switch(Pos.MemberID)
			{
			case SPACE_AREA_ACK_REGISTER_SHIP_ACK_MEMBER_IDS.SST_REGISTER_SHIP_ACK_RESULT:
				{
					Result=PacketMember.valueOf();
				}
				break;
			
			}
		}
			
	
		return this.m_pMsgHandler.RegisterShipAck(Result);
	}
	HandleMsgUnregisterShipAck(Packet: CSmartStruct): number
	{
		let Result: number;
		
		
		Result=0;
		
	
		
		let Pos=Packet.GetFirstMemberPosition();
		while(Pos.Pos>0)
		{
			let PacketMember=Packet.GetNextMember(Pos);
			switch(Pos.MemberID)
			{
			case SPACE_AREA_ACK_UNREGISTER_SHIP_ACK_MEMBER_IDS.SST_UNREGISTER_SHIP_ACK_RESULT:
				{
					Result=PacketMember.valueOf();
				}
				break;
			
			}
		}
			
	
		return this.m_pMsgHandler.UnregisterShipAck(Result);
	}
	HandleMsgFindShipAck(Packet: CSmartStruct): number
	{
		let Result: number;
		let ShipID: OBJECT_ID = new OBJECT_ID;
		
		
		Result=0;
		ShipID.ID = 0n;
		
	
		
		let Pos=Packet.GetFirstMemberPosition();
		while(Pos.Pos>0)
		{
			let PacketMember=Packet.GetNextMember(Pos);
			switch(Pos.MemberID)
			{
			case SPACE_AREA_ACK_FIND_SHIP_ACK_MEMBER_IDS.SST_FIND_SHIP_ACK_RESULT:
				{
					Result=PacketMember.valueOf();
				}
				break;
			case SPACE_AREA_ACK_FIND_SHIP_ACK_MEMBER_IDS.SST_FIND_SHIP_ACK_SHIP_ID:
				{
					ShipID.ID = PacketMember.toLong();
				}
				break;
			
			}
		}
			
	
		return this.m_pMsgHandler.FindShipAck(Result,ShipID);
	}
	HandleMsgRegisterShipyardAck(Packet: CSmartStruct): number
	{
		let Result: number;
		
		
		Result=0;
		
	
		
		let Pos=Packet.GetFirstMemberPosition();
		while(Pos.Pos>0)
		{
			let PacketMember=Packet.GetNextMember(Pos);
			switch(Pos.MemberID)
			{
			case SPACE_AREA_ACK_REGISTER_SHIPYARD_ACK_MEMBER_IDS.SST_REGISTER_SHIPYARD_ACK_RESULT:
				{
					Result=PacketMember.valueOf();
				}
				break;
			
			}
		}
			
	
		return this.m_pMsgHandler.RegisterShipyardAck(Result);
	}
	
};
