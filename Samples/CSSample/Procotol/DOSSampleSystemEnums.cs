using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using DOSSystem;
using DOSSample;

namespace DOSSample
{	

	
	public enum MODULE_IDS
	{
		MODULE_ID_DOSSAMPLE = 1,
		
	};
	
	public class MODULE_IDS_UTILS
	{
		public const int ENUM_COUNT = 1;
	};
	
	
	
	public enum GET_MODIFY_FLAG_TYPE
	{
		GMFT_COPY = 0,
		GMFT_AND = 1,
		GMFT_OR = 2,
		
	};
	
	public class GET_MODIFY_FLAG_TYPE_UTILS
	{
		public const int ENUM_COUNT = 3;
	};
	
	
	
	public enum DATA_OBJECT_SSTIDS
	{
		SST_DO_SHIP_INFO = 100,
		
	};
	
	public class DATA_OBJECT_SSTIDS_UTILS
	{
		public const int ENUM_COUNT = 1;
	};
	
	
	
	public enum DATA_OBJECT_FLAGS
	{
		DATA_OBJECT_FLAG_SHIP_INFO = 0,
		DATA_OBJECT_FLAG_MAX = 1,
		
	};
	
	public class DATA_OBJECT_FLAGS_UTILS
	{
		public const int ENUM_COUNT = 2;
	};
	
	
	
	public enum DOSSAMPLE_INTERFACES
	{
		DOSSAMPLE_INTERFACE_SPACE_AREA = 1,
		DOSSAMPLE_INTERFACE_SHIP = 2,
		DOSSAMPLE_INTERFACE_SHIPYARD = 3,
		
	};
	
	public class DOSSAMPLE_INTERFACES_UTILS
	{
		public const int ENUM_COUNT = 3;
	};
	
	
	
};
