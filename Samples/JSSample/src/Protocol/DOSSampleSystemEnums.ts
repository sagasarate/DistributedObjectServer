

export const enum MODULE_IDS
{
	MODULE_ID_DOSSAMPLE = 1,
	
};

export class MODULE_IDS_UTILS
{
	public static ENUM_COUNT: number = 1;
};


export const enum GET_MODIFY_FLAG_TYPE
{
	GMFT_COPY = 0,
	GMFT_AND = 1,
	GMFT_OR = 2,
	
};

export class GET_MODIFY_FLAG_TYPE_UTILS
{
	public static ENUM_COUNT: number = 3;
};


export const enum DATA_OBJECT_SSTIDS
{
	SST_DO_SHIP_INFO = 100,
	
};

export class DATA_OBJECT_SSTIDS_UTILS
{
	public static ENUM_COUNT: number = 1;
};


export const enum DATA_OBJECT_FLAGS
{
	DATA_OBJECT_FLAG_SHIP_INFO = 0,
	DATA_OBJECT_FLAG_MAX = 1,
	
};

export class DATA_OBJECT_FLAGS_UTILS
{
	public static ENUM_COUNT: number = 2;
};


export const enum DOSSAMPLE_INTERFACES
{
	DOSSAMPLE_INTERFACE_SPACE_AREA = 1,
	DOSSAMPLE_INTERFACE_SHIP = 2,
	DOSSAMPLE_INTERFACE_SHIPYARD = 3,
	
};

export class DOSSAMPLE_INTERFACES_UTILS
{
	public static ENUM_COUNT: number = 3;
};



export const DOMF_ALL_FULL: bigint[]=[
		0xFFFFFFFFFFFFFFFFn];