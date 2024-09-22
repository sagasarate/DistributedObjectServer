

export const enum SHIP_TYPE
{
	SMALL = 0,
	MEDIUM = 1,
	LARGE = 2,
	
};

export class SHIP_TYPE_UTILS
{
	public static ENUM_COUNT: number = 3;
};


export const enum OBJECT_TYPES
{
	OBJECT_TYPE_SPACE_AREA = 0x11,
	OBJECT_TYPE_SHIPYARD = 0x12,
	OBJECT_TYPE_SHIP = 0x13,
	
};

export class OBJECT_TYPES_UTILS
{
	public static ENUM_COUNT: number = 3;
};


export const enum MSG_RESULT
{
	MSR_SUCEED = 0,
	MSR_FAILED = -2,
	MSR_NOT_EXIST = 1,
	
};

export class MSG_RESULT_UTILS
{
	public static ENUM_COUNT: number = 3;
};


