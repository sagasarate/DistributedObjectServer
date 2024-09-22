#pragma once



//
class IShip:public CBaseMsgHandler
{
public:
	
	enum SHIP_METHODS
	{
		METHOD_ATTACK = 1,
		
	};
	
	
	
	
	
	/*************************************************
	函数名:	Attack
	用途:	
	参数:
		Damage				
	返回值:无特别意义
	*************************************************/
	virtual int Attack(int Damage ) {return COMMON_RESULT_MSG_NO_HANDLER;}
		
	
	
protected:	
	
	enum SHIP_ATTACK_MEMBER_IDS
	{
		SST_ATTACK_DAMAGE = 1,
		
	};
	
	
};
