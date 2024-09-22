using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using DOSSystem;
using DOSSample;

namespace DOSSample
{
	//
	public interface IShip
	{
		
		/*************************************************
		函数名:	Attack
		用途:	
		参数:
			Damage				
		返回值:无特别意义
		*************************************************/
		int Attack(int Damage );
			
		
	};
	
	public enum SHIP_METHODS
	{
		METHOD_ATTACK = 1,
		
	};
	
	public class SHIP_METHODS_UTILS
	{
		public const int ENUM_COUNT = 1;
	};
	
	
	public enum SHIP_ATTACK_MEMBER_IDS
	{
		SST_ATTACK_DAMAGE = 1,
		
	};
	
	public class SHIP_ATTACK_MEMBER_IDS_UTILS
	{
		public const int ENUM_COUNT = 1;
	};
	
};