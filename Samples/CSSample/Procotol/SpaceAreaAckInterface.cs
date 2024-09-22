using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using DOSSystem;
using DOSSample;

namespace DOSSample
{
	//
	public interface ISpaceAreaAck
	{
		
		/*************************************************
		函数名:	RegisterShipAck
		用途:	
		参数:
			Result				
		返回值:无特别意义
		*************************************************/
		int RegisterShipAck(short Result );
			
		
		/*************************************************
		函数名:	UnregisterShipAck
		用途:	
		参数:
			Result				
		返回值:无特别意义
		*************************************************/
		int UnregisterShipAck(short Result );
			
		
		/*************************************************
		函数名:	FindShipAck
		用途:	
		参数:
			Result				
			ShipID				
		返回值:无特别意义
		*************************************************/
		int FindShipAck(short Result ,OBJECT_ID ShipID );
			
		
		/*************************************************
		函数名:	RegisterShipyardAck
		用途:	
		参数:
			Result				
		返回值:无特别意义
		*************************************************/
		int RegisterShipyardAck(short Result );
			
		
	};
	
	
	public enum SPACE_AREA_ACK_REGISTER_SHIP_ACK_MEMBER_IDS
	{
		SST_REGISTER_SHIP_ACK_RESULT = 3,
		
	};
	
	public class SPACE_AREA_ACK_REGISTER_SHIP_ACK_MEMBER_IDS_UTILS
	{
		public const int ENUM_COUNT = 1;
	};
	
	public enum SPACE_AREA_ACK_UNREGISTER_SHIP_ACK_MEMBER_IDS
	{
		SST_UNREGISTER_SHIP_ACK_RESULT = 1,
		
	};
	
	public class SPACE_AREA_ACK_UNREGISTER_SHIP_ACK_MEMBER_IDS_UTILS
	{
		public const int ENUM_COUNT = 1;
	};
	
	public enum SPACE_AREA_ACK_FIND_SHIP_ACK_MEMBER_IDS
	{
		SST_FIND_SHIP_ACK_RESULT = 1,
		SST_FIND_SHIP_ACK_SHIP_ID = 2,
		
	};
	
	public class SPACE_AREA_ACK_FIND_SHIP_ACK_MEMBER_IDS_UTILS
	{
		public const int ENUM_COUNT = 2;
	};
	
	public enum SPACE_AREA_ACK_REGISTER_SHIPYARD_ACK_MEMBER_IDS
	{
		SST_REGISTER_SHIPYARD_ACK_RESULT = 1,
		
	};
	
	public class SPACE_AREA_ACK_REGISTER_SHIPYARD_ACK_MEMBER_IDS_UTILS
	{
		public const int ENUM_COUNT = 1;
	};
	
};