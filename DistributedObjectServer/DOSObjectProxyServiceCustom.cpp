#include "stdafx.h"



CDOSObjectProxyServiceCustom::CDOSObjectProxyServiceCustom()
{
	m_pServer = NULL;
	m_pProxyService = NULL;
}


CDOSObjectProxyServiceCustom::~CDOSObjectProxyServiceCustom()
{
}

void CDOSObjectProxyServiceCustom::Release()
{
	CNameObject::Release();
}
void CDOSObjectProxyServiceCustom::Destory()
{

	SAFE_RELEASE(m_pProxyService);


#ifdef WIN32
	if (FreeLibrary(m_PluginInfo.hModule))
#else
	if (dlclose(m_PluginInfo.hModule) == 0)
#endif
	{
		Log("�������ͷųɹ�%s", (LPCTSTR)m_PluginInfo.ModuleFileName);
	}
	else
	{
		Log("�������ͷ�ʧ��%s", (LPCTSTR)m_PluginInfo.ModuleFileName);
#ifndef WIN32
		Log("������Ϣ:%s", dlerror());
#endif
	}

	m_PluginInfo.hModule = NULL;
}
UINT CDOSObjectProxyServiceCustom::AddUseRef()
{
	return CNameObject::AddUseRef();
}
BYTE CDOSObjectProxyServiceCustom::GetProxyType()
{
	return m_Config.ProxyType;
}
void CDOSObjectProxyServiceCustom::SetID(UINT ID)
{
	CNameObject::SetID(ID);
}
UINT CDOSObjectProxyServiceCustom::GetID()
{
	return CNameObject::GetID();
}
bool CDOSObjectProxyServiceCustom::StartService()
{
	if (m_pProxyService)
		return m_pProxyService->StartService(this);
	return false;
}
bool CDOSObjectProxyServiceCustom::StartService(IDOSObjectProxyServiceOperator* pOperator)
{
	return StartService();
}
void CDOSObjectProxyServiceCustom::StopService()
{
	if (m_pProxyService)
		m_pProxyService->StopService();
}
bool CDOSObjectProxyServiceCustom::PushMessage(OBJECT_ID ObjectID, CDOSMessagePacket * pPacket)
{
	if (m_pProxyService)
		return m_pProxyService->PushMessage(ObjectID, pPacket);
	return false;
}

UINT  CDOSObjectProxyServiceCustom::GetConnectionCount()
{
	if (m_pProxyService)
		return m_pProxyService->GetConnectionCount();
	return 0;
}

float CDOSObjectProxyServiceCustom::GetCPUUsedRate()
{
	if (m_pProxyService)
		return m_pProxyService->GetCPUUsedRate();
	return 0;
}
float CDOSObjectProxyServiceCustom::GetCycleTime()
{
	if (m_pProxyService)
		return m_pProxyService->GetCycleTime();
	return 0;
}
UINT CDOSObjectProxyServiceCustom::GetMsgQueueLen()
{
	if (m_pProxyService)
		return m_pProxyService->GetMsgQueueLen();
	return 0;
}

UINT CDOSObjectProxyServiceCustom::GetGroupCount()
{
	if (m_pProxyService)
		return m_pProxyService->GetGroupCount();
	return 0;
}
float CDOSObjectProxyServiceCustom::GetGroupCPUUsedRate(UINT Index)
{
	if (m_pProxyService)
		return m_pProxyService->GetGroupCPUUsedRate(Index);
	return 0;
}
float CDOSObjectProxyServiceCustom::GetGroupCycleTime(UINT Index)
{
	if (m_pProxyService)
		return m_pProxyService->GetGroupCycleTime(Index);
	return 0;
}
UINT CDOSObjectProxyServiceCustom::GetGroupMsgQueueLen(UINT Index)
{
	if (m_pProxyService)
		return m_pProxyService->GetGroupMsgQueueLen(Index);
	return 0;
}


const CLIENT_PROXY_CONFIG& CDOSObjectProxyServiceCustom::GetConfig()
{
	return m_Config;
}

CNetServer * CDOSObjectProxyServiceCustom::GetNetServer()
{
	return m_pServer;
}
UINT CDOSObjectProxyServiceCustom::GetRouterID()
{
	return m_pServer->GetRouter()->GetRouterID();
}
bool CDOSObjectProxyServiceCustom::SendMessage(OBJECT_ID SenderID, OBJECT_ID ReceiverID, MSG_ID_TYPE MsgID, WORD MsgFlag, LPCVOID pData, UINT DataSize)
{
	return m_pServer->GetRouter()->RouterMessage(SenderID, ReceiverID, MsgID, MsgFlag, pData, DataSize);
}

bool CDOSObjectProxyServiceCustom::SendMessageMulti(OBJECT_ID SenderID, OBJECT_ID * pReceiverIDList, UINT ReceiverCount, bool IsSorted, MSG_ID_TYPE MsgID, WORD MsgFlag, LPCVOID pData, UINT DataSize)
{
	if (pReceiverIDList == NULL || ReceiverCount == 0)
	{
		return false;
	}
	int PacketSize = CDOSMessagePacket::CaculatePacketLength(DataSize, ReceiverCount);

	CDOSMessagePacket * pNewPacket = m_pServer->NewMessagePacket(PacketSize);
	if (pNewPacket == NULL)
	{
		PrintDOSLog(_T("������Ϣ�ڴ��ʧ�ܣ�"));
		return false;
	}
	pNewPacket->GetMessage().SetMsgID(MsgID);
	pNewPacket->GetMessage().SetSenderID(SenderID);
	pNewPacket->GetMessage().SetDataLength(DataSize);
	pNewPacket->GetMessage().SetMsgFlag(MsgFlag);
	if (pData)
		pNewPacket->GetMessage().PutMsgData(0, pData, DataSize);
	pNewPacket->SetTargetIDs(ReceiverCount, pReceiverIDList);
	if (!IsSorted)
	{
		pNewPacket->SortTargetIDs();
	}

	pNewPacket->MakePacketLength();

	bool Ret = m_pServer->GetRouter()->RouterMessage(pNewPacket);
	m_pServer->ReleaseMessagePacket(pNewPacket);
	return Ret;
}
CDOSMessagePacket * CDOSObjectProxyServiceCustom::NewMessagePacket(UINT DataSize, UINT ReceiverCount)
{
	if (ReceiverCount == 0)
	{
		return NULL;
	}
	int PacketSize = CDOSMessagePacket::CaculatePacketLength(DataSize, ReceiverCount);

	return m_pServer->NewMessagePacket(PacketSize);
}
bool CDOSObjectProxyServiceCustom::ReleaseMessagePacket(CDOSMessagePacket * pPacket)
{
	return m_pServer->ReleaseMessagePacket(pPacket);
}
bool CDOSObjectProxyServiceCustom::SendMessagePacket(CDOSMessagePacket * pPacket)
{
	return m_pServer->GetRouter()->RouterMessage(pPacket);
}

bool CDOSObjectProxyServiceCustom::Init(CDOSServer * pServer, CLIENT_PROXY_PLUGIN_INFO& PluginInfo)
{
	m_pServer = pServer;
	m_Config = PluginInfo;
	m_PluginInfo = PluginInfo;
		
	if (m_PluginInfo.ModuleFileName.IsEmpty())
#ifdef _DEBUG
		m_PluginInfo.ModuleFileName = m_PluginInfo.PluginName + "D";
#else
		m_PluginInfo.ModuleFileName = m_PluginInfo.PluginName;
#endif

#ifndef WIN32
	m_PluginInfo.ModuleFileName.Replace('\\', '/');
#endif
	m_PluginInfo.ModuleFileName = CFileTools::MakeModuleFullPath(NULL, m_PluginInfo.ModuleFileName);
	//��չ������
	CEasyString FileExt = CFileTools::GetPathFileExtName(m_PluginInfo.ModuleFileName);
	if (FileExt.GetLength() <= 1)
	{
#ifdef WIN32
		m_PluginInfo.ModuleFileName = m_PluginInfo.ModuleFileName + ".dll";
#else
		m_PluginInfo.ModuleFileName = m_PluginInfo.ModuleFileName + ".so";
#endif
	}

	if (PluginInfo.PluginName.IsEmpty())
		PluginInfo.PluginName = CFileTools::GetPathFileName(PluginInfo.ModuleFileName);

	//�������Ŀ¼����־Ŀ¼
	if (m_PluginInfo.ConfigDir.IsEmpty())
	{
		m_PluginInfo.ConfigDir = CFileTools::GetModulePath(NULL);
	}
	else
	{
		if (CFileTools::IsAbsolutePath(m_PluginInfo.ConfigDir))
			m_PluginInfo.ConfigDir = CFileTools::MakeFullPath(m_PluginInfo.ConfigDir);
		else
			m_PluginInfo.ConfigDir = CFileTools::MakeModuleFullPath(NULL, m_PluginInfo.ConfigDir);
	}

	if (m_PluginInfo.LogDir.IsEmpty())
	{
		m_PluginInfo.LogDir = CFileTools::MakeModuleFullPath(NULL, "Log");
	}
	else
	{
		if (CFileTools::IsAbsolutePath(m_PluginInfo.LogDir))
			m_PluginInfo.LogDir = CFileTools::MakeFullPath(m_PluginInfo.LogDir);
		else
			m_PluginInfo.LogDir = CFileTools::MakeModuleFullPath(NULL, m_PluginInfo.LogDir);
	}

	if (!CFileTools::IsDirExist(m_PluginInfo.LogDir))
	{
		if (!CFileTools::CreateDirEx(m_PluginInfo.LogDir))
		{
			Log("�޷�������־Ŀ¼:%s", (LPCTSTR)m_PluginInfo.LogDir);
			return false;
		}
	}

	

	Log("��ʼװ�ش�����%s", (LPCTSTR)m_PluginInfo.ModuleFileName);
#ifdef WIN32
	m_PluginInfo.hModule = LoadLibrary(m_PluginInfo.ModuleFileName);
#else
	m_PluginInfo.hModule = dlopen(m_PluginInfo.ModuleFileName, RTLD_NOW | RTLD_LOCAL | RTLD_DEEPBIND);
#endif
	if (m_PluginInfo.hModule)
	{
#ifdef WIN32
		if (CSystemConfig::GetInstance()->IsPreLoadModuleSym())
		{
			CExceptionParser::GetInstance()->SymLoadFromModule(m_PluginInfo.ModuleFileName);
		}
		m_PluginInfo.pInitFN = (CLIENT_PROXY_INIT_FN)GetProcAddress(m_PluginInfo.hModule, CLIENT_PROXY_INIT_FN_NAME);
		m_PluginInfo.pGetServiceFN = (CLIENT_PROXY_GET_SERVICE_FN)GetProcAddress(m_PluginInfo.hModule, CLIENT_PROXY_GET_SERVICE_FN_NAME);
		if (m_PluginInfo.pInitFN && m_PluginInfo.pGetServiceFN)
#else
		m_PluginInfo.pInitFN = (CLIENT_PROXY_INIT_FN)dlsym(m_PluginInfo.hModule, CLIENT_PROXY_INIT_FN_NAME);
		LPCTSTR InitFNError = dlerror();
		m_PluginInfo.pGetServiceFN = (CLIENT_PROXY_GET_SERVICE_FN)dlsym(m_PluginInfo.hModule, CLIENT_PROXY_GET_SERVICE_FN_NAME);
		LPCTSTR GetServiceFNError = dlerror();
		if (InitFNError == NULL && GetServiceFNError == NULL && m_PluginInfo.pInitFN && m_PluginInfo.pGetServiceFN)
#endif
		{

			CEasyString LogFileName;

			CServerLogPrinter* pLog;
			LogFileName.Format("%s/Plugin.%s", (LPCTSTR)m_PluginInfo.LogDir, (LPCTSTR)CFileTools::GetPathFileName(m_PluginInfo.PluginName));
			pLog = MONITORED_NEW(_T("CDOSObjectProxyServiceCustom"), CServerLogPrinter, CDOSMainThread::GetInstance(), CServerLogPrinter::LOM_CONSOLE | CServerLogPrinter::LOM_FILE,
				CSystemConfig::GetInstance()->GetLogLevel(), LogFileName, CSystemConfig::GetInstance()->GetLogCacheSize());
			pLog->SetBackup(CSystemConfig::GetInstance()->GetLogBackupDir(), CSystemConfig::GetInstance()->GetLogBackupDelay());
			CLogManager::GetInstance()->AddChannel(m_PluginInfo.LogChannel, pLog);
			SAFE_RELEASE(pLog);


			if (m_PluginInfo.pInitFN(m_PluginInfo.ID, m_PluginInfo.LogChannel, m_PluginInfo.ConfigDir, m_PluginInfo.LogDir))
			{
				m_pProxyService = m_PluginInfo.pGetServiceFN();
				if (m_pProxyService)
				{
					Log("������װ�سɹ�%s", (LPCTSTR)m_PluginInfo.ModuleFileName);
					m_PluginInfo.PluginStatus = PLUGIN_STATUS_LOADED;
					return true;
				}
				else
				{
					Log("������񴴽�ʧ��%s", (LPCTSTR)m_PluginInfo.ModuleFileName);

					return false;
				}
			}
			else
			{
				Log("��������ʼ��ʧ��%s", (LPCTSTR)m_PluginInfo.ModuleFileName);

				return false;
			}

		}
		else
		{
			Log("���Ϸ��Ĵ�����%s", (LPCTSTR)m_PluginInfo.ModuleFileName);
#ifndef WIN32
			Log("InitFN������Ϣ:%s", InitFNError);
			Log("GetServiceFN������Ϣ:%s", GetServiceFNError);
#endif
		}
	}
	else
	{
		Log("�޷�װ�ش�����(%d)%s", GetLastError(), (LPCTSTR)m_PluginInfo.ModuleFileName);
#ifndef WIN32
		Log("������Ϣ:%s", dlerror());
#endif
	}

	m_PluginInfo.PluginStatus = PLUGIN_STATUS_ERROR;

	return false;
}




