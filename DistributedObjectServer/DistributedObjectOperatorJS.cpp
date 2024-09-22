#include "stdafx.h"







CDistributedObjectOperatorJS::CDistributedObjectOperatorJS()
{
	FUNCTION_BEGIN;
	m_pManager = NULL;
	m_pVMInfo = NULL;
	m_JSOperatorObject = NULL;
	m_JSObject = NULL;
	ZeroMemory(m_JSObjectFuncs, sizeof(m_JSObjectFuncs));
	m_JSObjectID = NULL;
	m_IsCommandReceiver = false;
	FUNCTION_END;
}

CDistributedObjectOperatorJS::~CDistributedObjectOperatorJS()
{
}

bool CDistributedObjectOperatorJS::Init(CDistributedObjectManager* pManager, UINT PoolID, JS_DOS_OBJECT_INIT_INFO& InitInfo)
{
	FUNCTION_BEGIN;
	if (pManager == NULL || (InitInfo.DOSObject == NULL && InitInfo.DOSObjectClassName.IsEmpty()))
		return false;
	m_pManager = pManager;
	SetID(PoolID);
	m_InitInfo = InitInfo;
	return true;
	FUNCTION_END;
	return false;
}

bool CDistributedObjectOperatorJS::Initialize()
{
	if (CDOSBaseObject::Initialize())
	{
		JSVM_VM_INFO* pVMInfo = CJSVMHelper::GetInstance()->GetCurJSVM(m_InitInfo.PluginID);
		if (pVMInfo == NULL)
		{
			pVMInfo = CJSVMHelper::GetInstance()->CreateJSVM(m_InitInfo.PluginID, m_InitInfo.LogChannel, GetGroupIndex(), m_InitInfo.MainMoudulePath, m_InitInfo.ConfigDir, m_InitInfo.LogDir);
			if (pVMInfo)
			{
				pVMInfo->GroupIndex = GetObjectID().GroupIndex;
			}
		}
		if (pVMInfo)
		{
			return DoInit(pVMInfo);
		}
		else
		{
			LogJSVM("创建JSVM(%s)实例失败", (LPCTSTR)m_InitInfo.MainMoudulePath);
			return false;
		}
	}
	else
	{
		LogJSVM("执行CDOSBaseObject::Initialize失败");
		return false;
	}
}

bool CDistributedObjectOperatorJS::DoInit(JSVM_VM_INFO* pVMInfo)
{
	m_pVMInfo = pVMInfo;
	m_pVMInfo->ObjectCount++;
	JSVM::jsvm_handle_scope* Scope = JSVM::jsvm_create_handle_scope(m_pVMInfo->pVM);
	JSVM::jsvm_context* Context = JSVM::jsvm_get_context(Scope);
	JSVM::CAutoDisposer<JSVM::jsvm_handle_scope, JSVM::jsvm_release_handle_scope> Disposer(Scope);


	if (m_InitInfo.DOSObject)
	{
		m_JSObject = m_InitInfo.DOSObject;
	}
	else
	{
		JSVM::jsvm_value* Class = m_pVMInfo->GetClass(Context, m_InitInfo.DOSObjectClassName);
		if (Class)
		{
			JSVM::jsvm_value* obj = JSVM::jsvm_new_obj(Context, Class);
			if (obj && JSVM::jsvm_is_object(obj))
			{
				m_JSObject = JSVM::jsvm_create_persistent_value(Context, obj);
				if (m_JSObject == NULL)
				{
					LogJSVM("无法持久化DOS对象");
					return false;
				}
			}
			else
			{
				LogJSVM("无法创建对象:%s", (LPCTSTR)m_InitInfo.DOSObjectClassName);
				return false;
			}
		}
		else
		{
			LogJSVM("无法获得类:%s", (LPCTSTR)m_InitInfo.DOSObjectClassName);
			return false;
		}
	}

	if (m_pVMInfo->PersistentValues[JSVMPersistentValues_IDistributedObjectOperator] == NULL)
	{
		LogJSVM("JSOperatorClass未初始化");
		return false;
	}
	JSVM::jsvm_value* OperatorClass = JSVM::jsvm_get_value_from_persistent_value(Context, m_pVMInfo->PersistentValues[JSVMPersistentValues_IDistributedObjectOperator]);
	if (OperatorClass == NULL)
	{
		LogJSVM("获取JSOperatorClass失败");
		return false;
	}
	JSVM::jsvm_value* JSObject = JSVM::jsvm_get_value_from_persistent_value(Context, m_JSObject);
	if (JSObject == NULL)
	{
		LogJSVM("获取JSObject失败");
		return false;
	}
	JSVM::jsvm_value* JSOperator = JSVM::jsvm_new_obj(Context, OperatorClass);
	if (JSOperator == NULL)
	{
		LogJSVM("创建JSOperator失败");
		return false;
	}
	JSVM::jsvm_obj_set_internal_field(Context, JSOperator, (int)JSVM::JSVMInternalFieldIndex::This, this);
	for (int i = 0; i < DOSObjJSFuncs::JSFuncMax; i++)
	{
		if (m_JSObjectFuncs[i])
		{
			JSVM::jsvm_release_persistent_value(m_JSObjectFuncs[i]);
			m_JSObjectFuncs[i] = NULL;
		}
		JSVM::jsvm_value* Func = JSVM::jsvm_obj_get(Context, JSObject, DOSObjJSFuncNames[i]);
		if (Func && JSVM::jsvm_is_function(Func))
		{
			m_JSObjectFuncs[i] = JSVM::jsvm_create_persistent_value(Context, Func);
			if (i == DOSObjJSFuncs::JSFuncInitialize)
			{
				m_JSOperatorObject = JSVM::jsvm_create_persistent_value(Context, JSOperator);
				bool Ret = JSVM::JSCallThis<bool>(Context, JSObject, Func, JSOperator);
				JSVM::jsvm_process_exception(Scope);
				if (!Ret)
				{
					LogJSVM("DOS对象初始化失败");
					return false;
				}
			}
		}
		else
		{
			LogJSVM("DOS对象缺少函数%s", DOSObjJSFuncNames[i]);
			return false;
		}
	}
	return true;
}

void CDistributedObjectOperatorJS::Destory()
{
	if (m_IsCommandReceiver)
	{
		UnregisterCommandReceiver();
	}
	if (m_pVMInfo)
	{
		JSVM::jsvm_handle_scope* Scope = JSVM::jsvm_create_handle_scope(m_pVMInfo->pVM);
		JSVM::jsvm_context* Context = JSVM::jsvm_get_context(Scope);

		JSCallThis<DOSObjJSFuncs::JSFuncDestory>(Context);
		JSVM::jsvm_process_exception(Scope);
		for (JSVM::jsvm_persistent_value* func : m_JSObjectFuncs)
		{
			if (func)
				JSVM::jsvm_release_persistent_value(func);
		}
		ZeroMemory(m_JSObjectFuncs, sizeof(m_JSObjectFuncs));
		if (m_JSObject)
		{
			JSVM::jsvm_release_persistent_value(m_JSObject);
			m_JSObject = NULL;
		}
		if (m_JSObjectID)
		{
			JSVM::jsvm_release_persistent_value(m_JSObjectID);
			m_JSObjectID = NULL;
		}
		if (m_JSOperatorObject)
		{
			JSVM::jsvm_value* JSOperator = JSVM::jsvm_get_value_from_persistent_value(Context, m_JSOperatorObject);
			if (JSOperator)
			{
				//将Operator对应的c++对象this指针设为NULL,以免意外调用时崩溃
				JSVM::jsvm_obj_set_internal_field(Context, JSOperator, (int)JSVM::JSVMInternalFieldIndex::This, NULL);
			}
			JSVM::jsvm_release_persistent_value(m_JSOperatorObject);
			m_JSOperatorObject = NULL;
		}
		JSVM::jsvm_release_handle_scope(Scope);
		m_pVMInfo->ObjectCount--;
	}
	m_pVMInfo = NULL;
	if (m_pManager)
		m_pManager->DeleteObjectOperator(this);

	SetID(0);
	m_pManager = NULL;
	CDOSBaseObject::Destory();
}

void CDistributedObjectOperatorJS::Release()
{
	if (m_pManager)
		m_pManager->UnregisterObject(this);
}

bool CDistributedObjectOperatorJS::OnConsoleCommand(LPCTSTR szCommand)
{
	if (m_pVMInfo)
	{
		JSVM::jsvm_handle_scope* Scope = JSVM::jsvm_create_handle_scope(m_pVMInfo->pVM);
		JSVM::jsvm_context* Context = JSVM::jsvm_get_context(Scope);

		bool Ret = JSCallThis<bool, DOSObjJSFuncs::JSFuncOnConsoleCommand>(Context, szCommand);
		JSVM::jsvm_process_exception(Scope);
		JSVM::jsvm_release_handle_scope(Scope);
		return Ret;
	}
	return false;
}

UINT64 CDistributedObjectOperatorJS::GetReceiverID()
{
	return GetObjectID().ID;
}

void CDistributedObjectOperatorJS::ShutDown(UINT PluginID)
{
	CDOSMainThread::GetInstance()->QueryFreePlugin(PluginID);
}


bool CDistributedObjectOperatorJS::RegisterCommandReceiver()
{
	if (CDOSMainThread::GetInstance()->AddConsoleCommandReceiver(this))
	{
		m_IsCommandReceiver = true;
		return true;
	}
	return false;
}

bool CDistributedObjectOperatorJS::UnregisterCommandReceiver()
{
	m_IsCommandReceiver = false;
	if (CDOSMainThread::GetInstance()->DeleteConsoleCommandReceiver(this))
	{
		return true;
	}
	return false;
}

bool CDistributedObjectOperatorJS::OnMessage(CDOSMessage* pMessage)
{
	if (m_pVMInfo)
	{
		JSVM::jsvm_handle_scope* Scope = JSVM::jsvm_create_handle_scope(m_pVMInfo->pVM);
		JSVM::jsvm_context* Context = JSVM::jsvm_get_context(Scope);

		bool Ret = true;
		if (JSCallThis<int, DOSObjJSFuncs::JSFuncOnPreTranslateMessage>(Context, pMessage) == COMMON_RESULT_MSG_NO_HANDLER)
		{
			Ret = JSCallThis<int, DOSObjJSFuncs::JSFuncOnMessage>(Context, pMessage) != COMMON_RESULT_MSG_NO_HANDLER;
		}
		JSVM::jsvm_process_exception(Scope);
		JSVM::jsvm_release_handle_scope(Scope);
		return Ret;
	}
	return false;
}
bool CDistributedObjectOperatorJS::OnSystemMessage(CDOSMessage* pMessage)
{
	if (m_pVMInfo)
	{
		JSVM::jsvm_handle_scope* Scope = JSVM::jsvm_create_handle_scope(m_pVMInfo->pVM);
		JSVM::jsvm_context* Context = JSVM::jsvm_get_context(Scope);

		int Ret = JSCallThis<int, DOSObjJSFuncs::JSFuncOnSystemMessage>(Context, pMessage);
		JSVM::jsvm_process_exception(Scope);
		JSVM::jsvm_release_handle_scope(Scope);
		if (Ret == COMMON_RESULT_MSG_NO_HANDLER)
			return CDOSBaseObject::OnSystemMessage(pMessage);
	}
	else
	{
		return CDOSBaseObject::OnSystemMessage(pMessage);
	}
	return false;
}
void CDistributedObjectOperatorJS::OnConcernedObjectLost(OBJECT_ID ObjectID)
{
	if (m_pVMInfo)
	{
		JSVM::jsvm_handle_scope* Scope = JSVM::jsvm_create_handle_scope(m_pVMInfo->pVM);
		JSVM::jsvm_context* Context = JSVM::jsvm_get_context(Scope);

		JSCallThis<DOSObjJSFuncs::JSFuncOnConcernedObjectLost>(Context, ObjectID);
		JSVM::jsvm_process_exception(Scope);
		JSVM::jsvm_release_handle_scope(Scope);
	}
}
void CDistributedObjectOperatorJS::OnFindObject(OBJECT_ID CallerID)
{
	if (m_pVMInfo)
	{
		JSVM::jsvm_handle_scope* Scope = JSVM::jsvm_create_handle_scope(m_pVMInfo->pVM);
		JSVM::jsvm_context* Context = JSVM::jsvm_get_context(Scope);

		bool Ret = JSCallThis<bool, DOSObjJSFuncs::JSFuncOnFindObject>(Context, CallerID);
		JSVM::jsvm_process_exception(Scope);
		JSVM::jsvm_release_handle_scope(Scope);
		if (!Ret)
			CDOSBaseObject::OnFindObject(CallerID);
	}
	else
	{
		CDOSBaseObject::OnFindObject(CallerID);
	}
}
void CDistributedObjectOperatorJS::OnObjectReport(OBJECT_ID ObjectID, const void* pObjectInfoData, UINT DataSize)
{
	if (m_pVMInfo)
	{
		JSVM::jsvm_handle_scope* Scope = JSVM::jsvm_create_handle_scope(m_pVMInfo->pVM);
		JSVM::jsvm_context* Context = JSVM::jsvm_get_context(Scope);

		JSVM::jsvm_value* Data = JSVM::jsvm_create_binary_copy_data(Context, pObjectInfoData, DataSize);
		JSCallThis<DOSObjJSFuncs::JSFuncOnObjectReport>(Context, ObjectID, Data);
		JSVM::jsvm_process_exception(Scope);
		JSVM::jsvm_release_handle_scope(Scope);
	}
}
void CDistributedObjectOperatorJS::OnProxyObjectIPReport(OBJECT_ID ProxyObjectID, UINT Port, LPCSTR szIPString)
{
	if (m_pVMInfo)
	{
		JSVM::jsvm_handle_scope* Scope = JSVM::jsvm_create_handle_scope(m_pVMInfo->pVM);
		JSVM::jsvm_context* Context = JSVM::jsvm_get_context(Scope);

		JSCallThis<DOSObjJSFuncs::JSFuncOnProxyObjectIPReport>(Context, ProxyObjectID, Port, szIPString);
		JSVM::jsvm_process_exception(Scope);
		JSVM::jsvm_release_handle_scope(Scope);
	}
}
void CDistributedObjectOperatorJS::OnShutDown(BYTE Level, UINT Param)
{
	if (m_pVMInfo)
	{
		JSVM::jsvm_handle_scope* Scope = JSVM::jsvm_create_handle_scope(m_pVMInfo->pVM);
		JSVM::jsvm_context* Context = JSVM::jsvm_get_context(Scope);
		JSVM::jsvm_process_exception(Scope);
		JSCallThis<DOSObjJSFuncs::JSFuncOnShutDown>(Context, Level, Param);

		JSVM::jsvm_release_handle_scope(Scope);
	}
}
int CDistributedObjectOperatorJS::Update(int ProcessPacketLimit)
{
	int ProcessCount = 0;
	if (m_pVMInfo)
	{
		JSVM::jsvm_handle_scope* Scope = JSVM::jsvm_create_handle_scope(m_pVMInfo->pVM);
		JSVM::jsvm_context* Context = JSVM::jsvm_get_context(Scope);

		ProcessCount = JSCallThis<int, DOSObjJSFuncs::JSFuncUpdate>(Context, ProcessPacketLimit);
		JSVM::jsvm_process_exception(Scope);
		JSVM::jsvm_release_handle_scope(Scope);
	}
	return ProcessCount;
}
void CDistributedObjectOperatorJS::OnTimer(UINT ID, UINT64 Param, bool IsRepeat)
{
	if (m_pVMInfo)
	{
		JSVM::jsvm_handle_scope* Scope = JSVM::jsvm_create_handle_scope(m_pVMInfo->pVM);
		JSVM::jsvm_context* Context = JSVM::jsvm_get_context(Scope);

		JSVM::jsvm_value* param = NULL;
		if (Param)
		{
			JSVM::jsvm_persistent_value* ParamP = (JSVM::jsvm_persistent_value*)Param;
			param = JSVM::jsvm_get_value_from_persistent_value(Context, ParamP);
		}
		if (param == NULL)
			param = JSVM::jsvm_create_undefined(Context);

		JSCallThis<DOSObjJSFuncs::JSFuncOnTimer>(Context, ID, param, IsRepeat);
		JSVM::jsvm_process_exception(Scope);
		JSVM::jsvm_release_handle_scope(Scope);
	}
}

void CDistributedObjectOperatorJS::OnTimerRelease(UINT ID, UINT64 Param)
{
	if (m_pVMInfo)
	{
		JSVM::jsvm_handle_scope* Scope = JSVM::jsvm_create_handle_scope(m_pVMInfo->pVM);
		JSVM::jsvm_context* Context = JSVM::jsvm_get_context(Scope);

		JSVM::jsvm_value* param = NULL;
		JSVM::jsvm_persistent_value* ParamP = (JSVM::jsvm_persistent_value*)Param;
		if (ParamP)
			param = JSVM::jsvm_get_value_from_persistent_value(Context, ParamP);
		if (param == NULL)
			param = JSVM::jsvm_create_undefined(Context);

		JSCallThis<DOSObjJSFuncs::JSFuncOnTimerRelease>(Context, ID, param);
		JSVM::jsvm_process_exception(Scope);
		if (ParamP)
			JSVM::jsvm_release_persistent_value(ParamP);

		JSVM::jsvm_release_handle_scope(Scope);
	}
}


JSVM::jsvm_persistent_value* CDistributedObjectOperatorJS::CreateJSClass_IDistributedObjectOperator(JSVM::jsvm_context* Context)
{
	JSVM::jsvm_tmpl* class_tmpl = JSVM::jsvm_create_func_tmpl(Context, CDistributedObjectOperatorJS::JSOperatorConstructor, JSVM::jsvm_func_type::jsvm_func_constructor, NULL, "IDistributedObjectOperator");
	if (class_tmpl)
	{
		JSVM::jsvm_class_set_internal_field_count(class_tmpl, (int)JSVM::JSVMInternalFieldIndex::Max);
		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &CDistributedObjectOperatorJS::GetRouterID, "GetRouterID", false);
		JSVM_REGISTER_JS_CLASS_METHOD_WITH_CONTEXT(Context, class_tmpl, &CDistributedObjectOperatorJS::JSGetObjectID, "GetObjectID", false);
		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &CDistributedObjectOperatorJS::GetGroupIndex, "GetGroupIndex", false);
		JSVM_REGISTER_JS_CLASS_METHOD_WITH_CONTEXT(Context, class_tmpl, &CDistributedObjectOperatorJS::JSSendMessage, "SendMessage", false);
		JSVM_REGISTER_JS_CLASS_METHOD_WITH_CONTEXT(Context, class_tmpl, &CDistributedObjectOperatorJS::JSSendMessageMulti, "SendMessageMulti", false);
		JSVM_REGISTER_JS_CLASS_METHOD_WITH_CONTEXT(Context, class_tmpl, &CDistributedObjectOperatorJS::JSBroadcastMessageToProxyByMask, "BroadcastMessageToProxyByMask", false);
		JSVM_REGISTER_JS_CLASS_METHOD_WITH_CONTEXT(Context, class_tmpl, &CDistributedObjectOperatorJS::JSBroadcastMessageToProxyByGroup, "BroadcastMessageToProxyByGroup", false);

		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &CDistributedObjectOperatorJS::NewMessagePacket, "NewMessagePacket", false);
		JSVM_REGISTER_JS_CLASS_METHOD_WITH_CONTEXT(Context, class_tmpl, &CDistributedObjectOperatorJS::JSReleaseMessagePacket, "ReleaseMessagePacket", false);
		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &CDistributedObjectOperatorJS::SendMessagePacket, "SendMessagePacket", false);

		JSVM_REGISTER_JS_CLASS_METHOD_WITH_CONTEXT(Context, class_tmpl, &CDistributedObjectOperatorJS::JSRegisterMsgMap, "RegisterMsgMap", false);
		JSVM_REGISTER_JS_CLASS_METHOD_WITH_CONTEXT(Context, class_tmpl, &CDistributedObjectOperatorJS::JSUnregisterMsgMap, "UnregisterMsgMap", false);
		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &CDistributedObjectOperatorJS::RegisterGlobalMsgMap, "RegisterGlobalMsgMap", false);
		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &CDistributedObjectOperatorJS::UnregisterGlobalMsgMap, "UnregisterGlobalMsgMap", false);
		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &CDistributedObjectOperatorJS::SetUnhanleMsgReceiver, "SetUnhanleMsgReceiver", false);

		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &CDistributedObjectOperatorJS::AddConcernedObject, "AddConcernedObject", false);
		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &CDistributedObjectOperatorJS::DeleteConcernedObject, "DeleteConcernedObject", false);

		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &CDistributedObjectOperatorJS::FindObject, "FindObject", false);
		JSVM_REGISTER_JS_CLASS_METHOD_WITH_CONTEXT(Context, class_tmpl, &CDistributedObjectOperatorJS::JSReportObject, "ReportObject", false);
		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &CDistributedObjectOperatorJS::CloseProxyObject, "CloseProxyObject", false);
		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &CDistributedObjectOperatorJS::RequestProxyObjectIP, "RequestProxyObjectIP", false);

		JSVM_REGISTER_JS_CLASS_METHOD_WITH_CONTEXT(Context, class_tmpl, &CDistributedObjectOperatorJS::JSRegisterObject, "RegisterObject", false);
		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &CDistributedObjectOperatorJS::Release, "Release", false);

		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &CDistributedObjectOperatorJS::QueryShutDown, "QueryShutDown", false);
		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &CDistributedObjectOperatorJS::ShutDown, "ShutDown", false);
		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &CDistributedObjectOperatorJS::RegisterCommandReceiver, "RegisterCommandReceiver", false);
		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &CDistributedObjectOperatorJS::UnregisterCommandReceiver, "UnregisterCommandReceiver", false);

		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &CDistributedObjectOperatorJS::RegisterLogger, "RegisterLogger", false);
		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &CDistributedObjectOperatorJS::RegisterCSVLogger, "RegisterCSVLogger", false);

		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &CDistributedObjectOperatorJS::SetServerWorkStatus, "SetServerWorkStatus", false);
		JSVM_REGISTER_JS_CLASS_METHOD_WITH_CONTEXT(Context, class_tmpl, &CDistributedObjectOperatorJS::JSAddTimer, "AddTimer", false);
		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &CDistributedObjectOperatorJS::DeleteTimer, "DeleteTimer", false);

		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &CDistributedObjectOperatorJS::SetBroadcastMask, "SetBroadcastMask", false);
		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &CDistributedObjectOperatorJS::AddBroadcastMask, "AddBroadcastMask", false);
		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &CDistributedObjectOperatorJS::RemoveBroadcastMask, "RemoveBroadcastMask", false);
		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &CDistributedObjectOperatorJS::AddBroadcastGroup, "AddBroadcastGroup", false);
		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &CDistributedObjectOperatorJS::RemoveBroadcastGroup, "RemoveBroadcastGroup", false);

		JSVM::jsvm_value* Class = JSVM::jsvm_create_func(Context, class_tmpl);
		if (Class)
			return JSVM::jsvm_create_persistent_value(Context, Class);
	}
	return NULL;
}
JSVM::jsvm_persistent_value* CDistributedObjectOperatorJS::CreateJSClass_IDistributedObjectManager(JSVM::jsvm_context* Context)
{
	JSVM::jsvm_tmpl* class_tmpl = JSVM::jsvm_create_func_tmpl(Context, CDistributedObjectOperatorJS::JSOperatorConstructor, JSVM::jsvm_func_type::jsvm_func_constructor, NULL, "IDistributedObjectManager");
	if (class_tmpl)
	{
		JSVM::jsvm_class_set_internal_field_count(class_tmpl, (int)JSVM::JSVMInternalFieldIndex::Max);
		JSVM_REGISTER_JS_CLASS_METHOD_WITH_CONTEXT(Context, class_tmpl, &CDistributedObjectManager::JSRegisterObject, "RegisterObject", false);

		JSVM::jsvm_value* Class = JSVM::jsvm_create_func(Context, class_tmpl);
		if (Class)
			return JSVM::jsvm_create_persistent_value(Context, Class);
	}
	return NULL;
}
JSVM::jsvm_persistent_value* CDistributedObjectOperatorJS::CreateJSClass_IDOSMessage(JSVM::jsvm_context* Context)
{
	JSVM::jsvm_tmpl* class_tmpl = JSVM::jsvm_create_func_tmpl(Context, CDistributedObjectOperatorJS::JSOperatorConstructor, JSVM::jsvm_func_type::jsvm_func_constructor, NULL, "IDOSMessage");
	if (class_tmpl)
	{
		JSVM::jsvm_class_set_internal_field_count(class_tmpl, (int)JSVM::JSVMInternalFieldIndex::Max);
		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &JSIDOSMessage::Init, "Init", false);
		JSVM_REGISTER_JS_CLASS_PROPERTY(Context, class_tmpl, "MsgID", &JSIDOSMessage::GetMsgID, &JSIDOSMessage::SetMsgID, false);
		JSVM_REGISTER_JS_CLASS_PROPERTY(Context, class_tmpl, "MsgFlag", &JSIDOSMessage::GetMsgFlag, &JSIDOSMessage::SetMsgFlag, false);
		JSVM_REGISTER_JS_CLASS_PROPERTY(Context, class_tmpl, "DataLength", &JSIDOSMessage::GetDataLength, nullptr, false);
		JSVM_REGISTER_JS_CLASS_PROPERTY_WITH_CONTEXT(Context, class_tmpl, "Data", &JSIDOSMessage::get_Data, nullptr, false);
		JSVM_REGISTER_JS_CLASS_PROPERTY_WITH_CONTEXT(Context, class_tmpl, "DataPacket", &JSIDOSMessage::get_DataPacket, nullptr, false);
		JSVM_REGISTER_JS_CLASS_PROPERTY(Context, class_tmpl, "SenderID", &JSIDOSMessage::GetSenderID, &JSIDOSMessage::SetSenderID, false);

		JSVM::jsvm_value* Class = JSVM::jsvm_create_func(Context, class_tmpl);
		if (Class)
			return JSVM::jsvm_create_persistent_value(Context, Class);
	}
	return NULL;
}
JSVM::jsvm_persistent_value* CDistributedObjectOperatorJS::CreateJSClass_IDOSMessagePacket(JSVM::jsvm_context* Context)
{
	JSVM::jsvm_tmpl* class_tmpl = JSVM::jsvm_create_func_tmpl(Context, CDistributedObjectOperatorJS::JSOperatorConstructor, JSVM::jsvm_func_type::jsvm_func_constructor, NULL, "IDOSMessagePacket");
	if (class_tmpl)
	{
		JSVM::jsvm_class_set_internal_field_count(class_tmpl, (int)JSVM::JSVMInternalFieldIndex::Max);
		JSVM_REGISTER_JS_CLASS_METHOD(Context, class_tmpl, &JSIDOSMessagePacket::Init, "Init", false);
		JSVM_REGISTER_JS_CLASS_PROPERTY(Context, class_tmpl, "MsgID", &JSIDOSMessagePacket::get_MsgID, &JSIDOSMessagePacket::set_MsgID, false);
		JSVM_REGISTER_JS_CLASS_PROPERTY(Context, class_tmpl, "MsgFlag", &JSIDOSMessagePacket::get_MsgFlag, &JSIDOSMessagePacket::set_MsgFlag, false);
		JSVM_REGISTER_JS_CLASS_PROPERTY(Context, class_tmpl, "DataLength", &JSIDOSMessagePacket::get_DataLength, &JSIDOSMessagePacket::set_DataLength, false);
		JSVM_REGISTER_JS_CLASS_PROPERTY_WITH_CONTEXT(Context, class_tmpl, "Data", &JSIDOSMessagePacket::get_Data, &JSIDOSMessagePacket::set_Data, false);
		JSVM_REGISTER_JS_CLASS_PROPERTY_WITH_CONTEXT(Context, class_tmpl, "DataPacket", &JSIDOSMessagePacket::get_DataPacket, &JSIDOSMessagePacket::set_DataPacket, false);
		JSVM_REGISTER_JS_CLASS_PROPERTY(Context, class_tmpl, "SenderID", &JSIDOSMessagePacket::get_SenderID, &JSIDOSMessagePacket::set_SenderID, false);
		JSVM_REGISTER_JS_CLASS_PROPERTY_WITH_CONTEXT(Context, class_tmpl, "TargetIDs", &JSIDOSMessagePacket::get_TargetIDs, &JSIDOSMessagePacket::set_TargetIDs, false);
		JSVM_REGISTER_JS_CLASS_PROPERTY_WITH_CONTEXT(Context, class_tmpl, "EmptyDataPacket", &JSIDOSMessagePacket::get_EmptyDataPacket, nullptr, false);

		JSVM::jsvm_value* Class = JSVM::jsvm_create_func(Context, class_tmpl);
		if (Class)
			return JSVM::jsvm_create_persistent_value(Context, Class);
	}
	return NULL;
}

void CDistributedObjectOperatorJS::JSOperatorConstructor(JSVM::jsvm_callback_info* callback_info)
{

}
JSVM::jsvm_value* CDistributedObjectOperatorJS::JSGetObjectID(JSVM::jsvm_context* Context)
{
	if (m_JSObjectID)
	{
		return JSVM::jsvm_get_value_from_persistent_value(Context, m_JSObjectID);
	}
	else
	{
		JSVM::jsvm_value* value = JSVM::TypeConvertor<OBJECT_ID>::ToJS(Context, GetObjectID());
		if (value)
		{
			m_JSObjectID = JSVM::jsvm_create_persistent_value(Context, value);
		}
		return value;
	}
}
bool CDistributedObjectOperatorJS::JSSendMessage(JSVM::jsvm_context* Context, OBJECT_ID ReceiverID, OBJECT_ID SenderID, MSG_ID_TYPE MsgID, WORD MsgFlag, JSVM::jsvm_value* Data)
{
	size_t DataLen = 0;
	void* pData = JSVM::jsvm_get_value_binary(Context, Data, &DataLen);
	return SendMessage(ReceiverID, SenderID, MsgID, MsgFlag, pData, (UINT)DataLen);
}
bool CDistributedObjectOperatorJS::JSSendMessageMulti(JSVM::jsvm_context* Context, CEasyArray<OBJECT_ID> ReceiverIDList, bool IsSorted, OBJECT_ID SenderID, MSG_ID_TYPE MsgID, WORD MsgFlag, JSVM::jsvm_value* Data)
{
	size_t DataLen = 0;
	void* pData = JSVM::jsvm_get_value_binary(Context, Data, &DataLen);
	return SendMessageMulti(ReceiverIDList.GetBuffer(), (UINT)ReceiverIDList.GetCount(), IsSorted, SenderID, MsgID, MsgFlag, pData, (UINT)DataLen);
}
bool CDistributedObjectOperatorJS::JSBroadcastMessageToProxyByMask(JSVM::jsvm_context* Context, WORD RouterID, BYTE ProxyType, UINT64 Mask, OBJECT_ID SenderID, MSG_ID_TYPE MsgID, WORD MsgFlag, JSVM::jsvm_value* Data)
{
	size_t DataLen = 0;
	void* pData = JSVM::jsvm_get_value_binary(Context, Data, &DataLen);
	return BroadcastMessageToProxyByMask(RouterID, ProxyType, Mask, SenderID, MsgID, MsgFlag, pData, (UINT)DataLen);
}
bool CDistributedObjectOperatorJS::JSBroadcastMessageToProxyByGroup(JSVM::jsvm_context* Context, WORD RouterID, BYTE ProxyType, UINT64 GroupID, OBJECT_ID SenderID, MSG_ID_TYPE MsgID, WORD MsgFlag, JSVM::jsvm_value* Data)
{
	size_t DataLen = 0;
	void* pData = JSVM::jsvm_get_value_binary(Context, Data, &DataLen);
	return BroadcastMessageToProxyByGroup(RouterID, ProxyType, GroupID, SenderID, MsgID, MsgFlag, pData, (UINT)DataLen);
}

bool CDistributedObjectOperatorJS::JSReleaseMessagePacket(JSVM::jsvm_context* Context, JSVM::jsvm_value* pPacket)
{
	CDOSMessagePacket* pDOSPacket = JSVM::TypeConvertor<CDOSMessagePacket*>::FromJS(Context, pPacket);
	if (pDOSPacket)
	{
		bool Ret = ReleaseMessagePacket(pDOSPacket);
		JSVM::jsvm_obj_set_internal_field(Context, pPacket, (int)JSVM::JSVMInternalFieldIndex::This, NULL);
		return Ret;
	}
	else
	{
		JSVM::jsvm_throw_exception(Context, "invalid packet");
	}
	return false;
}

bool CDistributedObjectOperatorJS::JSRegisterMsgMap(JSVM::jsvm_context* Context, OBJECT_ID ProxyObjectID, CEasyArray<MSG_ID_TYPE> MsgIDList)
{
	return RegisterMsgMap(ProxyObjectID, MsgIDList.GetBuffer(), (int)MsgIDList.GetCount());
}
bool CDistributedObjectOperatorJS::JSUnregisterMsgMap(JSVM::jsvm_context* Context, OBJECT_ID ProxyObjectID, CEasyArray<MSG_ID_TYPE> MsgIDList)
{
	return UnregisterMsgMap(ProxyObjectID, MsgIDList.GetBuffer(), (int)MsgIDList.GetCount());
}

bool CDistributedObjectOperatorJS::JSReportObject(JSVM::jsvm_context* Context, OBJECT_ID TargetID, JSVM::jsvm_value* ObjectInfoData)
{
	size_t DataLen = 0;
	void* pData = JSVM::jsvm_get_value_binary(Context, ObjectInfoData, &DataLen);
	return ReportObject(TargetID, pData, (UINT)DataLen);
}

bool CDistributedObjectOperatorJS::JSRegisterObject(JSVM::jsvm_context* Context, JSVM::jsvm_value* ObjectRegisterInfo)
{
	return CDOSMainThread::GetInstance()->GetDistributedObjectManager()->RegisterObject(Context, ObjectRegisterInfo, m_pVMInfo);
}

UINT CDistributedObjectOperatorJS::JSAddTimer(JSVM::jsvm_context* Context, UINT64 TimeOut, JSVM::jsvm_value* Param, bool IsRepeat)
{
	JSVM::jsvm_persistent_value* ParamP = JSVM::jsvm_create_persistent_value(Context, Param);
	UINT TimerID = AddTimer(TimeOut, (UINT64)ParamP, IsRepeat);
	if (!TimerID)
		JSVM::jsvm_release_persistent_value(ParamP);
	return TimerID;
}


bool CDistributedObjectOperatorJS::RegisterLogger(UINT LogChannel, LPCTSTR FileName)
{
	CEasyString LogFileName;
	CEasyString ModulePath = CFileTools::GetModulePath(NULL);

	CServerLogPrinter* pLog;

	LogFileName.Format("%s/Log/%s", (LPCTSTR)ModulePath, FileName);
	pLog = MONITORED_NEW(_T("CDistributedObjectOperator"), CServerLogPrinter, CDOSMainThread::GetInstance(), CServerLogPrinter::LOM_CONSOLE | CServerLogPrinter::LOM_FILE,
		CSystemConfig::GetInstance()->GetLogLevel(), LogFileName, CSystemConfig::GetInstance()->GetLogCacheSize());
	CLogManager::GetInstance()->AddChannel(LogChannel, pLog);
	SAFE_RELEASE(pLog);
	return true;
}

bool CDistributedObjectOperatorJS::RegisterCSVLogger(UINT LogChannel, LPCTSTR FileName, LPCTSTR CSVLogHeader)
{
	CEasyString LogFileName;
	CEasyString ModulePath = CFileTools::GetModulePath(NULL);

	CCSVFileLogPrinter* pLog;

	LogFileName.Format("%s/Log/%s", (LPCTSTR)ModulePath, FileName);
	pLog = MONITORED_NEW(_T("CDistributedObjectOperator"), CCSVFileLogPrinter, CSystemConfig::GetInstance()->GetLogLevel(), LogFileName, CSVLogHeader, CSystemConfig::GetInstance()->GetLogCacheSize());
	CLogManager::GetInstance()->AddChannel(LogChannel, pLog);
	SAFE_RELEASE(pLog);
	return true;
}

void CDistributedObjectOperatorJS::SetServerWorkStatus(BYTE WorkStatus)
{
	CDOSMainThread::GetInstance()->SetServerStatus(SC_SST_SS_WORK_STATUS, CSmartValue(WorkStatus));
}


