import { CSmartValue, CSmartArray, CSmartStruct } from "../DOSSystem/SmartStruct";
import { DOSMakeMsgID, DOS_MSG_MAP, OBJECT_ID } from "../DOSSystem/DOSSystem";
import { MODULE_IDS, DOSSAMPLE_INTERFACES } from "./DOSSampleSystemEnums";
import { IShipAck } from "./ShipAckInterface";
import { SHIP_METHODS } from "./ShipInterface";


export class CShipAckMsgHandler
{
	protected m_pMsgHandler: IShipAck;
	public InitMsgMap(pMsgHandler: IShipAck,MsgMap: DOS_MSG_MAP): void
	{
		this.m_pMsgHandler=pMsgHandler;
		
	}

	
};
