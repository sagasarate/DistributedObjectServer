import { CSmartValue, CSmartArray, CSmartStruct } from "../DOSSystem/SmartStruct";
import { DOSMakeMsgID, DOS_MSG_MAP, OBJECT_ID } from "../DOSSystem/DOSSystem";
import { MODULE_IDS, DOSSAMPLE_INTERFACES } from "./DOSSampleSystemEnums";
import { IShip } from "./ShipInterface";
import { SHIP_METHODS } from "./ShipInterface";
import { SHIP_ATTACK_MEMBER_IDS } from "./ShipInterface"


export class CShipMsgHandler
{
	protected m_pMsgHandler: IShip;
	public InitMsgMap(pMsgHandler: IShip,MsgMap: DOS_MSG_MAP): void
	{
		this.m_pMsgHandler=pMsgHandler;
		MsgMap.set(DOSMakeMsgID(MODULE_IDS.MODULE_ID_DOSSAMPLE,DOSSAMPLE_INTERFACES.DOSSAMPLE_INTERFACE_SHIP,SHIP_METHODS.METHOD_ATTACK,false),{ThisPtr: this, HandlerFN: this.HandleMsgAttack});
		
	}

	HandleMsgAttack(Packet: CSmartStruct): number
	{
		let Damage: number;
		
		
		Damage=0;
		
	
		
		let Pos=Packet.GetFirstMemberPosition();
		while(Pos.Pos>0)
		{
			let PacketMember=Packet.GetNextMember(Pos);
			switch(Pos.MemberID)
			{
			case SHIP_ATTACK_MEMBER_IDS.SST_ATTACK_DAMAGE:
				{
					Damage=PacketMember.valueOf();
				}
				break;
			
			}
		}
			
	
		return this.m_pMsgHandler.Attack(Damage);
	}
	
};
