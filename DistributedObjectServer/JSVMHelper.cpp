#include "stdafx.h"


thread_local CEasyArray<JSVM_VM_INFO> CJSVMHelper::m_VMInfoList;


bool JSVM_VM_INFO::Init(UINT uiPluginID, UINT uiLogChannel, int iGroupIndex, LPCTSTR szMainMoudulePath, LPCTSTR szConfigDir, LPCTSTR szLogDir)
{
	if (!CFileTools::IsFileExist(szMainMoudulePath))
	{
		LogJSVM("主模块文件%s不存在", szMainMoudulePath);
		return false;
	}

	if (!CJSVMHelper::GetInstance()->GetMainVM())
	{
		LogJSVM("主实例未创建");
		return false;
	}

	bool InitSucceed = false;
	PluginID = uiPluginID;
	LogChannel = uiLogChannel;
	GroupIndex = iGroupIndex;
	MainMoudulePath = szMainMoudulePath;
	ConfigDir = szConfigDir;
	LogDir = szLogDir;

	CEasyString RootDir = CFileTools::GetPathDirectory(szMainMoudulePath);

	CEasyString ChildName;
	ChildName.Format("Group%d", GroupIndex);
	JSVM::child_vm_setting child_setting;
	child_setting.parent_vm = CJSVMHelper::GetInstance()->GetMainVM();
	child_setting.child_thread_id = GetCurThreadID();
	child_setting.child_url = MainMoudulePath;
	child_setting.child_name = ChildName;
	pVM = JSVM::jsvm_create_vm(RootDir, NULL, JSVM::jsvm_keep_event_loop_active, &child_setting);

	if (!pVM)
	{
		LogJSVM("新建JSVM实例失败");
		return false;
	}
	JSVM::jsvm_set_console_log_channel(pVM, LogChannel);
	GCTimer.SaveTime();
	ReleaseTimer.SetTimeOut(PLUGIN_RELASE_CHECK_TIME);
	ExportClasses.Create(16, 16, 16);
	JSVM::jsvm_handle_scope* scope = JSVM::jsvm_create_handle_scope(pVM);
	JSVM::jsvm_context* context = JSVM::jsvm_get_context(scope);
	JSVM::CAutoDisposer<JSVM::jsvm_handle_scope, JSVM::jsvm_release_handle_scope> Disposer(scope);

	PersistentValues[JSVMPersistentValues_IDistributedObjectOperator] = CDistributedObjectOperatorJS::CreateJSClass_IDistributedObjectOperator(context);
	PersistentValues[JSVMPersistentValues_IDistributedObjectManager] = CDistributedObjectOperatorJS::CreateJSClass_IDistributedObjectManager(context);
	PersistentValues[JSVMPersistentValues_IDOSMessage] = CDistributedObjectOperatorJS::CreateJSClass_IDOSMessage(context);
	PersistentValues[JSVMPersistentValues_IDOSMessagePacket] = CDistributedObjectOperatorJS::CreateJSClass_IDOSMessagePacket(context);

	JSVM::jsvm_value* value = JSVM::jsvm_create_uint32(context, PluginID);
	JSVM::jsvm_global_set(context, "$PLUGIN_ID", value);

	JSVM::jsvm_tmpl* tmpl = JSVM::jsvm_create_func_tmpl(context, JSVM::CallWrap<decltype(&CJSVMHelper::JSDOSPrintLog), &CJSVMHelper::JSDOSPrintLog>::CallbackFunc, JSVM::jsvm_func_type::jsvm_func_standalone, NULL, "DOSPrintLog");
	if (tmpl)
	{
		value = JSVM::jsvm_create_func(context, tmpl);
		if (value)
			JSVM::jsvm_global_set(context, "DOSPrintLog", value);
	}

	JSVM::JSCallGlobal<JSVM::jsvm_value*>(context, "require", szMainMoudulePath);
	if (JSVM::jsvm_process_exception(scope))
		return false;

	//回填信息
	JSVM::jsvm_value* plugin_info = JSVM::jsvm_global_get(context, "$PLUGIN_INFO");
	if (plugin_info && JSVM::jsvm_is_object(plugin_info))
	{
		JSVM::jsvm_value* value = JSVM::jsvm_create_uint32(context, PluginID);
		JSVM::jsvm_obj_set(context, plugin_info, "PluginID", value);

		value = JSVM::jsvm_create_uint32(context, LogChannel);
		JSVM::jsvm_obj_set(context, plugin_info, "LogChannel", value);

		value = JSVM::jsvm_create_int32(context, GroupIndex);
		JSVM::jsvm_obj_set(context, plugin_info, "GroupIndex", value);

		value = JSVM::jsvm_create_string_utf8(context, ConfigDir);
		JSVM::jsvm_obj_set(context, plugin_info, "ConfigDir", value);

		value = JSVM::jsvm_create_string_utf8(context, LogDir);
		JSVM::jsvm_obj_set(context, plugin_info, "LogDir", value);
	}
	else
	{
		LogJSVM("插件缺少全局变量:$PLUGIN_INFO");
		return false;
	}

	JSVM::jsvm_value* func = JSVM::jsvm_obj_get(context, plugin_info, PLUGIN_INIT_FN_NAME);
	if (!func || !JSVM::jsvm_is_function(func))
	{
		JSVM::jsvm_throw_exception_with_format(context, "全局变量$PLUGIN_INFO缺少插件初始化函数:%s", PLUGIN_INIT_FN_NAME);
		return false;
	}
	if (PersistentValues[JSVMPersistentValues_PluginInitFn])
		JSVM::jsvm_release_persistent_value(PersistentValues[JSVMPersistentValues_PluginInitFn]);
	PersistentValues[JSVMPersistentValues_PluginInitFn] = JSVM::jsvm_create_persistent_value(context, func);

	func = JSVM::jsvm_obj_get(context, plugin_info, PLUGIN_CHECK_RELEASE_FN_NAME);
	if (!func || !JSVM::jsvm_is_function(func))
	{
		JSVM::jsvm_throw_exception_with_format(context, "全局变量$PLUGIN_INFO缺少插件释放函数:%s", PLUGIN_CHECK_RELEASE_FN_NAME);
		return false;
	}
	if (PersistentValues[JSVMPersistentValues_PluginCheckReleaseFn])
		JSVM::jsvm_release_persistent_value(PersistentValues[JSVMPersistentValues_PluginCheckReleaseFn]);
	PersistentValues[JSVMPersistentValues_PluginCheckReleaseFn] = JSVM::jsvm_create_persistent_value(context, func);


	JSVM::jsvm_value* Classes = JSVM::jsvm_obj_get(context, plugin_info, "ExportClasses");
	if (!JSVM::jsvm_is_array(Classes))
	{
		LogJSVM("全局变量$PLUGIN_INFO缺少成员ExportClasses或者ExportClasses不是数组");
		return false;
	}
	UINT Len = JSVM::jsvm_get_array_length(context, Classes);
	for (UINT i = 0; i < Len; i++)
	{
		JSVM::jsvm_value* Class = JSVM::jsvm_obj_get_by_index(context, Classes, i);
		if (Class && JSVM::jsvm_is_function(Class))
		{
			if (JSVM::jsvm_is_class(context, Class, "OBJECT_ID"))
			{
				if (PersistentValues[JSVMPersistentValues_OBJECT_ID])
					JSVM::jsvm_release_persistent_value(PersistentValues[JSVMPersistentValues_OBJECT_ID]);
				PersistentValues[JSVMPersistentValues_OBJECT_ID] = JSVM::jsvm_create_persistent_value(context, Class);
			}
			else if (JSVM::jsvm_is_class(context, Class, "CSmartStruct"))
			{
				if (PersistentValues[JSVMPersistentValues_CSmartStruct])
					JSVM::jsvm_release_persistent_value(PersistentValues[JSVMPersistentValues_CSmartStruct]);
				PersistentValues[JSVMPersistentValues_CSmartStruct] = JSVM::jsvm_create_persistent_value(context, Class);
			}
			else
			{
				JSVM::jsvm_value* class_name = JSVM::jsvm_class_get_name(context, Class);
				if (class_name && JSVM::jsvm_is_string(class_name))
				{
					const char* name = JSVM::jsvm_get_value_string_utf8_no_buff(context, class_name);
					JS_CLASS_INFO* pInfo = NULL;
					ExportClasses.New(name, &pInfo);
					if (pInfo)
					{
						pInfo->Name = name;
						pInfo->Class = JSVM::jsvm_create_persistent_value(context, Class);
					}
					else
					{
						LogJSVM("导出类存储池溢出:%u/%u", ExportClasses.GetObjectCount(), ExportClasses.GetBufferSize());
					}
				}
			}
		}
	}

	if (PersistentValues[JSVMPersistentValues_OBJECT_ID] == NULL)
	{
		LogJSVM("$PLUGIN_INFO.ExportClasses未包含类:OBJECT_ID");
		return false;
	}
	if (PersistentValues[JSVMPersistentValues_CSmartStruct] == NULL)
	{
		LogJSVM("$PLUGIN_INFO.ExportClasses未包含类:CSmartStruct");
		return false;
	}
	return 	true;
}
void JSVM_VM_INFO::Release()
{
	for (JSVM::jsvm_persistent_value* Value : PersistentValues)
	{
		if (Value)
			JSVM::jsvm_release_persistent_value(Value);
	}
	ZeroMemory(PersistentValues, sizeof(PersistentValues));
	for (JS_CLASS_INFO& Info : ExportClasses)
	{
		if (Info.Class)
			JSVM::jsvm_release_persistent_value(Info.Class);
	}
	ExportClasses.Clear();

	if (pVM)
		JSVM::jsvm_vm_dispose(pVM);
	pVM = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CJSVMHelper::CJSVMHelper()
{
	m_pMainVM = NULL;
}
CJSVMHelper::~CJSVMHelper()
{
	Release();
}

bool CJSVMHelper::Init()
{
	CEasyString LogFileName = CFileTools::MakeModuleFullPath(NULL, "Log/JSVM");

	CServerLogPrinter* pLog = MONITORED_NEW(_T("CJSVMHelper"), CServerLogPrinter, CDOSMainThread::GetInstance(), CServerLogPrinter::LOM_FILE | CServerLogPrinter::LOM_CONSOLE,
		CSystemConfig::GetInstance()->GetLogLevel(), LogFileName, CSystemConfig::GetInstance()->GetLogCacheSize());
	pLog->SetBackup(CSystemConfig::GetInstance()->GetLogBackupDir(), CSystemConfig::GetInstance()->GetLogBackupDelay());
	CLogManager::GetInstance()->AddChannel(LOG_JSVM_CHANNEL, pLog);
	SAFE_RELEASE(pLog);

	const CDOSConfig::JSVM_CONFIG& Config = CDOSConfig::GetInstance()->GetJSVMConfig();
	CEasyString ModulePath = CFileTools::MakeModuleFullPath(NULL, Config.JSVMModuleFileName);
	if (!JSVM::JSVMApiManager::LoadApi(ModulePath))
	{
		LogJSVM("加载JSVM模块失败");
		return false;
	}
	m_GCTimer.SetTimeOut(Config.InitiativeGCInterval);

	CEasyString Params;
	if (Config.EnableDebug)
	{
		if (Config.IsDebugSuspend)
			Params.Format("--expose-internals --experimental-require-module --inspect-wait=%s:%u %s", (LPCTSTR)Config.DebugListenHost, Config.DebugListenPort, (LPCTSTR)Config.Options);
		else
			Params.Format("--expose-internals --experimental-require-module --inspect=%s:%u %s", (LPCTSTR)Config.DebugListenHost, Config.DebugListenPort, (LPCTSTR)Config.Options);
	}
	else
	{
		Params.Format("--expose-internals --experimental-require-module %s", (LPCTSTR)Config.Options);
	}
	if (JSVM::jsvm_platform_init(Params))
	{
		LogJSVM("JSVM模块初始化完毕,启动参数:%s", (LPCTSTR)Params);
		m_pMainVM = JSVM::jsvm_create_vm(CFileTools::GetPathDirectory(ModulePath), NULL, JSVM::jsvm_keep_event_loop_active);
		if (m_pMainVM)
		{
			LogJSVM("创建主实例成功");
			return true;
		}
		else
		{
			LogJSVM("创建主实例失败");
			return false;
		}
	}
	else
	{
		LogJSVM("初始化JSVM模块失败");
		return false;
	}
}
void CJSVMHelper::Release()
{
	CAutoLock Lock(m_CriticalSection);
	if (m_pMainVM)
	{
		JSVM::jsvm_vm_dispose(m_pMainVM);
		m_pMainVM = NULL;
	}
	for (JSVM_VM_INFO* pInfo : m_AllVMList)
	{
		pInfo->Release();
	}
	m_AllVMList.Clear();
	JSVM::jsvm_platform_dispose();
}

int CJSVMHelper::Update(int ProcessLimit)
{
	int ProcessCount = 0;
	if (m_pMainVM)
	{
		JSVM::jsvm_handle_scope* scope = JSVM::jsvm_create_handle_scope(m_pMainVM);
		JSVM::jsvm_event_loop(scope, ProcessLimit);
		JSVM::jsvm_release_handle_scope(scope);
	}

	OnDOSGroupUpdate(-1, ProcessLimit);
	return ProcessCount;
}


JSVM_VM_INFO* CJSVMHelper::CreateJSVM(UINT PluginID, UINT LogChannel, int GroupIndex, LPCTSTR szMainMoudulePath, LPCTSTR szConfigDir, LPCTSTR szLogDir)
{
	JSVM_VM_INFO* pVMInfo = m_VMInfoList.AddEmpty();
	if (pVMInfo->Init(PluginID, LogChannel, GroupIndex, szMainMoudulePath, szConfigDir, szLogDir))
	{
		RegisterVMInfo(pVMInfo);
		return pVMInfo;
	}
	else
	{
		pVMInfo->Release();
		m_VMInfoList.Delete(m_VMInfoList.GetCount() - 1);
		return NULL;
	}
}

JSVM_VM_INFO* CJSVMHelper::FindJSVM(UINT PluginID, int GroupIndex)
{
	CAutoLock Lock(m_CriticalSection);

	JSVM_VM_INFO** ppInfo = m_AllVMList.Find([PluginID, GroupIndex](JSVM_VM_INFO* pInfo)->bool {return pInfo->PluginID == PluginID && pInfo->GroupIndex == GroupIndex; });
	if (ppInfo)
		return *ppInfo;
	return NULL;
}

JSVM_VM_INFO* CJSVMHelper::GetCurJSVM(JSVM::jsvm_context* context)
{
	JSVM::jsvm_value* plugin_id = JSVM::jsvm_global_get(context, "$PLUGIN_ID");
	UINT PluginID = JSVM::jsvm_get_value_uint32(context, plugin_id);
	if (PluginID)
		return GetCurJSVM(PluginID);
	else
		JSVM::jsvm_throw_exception(context, "global var $PLUGIN_ID not exist");
	return NULL;
}

bool CJSVMHelper::RunPlugin(UINT PluginID, UINT LogChannel, LPCTSTR ConfigDir, LPCTSTR LogDir)
{
	JSVM_VM_INFO* pVMInfo = GetCurJSVM(PluginID);
	if (pVMInfo && pVMInfo->GroupIndex < 0)
	{
		JSVM::jsvm_handle_scope* scope = JSVM::jsvm_create_handle_scope(pVMInfo->pVM);
		JSVM::jsvm_context* context = JSVM::jsvm_get_context(scope);
		JSVM::CAutoDisposer<JSVM::jsvm_handle_scope, JSVM::jsvm_release_handle_scope> Disposer(scope);

		JSVM::jsvm_value* init_fn = NULL;
		if (pVMInfo->PersistentValues[JSVMPersistentValues_PluginInitFn])
			init_fn = JSVM::jsvm_get_value_from_persistent_value(context, pVMInfo->PersistentValues[JSVMPersistentValues_PluginInitFn]);
		bool ret = false;
		if (init_fn && JSVM::jsvm_is_function(init_fn))
		{
			JSVM::jsvm_value* obj_manager_class = NULL;
			if (pVMInfo->PersistentValues[JSVMPersistentValues_IDistributedObjectManager])
				obj_manager_class = JSVM::jsvm_get_value_from_persistent_value(context, pVMInfo->PersistentValues[JSVMPersistentValues_IDistributedObjectManager]);
			if (obj_manager_class)
			{
				JSVM::jsvm_value* obj_manager = JSVM::jsvm_new_obj(context, obj_manager_class);
				if (obj_manager && JSVM::jsvm_is_object(obj_manager))
				{
					JSVM::jsvm_obj_set_internal_field(context, obj_manager, (int)JSVM::JSVMInternalFieldIndex::This, CDOSMainThread::GetInstance()->GetDistributedObjectManager());
					ret = JSVM::JSCallGlobal<bool>(context, init_fn, obj_manager, PluginID, LogChannel, ConfigDir, LogDir);
				}
				else
				{
					LogJSVM("无法创建对象：IDistributedObjectManager");
				}
			}
			else
			{
				LogJSVM("无法获得类：IDistributedObjectManager");
			}
		}
		else
		{
			LogJSVM("无法获得插件初始化函数");
		}
		if (JSVM::jsvm_get_exception(scope, NULL, 0))
		{
			char Msg[4096];
			JSVM::jsvm_get_exception(scope, Msg, 4096);
			LogJSVM("%s", Msg);
		}

		return ret;
	}
	else
	{
		LogJSVM("无法找到插件%u的主实例", PluginID);
	}
	return false;
}

bool CJSVMHelper::QueryReleasePlugin(UINT PluginID)
{
	CAutoLock Lock(m_CriticalSection);

	int AllReleased = true;
	for (JSVM_VM_INFO* pJSVMInfo : m_AllVMList)
	{
		if (pJSVMInfo->PluginID == PluginID)
		{
			pJSVMInfo->NeedRelease = true;
			AllReleased = false;
		}

	}
	return AllReleased;
}

int CJSVMHelper::OnDOSGroupUpdate(int GroupIndex, int ProcessLimit)
{
	int ProcessCount = 0;
	const CDOSConfig::JSVM_CONFIG& Config = CDOSConfig::GetInstance()->GetJSVMConfig();
	for (int i = (int)m_VMInfoList.GetCount() - 1; i >= 0; i--)
	{
		JSVM_VM_INFO* pVMInfo = m_VMInfoList.GetObject(i);
		if (pVMInfo->pVM == NULL)
		{
			CJSVMHelper::GetInstance()->UnegisterVMInfo(pVMInfo);
			m_VMInfoList.Delete(i);
			continue;
		}
		if (pVMInfo->GroupIndex != GroupIndex)
		{
			LogJSVM("未在JSVM所属线程组执行更新(JSVM=%d,GroupIndex=%d)", pVMInfo->GroupIndex, GroupIndex);
			break;
		}
		JSVM::jsvm_handle_scope* scope = JSVM::jsvm_create_handle_scope(pVMInfo->pVM);

		bool CanRelease = false;
		if (pVMInfo->NeedRelease && pVMInfo->ReleaseTimer.IsTimeOut())
		{
			pVMInfo->ReleaseTimer.SaveTime();
			JSVM::jsvm_context* context = JSVM::jsvm_get_context(scope);
			JSVM::jsvm_value* check_release_fn = NULL;
			if (pVMInfo->PersistentValues[JSVMPersistentValues_PluginCheckReleaseFn])
				check_release_fn = JSVM::jsvm_get_value_from_persistent_value(context, pVMInfo->PersistentValues[JSVMPersistentValues_PluginCheckReleaseFn]);
			bool ret = false;
			if (check_release_fn && JSVM::jsvm_is_function(check_release_fn))
			{
				CanRelease = JSVM::JSCallGlobal<bool>(context, check_release_fn);
			}
			else
			{
				LogJSVM("无法获得插件释放检测函数");
			}
		}

		JSVM::jsvm_event_loop(scope, ProcessLimit);
		//主动GC
		if (Config.InitiativeGCInterval)
		{
			if (pVMInfo->GCTimer.IsTimeOut(Config.InitiativeGCInterval))
			{
				pVMInfo->GCTimer.SaveTime();
				JSVM::jsvm_gc(pVMInfo->pVM, Config.GCLevel, Config.FullGC);
			}
		}
		JSVM::jsvm_release_handle_scope(scope);

		if (CanRelease)
		{
			CJSVMHelper::GetInstance()->UnegisterVMInfo(pVMInfo);
			pVMInfo->Release();
			m_VMInfoList.Delete(i);
		}
	}
	return ProcessCount;
}
void CJSVMHelper::OnDOSGroupRelease(int GroupIndex)
{
	LogJSVM("线程组%d释放", GroupIndex);
	for (JSVM_VM_INFO& VMInfo : m_VMInfoList)
	{
		LogJSVM("未在JSVM所属线程组执行释放(JSVM=%d,GroupIndex=%d)", VMInfo.GroupIndex, GroupIndex);
		VMInfo.Release();
	}
	m_VMInfoList.Clear();
}

void CJSVMHelper::RegisterVMInfo(JSVM_VM_INFO* pVMInfo)
{
	CAutoLock Lock(m_CriticalSection);
	m_AllVMList.Add(pVMInfo);
}
void CJSVMHelper::UnegisterVMInfo(JSVM_VM_INFO* pVMInfo)
{
	CAutoLock Lock(m_CriticalSection);
	m_AllVMList.Remove(pVMInfo);
}
bool CJSVMHelper::JSDOSPrintLog(UINT LogChannel, int Level, LPCTSTR Tag, LPCTSTR Msg)
{
	return CLogManager::GetInstance()->PrintLogDirect(LogChannel, Level, Tag, Msg);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void JSIDOSMessage::JSConstructor()
{

}

JSVM::jsvm_value* JSIDOSMessage::get_Data(JSVM::jsvm_context* context)
{
	return JSVM::jsvm_create_binary(context, (void*)GetMsgData(), GetDataLength());
}

JSVM::jsvm_value* JSIDOSMessage::get_DataPacket(JSVM::jsvm_context* context)
{
	JSVM_VM_INFO* pVMInfo = CJSVMHelper::GetInstance()->GetCurJSVM(context);
	if (pVMInfo)
	{
		if (pVMInfo->PersistentValues[JSVMPersistentValues_CSmartStruct])
		{
			JSVM::jsvm_value* Class = JSVM::jsvm_get_value_from_persistent_value(context, pVMInfo->PersistentValues[JSVMPersistentValues_CSmartStruct]);
			if (Class)
			{
				JSVM::jsvm_value* Data = JSVM::jsvm_create_binary(context, (void*)GetMsgData(), GetDataLength());

				return JSVM::JSClassNew<JSVM::jsvm_value*>(context, Class, Data, 0, GetDataLength(), false);
			}
			else
			{
				JSVM::jsvm_throw_exception(context, "can not get class:CSmartStruct");
			}
		}
		else
		{
			JSVM::jsvm_throw_exception(context, "class:CSmartStruct not inited");
		}
	}
	else
	{
		JSVM::jsvm_throw_exception(context, "VMInfo not exist on cur thread");
	}
	return NULL;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void JSIDOSMessagePacket::JSConstructor()
{

}

JSVM::jsvm_value* JSIDOSMessagePacket::get_Data(JSVM::jsvm_context* context)
{
	return JSVM::jsvm_create_binary(context, (void*)GetMessage().GetMsgData(), GetMessage().GetDataLength());
}
void JSIDOSMessagePacket::set_Data(JSVM::jsvm_context* context, JSVM::jsvm_value* value)
{
	size_t Len = 0;
	void* Data = JSVM::jsvm_get_value_binary(context, value, &Len);
	if (Data)
	{
		MSG_LEN_TYPE MaxLen = GetMaxMsgDataLen();
		if (Len > MaxLen)
			Len = MaxLen;
		GetMessage().SetData(Data, (MSG_LEN_TYPE)Len);
	}
}

JSVM::jsvm_value* JSIDOSMessagePacket::get_DataPacket(JSVM::jsvm_context* context)
{
	JSVM_VM_INFO* pVMInfo = CJSVMHelper::GetInstance()->GetCurJSVM(context);
	if (pVMInfo)
	{
		if (pVMInfo->PersistentValues[JSVMPersistentValues_CSmartStruct])
		{
			JSVM::jsvm_value* Class = JSVM::jsvm_get_value_from_persistent_value(context, pVMInfo->PersistentValues[JSVMPersistentValues_CSmartStruct]);
			if (Class)
			{
				JSVM::jsvm_value* Data = JSVM::jsvm_create_binary(context, (void*)GetMessage().GetMsgData(), GetMessage().GetDataLength());

				return JSVM::JSClassNew<JSVM::jsvm_value*>(context, Class, Data, 0, GetMessage().GetDataLength(), false);
			}
			else
			{
				JSVM::jsvm_throw_exception(context, "can not get class:CSmartStruct");
			}
		}
		else
		{
			JSVM::jsvm_throw_exception(context, "class:CSmartStruct not inited");
		}
	}
	else
	{
		JSVM::jsvm_throw_exception(context, "VMInfo not exist on cur thread");
	}
	return NULL;
}
void JSIDOSMessagePacket::set_DataPacket(JSVM::jsvm_context* context, JSVM::jsvm_value* value)
{
	if (JSVM::jsvm_is_object(value))
	{
		JSVM::jsvm_value* metod = JSVM::jsvm_obj_get(context, value, "GetData");
		if (metod && JSVM::jsvm_is_function(metod))
		{
			JSVM::jsvm_value* Data = JSVM::JSCallThis<JSVM::jsvm_value*>(context, value, metod);
			if (Data)
			{
				size_t Len = 0;
				void* pData = JSVM::jsvm_get_value_binary(context, Data, &Len);
				if (pData)
				{
					MSG_LEN_TYPE MaxLen = GetMaxMsgDataLen();
					if (Len > MaxLen)
						Len = MaxLen;
					GetMessage().SetData(pData, (MSG_LEN_TYPE)Len);
				}
				else
				{
					JSVM::jsvm_throw_exception(context, "can not get packet data");
				}
			}
			else
			{
				JSVM::jsvm_throw_exception(context, "invalid or empty packet");
			}
		}
		else
		{
			JSVM::jsvm_throw_exception(context, "packet not have method:GetData");
		}
	}
	else
	{
		JSVM::jsvm_throw_exception(context, "invalid packet");
	}
}

JSVM::jsvm_value* JSIDOSMessagePacket::get_TargetIDs(JSVM::jsvm_context* context)
{
	JSVM::jsvm_value* List = JSVM::jsvm_create_array(context);
	UINT Count = GetTargetIDCount();
	const OBJECT_ID* pIDs = GetTargetIDs();
	for (UINT i = 0; i < Count; i++)
	{
		JSVM::jsvm_obj_set_by_index(context, List, i, JSVM::TypeConvertor<OBJECT_ID>::ToJS(context, pIDs[i]));
	}
	return List;
}
void JSIDOSMessagePacket::set_TargetIDs(JSVM::jsvm_context* context, JSVM::jsvm_value* value)
{
	UINT Len = JSVM::jsvm_get_array_length(context, value);
	UINT MaxCount = GetMaxTargetIDCount();
	if (Len > MaxCount)
	{
		LogJSVM("TargetID too more %u>%u", Len, MaxCount);
		Len = MaxCount;
	}
	SetTargetIDCount(0);
	for (UINT i = 0; i < Len; i++)
	{
		JSVM::jsvm_value* ID = JSVM::jsvm_obj_get_by_index(context, value, i);
		if (ID)
			AddTargetID(JSVM::TypeConvertor<OBJECT_ID>::FromJS(context, ID));
	}
}
JSVM::jsvm_value* JSIDOSMessagePacket::get_EmptyDataPacket(JSVM::jsvm_context* context)
{
	JSVM_VM_INFO* pVMInfo = CJSVMHelper::GetInstance()->GetCurJSVM(context);
	if (pVMInfo)
	{
		if (pVMInfo->PersistentValues[JSVMPersistentValues_CSmartStruct])
		{
			JSVM::jsvm_value* Class = JSVM::jsvm_get_value_from_persistent_value(context, pVMInfo->PersistentValues[JSVMPersistentValues_CSmartStruct]);
			if (Class)
			{
				JSVM::jsvm_value* Data = JSVM::jsvm_create_binary(context, (void*)GetMessage().GetMsgData(), GetMaxMsgDataLen());

				return JSVM::JSClassNew<JSVM::jsvm_value*>(context, Class, Data, 0, GetMessage().GetDataLength(), true);
			}
			else
			{
				JSVM::jsvm_throw_exception(context, "can not get class:CSmartStruct");
			}
		}
		else
		{
			JSVM::jsvm_throw_exception(context, "class:CSmartStruct not inited");
		}
	}
	else
	{
		JSVM::jsvm_throw_exception(context, "VMInfo not exist on cur thread");
	}
	return NULL;
}