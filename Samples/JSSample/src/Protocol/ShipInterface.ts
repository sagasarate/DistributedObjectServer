import { CSmartValue, CSmartArray, CSmartStruct } from "../DOSSystem/SmartStruct";
import { OBJECT_ID } from "../DOSSystem/DOSSystem";


//
export interface IShip
{
	
	/*************************************************
	函数名:	Attack
	用途:	
	参数:
		Damage				
	返回值:无特别意义
	*************************************************/
	Attack(Damage: number ): number
		
	
};

export const enum SHIP_METHODS
{
	METHOD_ATTACK = 1,
	
};

export class SHIP_METHODS_UTILS
{
	public static ENUM_COUNT: number = 1;
};

export const enum SHIP_ATTACK_MEMBER_IDS
{
	SST_ATTACK_DAMAGE = 1,
	
};

export class SHIP_ATTACK_MEMBER_IDS_UTILS
{
	public static ENUM_COUNT: number = 1;
};
