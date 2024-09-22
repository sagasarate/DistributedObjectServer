import { CSmartValue, CSmartArray, CSmartStruct } from "../DOSSystem/SmartStruct";
import { OBJECT_ID } from "../DOSSystem/DOSSystem";
import { SHIP_TYPE } from "./DOSSampleSpaceShipEnums"


//
export interface IShipyard
{
	
	/*************************************************
	函数名:	CreateShip
	用途:	
	参数:
		ShipType				
		ShipName				
	返回值:无特别意义
	*************************************************/
	CreateShip(ShipType: SHIP_TYPE ,ShipName: string ): number
		
	
};

export const enum SHIPYARD_METHODS
{
	METHOD_CREATE_SHIP = 1,
	
};

export class SHIPYARD_METHODS_UTILS
{
	public static ENUM_COUNT: number = 1;
};

export const enum SHIPYARD_CREATE_SHIP_MEMBER_IDS
{
	SST_CREATE_SHIP_SHIP_TYPE = 1,
	SST_CREATE_SHIP_SHIP_NAME = 2,
	
};

export class SHIPYARD_CREATE_SHIP_MEMBER_IDS_UTILS
{
	public static ENUM_COUNT: number = 2;
};
