#include "stdafx.h"


CDistributedObjectOperator::CDistributedObjectOperator()
{
	FUNCTION_BEGIN;
	m_pManager = NULL;
	m_pDistributedObject = NULL;
	m_IsCommandReceiver = false;
	FUNCTION_END;
}

CDistributedObjectOperator::~CDistributedObjectOperator()
{
}

bool CDistributedObjectOperator::Init(CDistributedObjectManager* pManager, UINT PoolID, IDistributedObject* pDistributedObject)
{
	FUNCTION_BEGIN;
	m_pManager = pManager;
	m_pDistributedObject = pDistributedObject;
	m_pDistributedObject->AddUseRef();
	SetID(PoolID);
	return true;
	FUNCTION_END;
	return false;
}



bool CDistributedObjectOperator::Initialize()
{
	FUNCTION_BEGIN;
	if (!CDOSBaseObject::Initialize())
		return false;

	return m_pDistributedObject->Initialize(this);
	FUNCTION_END;
	return false;
}

void CDistributedObjectOperator::Destory()
{
	FUNCTION_BEGIN;
	if (m_IsCommandReceiver)
	{
		UnregisterCommandReceiver();
	}
	m_pDistributedObject->Destory();
	SAFE_RELEASE(m_pDistributedObject);
	if (m_pManager)
		m_pManager->DeleteObjectOperator(this);
	SetID(0);
	m_pManager = NULL;
	CDOSBaseObject::Destory();
	FUNCTION_END;
}

bool CDistributedObjectOperator::OnConsoleCommand(LPCTSTR szCommand)
{
	return m_pDistributedObject->OnConsoleCommand(szCommand);
}
UINT64 CDistributedObjectOperator::GetReceiverID()
{
	return GetObjectID().ID;
}

UINT CDistributedObjectOperator::GetRouterID()
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::GetRouterID();
	FUNCTION_END;
	return 0;
}

OBJECT_ID CDistributedObjectOperator::GetObjectID()
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::GetObjectID();
	FUNCTION_END;
	return 0;
}

int CDistributedObjectOperator::GetGroupIndex()
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::GetGroupIndex();
	FUNCTION_END;
	return -1;
}

bool CDistributedObjectOperator::SendMessage(OBJECT_ID ReceiverID, OBJECT_ID SenderID, MSG_ID_TYPE MsgID, WORD MsgFlag, LPCVOID pData, UINT DataSize)
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::SendMessage(ReceiverID, SenderID, MsgID, MsgFlag, pData, DataSize);
	FUNCTION_END;
	return false;
}
bool CDistributedObjectOperator::SendMessageMulti(OBJECT_ID* pReceiverIDList, UINT ReceiverCount, bool IsSorted, OBJECT_ID SenderID, MSG_ID_TYPE MsgID, WORD MsgFlag, LPCVOID pData, UINT DataSize)
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::SendMessageMulti(pReceiverIDList, ReceiverCount, IsSorted, SenderID, MsgID, MsgFlag, pData, DataSize);
	FUNCTION_END;
	return false;
}
bool CDistributedObjectOperator::BroadcastMessageToProxyByMask(WORD RouterID, BYTE ProxyType, UINT64 Mask, OBJECT_ID SenderID, MSG_ID_TYPE MsgID, WORD MsgFlag, LPCVOID pData, UINT DataSize)
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::BroadcastMessageToProxyByMask(RouterID, ProxyType, Mask, SenderID, MsgID, MsgFlag, pData, DataSize);
	FUNCTION_END;
	return false;
}
bool CDistributedObjectOperator::BroadcastMessageToProxyByGroup(WORD RouterID, BYTE ProxyType, UINT64 GroupID, OBJECT_ID SenderID, MSG_ID_TYPE MsgID, WORD MsgFlag, LPCVOID pData, UINT DataSize)
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::BroadcastMessageToProxyByGroup(RouterID, ProxyType, GroupID, SenderID, MsgID, MsgFlag, pData, DataSize);
	FUNCTION_END;
	return false;
}
CDOSMessagePacket* CDistributedObjectOperator::NewMessagePacket(UINT DataSize, UINT ReceiverCount)
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::NewMessagePacket(DataSize, ReceiverCount);
	FUNCTION_END;
	return NULL;
}
bool CDistributedObjectOperator::ReleaseMessagePacket(CDOSMessagePacket* pPacket)
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::ReleaseMessagePacket(pPacket);
	FUNCTION_END;
	return false;
}
bool CDistributedObjectOperator::SendMessagePacket(CDOSMessagePacket* pPacket)
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::SendMessagePacket(pPacket);
	FUNCTION_END;
	return false;
}


bool CDistributedObjectOperator::RegisterMsgMap(OBJECT_ID ProxyObjectID, MSG_ID_TYPE* pMsgIDList, int CmdCount)
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::RegisterMsgMap(ProxyObjectID, pMsgIDList, CmdCount);
	FUNCTION_END;
	return false;
}
bool CDistributedObjectOperator::UnregisterMsgMap(OBJECT_ID ProxyObjectID, MSG_ID_TYPE* pMsgIDList, int CmdCount)
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::UnregisterMsgMap(ProxyObjectID, pMsgIDList, CmdCount);
	FUNCTION_END;
	return false;
}
bool CDistributedObjectOperator::RegisterGlobalMsgMap(ROUTE_ID_TYPE ProxyRouterID, BYTE ProxyType, MSG_ID_TYPE MsgID, int MapType)
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::RegisterGlobalMsgMap(ProxyRouterID, ProxyType, MsgID, MapType);
	FUNCTION_END;
	return false;
}
bool CDistributedObjectOperator::UnregisterGlobalMsgMap(ROUTE_ID_TYPE ProxyRouterID, BYTE ProxyType, MSG_ID_TYPE MsgID)
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::UnregisterGlobalMsgMap(ProxyRouterID, ProxyType, MsgID);
	FUNCTION_END;
	return false;
}

bool CDistributedObjectOperator::SetUnhanleMsgReceiver(ROUTE_ID_TYPE ProxyRouterID, BYTE ProxyType)
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::SetUnhanleMsgReceiver(ProxyRouterID, ProxyType);
	FUNCTION_END;
	return false;
}

bool CDistributedObjectOperator::AddConcernedObject(OBJECT_ID ObjectID, bool NeedTest)
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::AddConcernedObject(ObjectID, NeedTest);
	FUNCTION_END;
	return false;
}
bool CDistributedObjectOperator::DeleteConcernedObject(OBJECT_ID ObjectID)
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::DeleteConcernedObject(ObjectID);
	FUNCTION_END;
	return false;
}

bool CDistributedObjectOperator::FindObject(UINT ObjectType, bool OnlyLocal)
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::FindObject(ObjectType, OnlyLocal);
	FUNCTION_END;
	return false;
}

bool CDistributedObjectOperator::ReportObject(OBJECT_ID TargetID, const void* pObjectInfoData, UINT DataSize)
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::ReportObject(TargetID, pObjectInfoData, DataSize);
	FUNCTION_END;
	return false;
}

bool CDistributedObjectOperator::CloseProxyObject(OBJECT_ID ProxyObjectID, UINT Delay)
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::CloseProxyObject(ProxyObjectID, Delay);
	FUNCTION_END;
	return false;
}

bool CDistributedObjectOperator::RequestProxyObjectIP(OBJECT_ID ProxyObjectID)
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::RequestProxyObjectIP(ProxyObjectID);
	FUNCTION_END;
	return false;
}

bool CDistributedObjectOperator::RegisterObject(DOS_OBJ_REGISTER_INFO& ObjectRegisterInfo)
{
	FUNCTION_BEGIN;
	return m_pManager->RegisterObject(ObjectRegisterInfo);
	FUNCTION_END;
	return false;
}
void CDistributedObjectOperator::Release()
{
	FUNCTION_BEGIN;
	if (m_pManager)
		m_pManager->UnregisterObject(this);
	FUNCTION_END;
}

bool CDistributedObjectOperator::QueryShutDown(OBJECT_ID TargetID, BYTE Level, UINT Param)
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::QueryShutDown(TargetID, Level, Param);
	FUNCTION_END;
}

void CDistributedObjectOperator::ShutDown(UINT PluginID)
{
	FUNCTION_BEGIN;
	CDOSMainThread::GetInstance()->QueryFreePlugin(PluginID);
	FUNCTION_END;
}
bool CDistributedObjectOperator::RegisterCommandReceiver()
{
	if (CDOSMainThread::GetInstance()->AddConsoleCommandReceiver(this))
	{
		m_IsCommandReceiver = true;
		return true;
	}
	return false;
}

bool CDistributedObjectOperator::UnregisterCommandReceiver()
{
	m_IsCommandReceiver = false;
	if (CDOSMainThread::GetInstance()->DeleteConsoleCommandReceiver(this))
	{
		return true;
	}
	return false;
}



bool CDistributedObjectOperator::RegisterLogger(UINT LogChannel, LPCTSTR FileName)
{
	FUNCTION_BEGIN;
	CEasyString LogFileName;
	CEasyString ModulePath = CFileTools::GetModulePath(NULL);

	CServerLogPrinter* pLog;

	LogFileName.Format("%s/Log/%s", (LPCTSTR)ModulePath, FileName);
	pLog = MONITORED_NEW(_T("CDistributedObjectOperator"), CServerLogPrinter, CDOSMainThread::GetInstance(), CServerLogPrinter::LOM_CONSOLE | CServerLogPrinter::LOM_FILE,
		CSystemConfig::GetInstance()->GetLogLevel(), LogFileName, CSystemConfig::GetInstance()->GetLogCacheSize());
	CLogManager::GetInstance()->AddChannel(LogChannel, pLog);
	SAFE_RELEASE(pLog);
	return true;
	FUNCTION_END;
	return false;
}

bool CDistributedObjectOperator::RegisterCSVLogger(UINT LogChannel, LPCTSTR FileName, LPCTSTR CSVLogHeader)
{
	FUNCTION_BEGIN;
	CEasyString LogFileName;
	CEasyString ModulePath = CFileTools::GetModulePath(NULL);

	CCSVFileLogPrinter* pLog;

	LogFileName.Format("%s/Log/%s", (LPCTSTR)ModulePath, FileName);
	pLog = MONITORED_NEW(_T("CDistributedObjectOperator"), CCSVFileLogPrinter, CSystemConfig::GetInstance()->GetLogLevel(), LogFileName, CSVLogHeader, CSystemConfig::GetInstance()->GetLogCacheSize());
	CLogManager::GetInstance()->AddChannel(LogChannel, pLog);
	SAFE_RELEASE(pLog);
	return true;
	FUNCTION_END;
	return false;
}

void CDistributedObjectOperator::SetServerWorkStatus(BYTE WorkStatus)
{
	CDOSMainThread::GetInstance()->SetServerStatus(SC_SST_SS_WORK_STATUS, CSmartValue(WorkStatus));
}

UINT CDistributedObjectOperator::AddTimer(UINT64 TimeOut, UINT64 Param, bool IsRepeat)
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::AddTimer(TimeOut, Param, IsRepeat);
	FUNCTION_END;
	return 0;
}
bool CDistributedObjectOperator::DeleteTimer(UINT ID)
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::DeleteTimer(ID);
	FUNCTION_END;
	return false;
}

bool CDistributedObjectOperator::SetBroadcastMask(OBJECT_ID ProxyObjectID, UINT64 GroupID)
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::SetBroadcastMask(ProxyObjectID, GroupID);
	FUNCTION_END;
	return false;
}

bool CDistributedObjectOperator::AddBroadcastMask(OBJECT_ID ProxyObjectID, UINT64 Mask)
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::AddBroadcastMask(ProxyObjectID, Mask);
	FUNCTION_END;
	return false;
}

bool CDistributedObjectOperator::RemoveBroadcastMask(OBJECT_ID ProxyObjectID, UINT64 Mask)
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::RemoveBroadcastMask(ProxyObjectID, Mask);
	FUNCTION_END;
	return false;
}
bool CDistributedObjectOperator::AddBroadcastGroup(OBJECT_ID ProxyObjectID, UINT64 GroupID)
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::AddBroadcastGroup(ProxyObjectID, GroupID);
	FUNCTION_END;
	return false;
}
bool CDistributedObjectOperator::RemoveBroadcastGroup(OBJECT_ID ProxyObjectID, UINT64 GroupID)
{
	FUNCTION_BEGIN;
	return CDOSBaseObject::RemoveBroadcastGroup(ProxyObjectID, GroupID);
	FUNCTION_END;
	return false;
}


bool CDistributedObjectOperator::OnMessage(CDOSMessage* pMessage)
{
	OBJECT_EXCEPTION_CATCH_START;
	if (m_pDistributedObject->OnPreTranslateMessage(pMessage) == COMMON_RESULT_MSG_NO_HANDLER)
	{
		return m_pDistributedObject->OnMessage(pMessage) != COMMON_RESULT_MSG_NO_HANDLER;
	}
	return true;
	OBJECT_EXCEPTION_CATCH_END;
	return false;
}
bool CDistributedObjectOperator::OnSystemMessage(CDOSMessage* pMessage)
{
	OBJECT_EXCEPTION_CATCH_START;
	if (m_pDistributedObject->OnSystemMessage(pMessage) == COMMON_RESULT_MSG_NO_HANDLER)
	{
		return CDOSBaseObject::OnSystemMessage(pMessage);
	}
	return true;
	OBJECT_EXCEPTION_CATCH_END;
	return false;
}
void CDistributedObjectOperator::OnConcernedObjectLost(OBJECT_ID ObjectID)
{
	OBJECT_EXCEPTION_CATCH_START;
	m_pDistributedObject->OnConcernedObjectLost(ObjectID);
	OBJECT_EXCEPTION_CATCH_END;
}
void CDistributedObjectOperator::OnFindObject(OBJECT_ID CallerID)
{
	OBJECT_EXCEPTION_CATCH_START;
	if (!m_pDistributedObject->OnFindObject(CallerID))
	{
		CDOSBaseObject::OnFindObject(CallerID);
	}
	OBJECT_EXCEPTION_CATCH_END;
}
void CDistributedObjectOperator::OnObjectReport(OBJECT_ID ObjectID, const void* pObjectInfoData, UINT DataSize)
{
	OBJECT_EXCEPTION_CATCH_START;
	m_pDistributedObject->OnObjectReport(ObjectID, pObjectInfoData, DataSize);
	OBJECT_EXCEPTION_CATCH_END;
}
void CDistributedObjectOperator::OnProxyObjectIPReport(OBJECT_ID ProxyObjectID, UINT Port, LPCSTR szIPString)
{
	OBJECT_EXCEPTION_CATCH_START;
	m_pDistributedObject->OnProxyObjectIPReport(ProxyObjectID, Port, szIPString);
	OBJECT_EXCEPTION_CATCH_END;
}
void CDistributedObjectOperator::OnShutDown(BYTE Level, UINT Param)
{
	OBJECT_EXCEPTION_CATCH_START;
	m_pDistributedObject->OnShutDown(Level, Param);
	OBJECT_EXCEPTION_CATCH_END;
}
int CDistributedObjectOperator::Update(int ProcessPacketLimit)
{
	OBJECT_EXCEPTION_CATCH_START;
	return m_pDistributedObject->Update(ProcessPacketLimit);
	OBJECT_EXCEPTION_CATCH_END;
	return 0;
}
void CDistributedObjectOperator::OnTimer(UINT ID, UINT64 Param, bool IsRepeat)
{
	OBJECT_EXCEPTION_CATCH_START;
	m_pDistributedObject->OnTimer(ID, Param, IsRepeat);
	OBJECT_EXCEPTION_CATCH_END;
}

void CDistributedObjectOperator::OnTimerRelease(UINT ID, UINT64 Param)
{
	OBJECT_EXCEPTION_CATCH_START;
	m_pDistributedObject->OnTimerRelease(ID, Param);
	OBJECT_EXCEPTION_CATCH_END;
}



