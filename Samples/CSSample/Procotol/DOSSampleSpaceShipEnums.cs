using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
using DOSSystem;
using DOSSample;

namespace DOSSample
{	

	
	public enum SHIP_TYPE
	{
		SMALL = 0,
		MEDIUM = 1,
		LARGE = 2,
		
	};
	
	public class SHIP_TYPE_UTILS
	{
		public const int ENUM_COUNT = 3;
	};
	
	
	
	public enum OBJECT_TYPES
	{
		OBJECT_TYPE_SPACE_AREA = 0x11,
		OBJECT_TYPE_SHIPYARD = 0x12,
		OBJECT_TYPE_SHIP = 0x13,
		
	};
	
	public class OBJECT_TYPES_UTILS
	{
		public const int ENUM_COUNT = 3;
	};
	
	
	
	public enum MSG_RESULT
	{
		MSR_SUCEED = 0,
		MSR_FAILED = -2,
		MSR_NOT_EXIST = 1,
		
	};
	
	public class MSG_RESULT_UTILS
	{
		public const int ENUM_COUNT = 3;
	};
	
	
	
};
