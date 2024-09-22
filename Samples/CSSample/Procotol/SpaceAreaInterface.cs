using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using DOSSystem;
using DOSSample;

namespace DOSSample
{
	//
	public interface ISpaceArea
	{
		
		/*************************************************
		函数名:	RegisterShip
		用途:	
		参数:
			RegisterShipInfo				
		返回值:无特别意义
		*************************************************/
		int RegisterShip(ShipInfo RegisterShipInfo );
			
		
		/*************************************************
		函数名:	UnregisterShip
		用途:	
		参数:
			
		返回值:无特别意义
		*************************************************/
		int UnregisterShip();
			
		
		/*************************************************
		函数名:	FindShip
		用途:	
		参数:
			
		返回值:无特别意义
		*************************************************/
		int FindShip();
			
		
		/*************************************************
		函数名:	RegisterShipyard
		用途:	
		参数:
			
		返回值:无特别意义
		*************************************************/
		int RegisterShipyard();
			
		
	};
	
	public enum SPACE_AREA_METHODS
	{
		METHOD_REGISTER_SHIP = 1,
		METHOD_UNREGISTER_SHIP = 2,
		METHOD_FIND_SHIP = 3,
		METHOD_REGISTER_SHIPYARD = 4,
		
	};
	
	public class SPACE_AREA_METHODS_UTILS
	{
		public const int ENUM_COUNT = 4;
	};
	
	
	public enum SPACE_AREA_REGISTER_SHIP_MEMBER_IDS
	{
		SST_REGISTER_SHIP_REGISTER_SHIP_INFO = 1,
		
	};
	
	public class SPACE_AREA_REGISTER_SHIP_MEMBER_IDS_UTILS
	{
		public const int ENUM_COUNT = 1;
	};
	
	public enum SPACE_AREA_UNREGISTER_SHIP_MEMBER_IDS
	{
		
	};
	
	public class SPACE_AREA_UNREGISTER_SHIP_MEMBER_IDS_UTILS
	{
		public const int ENUM_COUNT = 0;
	};
	
	public enum SPACE_AREA_FIND_SHIP_MEMBER_IDS
	{
		
	};
	
	public class SPACE_AREA_FIND_SHIP_MEMBER_IDS_UTILS
	{
		public const int ENUM_COUNT = 0;
	};
	
	public enum SPACE_AREA_REGISTER_SHIPYARD_MEMBER_IDS
	{
		
	};
	
	public class SPACE_AREA_REGISTER_SHIPYARD_MEMBER_IDS_UTILS
	{
		public const int ENUM_COUNT = 0;
	};
	
};