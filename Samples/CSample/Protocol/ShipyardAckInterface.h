#pragma once



//
class IShipyardAck:public CBaseMsgHandler
{
public:
	
	
	
	
	/*************************************************
	函数名:	CreateShipAck
	用途:	
	参数:
		Result				
	返回值:无特别意义
	*************************************************/
	virtual int CreateShipAck(short Result ) {return COMMON_RESULT_MSG_NO_HANDLER;}
		
	
	
protected:	
	
	enum SHIPYARD_ACK_CREATE_SHIP_ACK_MEMBER_IDS
	{
		SST_CREATE_SHIP_ACK_RESULT = 3,
		
	};
	
	
};
