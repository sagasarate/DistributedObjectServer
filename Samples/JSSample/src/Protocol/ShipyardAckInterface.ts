import { CSmartValue, CSmartArray, CSmartStruct } from "../DOSSystem/SmartStruct";
import { OBJECT_ID } from "../DOSSystem/DOSSystem";


//
export interface IShipyardAck
{
	
	/*************************************************
	函数名:	CreateShipAck
	用途:	
	参数:
		Result				
	返回值:无特别意义
	*************************************************/
	CreateShipAck(Result: number ): number
		
	
};


export const enum SHIPYARD_ACK_CREATE_SHIP_ACK_MEMBER_IDS
{
	SST_CREATE_SHIP_ACK_RESULT = 3,
	
};

export class SHIPYARD_ACK_CREATE_SHIP_ACK_MEMBER_IDS_UTILS
{
	public static ENUM_COUNT: number = 1;
};
