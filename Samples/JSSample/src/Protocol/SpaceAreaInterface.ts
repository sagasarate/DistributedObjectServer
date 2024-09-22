import { CSmartValue, CSmartArray, CSmartStruct } from "../DOSSystem/SmartStruct";
import { OBJECT_ID } from "../DOSSystem/DOSSystem";
import { ShipInfo } from "./ShipInfo"


//
export interface ISpaceArea
{
	
	/*************************************************
	函数名:	RegisterShip
	用途:	
	参数:
		RegisterShipInfo				
	返回值:无特别意义
	*************************************************/
	RegisterShip(RegisterShipInfo: ShipInfo ): number
		
	
	/*************************************************
	函数名:	UnregisterShip
	用途:	
	参数:
		
	返回值:无特别意义
	*************************************************/
	UnregisterShip(): number
		
	
	/*************************************************
	函数名:	FindShip
	用途:	
	参数:
		
	返回值:无特别意义
	*************************************************/
	FindShip(): number
		
	
	/*************************************************
	函数名:	RegisterShipyard
	用途:	
	参数:
		
	返回值:无特别意义
	*************************************************/
	RegisterShipyard(): number
		
	
};

export const enum SPACE_AREA_METHODS
{
	METHOD_REGISTER_SHIP = 1,
	METHOD_UNREGISTER_SHIP = 2,
	METHOD_FIND_SHIP = 3,
	METHOD_REGISTER_SHIPYARD = 4,
	
};

export class SPACE_AREA_METHODS_UTILS
{
	public static ENUM_COUNT: number = 4;
};

export const enum SPACE_AREA_REGISTER_SHIP_MEMBER_IDS
{
	SST_REGISTER_SHIP_REGISTER_SHIP_INFO = 1,
	
};

export class SPACE_AREA_REGISTER_SHIP_MEMBER_IDS_UTILS
{
	public static ENUM_COUNT: number = 1;
};
export const enum SPACE_AREA_UNREGISTER_SHIP_MEMBER_IDS
{
	
};

export class SPACE_AREA_UNREGISTER_SHIP_MEMBER_IDS_UTILS
{
	public static ENUM_COUNT: number = 0;
};
export const enum SPACE_AREA_FIND_SHIP_MEMBER_IDS
{
	
};

export class SPACE_AREA_FIND_SHIP_MEMBER_IDS_UTILS
{
	public static ENUM_COUNT: number = 0;
};
export const enum SPACE_AREA_REGISTER_SHIPYARD_MEMBER_IDS
{
	
};

export class SPACE_AREA_REGISTER_SHIPYARD_MEMBER_IDS_UTILS
{
	public static ENUM_COUNT: number = 0;
};
