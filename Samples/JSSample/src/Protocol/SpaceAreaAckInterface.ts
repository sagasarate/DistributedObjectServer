import { CSmartValue, CSmartArray, CSmartStruct } from "../DOSSystem/SmartStruct";
import { OBJECT_ID } from "../DOSSystem/DOSSystem";


//
export interface ISpaceAreaAck
{
	
	/*************************************************
	函数名:	RegisterShipAck
	用途:	
	参数:
		Result				
	返回值:无特别意义
	*************************************************/
	RegisterShipAck(Result: number ): number
		
	
	/*************************************************
	函数名:	UnregisterShipAck
	用途:	
	参数:
		Result				
	返回值:无特别意义
	*************************************************/
	UnregisterShipAck(Result: number ): number
		
	
	/*************************************************
	函数名:	FindShipAck
	用途:	
	参数:
		Result				
		ShipID				
	返回值:无特别意义
	*************************************************/
	FindShipAck(Result: number ,ShipID: OBJECT_ID ): number
		
	
	/*************************************************
	函数名:	RegisterShipyardAck
	用途:	
	参数:
		Result				
	返回值:无特别意义
	*************************************************/
	RegisterShipyardAck(Result: number ): number
		
	
};


export const enum SPACE_AREA_ACK_REGISTER_SHIP_ACK_MEMBER_IDS
{
	SST_REGISTER_SHIP_ACK_RESULT = 3,
	
};

export class SPACE_AREA_ACK_REGISTER_SHIP_ACK_MEMBER_IDS_UTILS
{
	public static ENUM_COUNT: number = 1;
};
export const enum SPACE_AREA_ACK_UNREGISTER_SHIP_ACK_MEMBER_IDS
{
	SST_UNREGISTER_SHIP_ACK_RESULT = 1,
	
};

export class SPACE_AREA_ACK_UNREGISTER_SHIP_ACK_MEMBER_IDS_UTILS
{
	public static ENUM_COUNT: number = 1;
};
export const enum SPACE_AREA_ACK_FIND_SHIP_ACK_MEMBER_IDS
{
	SST_FIND_SHIP_ACK_RESULT = 1,
	SST_FIND_SHIP_ACK_SHIP_ID = 2,
	
};

export class SPACE_AREA_ACK_FIND_SHIP_ACK_MEMBER_IDS_UTILS
{
	public static ENUM_COUNT: number = 2;
};
export const enum SPACE_AREA_ACK_REGISTER_SHIPYARD_ACK_MEMBER_IDS
{
	SST_REGISTER_SHIPYARD_ACK_RESULT = 1,
	
};

export class SPACE_AREA_ACK_REGISTER_SHIPYARD_ACK_MEMBER_IDS_UTILS
{
	public static ENUM_COUNT: number = 1;
};
