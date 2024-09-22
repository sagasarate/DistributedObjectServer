import { CSmartValue, CSmartArray, CSmartStruct } from "../DOSSystem/SmartStruct";
import { DOSMakeMsgID, DOS_MSG_MAP, OBJECT_ID } from "../DOSSystem/DOSSystem";
import { MODULE_IDS, DOSSAMPLE_INTERFACES } from "./DOSSampleSystemEnums";
import { IShipyard } from "./ShipyardInterface";
import { SHIPYARD_METHODS } from "./ShipyardInterface";
import { SHIP_TYPE } from "./DOSSampleSpaceShipEnums"
import { SHIPYARD_CREATE_SHIP_MEMBER_IDS } from "./ShipyardInterface"


export class CShipyardMsgHandler
{
	protected m_pMsgHandler: IShipyard;
	public InitMsgMap(pMsgHandler: IShipyard,MsgMap: DOS_MSG_MAP): void
	{
		this.m_pMsgHandler=pMsgHandler;
		MsgMap.set(DOSMakeMsgID(MODULE_IDS.MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SHIPYARD,SHIPYARD_METHODS.METHOD_CREATE_SHIP,false),{ThisPtr: this, HandlerFN: this.HandleMsgCreateShip});
		
	}

	HandleMsgCreateShip(Packet: CSmartStruct): number
	{
		let ShipType: SHIP_TYPE;
		let ShipName: string;
		
		
		ShipType=0;
		ShipName="";
		
	
		
		let Pos=Packet.GetFirstMemberPosition();
		while(Pos.Pos>0)
		{
			let PacketMember=Packet.GetNextMember(Pos);
			switch(Pos.MemberID)
			{
			case SHIPYARD_CREATE_SHIP_MEMBER_IDS.SST_CREATE_SHIP_SHIP_TYPE:
				{
					ShipType=PacketMember.valueOf();
				}
				break;
			case SHIPYARD_CREATE_SHIP_MEMBER_IDS.SST_CREATE_SHIP_SHIP_NAME:
				{
					ShipName=PacketMember.toString();
				}
				break;
			
			}
		}
			
	
		return this.m_pMsgHandler.CreateShip(ShipType,ShipName);
	}
	
};
