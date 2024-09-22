#pragma once



//
class ISpaceAreaAck:public CBaseMsgHandler
{
public:
	
	
	
	
	/*************************************************
	函数名:	RegisterShipAck
	用途:	
	参数:
		Result				
	返回值:无特别意义
	*************************************************/
	virtual int RegisterShipAck(short Result ) {return COMMON_RESULT_MSG_NO_HANDLER;}
		
	
	
	/*************************************************
	函数名:	UnregisterShipAck
	用途:	
	参数:
		Result				
	返回值:无特别意义
	*************************************************/
	virtual int UnregisterShipAck(short Result ) {return COMMON_RESULT_MSG_NO_HANDLER;}
		
	
	
	/*************************************************
	函数名:	FindShipAck
	用途:	
	参数:
		Result				
		ShipID				
	返回值:无特别意义
	*************************************************/
	virtual int FindShipAck(short Result ,OBJECT_ID ShipID ) {return COMMON_RESULT_MSG_NO_HANDLER;}
		
	
	
	/*************************************************
	函数名:	RegisterShipyardAck
	用途:	
	参数:
		Result				
	返回值:无特别意义
	*************************************************/
	virtual int RegisterShipyardAck(short Result ) {return COMMON_RESULT_MSG_NO_HANDLER;}
		
	
	
protected:	
	
	enum SPACE_AREA_ACK_REGISTER_SHIP_ACK_MEMBER_IDS
	{
		SST_REGISTER_SHIP_ACK_RESULT = 3,
		
	};
	
	
	enum SPACE_AREA_ACK_UNREGISTER_SHIP_ACK_MEMBER_IDS
	{
		SST_UNREGISTER_SHIP_ACK_RESULT = 1,
		
	};
	
	
	enum SPACE_AREA_ACK_FIND_SHIP_ACK_MEMBER_IDS
	{
		SST_FIND_SHIP_ACK_RESULT = 1,
		SST_FIND_SHIP_ACK_SHIP_ID = 2,
		
	};
	
	
	enum SPACE_AREA_ACK_REGISTER_SHIPYARD_ACK_MEMBER_IDS
	{
		SST_REGISTER_SHIPYARD_ACK_RESULT = 1,
		
	};
	
	
};
