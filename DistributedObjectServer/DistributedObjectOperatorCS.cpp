#include "stdafx.h"


CDistributedObjectOperatorCS::CDistributedObjectOperatorCS()
{
	FUNCTION_BEGIN;
	m_pManager = NULL;
	m_hCSOperatorObject = 0;
	m_hCSObject = 0;
	m_IsCommandReceiver = false;
	FUNCTION_END;
}

CDistributedObjectOperatorCS::~CDistributedObjectOperatorCS()
{
}

bool CDistributedObjectOperatorCS::Init(CDistributedObjectManager* pManager, UINT PoolID, MONO_DOMAIN_INFO& MonoDomainInfo, MonoObject* pDistributedObject)
{
	FUNCTION_BEGIN;
	if (pDistributedObject == NULL)
		return false;
	m_pManager = pManager;
	SetID(PoolID);
	m_MonoDomainInfo = MonoDomainInfo;
	m_hCSObject = mono_gchandle_new(pDistributedObject, false);
	MonoObject* pOperatorObject = CCSVMHelper::GetInstance()->MonoCreateDistributedObjectOperator(m_MonoDomainInfo, this);
	if (pOperatorObject)
	{
		m_hCSOperatorObject = mono_gchandle_new(pOperatorObject, false);

		m_MonoClassInfo_DO.pClass = mono_object_get_class(pDistributedObject);
		m_MonoClassInfo_DO.pInitializeMethod = CCSVMHelper::GetInstance()->MonoGetClassMethod(m_MonoClassInfo_DO.pClass,
			MONO_CLASS_METHOD_NAME_DO_INITIALIZE, MONO_CLASS_METHOD_PARAM_DO_INITIALIZE);
		m_MonoClassInfo_DO.pDestoryMethod = CCSVMHelper::GetInstance()->MonoGetClassMethod(m_MonoClassInfo_DO.pClass,
			MONO_CLASS_METHOD_NAME_DO_DESTORY, MONO_CLASS_METHOD_PARAM_DO_DESTORY);
		m_MonoClassInfo_DO.pOnPreTranslateMessageMethod = CCSVMHelper::GetInstance()->MonoGetClassMethod(m_MonoClassInfo_DO.pClass,
			MONO_CLASS_METHOD_NAME_DO_ONPRETRANSLATEMESSAGE, MONO_CLASS_METHOD_PARAM_DO_ONPRETRANSLATEMESSAGE);
		m_MonoClassInfo_DO.pOnMessageMethod = CCSVMHelper::GetInstance()->MonoGetClassMethod(m_MonoClassInfo_DO.pClass,
			MONO_CLASS_METHOD_NAME_DO_ONMESSAGE, MONO_CLASS_METHOD_PARAM_DO_ONMESSAGE);
		m_MonoClassInfo_DO.pOnSystemMessageMethod = CCSVMHelper::GetInstance()->MonoGetClassMethod(m_MonoClassInfo_DO.pClass,
			MONO_CLASS_METHOD_NAME_DO_ONSYSTEMMESSAGE, MONO_CLASS_METHOD_PARAM_DO_ONSYSTEMMESSAGE);
		m_MonoClassInfo_DO.pOnConcernedObjectLostMethod = CCSVMHelper::GetInstance()->MonoGetClassMethod(m_MonoClassInfo_DO.pClass,
			MONO_CLASS_METHOD_NAME_DO_ONCONCERNEDOBJECTLOST, MONO_CLASS_METHOD_PARAM_DO_ONCONCERNEDOBJECTLOST);
		m_MonoClassInfo_DO.pOnFindObjectMethod = CCSVMHelper::GetInstance()->MonoGetClassMethod(m_MonoClassInfo_DO.pClass,
			MONO_CLASS_METHOD_NAME_DO_ONFINDOBJECT, MONO_CLASS_METHOD_PARAM_DO_ONFINDOBJECT);
		m_MonoClassInfo_DO.pOnObjectReportMethod = CCSVMHelper::GetInstance()->MonoGetClassMethod(m_MonoClassInfo_DO.pClass,
			MONO_CLASS_METHOD_NAME_DO_ONOBJECTREPORT, MONO_CLASS_METHOD_PARAM_DO_ONOBJECTREPORT);
		m_MonoClassInfo_DO.pOnProxyObjectIPReportMethod = CCSVMHelper::GetInstance()->MonoGetClassMethod(m_MonoClassInfo_DO.pClass,
			MONO_CLASS_METHOD_NAME_DO_ONPROXYOBJECTIPREPORT, MONO_CLASS_METHOD_PARAM_DO_ONPROXYOBJECTIPREPORT);
		m_MonoClassInfo_DO.pOnShutDownMethod = CCSVMHelper::GetInstance()->MonoGetClassMethod(m_MonoClassInfo_DO.pClass,
			MONO_CLASS_METHOD_NAME_DO_ONSHUTDOWN, MONO_CLASS_METHOD_PARAM_DO_ONSHUTDOWN);
		m_MonoClassInfo_DO.pUpdateMethod = CCSVMHelper::GetInstance()->MonoGetClassMethod(m_MonoClassInfo_DO.pClass,
			MONO_CLASS_METHOD_NAME_DO_UPDATE, MONO_CLASS_METHOD_PARAM_DO_UPDATE);
		m_MonoClassInfo_DO.pOnExceptionMethod = CCSVMHelper::GetInstance()->MonoGetClassMethod(m_MonoClassInfo_DO.pClass,
			MONO_CLASS_METHOD_NAME_DO_ONEXCEPTION, MONO_CLASS_METHOD_PARAM_DO_ONEXCEPTION);
		m_MonoClassInfo_DO.pOnConsoleCommandMethod = CCSVMHelper::GetInstance()->MonoGetClassMethod(m_MonoClassInfo_DO.pClass,
			MONO_CLASS_METHOD_NAME_DO_ONCONSOLECOMMAND, MONO_CLASS_METHOD_PARAM_DO_ONCONSOLECOMMAND);
		m_MonoClassInfo_DO.pOnTimerMethod = CCSVMHelper::GetInstance()->MonoGetClassMethod(m_MonoClassInfo_DO.pClass,
			MONO_CLASS_METHOD_NAME_DO_ONTIMER, MONO_CLASS_METHOD_PARAM_DO_ONTIMER);
		m_MonoClassInfo_DO.pOnTimerReleaseMethod = CCSVMHelper::GetInstance()->MonoGetClassMethod(m_MonoClassInfo_DO.pClass,
			MONO_CLASS_METHOD_NAME_DO_ONTIMERRELEASE, MONO_CLASS_METHOD_PARAM_DO_ONTIMERRELEASE);
		return true;
	}

	FUNCTION_END;
	return false;
}

bool CDistributedObjectOperatorCS::Initialize()
{
	FUNCTION_BEGIN;
	if (!CDOSBaseObject::Initialize())
		return false;
	return CallCSInitialize();
	FUNCTION_END;
	return false;
}

void CDistributedObjectOperatorCS::Destory()
{
	FUNCTION_BEGIN;
	if (m_IsCommandReceiver)
	{
		UnregisterCommandReceiver();
	}
	
	CallCSDestory();
	if (m_hCSObject)
	{
		mono_gchandle_free(m_hCSObject);
		m_hCSObject = 0;
	}
	if (m_hCSOperatorObject)
	{
		mono_gchandle_free(m_hCSOperatorObject);
		m_hCSOperatorObject = 0;
	}
	if (m_pManager)
		m_pManager->DeleteObjectOperator(this);
	SetID(0);
	m_pManager = NULL;
	CDOSBaseObject::Destory();
	FUNCTION_END;
}

void CDistributedObjectOperatorCS::Release()
{
	if (m_pManager)
		m_pManager->UnregisterObject(this);
}

bool CDistributedObjectOperatorCS::OnConsoleCommand(LPCTSTR szCommand)
{
	if (CallOnConsoleCommand(szCommand))
		return true;
	else
		return false;
}

UINT64 CDistributedObjectOperatorCS::GetReceiverID()
{
	return GetObjectID().ID;
}

void CDistributedObjectOperatorCS::ShutDown(UINT PluginID)
{
	CDOSMainThread::GetInstance()->QueryFreePlugin(PluginID);
}


bool CDistributedObjectOperatorCS::RegisterCommandReceiver()
{
	if (CDOSMainThread::GetInstance()->AddConsoleCommandReceiver(this))
	{
		m_IsCommandReceiver = true;
		return true;
	}
	return false;
}

bool CDistributedObjectOperatorCS::UnregisterCommandReceiver()
{
	m_IsCommandReceiver = false;
	if (CDOSMainThread::GetInstance()->DeleteConsoleCommandReceiver(this))
	{
		return true;
	}
	return false;
}

bool CDistributedObjectOperatorCS::OnMessage(CDOSMessage* pMessage)
{
	OBJECT_EXCEPTION_CATCH_START;
	if (CallCSOnPreTranslateMessage(pMessage->GetMsgID(), pMessage->GetMsgFlag(), pMessage->GetSenderID(), (const BYTE*)pMessage->GetMsgData(), pMessage->GetDataLength())== COMMON_RESULT_MSG_NO_HANDLER)
	{
		return CallCSOnMessage(pMessage->GetMsgID(), pMessage->GetMsgFlag(), pMessage->GetSenderID(), (const BYTE*)pMessage->GetMsgData(), pMessage->GetDataLength()) != COMMON_RESULT_MSG_NO_HANDLER;
	}
	return true;
	OBJECT_EXCEPTION_CATCH_END;
	return false;
}
bool CDistributedObjectOperatorCS::OnSystemMessage(CDOSMessage* pMessage)
{
	OBJECT_EXCEPTION_CATCH_START;
	if (CallCSOnSystemMessage(pMessage->GetMsgID(), pMessage->GetMsgFlag(), pMessage->GetSenderID(), (const BYTE*)pMessage->GetMsgData(), pMessage->GetDataLength())== COMMON_RESULT_MSG_NO_HANDLER)
	{
		return CDOSBaseObject::OnSystemMessage(pMessage);
	}
	return true;
	OBJECT_EXCEPTION_CATCH_END;
	return false;
}
void CDistributedObjectOperatorCS::OnConcernedObjectLost(OBJECT_ID ObjectID)
{
	OBJECT_EXCEPTION_CATCH_START;
	CallCSOnConcernedObjectLost(ObjectID);
	OBJECT_EXCEPTION_CATCH_END;
}
void CDistributedObjectOperatorCS::OnFindObject(OBJECT_ID CallerID)
{
	OBJECT_EXCEPTION_CATCH_START;
	if (!CallCSOnFindObject(CallerID))
	{
		CDOSBaseObject::OnFindObject(CallerID);
	}
	OBJECT_EXCEPTION_CATCH_END;
}
void CDistributedObjectOperatorCS::OnObjectReport(OBJECT_ID ObjectID, const void* pObjectInfoData, UINT DataSize)
{
	OBJECT_EXCEPTION_CATCH_START;
	CallCSOnObjectReport(ObjectID, pObjectInfoData, DataSize);
	OBJECT_EXCEPTION_CATCH_END;
}
void CDistributedObjectOperatorCS::OnProxyObjectIPReport(OBJECT_ID ProxyObjectID, UINT Port, LPCSTR szIPString)
{
	OBJECT_EXCEPTION_CATCH_START;
	CallCSOnProxyObjectIPReport(ProxyObjectID, Port, szIPString);
	OBJECT_EXCEPTION_CATCH_END;
}
void CDistributedObjectOperatorCS::OnShutDown(BYTE Level, UINT Param)
{
	OBJECT_EXCEPTION_CATCH_START;
	CallCSOnShutDown(Level, Param);
	OBJECT_EXCEPTION_CATCH_END;
}
int CDistributedObjectOperatorCS::Update(int ProcessPacketLimit)
{
	OBJECT_EXCEPTION_CATCH_START;
	return CallCSUpdate(ProcessPacketLimit);
	OBJECT_EXCEPTION_CATCH_END;
	return 0;
}
void CDistributedObjectOperatorCS::OnTimer(UINT ID, UINT64 Param, bool IsRepeat)
{
	OBJECT_EXCEPTION_CATCH_START;
	CallCSOnTimer(ID, Param, IsRepeat);
	OBJECT_EXCEPTION_CATCH_END;
}

void CDistributedObjectOperatorCS::OnTimerRelease(UINT ID, UINT64 Param)
{
	OBJECT_EXCEPTION_CATCH_START;
	CallCSOnTimerRelease(ID, Param);
	OBJECT_EXCEPTION_CATCH_END;
}

bool CDistributedObjectOperatorCS::CallCSInitialize()
{
	FUNCTION_BEGIN;
	if (m_MonoClassInfo_DO.pInitializeMethod)
	{
		mono_thread_attach(m_MonoDomainInfo.pMonoDomain);
		MonoObject* pObject = mono_gchandle_get_target(m_hCSObject);
		if (pObject)
		{
			MonoObject* pOperatorObject = mono_gchandle_get_target(m_hCSOperatorObject);
			if (pOperatorObject)
			{
				MonoObject* pException = NULL;
				LPVOID Params[1];
				Params[0] = pOperatorObject;
				MonoObject* pReturnValue = mono_runtime_invoke(m_MonoClassInfo_DO.pInitializeMethod, pObject, Params, &pException);
				if (pException)
				{
					CCSVMHelper::GetInstance()->ProcessMonoException(pException);
					CallCSOnException(pException);
				}
				else if (pReturnValue)
				{
					void* pValue = mono_object_unbox(pReturnValue);
					if (pValue)
					{
						return *((bool*)pValue);
					}
					else
					{
						Log("无法获取返回值");
					}
				}
				else
				{
					Log("没有返回值");
				}
			}
			else
			{
				LogMono("无法获得Operator对象");
			}

		}
		else
		{
			LogMono("无法获得DistributedObject对象");
		}
	}
	FUNCTION_END;
	return false;
}
void CDistributedObjectOperatorCS::CallCSDestory()
{
	FUNCTION_BEGIN;
	if (m_MonoClassInfo_DO.pDestoryMethod)
	{
		mono_domain_set(m_MonoDomainInfo.pMonoDomain, MONO_FALSE);
		MonoObject* pObject = mono_gchandle_get_target(m_hCSObject);
		if (pObject)
		{
			MonoObject* pException = NULL;
			mono_runtime_invoke(m_MonoClassInfo_DO.pDestoryMethod, pObject, NULL, &pException);
			if (pException)
			{
				CCSVMHelper::GetInstance()->ProcessMonoException(pException);
				CallCSOnException(pException);
			}
		}
		else
		{
			LogMono("无法获得DistributedObject对象");
		}

	}
	FUNCTION_END;
}
int CDistributedObjectOperatorCS::CallCSOnPreTranslateMessage(MSG_ID_TYPE MsgID, WORD MsgFlag, OBJECT_ID SenderID, const BYTE* pData, UINT DataSize)
{
	FUNCTION_BEGIN;
	if (m_MonoClassInfo_DO.pOnPreTranslateMessageMethod)
	{
		mono_domain_set(m_MonoDomainInfo.pMonoDomain, MONO_FALSE);
		MonoObject* pObject = mono_gchandle_get_target(m_hCSObject);
		if (pObject)
		{
			MonoObject* pException = NULL;
			MonoArray* pByteArray = CCSVMHelper::GetInstance()->MonoCreateByteArray(m_MonoDomainInfo, pData, DataSize);
			if (pByteArray)
			{
				LPVOID Params[4];
				Params[0] = &MsgID;
				Params[1] = &MsgFlag;
				Params[2] = &SenderID;
				Params[3] = pByteArray;
				MonoObject* pReturnValue = mono_runtime_invoke(m_MonoClassInfo_DO.pOnPreTranslateMessageMethod, pObject, Params, &pException);
				if (pException)
				{
					CCSVMHelper::GetInstance()->ProcessMonoException(pException);
					CallCSOnException(pException);
				}
				else if (pReturnValue)
				{
					void* pValue = mono_object_unbox(pReturnValue);
					if (pValue)
					{
						return (*((int*)pValue));
					}
					else
					{
						Log("无法获取返回值");
					}
				}
				else
				{
					Log("没有返回值");
				}
			}
			else
			{
				Log("无法创建对象");
			}
		}
		else
		{
			LogMono("无法获得DistributedObject对象");
		}


	}
	FUNCTION_END;
	return COMMON_RESULT_MSG_NO_HANDLER;
}
int CDistributedObjectOperatorCS::CallCSOnMessage(MSG_ID_TYPE MsgID, WORD MsgFlag, OBJECT_ID SenderID, const BYTE* pData, UINT DataSize)
{
	FUNCTION_BEGIN;
	if (m_MonoClassInfo_DO.pOnMessageMethod)
	{
		mono_domain_set(m_MonoDomainInfo.pMonoDomain, MONO_FALSE);
		MonoObject* pObject = mono_gchandle_get_target(m_hCSObject);
		if (pObject)
		{
			MonoObject* pException = NULL;
			MonoArray* pByteArray = CCSVMHelper::GetInstance()->MonoCreateByteArray(m_MonoDomainInfo, pData, DataSize);
			if (pByteArray)
			{
				LPVOID Params[4];
				Params[0] = &MsgID;
				Params[1] = &MsgFlag;
				Params[2] = &SenderID;
				Params[3] = pByteArray;
				MonoObject* pReturnValue = mono_runtime_invoke(m_MonoClassInfo_DO.pOnMessageMethod, pObject, Params, &pException);
				if (pException)
				{
					CCSVMHelper::GetInstance()->ProcessMonoException(pException);
					CallCSOnException(pException);
				}
				else if (pReturnValue)
				{
					void* pValue = mono_object_unbox(pReturnValue);
					if (pValue)
					{
						return (*((int*)pValue));
					}
					else
					{
						Log("无法获取返回值");
					}
				}
				else
				{
					Log("没有返回值");
				}
			}
			else
			{
				Log("无法创建对象");
			}
		}
		else
		{
			LogMono("无法获得DistributedObject对象");
		}


	}
	FUNCTION_END;
	return COMMON_RESULT_MSG_NO_HANDLER;
}
int CDistributedObjectOperatorCS::CallCSOnSystemMessage(MSG_ID_TYPE MsgID, WORD MsgFlag, OBJECT_ID SenderID, const BYTE* pData, UINT DataSize)
{
	FUNCTION_BEGIN;
	if (m_MonoClassInfo_DO.pOnSystemMessageMethod)
	{
		mono_domain_set(m_MonoDomainInfo.pMonoDomain, MONO_FALSE);
		MonoObject* pObject = mono_gchandle_get_target(m_hCSObject);
		if (pObject)
		{
			MonoObject* pException = NULL;
			MonoArray* pByteArray = CCSVMHelper::GetInstance()->MonoCreateByteArray(m_MonoDomainInfo, pData, DataSize);
			if (pByteArray)
			{
				LPVOID Params[4];
				Params[0] = &MsgID;
				Params[1] = &MsgFlag;
				Params[2] = &SenderID;
				Params[3] = pByteArray;
				MonoObject* pReturnValue = mono_runtime_invoke(m_MonoClassInfo_DO.pOnSystemMessageMethod, pObject, Params, &pException);
				if (pException)
				{
					CCSVMHelper::GetInstance()->ProcessMonoException(pException);
					CallCSOnException(pException);
				}
				else if (pReturnValue)
				{
					void* pValue = mono_object_unbox(pReturnValue);
					if (pValue)
					{
						return (*((int*)pValue));
					}
					else
					{
						Log("无法获取返回值");
					}
				}
				else
				{
					Log("没有返回值");
				}
			}
			else
			{
				Log("无法创建对象");
			}
		}
		else
		{
			LogMono("无法获得DistributedObject对象");
		}


	}
	FUNCTION_END;
	return COMMON_RESULT_MSG_NO_HANDLER;
}
void CDistributedObjectOperatorCS::CallCSOnConcernedObjectLost(OBJECT_ID ObjectID)
{
	FUNCTION_BEGIN;
	if (m_MonoClassInfo_DO.pOnConcernedObjectLostMethod)
	{
		mono_domain_set(m_MonoDomainInfo.pMonoDomain, MONO_FALSE);
		MonoObject* pObject = mono_gchandle_get_target(m_hCSObject);
		if (pObject)
		{
			MonoObject* pException = NULL;
			LPVOID Params[1];
			Params[0] = &ObjectID;
			MonoObject* pReturnValue = mono_runtime_invoke(m_MonoClassInfo_DO.pOnConcernedObjectLostMethod, pObject, Params, &pException);
			if (pException)
			{
				CCSVMHelper::GetInstance()->ProcessMonoException(pException);
				CallCSOnException(pException);
			}
		}
		else
		{
			LogMono("无法获得DistributedObject对象");
		}

	}
	FUNCTION_END;
}
bool CDistributedObjectOperatorCS::CallCSOnFindObject(OBJECT_ID CallerID)
{
	FUNCTION_BEGIN;
	if (m_MonoClassInfo_DO.pOnFindObjectMethod)
	{
		mono_domain_set(m_MonoDomainInfo.pMonoDomain, MONO_FALSE);
		MonoObject* pObject = mono_gchandle_get_target(m_hCSObject);
		if (pObject)
		{
			MonoObject* pException = NULL;
			LPVOID Params[1];
			Params[0] = &CallerID;
			MonoObject* pReturnValue = mono_runtime_invoke(m_MonoClassInfo_DO.pOnFindObjectMethod, pObject, Params, &pException);
			if (pException)
			{
				CCSVMHelper::GetInstance()->ProcessMonoException(pException);
				CallCSOnException(pException);
			}
			else if (pReturnValue)
			{
				void* pValue = mono_object_unbox(pReturnValue);
				if (pValue)
				{
					if (*((bool*)pValue))
						return true;
					else
						return false;
				}
				else
				{
					Log("无法获取返回值");
				}
			}
			else
			{
				Log("没有返回值");
			}

		}
		else
		{
			LogMono("无法获得DistributedObject对象");
		}

	}
	FUNCTION_END;
	return false;
}
void CDistributedObjectOperatorCS::CallCSOnObjectReport(OBJECT_ID ObjectID, const void* pObjectInfoData, UINT DataSize)
{
	FUNCTION_BEGIN;
	if (m_MonoClassInfo_DO.pOnObjectReportMethod)
	{
		mono_domain_set(m_MonoDomainInfo.pMonoDomain, MONO_FALSE);
		MonoObject* pObject = mono_gchandle_get_target(m_hCSObject);
		if (pObject)
		{
			MonoObject* pException = NULL;
			MonoArray* pParam1 = CCSVMHelper::GetInstance()->MonoCreateByteArray(m_MonoDomainInfo, pObjectInfoData, DataSize);
			if (pParam1)
			{
				LPVOID Params[2];
				Params[0] = &ObjectID;
				Params[1] = pParam1;
				MonoObject* pReturnValue = mono_runtime_invoke(m_MonoClassInfo_DO.pOnObjectReportMethod, pObject, Params, &pException);
				if (pException)
				{
					CCSVMHelper::GetInstance()->ProcessMonoException(pException);
					CallCSOnException(pException);
				}
			}
			else
			{
				Log("无法创建对象");
			}
		}
		else
		{
			LogMono("无法获得DistributedObject对象");
		}

	}
	FUNCTION_END;
}
void CDistributedObjectOperatorCS::CallCSOnProxyObjectIPReport(OBJECT_ID ProxyObjectID, UINT Port, LPCSTR szIPString)
{
	FUNCTION_BEGIN;
	if (m_MonoClassInfo_DO.pOnProxyObjectIPReportMethod)
	{
		mono_domain_set(m_MonoDomainInfo.pMonoDomain, MONO_FALSE);
		MonoObject* pObject = mono_gchandle_get_target(m_hCSObject);
		if (pObject)
		{
			MonoObject* pException = NULL;
			MonoString* pParam1 = CCSVMHelper::GetInstance()->MonoCreateString(m_MonoDomainInfo, szIPString, 0);
			if (pParam1)
			{
				LPVOID Params[4];
				Params[0] = &ProxyObjectID;
				Params[1] = &Port;
				Params[2] = pParam1;
				MonoObject* pReturnValue = mono_runtime_invoke(m_MonoClassInfo_DO.pOnProxyObjectIPReportMethod, pObject, Params, &pException);
				if (pException)
				{
					CCSVMHelper::GetInstance()->ProcessMonoException(pException);
					CallCSOnException(pException);
				}
			}
			else
			{
				Log("无法创建对象");
			}
		}
		else
		{
			LogMono("无法获得DistributedObject对象");
		}

	}
	FUNCTION_END;
}
void CDistributedObjectOperatorCS::CallCSOnShutDown(BYTE Level, UINT Param)
{
	FUNCTION_BEGIN;
	if (m_MonoClassInfo_DO.pOnShutDownMethod)
	{
		mono_domain_set(m_MonoDomainInfo.pMonoDomain, MONO_FALSE);
		MonoObject* pObject = mono_gchandle_get_target(m_hCSObject);
		if (pObject)
		{
			MonoObject* pException = NULL;
			LPVOID Params[2];
			Params[0] = &Level;
			Params[1] = &Param;
			MonoObject* pReturnValue = mono_runtime_invoke(m_MonoClassInfo_DO.pOnShutDownMethod, pObject, Params, &pException);
			if (pException)
			{
				CCSVMHelper::GetInstance()->ProcessMonoException(pException);
				CallCSOnException(pException);
			}
		}
		else
		{
			LogMono("无法获得DistributedObject对象");
		}

	}
	FUNCTION_END;
}
int CDistributedObjectOperatorCS::CallCSUpdate(int ProcessPacketLimit)
{
	FUNCTION_BEGIN;
	if (m_MonoClassInfo_DO.pUpdateMethod)
	{
		mono_domain_set(m_MonoDomainInfo.pMonoDomain, MONO_FALSE);
		MonoObject* pObject = mono_gchandle_get_target(m_hCSObject);
		if (pObject)
		{
			MonoObject* pException = NULL;
			LPVOID Params[1];
			Params[0] = &ProcessPacketLimit;
			MonoObject* pReturnValue = mono_runtime_invoke(m_MonoClassInfo_DO.pUpdateMethod, pObject, Params, &pException);
			if (pException)
			{
				CCSVMHelper::GetInstance()->ProcessMonoException(pException);
				CallCSOnException(pException);
			}
			else if (pReturnValue)
			{
				if (mono_object_get_size(pReturnValue) >= sizeof(int))
				{
					void* pValue = mono_object_unbox(pReturnValue);
					if (pValue)
					{
						return *((int*)pValue);
					}
					else
					{
						Log("无法获取返回值");
					}
				}
				else
				{
					Log("返回值类型错误");
					return 1;
				}
			}
			else
			{
				Log("没有返回值");
			}
		}
		else
		{
			LogMono("无法获得DistributedObject对象");
		}


	}
	FUNCTION_END;
	return 0;
}

void CDistributedObjectOperatorCS::CallCSOnException(MonoObject* pPostException)
{
	FUNCTION_BEGIN;
	if (m_MonoClassInfo_DO.pOnExceptionMethod)
	{
		mono_domain_set(m_MonoDomainInfo.pMonoDomain, MONO_FALSE);
		MonoObject* pObject = mono_gchandle_get_target(m_hCSObject);
		if (pObject)
		{
			MonoObject* pException = NULL;
			LPVOID Params[1];
			Params[0] = pPostException;
			MonoObject* pReturnValue = mono_runtime_invoke(m_MonoClassInfo_DO.pOnExceptionMethod, pObject, Params, &pException);
			if (pException)
			{
				CCSVMHelper::GetInstance()->ProcessMonoException(pException);
			}
		}
		else
		{
			LogMono("无法获得DistributedObject对象");
		}


	}
	FUNCTION_END;
}

bool CDistributedObjectOperatorCS::CallOnConsoleCommand(LPCTSTR szCommand)
{
	if (m_MonoClassInfo_DO.pOnConsoleCommandMethod)
	{
		mono_domain_set(m_MonoDomainInfo.pMonoDomain, MONO_FALSE);
		MonoObject* pObject = mono_gchandle_get_target(m_hCSObject);
		if (pObject)
		{
			MonoString* pParam1 = CCSVMHelper::GetInstance()->MonoCreateString(m_MonoDomainInfo, szCommand, 0);
			if (pParam1)
			{
				MonoObject* pException = NULL;
				LPVOID Params[1];
				Params[0] = pParam1;
				MonoObject* pReturnValue = mono_runtime_invoke(m_MonoClassInfo_DO.pOnConsoleCommandMethod, pObject, Params, &pException);
				if (pException)
				{
					CCSVMHelper::GetInstance()->ProcessMonoException(pException);
					CallCSOnException(pException);
				}
				else if (pReturnValue)
				{
					void* pValue = mono_object_unbox(pReturnValue);
					if (pValue)
					{
						if (*((bool*)pValue))
							return true;
						else
							return false;
					}
					else
					{
						Log("无法获取返回值");
					}
				}
				else
				{
					Log("没有返回值");
				}
			}
			else
			{
				Log("无法创建对象");
			}
		}
		else
		{
			LogMono("无法获得DistributedObject对象");
		}
	}
	return false;
}

void CDistributedObjectOperatorCS::CallCSOnTimer(UINT ID, UINT64 Param, bool IsRepeat)
{
	FUNCTION_BEGIN;
	if (m_MonoClassInfo_DO.pOnTimerMethod)
	{
		mono_domain_set(m_MonoDomainInfo.pMonoDomain, MONO_FALSE);
		MonoObject* pObject = mono_gchandle_get_target(m_hCSObject);
		if (pObject)
		{
			MonoObject* pException = NULL;
			LPVOID Params[3];
			Params[0] = &ID;
			if (Param)
				Params[1] = mono_gchandle_get_target((UINT)Param);
			else
				Params[1] = NULL;
			Params[2] = &IsRepeat;
			MonoObject* pReturnValue = mono_runtime_invoke(m_MonoClassInfo_DO.pOnTimerMethod, pObject, Params, &pException);
			if (pException)
			{
				CCSVMHelper::GetInstance()->ProcessMonoException(pException);
				CallCSOnException(pException);
			}
		}
		else
		{
			LogMono("无法获得DistributedObject对象");
		}

	}
	FUNCTION_END;
}

void CDistributedObjectOperatorCS::CallCSOnTimerRelease(UINT ID, UINT64 Param)
{
	FUNCTION_BEGIN;
	if (m_MonoClassInfo_DO.pOnTimerReleaseMethod)
	{
		mono_domain_set(m_MonoDomainInfo.pMonoDomain, MONO_FALSE);
		MonoObject* pObject = mono_gchandle_get_target(m_hCSObject);
		if (pObject)
		{
			MonoObject* pException = NULL;
			LPVOID Params[2];
			Params[0] = &ID;
			if (Param)
				Params[1] = mono_gchandle_get_target((UINT)Param);
			else
				Params[1] = NULL;
			MonoObject* pReturnValue = mono_runtime_invoke(m_MonoClassInfo_DO.pOnTimerReleaseMethod, pObject, Params, &pException);
			mono_gchandle_free((UINT)Param);
			if (pException)
			{
				CCSVMHelper::GetInstance()->ProcessMonoException(pException);
				CallCSOnException(pException);
			}
		}
		else
		{
			LogMono("无法获得DistributedObject对象");
		}

	}
	FUNCTION_END;
}

UINT CDistributedObjectOperatorCS::InternalCallGetRouterID()
{
	return CDOSMainThread::GetInstance()->GetRouter()->GetRouterID();
}
UINT64 CDistributedObjectOperatorCS::InternalCallGetObjectID(CDistributedObjectOperatorCS* pOperator)
{
	if (pOperator)
		return pOperator->GetObjectID().ID;
	return 0;
}
int CDistributedObjectOperatorCS::InternalCallGetGroupIndex(CDistributedObjectOperatorCS* pOperator)
{
	if (pOperator)
		return pOperator->GetGroupIndex();
	return false;
}
bool CDistributedObjectOperatorCS::InternalCallSendMessage(CDistributedObjectOperatorCS* pOperator, OBJECT_ID ReceiverID, OBJECT_ID SenderID, UINT MsgID, WORD MsgFlag, MonoArray* Data, int StartIndex, int DataLen)
{
	if (pOperator)
	{
		size_t ArrayLen = 0;
		BYTE* pBuff = CCSVMHelper::GetInstance()->MonoGetByteArray(Data, ArrayLen);
		if ((size_t)StartIndex < ArrayLen)
		{
			if ((size_t)StartIndex + (size_t)DataLen > ArrayLen)
				DataLen = (int)(ArrayLen - (size_t)StartIndex);
			return pOperator->SendMessage(ReceiverID, SenderID, MsgID, MsgFlag, pBuff + StartIndex, DataLen);
		}
	}
	return false;
}
bool CDistributedObjectOperatorCS::InternalCallSendMessageMulti(CDistributedObjectOperatorCS* pOperator, MonoArray* ReceiverIDList, bool IsSorted, OBJECT_ID SenderID, UINT MsgID, WORD MsgFlag, MonoArray* Data, int StartIndex, int DataLen)
{
	if (pOperator)
	{
		CEasyArray<OBJECT_ID> ObjectIDList(_T("CDistributedObjectOperatorCS"));
		if (CCSVMHelper::GetInstance()->MonoGetObjectIDList(pOperator->m_MonoDomainInfo, ReceiverIDList, ObjectIDList))
		{
			size_t ArrayLen = 0;
			BYTE* pBuff = CCSVMHelper::GetInstance()->MonoGetByteArray(Data, ArrayLen);
			if ((size_t)StartIndex < ArrayLen)
			{
				if ((size_t)StartIndex + (size_t)DataLen > ArrayLen)
					DataLen = (int)(ArrayLen - (size_t)StartIndex);
				return pOperator->SendMessageMulti(ObjectIDList.GetBuffer(), (UINT)ObjectIDList.GetCount(), IsSorted, SenderID, MsgID, MsgFlag, pBuff + StartIndex, DataLen);
			}
		}
	}
	return false;
}
bool CDistributedObjectOperatorCS::InternalCallBroadcastMessageToProxyByMask(CDistributedObjectOperatorCS* pOperator, WORD RouterID, BYTE ProxyType, UINT64 Mask, OBJECT_ID SenderID, UINT MsgID, WORD MsgFlag, MonoArray* Data, int StartIndex, int DataLen)
{
	if (pOperator)
	{
		size_t ArrayLen = 0;
		BYTE* pBuff = CCSVMHelper::GetInstance()->MonoGetByteArray(Data, ArrayLen);
		if ((size_t)StartIndex < ArrayLen)
		{
			if ((size_t)StartIndex + (size_t)DataLen > ArrayLen)
				DataLen = (int)(ArrayLen - (size_t)StartIndex);
			return pOperator->BroadcastMessageToProxyByMask(RouterID, ProxyType, Mask, SenderID, MsgID, MsgFlag, pBuff + StartIndex, DataLen);
		}
	}
	return false;
}
bool CDistributedObjectOperatorCS::InternalCallBroadcastMessageToProxyByGroup(CDistributedObjectOperatorCS* pOperator, WORD RouterID, BYTE ProxyType, UINT64 GroupID, OBJECT_ID SenderID, UINT MsgID, WORD MsgFlag, MonoArray* Data, int StartIndex, int DataLen)
{
	if (pOperator)
	{
		size_t ArrayLen = 0;
		BYTE* pBuff = CCSVMHelper::GetInstance()->MonoGetByteArray(Data, ArrayLen);
		if ((size_t)StartIndex < ArrayLen)
		{
			if ((size_t)StartIndex + (size_t)DataLen > ArrayLen)
				DataLen = (int)(ArrayLen - (size_t)StartIndex);
			return pOperator->BroadcastMessageToProxyByGroup(RouterID, ProxyType, GroupID, SenderID, MsgID, MsgFlag, pBuff + StartIndex, DataLen);
		}
	}
	return false;
}
bool CDistributedObjectOperatorCS::InternalCallRegisterMsgMap(CDistributedObjectOperatorCS* pOperator, OBJECT_ID ProxyObjectID, MonoArray* MsgIDList)
{
	if (pOperator)
	{
		size_t ArrayLen = 0;
		MSG_ID_TYPE* pBuff = CCSVMHelper::GetInstance()->MonoGetMsgIDArray(MsgIDList, ArrayLen);
		if (pBuff)
		{
			return pOperator->RegisterMsgMap(ProxyObjectID, pBuff, (int)ArrayLen);
		}
	}
	return false;
}
bool CDistributedObjectOperatorCS::InternalCallUnregisterMsgMap(CDistributedObjectOperatorCS* pOperator, OBJECT_ID ProxyObjectID, MonoArray* MsgIDList)
{
	if (pOperator)
	{
		size_t ArrayLen = 0;
		MSG_ID_TYPE* pBuff = CCSVMHelper::GetInstance()->MonoGetMsgIDArray(MsgIDList, ArrayLen);
		if (pBuff)
		{
			return pOperator->UnregisterMsgMap(ProxyObjectID, pBuff, (int)ArrayLen);
		}
	}
	return false;
}
bool CDistributedObjectOperatorCS::InternalCallRegisterGlobalMsgMap(CDistributedObjectOperatorCS* pOperator, WORD ProxyRouterID, BYTE ProxyType, UINT MsgID, int MapType)
{
	if (pOperator)
	{
		return pOperator->RegisterGlobalMsgMap(ProxyRouterID, ProxyType, MsgID, MapType);
	}
	return false;
}
bool CDistributedObjectOperatorCS::InternalCallUnregisterGlobalMsgMap(CDistributedObjectOperatorCS* pOperator, WORD ProxyRouterID, BYTE ProxyType, UINT MsgID)
{
	if (pOperator)
	{
		return pOperator->UnregisterGlobalMsgMap(ProxyRouterID, ProxyType, MsgID);
	}
	return false;
}
bool CDistributedObjectOperatorCS::InternalCallSetUnhanleMsgReceiver(CDistributedObjectOperatorCS* pOperator, WORD ProxyRouterID, BYTE ProxyType)
{
	if (pOperator)
	{
		return pOperator->SetUnhanleMsgReceiver(ProxyRouterID, ProxyType);
	}
	return false;
}
bool CDistributedObjectOperatorCS::InternalCallAddConcernedObject(CDistributedObjectOperatorCS* pOperator, OBJECT_ID ObjectID, bool NeedTest)
{
	return pOperator->AddConcernedObject(ObjectID, NeedTest);
}
bool CDistributedObjectOperatorCS::InternalCallDeleteConcernedObject(CDistributedObjectOperatorCS* pOperator, OBJECT_ID ObjectID)
{
	if (pOperator)
	{
		return pOperator->DeleteConcernedObject(ObjectID);
	}
	return false;
}
bool CDistributedObjectOperatorCS::InternalCallFindObject(CDistributedObjectOperatorCS* pOperator, UINT ObjectType, bool OnlyLocal)
{
	if (pOperator)
		return pOperator->FindObject(ObjectType, OnlyLocal);
	return false;
}
bool CDistributedObjectOperatorCS::InternalCallReportObject(CDistributedObjectOperatorCS* pOperator, OBJECT_ID TargetID, MonoArray* Data, int StartIndex, int DataLen)
{
	if (pOperator)
	{
		size_t ArrayLen = 0;
		BYTE* pBuff = CCSVMHelper::GetInstance()->MonoGetByteArray(Data, ArrayLen);
		if ((size_t)StartIndex < ArrayLen)
		{
			if ((size_t)StartIndex + (size_t)DataLen > ArrayLen)
				DataLen = (int)(ArrayLen - (size_t)StartIndex);
			return pOperator->ReportObject(TargetID, pBuff, DataLen);
		}
	}
	return false;
}
bool CDistributedObjectOperatorCS::InternalCallCloseProxyObject(CDistributedObjectOperatorCS* pOperator, OBJECT_ID ProxyObjectID, UINT Delay)
{
	if (pOperator)
	{
		return pOperator->CloseProxyObject(ProxyObjectID, Delay);
	}
	return false;
}
bool CDistributedObjectOperatorCS::InternalCallRequestProxyObjectIP(CDistributedObjectOperatorCS* pOperator, OBJECT_ID ProxyObjectID)
{
	if (pOperator)
	{
		return pOperator->RequestProxyObjectIP(ProxyObjectID);
	}
	return false;
}
bool CDistributedObjectOperatorCS::InternalCallRegisterObjectStatic(UINT PluginID, MonoObject* ObjectRegisterInfo)
{
	MONO_DOMAIN_INFO* pMonoDomainInfo = CDOSMainThread::GetInstance()->GetPluginDomain(PluginID);
	if (pMonoDomainInfo)
	{
		DOS_OBJECT_REGISTER_INFO_FOR_CS RegisterInfo;
		if (CCSVMHelper::GetInstance()->MonoGetDORI(*pMonoDomainInfo, ObjectRegisterInfo, RegisterInfo))
		{
			return CDOSMainThread::GetInstance()->GetDistributedObjectManager()->RegisterObject(RegisterInfo, *pMonoDomainInfo);
		}
	}
	return false;
}
bool CDistributedObjectOperatorCS::InternalCallRegisterObject(CDistributedObjectOperatorCS* pOperator, MonoObject* ObjectRegisterInfo)
{
	if (pOperator)
	{
		DOS_OBJECT_REGISTER_INFO_FOR_CS RegisterInfo;
		if (CCSVMHelper::GetInstance()->MonoGetDORI(pOperator->m_MonoDomainInfo, ObjectRegisterInfo, RegisterInfo))
		{
			return CDOSMainThread::GetInstance()->GetDistributedObjectManager()->RegisterObject(RegisterInfo, pOperator->m_MonoDomainInfo);
		}
	}
	return false;
}
void CDistributedObjectOperatorCS::InternalCallRelease(CDistributedObjectOperatorCS* pOperator)
{
	if (pOperator)
		pOperator->Release();
}
bool CDistributedObjectOperatorCS::InternalCallQueryShutDown(CDistributedObjectOperatorCS* pOperator, OBJECT_ID TargetID, BYTE Level, UINT Param)
{
	if (pOperator)
	{
		return pOperator->QueryShutDown(TargetID, Level, Param);
	}
	return false;
}
void CDistributedObjectOperatorCS::InternalCallShutDown(CDistributedObjectOperatorCS* pOperator, UINT PluginID)
{
	if (pOperator)
		pOperator->ShutDown(PluginID);
}
bool CDistributedObjectOperatorCS::InternalCallRegisterLogger(UINT LogChannel, MonoString* FileName)
{
	char* szFileName = mono_string_to_utf8(FileName);
	if (szFileName)
	{
		CEasyString LogFileName;
		CEasyString ModulePath = CFileTools::GetModulePath(NULL);
		LogFileName.Format("%s/Log/%s", (LPCTSTR)ModulePath, szFileName);

		CServerLogPrinter* pLog = MONITORED_NEW(_T("CDistributedObjectOperatorCS"), CServerLogPrinter, CDOSMainThread::GetInstance(), CServerLogPrinter::LOM_CONSOLE | CServerLogPrinter::LOM_FILE,
			CSystemConfig::GetInstance()->GetLogLevel(), LogFileName, CSystemConfig::GetInstance()->GetLogCacheSize());
		CLogManager::GetInstance()->AddChannel(LogChannel, pLog);
		SAFE_RELEASE(pLog);
		mono_free(szFileName);
		return true;
	}
	return false;
}
bool CDistributedObjectOperatorCS::InternalCallRegisterCSVLogger(UINT LogChannel, MonoString* FileName, MonoString* CSVLogHeader)
{
	char* szFileName = mono_string_to_utf8(FileName);
	if (szFileName)
	{
		bool Ret = false;
		char* szCSVLogHeader = mono_string_to_utf8(CSVLogHeader);
		if (szCSVLogHeader)
		{
			CEasyString LogFileName;
			CEasyString ModulePath = CFileTools::GetModulePath(NULL);
			LogFileName.Format("%s/Log/%s", (LPCTSTR)ModulePath, szFileName);

			CCSVFileLogPrinter* pLog = MONITORED_NEW(_T("CDistributedObjectOperatorCS"), CCSVFileLogPrinter, CSystemConfig::GetInstance()->GetLogLevel(), LogFileName, szCSVLogHeader, CSystemConfig::GetInstance()->GetLogCacheSize());
			CLogManager::GetInstance()->AddChannel(LogChannel, pLog);
			SAFE_RELEASE(pLog);
			Ret = true;
			mono_free(szCSVLogHeader);
		}
		mono_free(szFileName);
		return Ret;
	}
	return false;
}

bool CDistributedObjectOperatorCS::InternalCallRegisterCommandReceiver(CDistributedObjectOperatorCS* pOperator)
{
	if (pOperator)
	{
		return pOperator->RegisterCommandReceiver();
	}
	return false;
}
bool CDistributedObjectOperatorCS::InternalCallUnregisterCommandReceiver(CDistributedObjectOperatorCS* pOperator)
{
	if (pOperator)
	{
		return pOperator->UnregisterCommandReceiver();
	}
	return false;
}
void CDistributedObjectOperatorCS::InternalCallSetServerWorkStatus(CDistributedObjectOperatorCS* pOperator, BYTE WorkStatus)
{
	if (pOperator)
	{
		CDOSMainThread::GetInstance()->SetServerStatus(SC_SST_SS_WORK_STATUS, CSmartValue(WorkStatus));
	}
}
UINT CDistributedObjectOperatorCS::InternalCallAddTimer(CDistributedObjectOperatorCS* pOperator, UINT64 TimeOut, MonoObject* pParam, bool IsRepeat)
{
	if (pOperator)
	{
		UINT Param = 0;
		if (pParam)
			Param = mono_gchandle_new(pParam, false);
		UINT TimerID = pOperator->CDOSBaseObject::AddTimer(TimeOut, Param, IsRepeat);
		if (!TimerID)
			mono_gchandle_free(Param);
		return TimerID;
	}
	return 0;
}
bool CDistributedObjectOperatorCS::InternalCallDeleteTimer(CDistributedObjectOperatorCS* pOperator, UINT ID)
{
	if (pOperator)
	{
		return pOperator->CDOSBaseObject::DeleteTimer(ID);
	}
	return false;
}

bool CDistributedObjectOperatorCS::InternalCallSetBroadcastMask(CDistributedObjectOperatorCS* pOperator, OBJECT_ID ProxyObjectID, UINT64 Mask)
{
	if (pOperator)
	{
		return pOperator->CDOSBaseObject::SetBroadcastMask(ProxyObjectID, Mask);
	}
	return false;
}
bool CDistributedObjectOperatorCS::InternalCallAddBroadcastMask(CDistributedObjectOperatorCS* pOperator, OBJECT_ID ProxyObjectID, UINT64 Mask)
{
	if (pOperator)
	{
		return pOperator->CDOSBaseObject::AddBroadcastMask(ProxyObjectID, Mask);
	}
	return false;
}
bool CDistributedObjectOperatorCS::InternalCallRemoveBroadcastMask(CDistributedObjectOperatorCS* pOperator, OBJECT_ID ProxyObjectID, UINT64 Mask)
{
	if (pOperator)
	{
		return pOperator->CDOSBaseObject::RemoveBroadcastMask(ProxyObjectID, Mask);
	}
	return false;
}
bool CDistributedObjectOperatorCS::InternalCallAddBroadcastGroup(CDistributedObjectOperatorCS* pOperator, OBJECT_ID ProxyObjectID, UINT64 GroupID)
{
	if (pOperator)
	{
		return pOperator->CDOSBaseObject::AddBroadcastGroup(ProxyObjectID, GroupID);
	}
	return false;
}
bool CDistributedObjectOperatorCS::InternalCallRemoveBroadcastGroup(CDistributedObjectOperatorCS* pOperator, OBJECT_ID ProxyObjectID, UINT64 GroupID)
{
	if (pOperator)
	{
		return pOperator->CDOSBaseObject::RemoveBroadcastGroup(ProxyObjectID, GroupID);
	}
	return false;
}