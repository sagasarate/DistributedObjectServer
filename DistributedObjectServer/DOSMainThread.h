#pragma once

#define SERVER_UPDATE_LIMIT			16
#define SERVER_LINK_UPDATE_LIMIT	128
#define CLIENT_UPDATE_LIMIT			128		
#define MS_UDP_UPDATE_LIMIT			16
#define AS_UDP_UPDATE_LIMIT			64
#define CHAT_UDP_UPDATE_LIMIT		16

class CDOSMainThread :
	public CDOSServerThread,public CStaticObject4<CDOSMainThread>
{
protected:		
	enum STATUS
	{
		STATUS_NONE,
		STATUS_PROXY_PLUGIN_LOAD,
		STATUS_PLUGIN_LOAD,
		STATUS_WORKING,
		STATUS_SHUTDOWN,
	};
	
	STATUS										m_Status;

	CDistributedObjectManager					m_PluginObjectManager;
	CEasyArray<PLUGIN_INFO>						m_PluginList;
	CEasyArray<UINT>							m_PluginReleaseList;
	CEasyCriticalSection						m_PluginReleaseLock;
	CEasyTimer									m_PluginReleaseCheckTimer;
	

	DECLARE_CLASS_INFO_STATIC(CDOSMainThread)

public:
	CDOSMainThread(void);
	virtual ~CDOSMainThread(void);

	virtual bool OnStart();

	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);
	
	virtual void OnBeginTerminate();
	virtual bool OnTerminating();	

	virtual void OnTerminate();

	

	virtual int GetClientCount();
	virtual LPCTSTR GetConfigFileName();

	CDistributedObjectManager * GetDistributedObjectManager();

	bool QueryFreePlugin(UINT PluginID);
	MONO_DOMAIN_INFO* GetPluginDomain(UINT PluginID);

	bool AddConsoleCommandReceiver(IConsoleCommandReceiver * pReceiver);
	bool DeleteConsoleCommandReceiver(IConsoleCommandReceiver* pReceiver);

protected:
	void StartPluginsLoad();
	bool DoPluginsLoad();
	void FreePlugins();
	//void CompileLibs();
	void StartProxyPluginsLoad();
	bool DoProxyPluginsLoad();

	bool LoadPlugin(PLUGIN_INFO& PluginInfo);
	bool FreePlugin(PLUGIN_INFO& PluginInfo);
	bool FreePlugin(UINT PluginID);

	bool LoadNativePlugin(PLUGIN_INFO& PluginInfo);
	bool FreeNativePlugin(PLUGIN_INFO& PluginInfo);

	bool LoadCSPlugin(PLUGIN_INFO& PluginInfo);
	bool FreeCSPlugin(PLUGIN_INFO& PluginInfo);


	bool LoadJSPlugin(PLUGIN_INFO& PluginInfo);
	bool FreeJSPlugin(PLUGIN_INFO& PluginInfo);

	int ShowObjectCount(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount);
	int ShowGroupInfo(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount);
	int ListPlugin(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount);
	int ReleasePlugin(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount);

	

	//CEasyString GetProjectGUID(LPCTSTR PrjName);
	//bool CreateCSProj(LPCTSTR szPrjName, LPCTSTR szPrjDir, const CEasyArray<CEasyString>& SourceDirs, LPCTSTR szOutFileName);
	//
	//void PrintMCSMsg(LPTSTR szMsg);

	//bool CallMCS(CEasyArray<CEasyString>& SourceList, CEasyArray<CEasyString>& LibList, LPCTSTR szOutFileName, bool IsDebug, HANDLE& hMCSProcess);
};


inline CDistributedObjectManager * CDOSMainThread::GetDistributedObjectManager()
{
	return &m_PluginObjectManager;
}

