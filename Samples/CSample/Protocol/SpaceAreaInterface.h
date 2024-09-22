#pragma once



//
class ISpaceArea:public CBaseMsgHandler
{
public:
	
	enum SPACE_AREA_METHODS
	{
		METHOD_REGISTER_SHIP = 1,
		METHOD_UNREGISTER_SHIP = 2,
		METHOD_FIND_SHIP = 3,
		METHOD_REGISTER_SHIPYARD = 4,
		
	};
	
	
	
	
	
	/*************************************************
	函数名:	RegisterShip
	用途:	
	参数:
		RegisterShipInfo				
	返回值:无特别意义
	*************************************************/
	virtual int RegisterShip(ShipInfo RegisterShipInfo ) {return COMMON_RESULT_MSG_NO_HANDLER;}
		
	
	
	/*************************************************
	函数名:	UnregisterShip
	用途:	
	参数:
		
	返回值:无特别意义
	*************************************************/
	virtual int UnregisterShip() {return COMMON_RESULT_MSG_NO_HANDLER;}
		
	
	
	/*************************************************
	函数名:	FindShip
	用途:	
	参数:
		
	返回值:无特别意义
	*************************************************/
	virtual int FindShip() {return COMMON_RESULT_MSG_NO_HANDLER;}
		
	
	
	/*************************************************
	函数名:	RegisterShipyard
	用途:	
	参数:
		
	返回值:无特别意义
	*************************************************/
	virtual int RegisterShipyard() {return COMMON_RESULT_MSG_NO_HANDLER;}
		
	
	
protected:	
	
	enum SPACE_AREA_REGISTER_SHIP_MEMBER_IDS
	{
		SST_REGISTER_SHIP_REGISTER_SHIP_INFO = 1,
		
	};
	
	
	enum SPACE_AREA_UNREGISTER_SHIP_MEMBER_IDS
	{
		
	};
	
	
	enum SPACE_AREA_FIND_SHIP_MEMBER_IDS
	{
		
	};
	
	
	enum SPACE_AREA_REGISTER_SHIPYARD_MEMBER_IDS
	{
		
	};
	
	
};
