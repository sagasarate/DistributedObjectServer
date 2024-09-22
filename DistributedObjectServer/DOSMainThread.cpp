#include "stdafx.h"

#ifndef WIN32
#include <sys/wait.h>
#endif

#define SERVER_STAT_TIME		(5*60*1000)

IMPLEMENT_CLASS_INFO_STATIC(CDOSMainThread, CDOSServerThread);

CDOSMainThread::CDOSMainThread(void)
{
	FUNCTION_BEGIN;
	m_Status = STATUS_NONE;
	m_PluginList.SetTag(_T("CDOSMainThread"));
	m_PluginReleaseList.SetTag(_T("CDOSMainThread"));
	FUNCTION_END;
}

CDOSMainThread::~CDOSMainThread(void)
{
}

bool CDOSMainThread::OnStart()
{
	FUNCTION_BEGIN;



	CDOSServerThread::SetConfig(CDOSConfig::GetInstance()->GetDOSConfig());

	if (CDOSConfig::GetInstance()->GetMonoConfig().Enable)
	{
		if (!CCSVMHelper::GetInstance()->Init())
			return false;
	}
	else
	{
		Log("mono未启用");
	}

	if (CDOSConfig::GetInstance()->GetJSVMConfig().Enable)
	{
		if (CJSVMHelper::GetInstance()->Init())
		{
			m_ServerConfig.ObjectConfig.DOSGroupUpdateFNList.Add(&CJSVMHelper::OnDOSGroupUpdate);
			m_ServerConfig.ObjectConfig.DOSGroupDestoryFNList.Add(&CJSVMHelper::OnDOSGroupRelease);
		}
		else
		{
			return false;
		}
	}
	else
	{
		Log("JSVM未启用");
	}


	if (!CDOSServerThread::OnStart())
		return false;



	if (!m_PluginObjectManager.Init(GetObjectManager()))
	{
		Log("初始化插件对象管理器失败");
		return false;
	}

	StartProxyPluginsLoad();

	m_ESFactionList.AddCFunction("ShowObjectCount", 0, this, &CDOSMainThread::ShowObjectCount);
	m_ESFactionList.AddCFunction("ShowGroupInfo", 0, this, &CDOSMainThread::ShowGroupInfo);
	m_ESFactionList.AddCFunction("ListPlugin", 0, this, &CDOSMainThread::ListPlugin);
	m_ESFactionList.AddCFunction("ReleasePlugin", 1, this, &CDOSMainThread::ReleasePlugin);

	m_PluginReleaseCheckTimer.SaveTime();

	CMemoryAllocatee::RefreshEnv();

	return true;
	FUNCTION_END;
	return false;
}



int CDOSMainThread::Update(int ProcessPacketLimit)
{
	FUNCTION_BEGIN;
	int Process = 0;

	Process += CDOSServerThread::Update(SERVER_UPDATE_LIMIT);


	switch (m_Status)
	{
	case STATUS_PROXY_PLUGIN_LOAD:
		DoProxyPluginsLoad();
		break;
	case STATUS_PLUGIN_LOAD:
		DoPluginsLoad();
		break;
	case STATUS_WORKING:
	{
		if (m_PluginReleaseCheckTimer.IsTimeOut(PLUGIN_RELASE_CHECK_TIME))
		{
			m_PluginReleaseCheckTimer.SaveTime();

			CAutoLock Lock(m_PluginReleaseLock);

			for (int i = (int)m_PluginReleaseList.GetCount() - 1; i >= 0; i--)
			{
				if (FreePlugin(m_PluginReleaseList[i]))
				{
					m_PluginReleaseList.Delete(i);
				}
			}
		}


		UINT RunningPluginCount = 0;

		for (UINT i = 0; i < m_PluginList.GetCount(); i++)
		{
			PLUGIN_INFO& PluginInfo = m_PluginList[i];
			if (PluginInfo.PluginStatus != PLUGIN_STATUS_NONE && PluginInfo.PluginStatus != PLUGIN_STATUS_ERROR)
			{
				RunningPluginCount++;
			}
		}

		if (RunningPluginCount == 0 && CDOSConfig::GetInstance()->ExistWhenNoPlugin())
		{
			Log("CMainThread::Update:已无活动插件存在，服务器关闭");
			QueryShowDown();
		}
	}
	break;
	}

	if (CDOSConfig::GetInstance()->GetMonoConfig().Enable)
		Process += CCSVMHelper::GetInstance()->Update();

	if (CDOSConfig::GetInstance()->GetJSVMConfig().Enable)
		Process += CJSVMHelper::GetInstance()->Update();

	return Process;
	FUNCTION_END;
	return 1;
}

void CDOSMainThread::OnBeginTerminate()
{
	FUNCTION_BEGIN;
	FUNCTION_END;
}

bool CDOSMainThread::OnTerminating()
{
	FUNCTION_BEGIN;
	FUNCTION_END;
	return false;
}

void CDOSMainThread::OnTerminate()
{
	FUNCTION_BEGIN;

	CDOSServerThread::OnTerminate();

	FreePlugins();


	m_PluginObjectManager.Destory();

	if (CDOSConfig::GetInstance()->GetMonoConfig().Enable)
		CCSVMHelper::GetInstance()->Release();

	if (CDOSConfig::GetInstance()->GetJSVMConfig().Enable)
		CJSVMHelper::GetInstance()->Release();

	FUNCTION_END;
}



int CDOSMainThread::GetClientCount()
{
	FUNCTION_BEGIN;

	FUNCTION_END;
	return 0;
}

LPCTSTR CDOSMainThread::GetConfigFileName()
{
	FUNCTION_BEGIN;
	return CONFIG_FILE_NAME;
	FUNCTION_END;
	return "";
}

bool CDOSMainThread::QueryFreePlugin(UINT PluginID)
{
	LogDebug("请求释放插件%u", PluginID);
	CAutoLock Lock(m_PluginReleaseLock);
	bool IsExist = false;
	for (UINT i = 0; i < m_PluginReleaseList.GetCount(); i++)
	{
		if (m_PluginReleaseList[i] == PluginID)
		{
			IsExist = true;
			break;
		}
	}
	if (!IsExist)
	{
		m_PluginReleaseList.Add(PluginID);
		return true;
	}
	else
	{
		return false;
	}
}

MONO_DOMAIN_INFO* CDOSMainThread::GetPluginDomain(UINT PluginID)
{
	for (UINT i = 0; i < m_PluginList.GetCount(); i++)
	{
		if (m_PluginList[i].ID == PluginID && m_PluginList[i].PluginType == PLUGIN_TYPE_CS)
		{
			return &m_PluginList[i].MonoDomainInfo;
		}
	}
	return NULL;
}




//MonoObject * CDOSMainThread::MonoCreateDOSObjectID(MONO_DOMAIN_INFO& DomainInfo, OBJECT_ID ObjectID)
//{
//	if (DomainInfo.pMonoDomain && DomainInfo.pMonoClass_ObjectID && DomainInfo.pMonoClassMethod_ObjectID_Ctor)
//	{
// 		MonoObject * pObject = mono_object_new(DomainInfo.pMonoDomain, DomainInfo.pMonoClass_ObjectID);
//		if (pObject)
//		{
//			MonoObject * pException = NULL;
//			void * Params[1];
//			Params[0] = &ObjectID.ID;
//			mono_runtime_invoke(DomainInfo.pMonoClassMethod_ObjectID_Ctor, pObject, Params, &pException);
//			if (pException)
//			{
//				MonoString * pMsg = mono_object_to_string(pException, NULL);
//				if (pMsg)
//				{
//					char * pBuff = mono_string_to_utf8(pMsg);
//					LogMono("%s", pBuff);
//					mono_free(pBuff);
//				}
//			}
//			else
//			{
//				return pObject;
//			}
//		}
//		else
//		{
//			LogMono("CDOSMainThread::MonoCreateDOSObjectID:创建对象失败");
//		}
//	}
//	return NULL;
//}


void CDOSMainThread::StartPluginsLoad()
{
	m_Status = STATUS_PLUGIN_LOAD;

	m_PluginList = CDOSConfig::GetInstance()->GetPluginList();

	Log("一共有%u个插件需要加载", m_PluginList.GetCount());

	for (UINT i = 0; i < m_PluginList.GetCount(); i++)
	{
		PLUGIN_INFO& PluginInfo = m_PluginList[i];
		PluginInfo.ID = i + 1;
		PluginInfo.LogChannel = PLUGIN_LOG_CHANNEL_START + i + 1;
		PluginInfo.PluginStatus = PLUGIN_STATUS_NONE;
	}
}
bool CDOSMainThread::DoPluginsLoad()
{
	FUNCTION_BEGIN;

	for (int i = (int)m_PluginList.GetCount() - 1; i >= 0; i--)
	{
		PLUGIN_INFO& PluginInfo = m_PluginList[i];
		switch (PluginInfo.PluginStatus)
		{
		case PLUGIN_STATUS_NONE:
			if (!LoadPlugin(PluginInfo))
			{
				Log("插件(%u)%s加载失败，被移除", PluginInfo.ID, (LPCTSTR)PluginInfo.PluginName);
			}
			break;
		}
	}


	Log("插件装载完毕");

#ifdef WIN32
	CExceptionParser::GetInstance()->OnFinishModuleLoad();
#endif
	m_Status = STATUS_WORKING;
	return true;
	FUNCTION_END;
	return false;
}

void CDOSMainThread::FreePlugins()
{
	FUNCTION_BEGIN;

	for (size_t i = 0; i < m_PluginList.GetCount(); i++)
	{
		FreePlugin(m_PluginList[i]);
	}

	FUNCTION_END;
}

//void CDOSMainThread::CompileLibs()
//{
//	FUNCTION_BEGIN;
//
//	m_Status = STATUS_COMPILE_LIBS;
//	m_LibList = CDOSConfig::GetInstance()->GetLibList();
//	for (UINT i = 0; i < m_LibList.GetCount(); i++)
//	{
//		CompileCSharpLib(m_LibList[i]);
//		break;
//	}
//
//	FUNCTION_END;
//}
void CDOSMainThread::StartProxyPluginsLoad()
{
	m_Status = STATUS_PROXY_PLUGIN_LOAD;
	const CEasyArray<CLIENT_PROXY_PLUGIN_INFO>& PluginList = CDOSConfig::GetInstance()->GetProxyPluginList();

	Log("一共有%u个代理插件需要加载", PluginList.GetCount());
}
bool CDOSMainThread::DoProxyPluginsLoad()
{
	const CEasyArray<CLIENT_PROXY_PLUGIN_INFO>& PluginList = CDOSConfig::GetInstance()->GetProxyPluginList();


	for (UINT i = 0; i < PluginList.GetCount(); i++)
	{
		CLIENT_PROXY_PLUGIN_INFO PluginInfo = PluginList[i];

		if (PluginInfo.ProxyMode == CLIENT_PROXY_MODE_CUSTOM)
		{
			PluginInfo.ID = i + 1;
			PluginInfo.LogChannel = PROXY_PLUGIN_LOG_CHANNEL_START + i + 1;
			PluginInfo.PluginStatus = PLUGIN_STATUS_NONE;
			CDOSObjectProxyServiceCustom* pProxy = MONITORED_NEW(_T("CDOSMainThread"), CDOSObjectProxyServiceCustom);

			if (pProxy->Init(this, PluginInfo))
			{
				if (GetProxyManager()->RegisterProxyService(pProxy))
				{
					return true;
				}
				else
				{
					SAFE_RELEASE(pProxy);
				}
			}
			else
			{
				SAFE_RELEASE(pProxy);
			}
		}
	}

	Log("代理插件装载完毕");
	StartPluginsLoad();
	return true;
}

bool CDOSMainThread::LoadPlugin(PLUGIN_INFO& PluginInfo)
{
	FUNCTION_BEGIN;

	//检查配置目录和日志目录
	if (PluginInfo.ConfigDir.IsEmpty())
	{
		PluginInfo.ConfigDir = CFileTools::GetModulePath(NULL);
	}
	else
	{
		if (CFileTools::IsAbsolutePath(PluginInfo.ConfigDir))
			PluginInfo.ConfigDir = CFileTools::MakeFullPath(PluginInfo.ConfigDir);
		else
			PluginInfo.ConfigDir = CFileTools::MakeModuleFullPath(NULL, PluginInfo.ConfigDir);
	}

	if (PluginInfo.LogDir.IsEmpty())
	{
		PluginInfo.LogDir = CFileTools::MakeModuleFullPath(NULL, "Log");
	}
	else
	{
		if (CFileTools::IsAbsolutePath(PluginInfo.LogDir))
			PluginInfo.LogDir = CFileTools::MakeFullPath(PluginInfo.LogDir);
		else
			PluginInfo.LogDir = CFileTools::MakeModuleFullPath(NULL, PluginInfo.LogDir);
	}

	if (!CFileTools::IsDirExist(PluginInfo.LogDir))
	{
		if (!CFileTools::CreateDirEx(PluginInfo.LogDir))
		{
			Log("无法创建日志目录:%s", (LPCTSTR)PluginInfo.LogDir);
			return false;
		}
	}

	switch (PluginInfo.PluginType)
	{
	case PLUGIN_TYPE_NATIVE:
		return LoadNativePlugin(PluginInfo);
		break;
	case PLUGIN_TYPE_CS:
		return LoadCSPlugin(PluginInfo);
		break;
	case PLUGIN_TYPE_JS:
		return LoadJSPlugin(PluginInfo);
		break;
	default:
		Log("CMainThread::LoadPlugin:不支持的插件类型%d,%s", PluginInfo.PluginType, (LPCTSTR)PluginInfo.PluginName);
	}

	FUNCTION_END;
	return false;
}

bool CDOSMainThread::FreePlugin(PLUGIN_INFO& PluginInfo)
{
	FUNCTION_BEGIN;

	switch (PluginInfo.PluginType)
	{
	case PLUGIN_TYPE_NATIVE:
		return FreeNativePlugin(PluginInfo);
		break;
	case PLUGIN_TYPE_CS:
		return FreeCSPlugin(PluginInfo);
		break;
	case PLUGIN_TYPE_JS:
		return FreeJSPlugin(PluginInfo);
		break;
	default:
		Log("CMainThread::FreePlugin:不支持的插件类型%d,%s", PluginInfo.PluginType, (LPCTSTR)PluginInfo.PluginName);
	}
	return false;
	FUNCTION_END;
	return false;
}
bool CDOSMainThread::FreePlugin(UINT PluginID)
{
	FUNCTION_BEGIN;

	for (size_t i = 0; i < m_PluginList.GetCount(); i++)
	{
		if (m_PluginList[i].ID == PluginID)
		{
			LogDebug("释放插件(%u)%s",
				m_PluginList[i].ID, (LPCTSTR)m_PluginList[i].PluginName);

			if (FreePlugin(m_PluginList[i]))
			{
				LogDebug("插件释放成功(%u)%s", m_PluginList[i].ID, (LPCTSTR)m_PluginList[i].PluginName);
				m_PluginList.Delete(i);
				LogDebug("剩余%u个插件", m_PluginList.GetCount());
				return true;
			}
			else
			{
				LogDebug("插件释放失败(%u)%s", m_PluginList[i].ID, (LPCTSTR)m_PluginList[i].PluginName);
				return false;
			}
		}
	}

	Log("插件%u未找到", PluginID);
	FUNCTION_END;
	return false;
}


bool CDOSMainThread::LoadNativePlugin(PLUGIN_INFO& PluginInfo)
{
	FUNCTION_BEGIN;

	if (PluginInfo.ModuleFileName.IsEmpty())
#ifdef _DEBUG
		PluginInfo.ModuleFileName = PluginInfo.PluginName + "D";
#else
		PluginInfo.ModuleFileName = PluginInfo.PluginName;
#endif

#ifndef WIN32
	PluginInfo.ModuleFileName.Replace('\\', '/');
#endif
	PluginInfo.ModuleFileName = CFileTools::MakeModuleFullPath(NULL, PluginInfo.ModuleFileName);
	//扩展名补足
	CEasyString FileExt = CFileTools::GetPathFileExtName(PluginInfo.ModuleFileName);
	if (FileExt.GetLength() <= 1)
	{
#ifdef WIN32
		PluginInfo.ModuleFileName = PluginInfo.ModuleFileName + ".dll";
#else
		PluginInfo.ModuleFileName = PluginInfo.ModuleFileName + ".so";
#endif
	}

	if (PluginInfo.PluginName.IsEmpty())
		PluginInfo.PluginName = CFileTools::GetPathFileName(PluginInfo.ModuleFileName);

	Log("开始装载插件(%u)%s,配置目录:%s,日志目录:%s",
		PluginInfo.ID,
		(LPCTSTR)PluginInfo.ModuleFileName,
		(LPCTSTR)PluginInfo.ConfigDir,
		(LPCTSTR)PluginInfo.LogDir);



#ifdef WIN32
	PluginInfo.hModule = LoadLibrary(PluginInfo.ModuleFileName);
#else
	PluginInfo.hModule = dlopen(PluginInfo.ModuleFileName, RTLD_NOW | RTLD_LOCAL | RTLD_DEEPBIND);
#endif
	if (PluginInfo.hModule)
	{
#ifdef WIN32
		if (CSystemConfig::GetInstance()->IsPreLoadModuleSym())
		{
			CExceptionParser::GetInstance()->SymLoadFromModule(PluginInfo.ModuleFileName);
		}
		PluginInfo.pInitFN = (PLUGIN_INIT_FN)GetProcAddress(PluginInfo.hModule, PLUGIN_INIT_FN_NAME);
		PluginInfo.pCheckReleaseFN = (PLUGIN_CHECK_RELEASE_FN)GetProcAddress(PluginInfo.hModule, PLUGIN_CHECK_RELEASE_FN_NAME);
		if (PluginInfo.pInitFN && PluginInfo.pCheckReleaseFN)
#else
		//dlerror();
		PluginInfo.pInitFN = (PLUGIN_INIT_FN)dlsym(PluginInfo.hModule, PLUGIN_INIT_FN_NAME);
		LPCTSTR InitFNError = dlerror();
		PluginInfo.pCheckReleaseFN = (PLUGIN_CHECK_RELEASE_FN)dlsym(PluginInfo.hModule, PLUGIN_CHECK_RELEASE_FN_NAME);
		LPCTSTR CheckReleaseFNError = dlerror();
		if (InitFNError == NULL && CheckReleaseFNError == NULL && PluginInfo.pInitFN && PluginInfo.pCheckReleaseFN)
#endif
		{
			CEasyString LogFileName;

			CServerLogPrinter* pLog;

			LogFileName.Format("%s/Plugin.%s", (LPCTSTR)PluginInfo.LogDir, (LPCTSTR)CFileTools::GetPathFileName(PluginInfo.PluginName));
			pLog = MONITORED_NEW(_T("CDOSMainThread"), CServerLogPrinter, this, CServerLogPrinter::LOM_CONSOLE | CServerLogPrinter::LOM_FILE,
				CSystemConfig::GetInstance()->GetLogLevel(), LogFileName, CSystemConfig::GetInstance()->GetLogCacheSize());
			pLog->SetBackup(CSystemConfig::GetInstance()->GetLogBackupDir(), CSystemConfig::GetInstance()->GetLogBackupDelay());
			CLogManager::GetInstance()->AddChannel(PluginInfo.LogChannel, pLog);
			SAFE_RELEASE(pLog);

			if ((*(PluginInfo.pInitFN))(&m_PluginObjectManager, PluginInfo.ID, PluginInfo.LogChannel, PluginInfo.ConfigDir, PluginInfo.LogDir))
			{
				PluginInfo.PluginStatus = PLUGIN_STATUS_LOADED;
				Log("插件装载成功%s", (LPCTSTR)PluginInfo.ModuleFileName);
#ifdef WIN32
				MEMORY_BASIC_INFORMATION mbi;
				ZeroMemory(&mbi, sizeof(mbi));
				if (VirtualQuery((void*)PluginInfo.pInitFN, &mbi, sizeof(mbi)))
				{
					TCHAR ModuleName[MAX_PATH];
					GetModuleFileName((HINSTANCE)mbi.AllocationBase, ModuleName, MAX_PATH);
					Log("插件模块信息:BaseAddress=0x%llX,ModuleName=%s", mbi.AllocationBase, ModuleName);
				}
#else
				Dl_info DLInfo;
				if (dladdr((void*)PluginInfo.pInitFN, &DLInfo))
				{
					Log("插件模块信息:BaseAddress=%p,ModuleName=%s", DLInfo.dli_fbase, DLInfo.dli_fname);
				}
#endif
				return true;
			}
			else
			{
				Log("插件初始化失败%s", (LPCTSTR)PluginInfo.ModuleFileName);
			}
		}
		else
		{
			Log("不合法的插件%s", (LPCTSTR)PluginInfo.ModuleFileName);
#ifndef WIN32
			Log("InitFN错误信息:%s", InitFNError);
			Log("CheckReleaseFN错误信息:%s", CheckReleaseFNError);
#endif
	}
}
	else
	{
		Log("无法装载插件(%d)%s", GetLastError(), (LPCTSTR)PluginInfo.ModuleFileName);
#ifndef WIN32
		Log("错误信息:%s", dlerror());
#endif
	}

	PluginInfo.PluginStatus = PLUGIN_STATUS_ERROR;
	FUNCTION_END;
	return false;
}
bool CDOSMainThread::FreeNativePlugin(PLUGIN_INFO& PluginInfo)
{
	FUNCTION_BEGIN;
	if (PluginInfo.pCheckReleaseFN)
	{
		if ((*(PluginInfo.pCheckReleaseFN))())
		{
			if (PluginInfo.CanUnload)
			{
				Log("准备卸载%s", (LPCTSTR)PluginInfo.ModuleFileName);
#ifdef WIN32
				if (FreeLibrary(PluginInfo.hModule))
				{
					Log("插件卸载成功%s", (LPCTSTR)PluginInfo.ModuleFileName);
				}
				else
				{
					Log("插件卸载失败%s:%d", (LPCTSTR)PluginInfo.ModuleFileName, GetLastError());
				}
#else
				if (dlclose(PluginInfo.hModule) == 0)
				{
					Log("插件卸载成功%s", (LPCTSTR)PluginInfo.ModuleFileName);
				}
				else
				{
					LPCTSTR UnloadError = dlerror();
					Log("插件卸载失败%s:%s", (LPCTSTR)PluginInfo.ModuleFileName, UnloadError);
				}
#endif
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		Log("没有插件释放检测函数");
	}


	return false;
	FUNCTION_END;
	return false;
}

bool CDOSMainThread::LoadCSPlugin(PLUGIN_INFO& PluginInfo)
{
	FUNCTION_BEGIN;

	if (!CDOSConfig::GetInstance()->GetMonoConfig().Enable)
	{
		LogMono("Mono未启用");
		return false;
	}


	if (PluginInfo.ModuleFileName.IsEmpty())
#ifdef _DEBUG
		PluginInfo.ModuleFileName = PluginInfo.PluginName + "D";
#else
		PluginInfo.ModuleFileName = PluginInfo.PluginName;
#endif

#ifndef WIN32
	PluginInfo.ModuleFileName.Replace('\\', '/');
#endif

	//扩展名补足
	CEasyString FileExt = CFileTools::GetPathFileExtName(PluginInfo.ModuleFileName);
	if (FileExt.GetLength() <= 1)
	{
		PluginInfo.ModuleFileName = PluginInfo.ModuleFileName + ".dll";
	}

	PluginInfo.ModuleFileName = CFileTools::MakeModuleFullPath(NULL, PluginInfo.ModuleFileName);

	if (PluginInfo.PluginName.IsEmpty())
		PluginInfo.PluginName = CFileTools::GetPathFileName(PluginInfo.ModuleFileName);


	Log("开始装载插件(%u)%s,配置目录:%s,日志目录:%s",
		PluginInfo.ID,
		(LPCTSTR)PluginInfo.ModuleFileName,
		(LPCTSTR)PluginInfo.ConfigDir,
		(LPCTSTR)PluginInfo.LogDir);

	PluginInfo.PluginStatus = PLUGIN_STATUS_ERROR;
	if (CCSVMHelper::GetInstance()->InitPluginDomain(PluginInfo.MonoDomainInfo, PluginInfo.PluginName))
	{
		mono_domain_set(PluginInfo.MonoDomainInfo.pMonoDomain, false);
		PluginInfo.MonoDomainInfo.pCSPluginAssembly = mono_domain_assembly_open(PluginInfo.MonoDomainInfo.pMonoDomain, PluginInfo.ModuleFileName);
		if (PluginInfo.MonoDomainInfo.pCSPluginAssembly)
		{
			MonoImage* pImage = mono_assembly_get_image(PluginInfo.MonoDomainInfo.pCSPluginAssembly);
			if (CDOSConfig::GetInstance()->GetMonoConfig().EnableDebug)
				mono_debug_open_image_from_memory(pImage, NULL, 0);
			bool Ret = false;
			MonoClass* pClass = mono_class_from_name(pImage, PluginInfo.MainClassNameSpace, PluginInfo.MainClass);
			if (pClass)
			{
				MonoObject* pMainObj = mono_object_new(PluginInfo.MonoDomainInfo.pMonoDomain, pClass);
				if (pMainObj)
				{
					MonoMethod* pCtorMethod = mono_class_get_method_from_name(pClass, ".ctor", 0);
					if (pCtorMethod)
					{
						MonoObject* pException = NULL;
						mono_runtime_invoke(pCtorMethod, pMainObj, NULL, &pException);
						if (pException)
						{
							CCSVMHelper::GetInstance()->ProcessMonoException(pException);
						}
						else
						{
							MonoMethod* pInitMethod;
							pInitMethod = mono_class_get_method_from_name(pClass, PLUGIN_INIT_FN_NAME, 4);
							if (pInitMethod)
							{
								CEasyString LogFileName;


								CServerLogPrinter* pLog;

								LogFileName.Format("%s/Plugin.%s", (LPCTSTR)PluginInfo.LogDir, (LPCTSTR)CFileTools::GetPathFileName(PluginInfo.PluginName));
								pLog = MONITORED_NEW(_T("CDOSMainThread"), CServerLogPrinter, this, CServerLogPrinter::LOM_CONSOLE | CServerLogPrinter::LOM_FILE,
									CSystemConfig::GetInstance()->GetLogLevel(), LogFileName, CSystemConfig::GetInstance()->GetLogCacheSize());
								pLog->SetBackup(CSystemConfig::GetInstance()->GetLogBackupDir(), CSystemConfig::GetInstance()->GetLogBackupDelay());
								CLogManager::GetInstance()->AddChannel(PluginInfo.LogChannel, pLog);
								SAFE_RELEASE(pLog);

								pException = NULL;
								LPVOID Params[4];
								Params[0] = &PluginInfo.ID;
								Params[1] = &PluginInfo.LogChannel;
								Params[2] = CCSVMHelper::GetInstance()->MonoCreateString(PluginInfo.MonoDomainInfo,
									PluginInfo.ConfigDir, PluginInfo.ConfigDir.GetLength());
								Params[3] = CCSVMHelper::GetInstance()->MonoCreateString(PluginInfo.MonoDomainInfo,
									PluginInfo.LogDir, PluginInfo.LogDir.GetLength());
								MonoObject* pReturnValue = mono_runtime_invoke(pInitMethod, pMainObj, Params, &pException);
								if (pException)
								{
									CCSVMHelper::GetInstance()->ProcessMonoException(pException);
								}
								else if (pReturnValue)
								{
									void* pValue = mono_object_unbox(pReturnValue);
									if (pValue)
									{
										Ret = *((bool*)pValue);
										if (Ret)
										{
											PluginInfo.hCSMainObj = mono_gchandle_new(pMainObj, false);
											PluginInfo.PluginStatus = PLUGIN_STATUS_LOADED;
											Log("插件装载成功%s", (LPCTSTR)PluginInfo.ModuleFileName);
											return true;
										}
									}
									else
									{
										Log("插件初始化函数返回值类型不正确");
									}
								}
								else
								{
									LogMono("插件初始化函数没有返回值");
								}
							}
							else
							{
								LogMono("无法找到插件初始化函数");
							}
						}
					}
					else
					{
						LogMono("无法找到构造函数");
					}
				}
				else
				{
					LogMono("无法创建插件对象");
				}
			}
			else
			{
				LogMono("无法找到插件类[%s.%s]", (LPCTSTR)PluginInfo.MainClassNameSpace, (LPCTSTR)PluginInfo.MainClass);
			}
		}
		else
		{
			LogMono("无法加载模块%s", (LPCTSTR)PluginInfo.ModuleFileName);
		}
		CCSVMHelper::GetInstance()->ReleasePluginDomain(PluginInfo.MonoDomainInfo);
	}
	else
	{
		LogMono("无法初始化模块domain%s", (LPCTSTR)PluginInfo.ModuleFileName);
	}

	FUNCTION_END;
	return false;
}

bool CDOSMainThread::FreeCSPlugin(PLUGIN_INFO& PluginInfo)
{
	FUNCTION_BEGIN;


	if (PluginInfo.MonoDomainInfo.pCSPluginAssembly)
	{
		bool Ret = false;

		mono_domain_set(PluginInfo.MonoDomainInfo.pMonoDomain, false);

		MonoClass* pClass = mono_class_from_name(mono_assembly_get_image(PluginInfo.MonoDomainInfo.pCSPluginAssembly),
			PluginInfo.MainClassNameSpace, PluginInfo.MainClass);
		if (pClass && PluginInfo.hCSMainObj)
		{
			MonoObject* pMainObj = mono_gchandle_get_target(PluginInfo.hCSMainObj);
			if (pMainObj)
			{
				MonoMethod* pReleaseMethod;
				pReleaseMethod = mono_class_get_method_from_name(pClass, PLUGIN_CHECK_RELEASE_FN_NAME, 0);
				if (pReleaseMethod)
				{
					MonoObject* pException = NULL;
					MonoObject* pReturnValue = mono_runtime_invoke(pReleaseMethod, pMainObj, NULL, &pException);
					if (pException)
					{
						CCSVMHelper::GetInstance()->ProcessMonoException(pException);
					}
					else if (pReturnValue)
					{
						void* pValue = mono_object_unbox(pReturnValue);
						if (pValue)
						{
							Ret = *((bool*)pValue);
						}
						else
						{
							Log("返回值类型不正确");
						}
					}
					else
					{
						LogMono("插件释放检测函数没有返回值");
					}
				}
				else
				{
					LogMono("无法找到插件[%s]释放检测函数",
						(LPCTSTR)PluginInfo.ModuleFileName);
				}
			}
		}
		else
		{
			LogMono("无法找到插件[%s]类[%s.%s]",
				(LPCTSTR)PluginInfo.ModuleFileName, (LPCTSTR)PluginInfo.MainClassNameSpace, (LPCTSTR)PluginInfo.MainClass);
		}

		if (Ret)
		{
			if (PluginInfo.hCSMainObj)
			{
				mono_gchandle_free(PluginInfo.hCSMainObj);
				PluginInfo.hCSMainObj = 0;
			}
			CCSVMHelper::GetInstance()->ReleasePluginDomain(PluginInfo.MonoDomainInfo);
			LogMono("插件[%s]已释放", (LPCTSTR)PluginInfo.ModuleFileName);
		}
		return Ret;
	}
	else
	{
		LogMono("插件[%s]未加载", (LPCTSTR)PluginInfo.ModuleFileName);
		return false;
	}
	FUNCTION_END;
	return false;
}

bool CDOSMainThread::LoadJSPlugin(PLUGIN_INFO& PluginInfo)
{
	if (!CDOSConfig::GetInstance()->GetJSVMConfig().Enable)
	{
		LogMono("JSVM未启用");
		return false;
	}

	if (PluginInfo.ModuleFileName.IsEmpty())
#ifdef _DEBUG
		PluginInfo.ModuleFileName = PluginInfo.PluginName + "D";
#else
		PluginInfo.ModuleFileName = PluginInfo.PluginName;
#endif

#ifndef WIN32
	PluginInfo.ModuleFileName.Replace('\\', '/');
#endif

	PluginInfo.ModuleFileName = CFileTools::MakeModuleFullPath(NULL, PluginInfo.ModuleFileName);

	if (PluginInfo.PluginName.IsEmpty())
		PluginInfo.PluginName = CFileTools::GetPathFileName(PluginInfo.ModuleFileName);


	Log("开始装载插件(%u)%s,配置目录:%s,日志目录:%s",
		PluginInfo.ID,
		(LPCTSTR)PluginInfo.ModuleFileName,
		(LPCTSTR)PluginInfo.ConfigDir,
		(LPCTSTR)PluginInfo.LogDir);

	CEasyString LogFileName;

	CServerLogPrinter* pLog;

	LogFileName.Format("%s/Plugin.%s", (LPCTSTR)PluginInfo.LogDir, (LPCTSTR)CFileTools::GetPathFileName(PluginInfo.PluginName));
	pLog = MONITORED_NEW(_T("CDOSMainThread"), CServerLogPrinter, this, CServerLogPrinter::LOM_CONSOLE | CServerLogPrinter::LOM_FILE,
		CSystemConfig::GetInstance()->GetLogLevel(), LogFileName, CSystemConfig::GetInstance()->GetLogCacheSize());
	pLog->SetBackup(CSystemConfig::GetInstance()->GetLogBackupDir(), CSystemConfig::GetInstance()->GetLogBackupDelay());
	CLogManager::GetInstance()->AddChannel(PluginInfo.LogChannel, pLog);
	SAFE_RELEASE(pLog);

	if (CJSVMHelper::GetInstance()->CreateJSVM(PluginInfo.ID, PluginInfo.LogChannel, -1, PluginInfo.ModuleFileName, PluginInfo.ConfigDir, PluginInfo.LogDir) == NULL)
		return false;
	if (CJSVMHelper::GetInstance()->RunPlugin(PluginInfo.ID, PluginInfo.LogChannel, PluginInfo.ConfigDir, PluginInfo.LogDir))
	{
		PluginInfo.PluginStatus = PLUGIN_STATUS_LOADED;
		return true;
	}
	else
	{
		PluginInfo.PluginStatus = PLUGIN_STATUS_ERROR;
		return false;
	}
}
bool CDOSMainThread::FreeJSPlugin(PLUGIN_INFO& PluginInfo)
{
	return CJSVMHelper::GetInstance()->QueryReleasePlugin(PluginInfo.ID);
}

int CDOSMainThread::ShowObjectCount(CESThread* pESThread, ES_BOLAN* pResult, ES_BOLAN* pParams, int ParamCount)
{
	FUNCTION_BEGIN;
	GetDistributedObjectManager()->PrintObjectCount();
	FUNCTION_END;
	return 0;
}

int  CDOSMainThread::ShowGroupInfo(CESThread* pESThread, ES_BOLAN* pResult, ES_BOLAN* pParams, int ParamCount)
{
	FUNCTION_BEGIN;
	GetObjectManager()->PrintGroupInfo(SERVER_LOG_CHANNEL);
	FUNCTION_END;
	return 0;
}

int CDOSMainThread::ListPlugin(CESThread* pESThread, ES_BOLAN* pResult, ES_BOLAN* pParams, int ParamCount)
{
	FUNCTION_BEGIN;
	for (size_t i = 0; i < m_PluginList.GetCount(); i++)
	{
		Log("ID=%u,Name=%s,File=%s", m_PluginList[i].ID, (LPCTSTR)m_PluginList[i].PluginName, (LPCTSTR)m_PluginList[i].ModuleFileName);
	}
	FUNCTION_END;
	return 0;
}
int CDOSMainThread::ReleasePlugin(CESThread* pESThread, ES_BOLAN* pResult, ES_BOLAN* pParams, int ParamCount)
{
	FUNCTION_BEGIN;
	FreePlugin(pParams[0]);
	FUNCTION_END;
	return 0;
}



bool CDOSMainThread::AddConsoleCommandReceiver(IConsoleCommandReceiver* pReceiver)
{
	for (UINT i = 0; i < m_ConsoleCommandReceiverList.GetCount(); i++)
	{
		if (m_ConsoleCommandReceiverList[i] == pReceiver)
			return false;
	}
	m_ConsoleCommandReceiverList.Add(pReceiver);
	Log("对象0x%llX已注册为控制台命令接收者", pReceiver->GetReceiverID());
	return true;
}
bool CDOSMainThread::DeleteConsoleCommandReceiver(IConsoleCommandReceiver* pReceiver)
{
	for (UINT i = 0; i < m_ConsoleCommandReceiverList.GetCount(); i++)
	{
		if (m_ConsoleCommandReceiverList[i] == pReceiver)
		{
			m_ConsoleCommandReceiverList.Delete(i);
			Log("控制台命令接收者0x%llX已注销", pReceiver->GetReceiverID());
			return true;
		}
	}

	return false;
}

