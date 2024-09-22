using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using DOSSystem;
using DOSSample;

namespace DOSSample
{
	//
	public interface IShipyard
	{
		
		/*************************************************
		函数名:	CreateShip
		用途:	
		参数:
			ShipType				
			ShipName				
		返回值:无特别意义
		*************************************************/
		int CreateShip(SHIP_TYPE ShipType ,string ShipName );
			
		
	};
	
	public enum SHIPYARD_METHODS
	{
		METHOD_CREATE_SHIP = 1,
		
	};
	
	public class SHIPYARD_METHODS_UTILS
	{
		public const int ENUM_COUNT = 1;
	};
	
	
	public enum SHIPYARD_CREATE_SHIP_MEMBER_IDS
	{
		SST_CREATE_SHIP_SHIP_TYPE = 1,
		SST_CREATE_SHIP_SHIP_NAME = 2,
		
	};
	
	public class SHIPYARD_CREATE_SHIP_MEMBER_IDS_UTILS
	{
		public const int ENUM_COUNT = 2;
	};
	
};