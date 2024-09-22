using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using DOSSystem;
using DOSSample;

namespace DOSSample
{
	//
	public interface IShipyardAck
	{
		
		/*************************************************
		函数名:	CreateShipAck
		用途:	
		参数:
			Result				
		返回值:无特别意义
		*************************************************/
		int CreateShipAck(short Result );
			
		
	};
	
	
	public enum SHIPYARD_ACK_CREATE_SHIP_ACK_MEMBER_IDS
	{
		SST_CREATE_SHIP_ACK_RESULT = 3,
		
	};
	
	public class SHIPYARD_ACK_CREATE_SHIP_ACK_MEMBER_IDS_UTILS
	{
		public const int ENUM_COUNT = 1;
	};
	
};