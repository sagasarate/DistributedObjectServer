import { CSmartValue, CSmartArray, CSmartStruct } from "../DOSSystem/SmartStruct";
import { DOSMakeMsgID, DOS_MSG_MAP, OBJECT_ID } from "../DOSSystem/DOSSystem";
import { MODULE_IDS, DOSSAMPLE_INTERFACES } from "./DOSSampleSystemEnums";
import { IShipyardAck } from "./ShipyardAckInterface";
import { SHIPYARD_METHODS } from "./ShipyardInterface";
import { SHIPYARD_ACK_CREATE_SHIP_ACK_MEMBER_IDS } from "./ShipyardAckInterface"


export class CShipyardAckMsgHandler
{
	protected m_pMsgHandler: IShipyardAck;
	public InitMsgMap(pMsgHandler: IShipyardAck,MsgMap: DOS_MSG_MAP): void
	{
		this.m_pMsgHandler=pMsgHandler;
		MsgMap.set(DOSMakeMsgID(MODULE_IDS.MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SHIPYARD,SHIPYARD_METHODS.METHOD_CREATE_SHIP,true),{ThisPtr: this, HandlerFN: this.HandleMsgCreateShipAck});
		
	}

	HandleMsgCreateShipAck(Packet: CSmartStruct): number
	{
		let Result: number;
		
		
		Result=0;
		
	
		
		let Pos=Packet.GetFirstMemberPosition();
		while(Pos.Pos>0)
		{
			let PacketMember=Packet.GetNextMember(Pos);
			switch(Pos.MemberID)
			{
			case SHIPYARD_ACK_CREATE_SHIP_ACK_MEMBER_IDS.SST_CREATE_SHIP_ACK_RESULT:
				{
					Result=PacketMember.valueOf();
				}
				break;
			
			}
		}
			
	
		return this.m_pMsgHandler.CreateShipAck(Result);
	}
	
};
