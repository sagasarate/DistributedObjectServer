#pragma once

enum DOSObjJSFuncs
{
	JSFuncInitialize,
	JSFuncDestory,
	JSFuncOnPreTranslateMessage,
	JSFuncOnMessage,
	JSFuncOnSystemMessage,
	JSFuncOnConcernedObjectLost,
	JSFuncOnFindObject,
	JSFuncOnObjectReport,
	JSFuncOnProxyObjectIPReport,
	JSFuncOnShutDown,
	JSFuncUpdate,
	JSFuncOnConsoleCommand,
	JSFuncOnTimer,
	JSFuncOnTimerRelease,
	JSFuncMax
};

const LPCTSTR DOSObjJSFuncNames[DOSObjJSFuncs::JSFuncMax] =
{
	"Initialize",
	"Destory",
	"OnPreTranslateMessage",
	"OnMessage",
	"OnSystemMessage",
	"OnConcernedObjectLost",
	"OnFindObject",
	"OnObjectReport",
	"OnProxyObjectIPReport",
	"OnShutDown",
	"Update",
	"OnConsoleCommand",
	"OnTimer",
	"OnTimerRelease",
};

struct JS_DOS_OBJECT_INIT_INFO
{
	UINT PluginID;
	UINT LogChannel;
	CEasyString MainMoudulePath;
	CEasyString ConfigDir;
	CEasyString LogDir;
	CEasyString	DOSObjectClassName;
	JSVM::jsvm_persistent_value* DOSObject;
	JS_DOS_OBJECT_INIT_INFO()
	{
		PluginID = 0;
		LogChannel = 0;
		DOSObject = NULL;
	}
};

class CDistributedObjectManager;

class CDistributedObjectOperatorJS :
	public CDOSBaseObject,
	public IConsoleCommandReceiver
{
protected:
	CDistributedObjectManager*		m_pManager;
	bool							m_IsCommandReceiver;

	JSVM_VM_INFO*					m_pVMInfo;
	JS_DOS_OBJECT_INIT_INFO			m_InitInfo;
	JSVM::jsvm_persistent_value*	m_JSOperatorObject;
	JSVM::jsvm_persistent_value*	m_JSObject;
	JSVM::jsvm_persistent_value*	m_JSObjectFuncs[DOSObjJSFuncs::JSFuncMax];
	JSVM::jsvm_persistent_value*	m_JSObjectID;

public:
	CDistributedObjectOperatorJS();
	~CDistributedObjectOperatorJS();

	bool Init(CDistributedObjectManager* pManager, UINT PoolID, JS_DOS_OBJECT_INIT_INFO& InitInfo);
	virtual bool Initialize() override;
	virtual void Destory() override;
	virtual void Release() override;

	virtual bool OnConsoleCommand(LPCTSTR szCommand) override;
	virtual UINT64 GetReceiverID() override;
	void ShutDown(UINT PluginID);

protected:
	bool DoInit(JSVM_VM_INFO* pVMInfo);
	bool RegisterCommandReceiver();
	bool UnregisterCommandReceiver();


	virtual bool OnMessage(CDOSMessage* pMessage) override;
	virtual bool OnSystemMessage(CDOSMessage* pMessage) override;
	virtual void OnConcernedObjectLost(OBJECT_ID ObjectID) override;
	virtual void OnFindObject(OBJECT_ID CallerID) override;
	virtual void OnObjectReport(OBJECT_ID ObjectID, const void* pObjectInfoData, UINT DataSize) override;
	virtual void OnProxyObjectIPReport(OBJECT_ID ProxyObjectID, UINT Port, LPCSTR szIPString) override;
	virtual void OnShutDown(BYTE Level, UINT Param) override;
	virtual int Update(int ProcessPacketLimit = DEFAULT_SERVER_PROCESS_PACKET_LIMIT) override;
	virtual void OnTimer(UINT ID, UINT64 Param, bool IsRepeat) override;
	virtual void OnTimerRelease(UINT ID, UINT64 Param) override;

	template<DOSObjJSFuncs func, typename... ArgTypes>
	void JSCallThis(JSVM::jsvm_context* context, ArgTypes... Args);

	template<typename RT, DOSObjJSFuncs func, typename... ArgTypes>
	typename std::enable_if<!std::is_void<RT>::value, RT>::type JSCallThis(JSVM::jsvm_context* context, ArgTypes... Args);

public:
	static JSVM::jsvm_persistent_value* CreateJSClass_IDistributedObjectOperator(JSVM::jsvm_context* Context);
	static JSVM::jsvm_persistent_value* CreateJSClass_IDistributedObjectManager(JSVM::jsvm_context* Context);
	static JSVM::jsvm_persistent_value* CreateJSClass_IDOSMessage(JSVM::jsvm_context* Context);
	static JSVM::jsvm_persistent_value* CreateJSClass_IDOSMessagePacket(JSVM::jsvm_context* Context);

	static void JSOperatorConstructor(JSVM::jsvm_callback_info* callback_info);
	JSVM::jsvm_value* JSGetObjectID(JSVM::jsvm_context* Context);
	bool JSSendMessage(JSVM::jsvm_context* Context, OBJECT_ID ReceiverID, OBJECT_ID SenderID, MSG_ID_TYPE MsgID, WORD MsgFlag, JSVM::jsvm_value* Data);
	bool JSSendMessageMulti(JSVM::jsvm_context* Context, CEasyArray<OBJECT_ID> ReceiverIDList, bool IsSorted, OBJECT_ID SenderID, MSG_ID_TYPE MsgID, WORD MsgFlag, JSVM::jsvm_value* Data);
	bool JSBroadcastMessageToProxyByMask(JSVM::jsvm_context* Context, WORD RouterID, BYTE ProxyType, UINT64 Mask, OBJECT_ID SenderID, MSG_ID_TYPE MsgID, WORD MsgFlag, JSVM::jsvm_value* Data);
	bool JSBroadcastMessageToProxyByGroup(JSVM::jsvm_context* Context, WORD RouterID, BYTE ProxyType, UINT64 GroupID, OBJECT_ID SenderID, MSG_ID_TYPE MsgID, WORD MsgFlag, JSVM::jsvm_value* Data);

	bool JSReleaseMessagePacket(JSVM::jsvm_context* Context, JSVM::jsvm_value* pPacket);

	bool JSRegisterMsgMap(JSVM::jsvm_context* Context, OBJECT_ID ProxyObjectID, CEasyArray<MSG_ID_TYPE> MsgIDList);
	bool JSUnregisterMsgMap(JSVM::jsvm_context* Context, OBJECT_ID ProxyObjectID, CEasyArray<MSG_ID_TYPE> MsgIDList);

	bool JSReportObject(JSVM::jsvm_context* Context, OBJECT_ID TargetID, JSVM::jsvm_value* ObjectInfoData);

	bool JSRegisterObject(JSVM::jsvm_context* Context, JSVM::jsvm_value* ObjectRegisterInfo);

	UINT JSAddTimer(JSVM::jsvm_context* Context, UINT64 TimeOut, JSVM::jsvm_value* Param, bool IsRepeat);

	bool RegisterLogger(UINT LogChannel, LPCTSTR FileName);
	bool RegisterCSVLogger(UINT LogChannel, LPCTSTR FileName, LPCTSTR CSVLogHeader);
	void SetServerWorkStatus(BYTE WorkStatus);
};

template<DOSObjJSFuncs func, typename... ArgTypes>
void CDistributedObjectOperatorJS::JSCallThis(JSVM::jsvm_context* context, ArgTypes... Args)
{
	if (func < DOSObjJSFuncs::JSFuncMax)
	{
		if (m_JSObject && m_JSObjectFuncs[func])
		{
			JSVM::jsvm_value* Func = JSVM::jsvm_get_value_from_persistent_value(context, m_JSObjectFuncs[func]);
			JSVM::jsvm_value* JSObject = JSVM::jsvm_get_value_from_persistent_value(context, m_JSObject);
			if (JSObject && Func)
			{
				JSVM::JSCallThis(context, JSObject, Func, std::forward<ArgTypes>(Args)...);
			}
			else
			{
				LogJSVM("DOS对象无法获取或者对象上函数%s无法获取", DOSObjJSFuncNames[func]);
			}
		}
		else
		{
			LogJSVM("DOS对象不存在或者对象上不存在函数%s", DOSObjJSFuncNames[func]);
		}
	}
	else
	{
		LogJSVM("DOS对象函数类型%d不存在", func);
	}
};

template<typename RT, DOSObjJSFuncs func, typename... ArgTypes>
typename std::enable_if<!std::is_void<RT>::value, RT>::type CDistributedObjectOperatorJS::JSCallThis(JSVM::jsvm_context* context, ArgTypes... Args)
{
	if (func < DOSObjJSFuncs::JSFuncMax)
	{
		if (m_JSObject && m_JSObjectFuncs[func])
		{
			JSVM::jsvm_value* Func = JSVM::jsvm_get_value_from_persistent_value(context, m_JSObjectFuncs[func]);
			JSVM::jsvm_value* JSObject = JSVM::jsvm_get_value_from_persistent_value(context, m_JSObject);
			if (JSObject && Func)
			{
				return JSVM::JSCallThis<RT>(context, JSObject, Func, std::forward<ArgTypes>(Args)...);
			}
			else
			{
				LogJSVM("DOS对象无法获取或者对象上函数%s无法获取", DOSObjJSFuncNames[func]);
			}
		}
		else
		{
			LogJSVM("DOS对象不存在或者对象上不存在函数%s", DOSObjJSFuncNames[func]);
		}
	}
	else
	{
		LogJSVM("DOS对象函数类型%d不存在", func);
	}
	return RT();
};
