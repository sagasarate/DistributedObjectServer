#pragma once



//
class IShipyard:public CBaseMsgHandler
{
public:
	
	enum SHIPYARD_METHODS
	{
		METHOD_CREATE_SHIP = 1,
		
	};
	
	
	
	
	
	/*************************************************
	函数名:	CreateShip
	用途:	
	参数:
		ShipType				
		ShipName				
	返回值:无特别意义
	*************************************************/
	virtual int CreateShip(SHIP_TYPE ShipType ,CEasyString ShipName ) {return COMMON_RESULT_MSG_NO_HANDLER;}
		
	
	
protected:	
	
	enum SHIPYARD_CREATE_SHIP_MEMBER_IDS
	{
		SST_CREATE_SHIP_SHIP_TYPE = 1,
		SST_CREATE_SHIP_SHIP_NAME = 2,
		
	};
	
	
};
