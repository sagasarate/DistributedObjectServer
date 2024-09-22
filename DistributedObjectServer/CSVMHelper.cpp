#include "stdafx.h"

CCSVMHelper::CCSVMHelper()
{
	m_pMonoMainDomain = NULL;
}
CCSVMHelper::~CCSVMHelper()
{

}
bool CCSVMHelper::Init()
{
	FUNCTION_BEGIN;

	const CDOSConfig::MONO_CONFIG& Config = CDOSConfig::GetInstance()->GetMonoConfig();

	CEasyString LogFileName = CFileTools::MakeModuleFullPath(NULL, "Log/Mono");

	CServerLogPrinter* pLog = MONITORED_NEW(_T("CCSVMHelper"), CServerLogPrinter, CDOSMainThread::GetInstance(), CServerLogPrinter::LOM_FILE | CServerLogPrinter::LOM_CONSOLE,
		CSystemConfig::GetInstance()->GetLogLevel(), LogFileName, CSystemConfig::GetInstance()->GetLogCacheSize());
	pLog->SetBackup(CSystemConfig::GetInstance()->GetLogBackupDir(), CSystemConfig::GetInstance()->GetLogBackupDelay());
	CLogManager::GetInstance()->AddChannel(LOG_MONO_CHANNEL, pLog);
	SAFE_RELEASE(pLog);


	CEasyString ModulePath = CFileTools::MakeModuleFullPath(NULL, Config.MonoModuleFileName);
	if (!MonoApiManager::LoadMonoApi(ModulePath))
	{
		LogMono("加载mono模块失败");
		return false;
	}

	mono_assembly_setrootdir(CFileTools::GetPathDirectory(ModulePath));

	m_MonoBaseGCTimer.SetTimeOut(Config.BaseGCInterval);
	m_MonoAdvanceGCTimer.SetTimeOut(Config.BaseGCInterval * Config.AdvanceGCMul);
	m_MonoFullGCTimer.SetTimeOut(Config.FullGCInterval);

	mono_trace_set_log_handler(MonoLog, this);
	mono_trace_set_print_handler(MonoPrint);
	mono_trace_set_printerr_handler(MonoPrintErr);

	if (!Config.LibraryDir.IsEmpty())
		mono_set_assemblies_path(CFileTools::MakeModuleFullPath(NULL, Config.LibraryDir));
		
	if (!Config.ConfigDir.IsEmpty())
		mono_set_config_dir(CFileTools::MakeModuleFullPath(NULL, Config.ConfigDir));
	if (Config.ConfigFilePath.IsEmpty())
		mono_config_parse(NULL);
	else
		mono_config_parse(CFileTools::MakeModuleFullPath(NULL, Config.ConfigFilePath));
	CEasyArray<char*> Args;
	CEasyArray<CEasyString> ArgStrs;
	Args.Add("--gc-params=major=marksweep-conc-par");
	if (!Config.Options.IsEmpty())
	{
		LogMono("虚拟机附加参数:%s", (LPCTSTR)Config.Options);
		
		CStringSplitter Splitter(Config.Options, "--");
		for (UINT i = 0; i < Splitter.GetCount(); i++)
		{
			CEasyString Arg = Splitter[i];
			Arg.Trim();
			if (Arg.GetLength())
				ArgStrs.Add("--" + Arg);
		}
		if (ArgStrs.GetCount())
		{
			Args.Reserve(Args.GetCount() + ArgStrs.GetCount());
			for (CEasyString& Arg : ArgStrs)
				Args.Add(Arg);
		}
	}

	mono_jit_parse_options(Args.GetCount(), Args.GetBuffer());

	CEasyString DebugConfigStr;
	if (Config.EnableDebug)
	{
		DebugConfigStr.Format("transport=dt_socket,address=%s:%u,server=y,suspend=%s",
			(LPCTSTR)Config.DebugListenHost,
			Config.DebugListenPort,
			Config.IsDebugSuspend ? "y" : "n");

		mono_debugger_agent_parse_options(DebugConfigStr);
		mono_debug_init(MONO_DEBUG_FORMAT_MONO);
	}

	m_pMonoMainDomain = mono_jit_init_version("DistributedObjectServer", "v4.0.30319");
	if (m_pMonoMainDomain)
	{
		RegisterMonoFunctions();

		theApp.ReInitSignals();

		LogMono("mono初始化完毕");
		return true;
	}
	else
	{
		LogMono("MonoDomain创建失败");
	}

	FUNCTION_END;
	return false;
}

void CCSVMHelper::Release()
{
	if (m_pMonoMainDomain)
	{
		mono_jit_cleanup(m_pMonoMainDomain);
		m_pMonoMainDomain = NULL;
	}
}

bool CCSVMHelper::InitPluginDomain(MONO_DOMAIN_INFO& MonoDomainInfo, LPCTSTR szName)
{
	if (m_pMonoMainDomain)
	{
		const CDOSConfig::MONO_CONFIG& Config = CDOSConfig::GetInstance()->GetMonoConfig();

		MonoDomainInfo.pMonoDomain = mono_domain_create_appdomain((char*)szName,
			(Config.ConfigFilePath.IsEmpty() ? NULL : (char*)((LPCTSTR)Config.ConfigFilePath)));

		if (MonoDomainInfo.pMonoDomain)
		{
			CEasyString SystemDllFileName = CFileTools::MakeModuleFullPath(NULL, Config.LibraryDir + DIR_SLASH + MONO_ASSEMBLY_DOSSYSTEM);
			MonoDomainInfo.pMonoAssembly_DOSSystem = mono_domain_assembly_open(MonoDomainInfo.pMonoDomain, SystemDllFileName);
			if (MonoDomainInfo.pMonoAssembly_DOSSystem)
			{
				MonoImage* pImage = mono_assembly_get_image(MonoDomainInfo.pMonoAssembly_DOSSystem);
				if (Config.EnableDebug)
					mono_debug_open_image_from_memory(pImage, NULL, 0);

				MonoDomainInfo.pMonoClass_ObjectID = MonoGetClass(pImage, MONO_NAME_SPACE_DOSSYSTEM, MONO_CLASS_NAME_OBJECT_ID);
				if (MonoDomainInfo.pMonoClass_ObjectID)
				{
					MonoDomainInfo.pMonoClassField_ObjectID_ID = MonoGetClassField(MonoDomainInfo.pMonoClass_ObjectID, MONO_CLASS_FIELD_NAME_OBJECT_ID_ID);
					MonoDomainInfo.pMonoClassMethod_ObjectID_Ctor = MonoGetClassMethod(MonoDomainInfo.pMonoClass_ObjectID,
						MONO_CLASS_METHOD_NAME_OBJECT_ID_CTOR, MONO_CLASS_METHOD_PARAM_OBJECT_ID_CTOR);
				}

				MonoDomainInfo.pMonoClass_DistributedObjectOperator = MonoGetClass(pImage, MONO_NAME_SPACE_DOSSYSTEM, MONO_CLASS_NAME_DISTRIBUTED_OBJECT_OPERATOR);
				if (MonoDomainInfo.pMonoClass_DistributedObjectOperator)
				{
					MonoDomainInfo.pMonoClassMethod_DistributedObjectOperator_Ctor = MonoGetClassMethod(MonoDomainInfo.pMonoClass_DistributedObjectOperator,
						MONO_CLASS_METHOD_NAME_DOO_CTOR, MONO_CLASS_METHOD_PARAM_DOO_CTOR);
				}

				MonoDomainInfo.MonoClassInfo_DORI.pClass = MonoGetClass(pImage, MONO_NAME_SPACE_DOSSYSTEM, MONO_CLASS_NAME_DOS_OBJ_REGISTER_INFO);
				if (MonoDomainInfo.MonoClassInfo_DORI.pClass)
				{
					MonoDomainInfo.MonoClassInfo_DORI.pFeildObjectID = MonoGetClassField(MonoDomainInfo.MonoClassInfo_DORI.pClass, MONO_CLASS_FIELD_NAME_DORI_OBJECT_ID);
					MonoDomainInfo.MonoClassInfo_DORI.pFeildObjectTypeName = MonoGetClassField(MonoDomainInfo.MonoClassInfo_DORI.pClass, MONO_CLASS_FIELD_NAME_DORI_OBJECT_TYPE_NAME);
					MonoDomainInfo.MonoClassInfo_DORI.pFeildWeight = MonoGetClassField(MonoDomainInfo.MonoClassInfo_DORI.pClass, MONO_CLASS_FIELD_NAME_DORI_WEIGHT);
					MonoDomainInfo.MonoClassInfo_DORI.pFeildObjectGroupIndex = MonoGetClassField(MonoDomainInfo.MonoClassInfo_DORI.pClass, MONO_CLASS_FIELD_NAME_DORI_OBJECT_GROUP_INDEX);
					MonoDomainInfo.MonoClassInfo_DORI.pFeildMsgQueueSize = MonoGetClassField(MonoDomainInfo.MonoClassInfo_DORI.pClass, MONO_CLASS_FIELD_NAME_DORI_MSG_QUEUE_SIZE);
					MonoDomainInfo.MonoClassInfo_DORI.pFeildMsgProcessLimit = MonoGetClassField(MonoDomainInfo.MonoClassInfo_DORI.pClass, MONO_CLASS_FIELD_NAME_DORI_MSG_PROCESS_LIMIT);
					MonoDomainInfo.MonoClassInfo_DORI.pFeildFlag = MonoGetClassField(MonoDomainInfo.MonoClassInfo_DORI.pClass, MONO_CLASS_FIELD_NAME_DORI_FLAG);
					MonoDomainInfo.MonoClassInfo_DORI.pFeildObject = MonoGetClassField(MonoDomainInfo.MonoClassInfo_DORI.pClass, MONO_CLASS_FIELD_NAME_DORI_OBJECT);
				}

				return true;
			}
			else
			{
				LogMono("无法加载系统库%s", (LPCTSTR)SystemDllFileName);
			}
		}
		else
		{
			LogMono("无法创建Domain");
		}
	}
	else
	{
		LogMono("Mono未初始化");
	}

	return false;
}


bool CCSVMHelper::ReleasePluginDomain(MONO_DOMAIN_INFO& MonoDomainInfo)
{
	if (MonoDomainInfo.pMonoDomain)
	{

		mono_domain_set(m_pMonoMainDomain, MONO_TRUE);
		MonoObject* pException = NULL;
		mono_domain_try_unload(MonoDomainInfo.pMonoDomain, &pException);
		if (pException)
		{
			ProcessMonoException(pException);
		}
		MonoDomainInfo.pMonoAssembly_DOSSystem = NULL;
		MonoDomainInfo.pMonoDomain = NULL;
	}
	MonoDomainInfo.pCSPluginAssembly = NULL;
	return true;
}

int CCSVMHelper::Update(int ProcessLimit)
{
	const CDOSConfig::MONO_CONFIG& Config = CDOSConfig::GetInstance()->GetMonoConfig();
	int ProcessCount = 0;
	if (m_MonoFullGCTimer.IsTimeOut())
	{
		m_MonoFullGCTimer.SetTimeOut(Config.FullGCInterval);
		mono_gc_collect(mono_gc_max_generation());
	}
	else if (m_MonoAdvanceGCTimer.IsTimeOut())
	{
		m_MonoAdvanceGCTimer.SetTimeOut(Config.BaseGCInterval * Config.AdvanceGCMul);
		mono_gc_collect(Config.AdvanceGCMul - 1);
	}
	else if (m_MonoBaseGCTimer.IsTimeOut())
	{
		m_MonoBaseGCTimer.SetTimeOut(Config.BaseGCInterval);
		mono_gc_collect(0);
	}
	return ProcessCount;
}

bool CCSVMHelper::IsMonoArrayElementTypeMatch(MonoArray* pArray, MonoClass* pClass)
{
	MonoClass* pArrayClass = mono_object_get_class((MonoObject*)pArray);
	if (pArrayClass)
	{
		MonoClass* pElementClass = mono_class_get_element_class(pArrayClass);
		if (pElementClass)
		{
			return mono_class_get_type_token(pElementClass) == mono_class_get_type_token(pClass);
		}
	}

	return false;
}


MonoArray* CCSVMHelper::MonoCreateByteArray(MONO_DOMAIN_INFO& DomainInfo, const void* pData, size_t DataSize)
{
	if (DomainInfo.pMonoDomain)
	{
		MonoArray* pArray = mono_array_new(DomainInfo.pMonoDomain, mono_get_byte_class(), DataSize);
		if (pArray)
		{
			if (pData && DataSize)
			{
				char* pBuff = mono_array_addr_with_size(pArray, sizeof(BYTE), 0);
				if (pBuff)
				{
					memcpy(pBuff, pData, DataSize);
				}
				else
				{
					Log("CCSVMHelper::MonoCreateByteArray:获得字节数组内存地址失败");
				}
			}
			return pArray;
		}
		else
		{
			Log("CCSVMHelper::MonoCreateByteArray:无法创建字节数组");
		}
	}
	return NULL;
}



BYTE* CCSVMHelper::MonoGetByteArray(MonoArray* pArray, size_t& DataSize)
{
	DataSize = mono_array_length(pArray);
	if (DataSize)
	{
		return (BYTE*)mono_array_addr_with_size(pArray, sizeof(BYTE), 0);
	}
	return NULL;
}

MSG_ID_TYPE* CCSVMHelper::MonoGetMsgIDArray(MonoArray* pArray, size_t& DataSize)
{
	DataSize = mono_array_length(pArray);
	if (DataSize)
	{
		return (MSG_ID_TYPE*)mono_array_addr_with_size(pArray, sizeof(MSG_ID_TYPE), 0);
	}
	return NULL;
}

OBJECT_ID CCSVMHelper::MonoGetObjectID(MONO_DOMAIN_INFO& DomainInfo, MonoObject* pObjectID)
{
	if (DomainInfo.pMonoClass_ObjectID && DomainInfo.pMonoClassField_ObjectID_ID && pObjectID)
	{
		MonoClass* pClass = mono_object_get_class(pObjectID);
		if (pClass)
		{
			if (mono_class_get_type_token(DomainInfo.pMonoClass_ObjectID) == mono_class_get_type_token(pClass))
			{
				OBJECT_ID ObjectID;
				mono_field_get_value(pObjectID, DomainInfo.pMonoClassField_ObjectID_ID, &ObjectID.ID);
				return ObjectID;
			}
			else
			{
				LogMono("对象类型不符合");
			}
		}
		else
		{
			LogMono("无法获取对象的类");
		}
	}
	return 0;
}

MonoString* CCSVMHelper::MonoCreateString(MONO_DOMAIN_INFO& DomainInfo, LPCTSTR szStr, size_t StrLen)
{
	if (DomainInfo.pMonoDomain)
	{
		if (StrLen <= 0)
			StrLen = _tcslen(szStr);
		return mono_string_new_len(DomainInfo.pMonoDomain, szStr, (UINT)StrLen);
	}
	return NULL;
}

MonoObject* CCSVMHelper::MonoCreateDistributedObjectOperator(MONO_DOMAIN_INFO& DomainInfo, CDistributedObjectOperatorCS* pOperator)
{
	if (DomainInfo.pMonoDomain && DomainInfo.pMonoClass_DistributedObjectOperator && DomainInfo.pMonoClassMethod_DistributedObjectOperator_Ctor)
	{
		MonoObject* pObject = mono_object_new(DomainInfo.pMonoDomain, DomainInfo.pMonoClass_DistributedObjectOperator);
		if (pObject)
		{
			MonoObject* pException = NULL;
			void* Params[1];
			Params[0] = &pOperator;
			mono_runtime_invoke(DomainInfo.pMonoClassMethod_DistributedObjectOperator_Ctor, pObject, Params, &pException);
			if (pException)
			{
				ProcessMonoException(pException);
			}
			else
			{
				return pObject;
			}
		}
		else
		{
			LogMono("创建对象失败");
		}
	}
	return NULL;
}


bool CCSVMHelper::MonoGetDORI(MONO_DOMAIN_INFO& DomainInfo, MonoObject* pObject, DOS_OBJECT_REGISTER_INFO_FOR_CS& RegisterInfo)
{
	DOS_OBJECT_REGISTER_INFO_FOR_CS Info;
	if (DomainInfo.MonoClassInfo_DORI.IsValid())
	{
		MonoClass* pClass = mono_object_get_class(pObject);
		if (pClass)
		{
			if (mono_class_get_type_token(DomainInfo.MonoClassInfo_DORI.pClass) == mono_class_get_type_token(pClass))
			{
				mono_field_get_value(pObject, DomainInfo.MonoClassInfo_DORI.pFeildObjectID, &RegisterInfo.ObjectID);
				MonoString* pStr = NULL;
				mono_field_get_value(pObject, DomainInfo.MonoClassInfo_DORI.pFeildObjectTypeName, &pStr);
				if (pStr)
				{
					char* szStr = mono_string_to_utf8(pStr);
					if (szStr)
					{
						RegisterInfo.ObjectTypeName = szStr;
						mono_free(szStr);
					}
				}
				mono_field_get_value(pObject, DomainInfo.MonoClassInfo_DORI.pFeildWeight, &RegisterInfo.Weight);
				mono_field_get_value(pObject, DomainInfo.MonoClassInfo_DORI.pFeildObjectGroupIndex, &RegisterInfo.ObjectGroupIndex);
				mono_field_get_value(pObject, DomainInfo.MonoClassInfo_DORI.pFeildMsgQueueSize, &RegisterInfo.MsgQueueSize);
				mono_field_get_value(pObject, DomainInfo.MonoClassInfo_DORI.pFeildMsgProcessLimit, &RegisterInfo.MsgProcessLimit);
				mono_field_get_value(pObject, DomainInfo.MonoClassInfo_DORI.pFeildFlag, &RegisterInfo.Flag);
				mono_field_get_value(pObject, DomainInfo.MonoClassInfo_DORI.pFeildObject, &RegisterInfo.pObject);

				return true;
			}
			else
			{
				LogMono("对象类型不符合");
			}
		}
		else
		{
			LogMono("无法获取对象的类");
		}
	}
	return false;
}

bool CCSVMHelper::MonoGetObjectIDList(MONO_DOMAIN_INFO& DomainInfo, MonoArray* pArray, CEasyArray<OBJECT_ID>& ObjectIDList)
{
	if (DomainInfo.pMonoDomain && DomainInfo.pMonoClass_ObjectID && DomainInfo.pMonoClassMethod_ObjectID_Ctor)
	{
		size_t Len = mono_array_length(pArray);
		if (Len)
		{
			ObjectIDList.Resize(Len);
			for (size_t i = 0; i < Len; i++)
			{
				UINT64* pID = (UINT64*)mono_array_addr_with_size(pArray, sizeof(UINT64), i);
				if (pID)
				{
					ObjectIDList[i].ID = *pID;
				}
			}
		}
		return true;
	}
	return false;
}

void CCSVMHelper::MonoLog(const char* log_domain, const char* log_level, const char* message, mono_bool fatal, void* user_data)
{
	PrintImportantLog("[MonoLog][%s]%s", log_level, message);
}

void CCSVMHelper::MonoPrint(const char* string, mono_bool is_stdout)
{
	PrintImportantLog("[MonoPrint]%s", string);
}

void CCSVMHelper::MonoPrintErr(const char* string, mono_bool is_stdout)
{
	PrintImportantLog("[MonoPrintErr]%s", string);
}




void CCSVMHelper::RegisterMonoFunctions()
{
	FUNCTION_BEGIN;
	mono_add_internal_call("DOSSystem.Logger::InternalLog(uint,DOSSystem.LOG_LEVEL,string,string)", (void*)CCSVMHelper::MonoInternalCallLog);

	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallGetRouterID()",
		(void*)CDistributedObjectOperatorCS::InternalCallGetRouterID);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallGetObjectID(intptr)",
		(void*)CDistributedObjectOperatorCS::InternalCallGetObjectID);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallGetGroupIndex(intptr)",
		(void*)CDistributedObjectOperatorCS::InternalCallGetGroupIndex);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallSendMessage(intptr,DOSSystem.OBJECT_ID,DOSSystem.OBJECT_ID,uint,uint16,byte[],int,int)",
		(void*)CDistributedObjectOperatorCS::InternalCallSendMessage);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallSendMessageMulti(intptr,DOSSystem.OBJECT_ID[],bool,DOSSystem.OBJECT_ID,uint,uint16,byte[],int,int)",
		(void*)CDistributedObjectOperatorCS::InternalCallSendMessageMulti);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallBroadcastMessageToProxyByMask(intptr,ushort,byte,ulong,DOSSystem.OBJECT_ID,uint,uint16,byte[],int,int)",
		(void*)CDistributedObjectOperatorCS::InternalCallBroadcastMessageToProxyByMask);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallBroadcastMessageToProxyByGroup(intptr,ushort,byte,ulong,DOSSystem.OBJECT_ID,uint,uint16,byte[],int,int)",
		(void*)CDistributedObjectOperatorCS::InternalCallBroadcastMessageToProxyByGroup);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallRegisterMsgMap(intptr,DOSSystem.OBJECT_ID,uint[])",
		(void*)CDistributedObjectOperatorCS::InternalCallRegisterMsgMap);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallUnregisterMsgMap(intptr,DOSSystem.OBJECT_ID,uint[])",
		(void*)CDistributedObjectOperatorCS::InternalCallUnregisterMsgMap);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallRegisterGlobalMsgMap(intptr,uint16,byte,uint,int)",
		(void*)CDistributedObjectOperatorCS::InternalCallRegisterGlobalMsgMap);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallUnregisterGlobalMsgMap(intptr,uint16,byte,uint)",
		(void*)CDistributedObjectOperatorCS::InternalCallUnregisterGlobalMsgMap);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallSetUnhanleMsgReceiver(intptr,uint16,byte)",
		(void*)CDistributedObjectOperatorCS::InternalCallSetUnhanleMsgReceiver);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallAddConcernedObject(intptr,DOSSystem.OBJECT_ID,bool)",
		(void*)CDistributedObjectOperatorCS::InternalCallAddConcernedObject);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallDeleteConcernedObject(intptr,DOSSystem.OBJECT_ID,bool)",
		(void*)CDistributedObjectOperatorCS::InternalCallDeleteConcernedObject);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallFindObject(intptr,uint,bool)",
		(void*)CDistributedObjectOperatorCS::InternalCallFindObject);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallReportObject(intptr,DOSSystem.OBJECT_ID,byte[],int,int)",
		(void*)CDistributedObjectOperatorCS::InternalCallReportObject);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallCloseProxyObject(intptr,DOSSystem.OBJECT_ID,uint)",
		(void*)CDistributedObjectOperatorCS::InternalCallCloseProxyObject);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallRequestProxyObjectIP(intptr,DOSSystem.OBJECT_ID)",
		(void*)CDistributedObjectOperatorCS::InternalCallRequestProxyObjectIP);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallRegisterObject(uint,DOSSystem.DOS_OBJ_REGISTER_INFO)",
		(void*)CDistributedObjectOperatorCS::InternalCallRegisterObjectStatic);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallRegisterObject(intptr,DOSSystem.DOS_OBJ_REGISTER_INFO)",
		(void*)CDistributedObjectOperatorCS::InternalCallRegisterObject);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallRelease(intptr)",
		(void*)CDistributedObjectOperatorCS::InternalCallRelease);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallQueryShutDown(intptr,DOSSystem.OBJECT_ID,byte,uint)",
		(void*)CDistributedObjectOperatorCS::InternalCallQueryShutDown);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallShutDown(intptr,uint)",
		(void*)CDistributedObjectOperatorCS::InternalCallShutDown);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallRegisterLogger(uint,string)",
		(void*)CDistributedObjectOperatorCS::InternalCallRegisterLogger);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallRegisterCSVLogger(uint,string,string)",
		(void*)CDistributedObjectOperatorCS::InternalCallRegisterCSVLogger);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallRegisterCommandReceiver(intptr)",
		(void*)CDistributedObjectOperatorCS::InternalCallRegisterCommandReceiver);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallUnregisterCommandReceiver(intptr)",
		(void*)CDistributedObjectOperatorCS::InternalCallUnregisterCommandReceiver);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallSetServerWorkStatus(intptr,byte)",
		(void*)CDistributedObjectOperatorCS::InternalCallSetServerWorkStatus);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallAddTimer(intptr,ulong,object,bool)",
		(void*)CDistributedObjectOperatorCS::InternalCallAddTimer);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallDeleteTimer(intptr,uint)",
		(void*)CDistributedObjectOperatorCS::InternalCallDeleteTimer);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallSetBroadcastMask(intptr,DOSSystem.OBJECT_ID,ulong)",
		(void*)CDistributedObjectOperatorCS::InternalCallSetBroadcastMask);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallAddBroadcastMask(intptr,DOSSystem.OBJECT_ID,ulong)",
		(void*)CDistributedObjectOperatorCS::InternalCallAddBroadcastMask);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallRemoveBroadcastMask(intptr,DOSSystem.OBJECT_ID,ulong)",
		(void*)CDistributedObjectOperatorCS::InternalCallRemoveBroadcastMask);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallAddBroadcastGroup(intptr,DOSSystem.OBJECT_ID,ulong)",
		(void*)CDistributedObjectOperatorCS::InternalCallAddBroadcastGroup);
	mono_add_internal_call("DOSSystem.DistributedObjectOperator::InternalCallRemoveBroadcastGroup(intptr,DOSSystem.OBJECT_ID,ulong)",
		(void*)CDistributedObjectOperatorCS::InternalCallRemoveBroadcastGroup);

	FUNCTION_END;
}



void CCSVMHelper::DumpMonoClass(MonoImage* pMonoImage, LPCTSTR szNameSpace, LPCTSTR szClassName)
{
	MonoClass* pClass = MonoGetClass(pMonoImage, szNameSpace, szClassName);
	if (pClass)
	{
		LogMono("%s.%s", szNameSpace, szClassName);
		void* itr = NULL;
		while (MonoClassField* pField = mono_class_get_fields(pClass, &itr))
		{
			LPCSTR szName = mono_field_get_name(pField);
			if (szName)
			{
				LPSTR szType = NULL;
				MonoType* pType = mono_field_get_type(pField);
				if (pType)
					szType = mono_type_get_name(pType);
				LogMono("    Field:%s %s", szType ? szType : "unknow", szName);
				SAFE_MONO_FREE(szType);
			}
		}
		itr = NULL;
		while (MonoMethod* pMethod = mono_class_get_methods(pClass, &itr))
		{
			CStringBuilder StrBuilder;
			LPCSTR szName = mono_method_get_name(pMethod);
			MonoMethodSignature* pSign = mono_method_signature(pMethod);
			if (szName && pSign)
			{
				LPSTR szType = NULL;
				MonoType* pType = mono_signature_get_return_type(pSign);
				if (pType)
					szType = mono_type_get_name(pType);
				StrBuilder.AppendFormated("    Method:%s %s(", szType ? szType : "unknow", szName);
				SAFE_MONO_FREE(szType);
				void* sub_itr = NULL;
				int Count = 0;
				while (pType = mono_signature_get_params(pSign, &sub_itr))
				{
					szType = NULL;
					if (pType)
						szType = mono_type_get_name(pType);
					if (Count)
						StrBuilder.AppendFormated(", %s", szType ? szType : "unknow");
					else
						StrBuilder.AppendFormated("%s", szType ? szType : "unknow");
					SAFE_MONO_FREE(szType);
					Count++;
				}
				StrBuilder.Append(")");
				LogMono("%s", (LPCTSTR)StrBuilder.GetStr());
			}
		}
	}
}

void AssemblyPrinter(void* assembly, void* user_data)
{
	MonoAssemblyName* pName = mono_assembly_get_name((MonoAssembly*)assembly);
	if (pName)
	{
		char* szName = mono_stringify_assembly_name(pName);
		LogMono("%s", szName);
		SAFE_MONO_FREE(szName);
	}
}
void CCSVMHelper::DumpAssemblys()
{
	LogMono("Assembly dump start");
	mono_assembly_foreach(AssemblyPrinter, NULL);
	LogMono("Assembly dump end");
}

MonoClass* CCSVMHelper::MonoGetClass(MonoImage* pMonoImage, LPCTSTR szNameSpace, LPCTSTR szClassName)
{
	MonoClass* pMonoClass = NULL;
	FUNCTION_BEGIN;
	pMonoClass = mono_class_from_name(pMonoImage, szNameSpace, szClassName);
	if (pMonoClass == NULL)
	{
		LogMono("无法找到类[%s.%s]",
			szNameSpace, szClassName);
	}
	FUNCTION_END;
	return pMonoClass;
}

MonoClassField* CCSVMHelper::MonoGetClassField(MonoClass* pMonoClass, LPCTSTR szFieldName, bool beRecursive)
{
	MonoClassField* pMonoClassField = NULL;
	do {
		pMonoClassField = mono_class_get_field_from_name(pMonoClass, szFieldName);
		if (pMonoClassField)
			break;
		if (beRecursive)
			pMonoClass = mono_class_get_parent(pMonoClass);
	} while (pMonoClass && beRecursive);
	if (pMonoClassField == NULL)
	{
		LogMono("无法在类[%s]中找到成员[%s]", mono_class_get_name(pMonoClass), szFieldName);
	}
	return pMonoClassField;
}

MonoMethod* CCSVMHelper::MonoGetClassMethod(MonoClass* pMonoClass, LPCTSTR szMethodName, int ParamCount, bool beRecursive)
{
	MonoMethod* pMonoMethod = NULL;
	do {
		pMonoMethod = mono_class_get_method_from_name(pMonoClass, szMethodName, ParamCount);
		if (pMonoMethod)
			break;
		if (beRecursive)
			pMonoClass = mono_class_get_parent(pMonoClass);
	} while (pMonoClass && beRecursive);
	if (pMonoMethod == NULL)
	{
		LogMono("无法在类[%s]中找到成员函数[%s,%d]", mono_class_get_name(pMonoClass), szMethodName, ParamCount);
	}
	return pMonoMethod;
}

void CCSVMHelper::ProcessMonoException(MonoObject* pException)
{
	MonoString* pMsg = mono_object_to_string(pException, NULL);
	if (pMsg)
	{
		char* pBuff = mono_string_to_utf8(pMsg);
		LogMono("%s", pBuff);
		mono_free(pBuff);
	}
}
void CCSVMHelper::MonoInternalCallLog(UINT LogChannel, int LogLevel, MonoString* pTag, MonoString* pMsg)
{
	CEasyString Tag = mono_string_to_easy_string(pTag);
	CEasyString Msg = mono_string_to_easy_string(pMsg);

	ILogPrinter* pLogPrinter = CLogManager::GetInstance()->GetChannel(LogChannel);
	if (pLogPrinter == NULL)
	{
		pLogPrinter = CLogManager::GetInstance()->GetChannel(LOG_MONO_CHANNEL);
	}
	pLogPrinter->PrintLogDirect(ILogPrinter::LOG_LEVEL_NORMAL, Tag, Msg);

}


