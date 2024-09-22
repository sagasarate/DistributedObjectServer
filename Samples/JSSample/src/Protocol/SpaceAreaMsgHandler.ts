import { CSmartValue, CSmartArray, CSmartStruct } from "../DOSSystem/SmartStruct";
import { DOSMakeMsgID, DOS_MSG_MAP, OBJECT_ID } from "../DOSSystem/DOSSystem";
import { MODULE_IDS, DOSSAMPLE_INTERFACES } from "./DOSSampleSystemEnums";
import { ISpaceArea } from "./SpaceAreaInterface";
import { SPACE_AREA_METHODS } from "./SpaceAreaInterface";
import { ShipInfo } from "./ShipInfo"
import { DOMF_SHIP_INFO_FOR_REGISTER } from "./DataObjectModifyFlags"
import { SPACE_AREA_REGISTER_SHIP_MEMBER_IDS,SPACE_AREA_UNREGISTER_SHIP_MEMBER_IDS,SPACE_AREA_FIND_SHIP_MEMBER_IDS,SPACE_AREA_REGISTER_SHIPYARD_MEMBER_IDS } from "./SpaceAreaInterface"


export class CSpaceAreaMsgHandler
{
	protected m_pMsgHandler: ISpaceArea;
	public InitMsgMap(pMsgHandler: ISpaceArea,MsgMap: DOS_MSG_MAP): void
	{
		this.m_pMsgHandler=pMsgHandler;
		MsgMap.set(DOSMakeMsgID(MODULE_IDS.MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA,SPACE_AREA_METHODS.METHOD_REGISTER_SHIP,false),{ThisPtr: this, HandlerFN: this.HandleMsgRegisterShip});
		MsgMap.set(DOSMakeMsgID(MODULE_IDS.MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA,SPACE_AREA_METHODS.METHOD_UNREGISTER_SHIP,false),{ThisPtr: this, HandlerFN: this.HandleMsgUnregisterShip});
		MsgMap.set(DOSMakeMsgID(MODULE_IDS.MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA,SPACE_AREA_METHODS.METHOD_FIND_SHIP,false),{ThisPtr: this, HandlerFN: this.HandleMsgFindShip});
		MsgMap.set(DOSMakeMsgID(MODULE_IDS.MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SPACE_AREA,SPACE_AREA_METHODS.METHOD_REGISTER_SHIPYARD,false),{ThisPtr: this, HandlerFN: this.HandleMsgRegisterShipyard});
		
	}

	HandleMsgRegisterShip(Packet: CSmartStruct): number
	{
		let RegisterShipInfo: ShipInfo=new ShipInfo();
		
		
		RegisterShipInfo.Clear();
		
	
		
		let Pos=Packet.GetFirstMemberPosition();
		while(Pos.Pos>0)
		{
			let PacketMember=Packet.GetNextMember(Pos);
			switch(Pos.MemberID)
			{
			case SPACE_AREA_REGISTER_SHIP_MEMBER_IDS.SST_REGISTER_SHIP_REGISTER_SHIP_INFO:
				{
					RegisterShipInfo.ParsePacket(PacketMember.toSmartStruct(),DOMF_SHIP_INFO_FOR_REGISTER);
				}
				break;
			
			}
		}
			
	
		return this.m_pMsgHandler.RegisterShip(RegisterShipInfo);
	}
	HandleMsgUnregisterShip(Packet: CSmartStruct): number
	{
		
		
		
	
		return this.m_pMsgHandler.UnregisterShip();
	}
	HandleMsgFindShip(Packet: CSmartStruct): number
	{
		
		
		
	
		return this.m_pMsgHandler.FindShip();
	}
	HandleMsgRegisterShipyard(Packet: CSmartStruct): number
	{
		
		
		
	
		return this.m_pMsgHandler.RegisterShipyard();
	}
	
};
