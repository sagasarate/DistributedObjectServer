#pragma once

enum JSVMPersistentValues
{
	JSVMPersistentValues_IDistributedObjectOperator,
	JSVMPersistentValues_IDistributedObjectManager,
	JSVMPersistentValues_IDOSMessage,
	JSVMPersistentValues_IDOSMessagePacket,
	JSVMPersistentValues_OBJECT_ID,
	JSVMPersistentValues_CSmartStruct,
	JSVMPersistentValues_PluginInitFn,
	JSVMPersistentValues_PluginCheckReleaseFn,
	JSVMPersistentValues_Max,
};

struct JS_CLASS_INFO
{
	CEasyString						Name;
	JSVM::jsvm_persistent_value* Class;
};
class CDistributedObjectOperatorJS;
struct JSVM_VM_INFO
{
	UINT										PluginID;
	UINT										LogChannel;
	CEasyString									MainMoudulePath;
	CEasyString									ConfigDir;
	CEasyString									LogDir;

	int											GroupIndex;
	JSVM::jsvm_vm*								pVM;
	int											ObjectCount;
	JSVM::jsvm_persistent_value*				PersistentValues[JSVMPersistentValues_Max];
	CStaticMap<CEasyString, JS_CLASS_INFO>		ExportClasses;

	CEasyBuffer									MoudleSnapshot;
	CEasyTimer									GCTimer;
	CEasyTimer									ReleaseTimer;
	volatile bool								NeedRelease;
	JSVM_VM_INFO()
	{
		PluginID = 0;
		LogChannel = 0;
		GroupIndex = -1;
		pVM = NULL;
		ObjectCount = 0;
		ZeroMemory(PersistentValues, sizeof(PersistentValues));
		NeedRelease = false;
	}
	bool Init(UINT uiPluginID, UINT uiLogChannel, int iGroupIndex, LPCTSTR szMainMoudulePath, LPCTSTR szConfigDir, LPCTSTR szLogDir);
	void Release();
	JSVM::jsvm_value* GetClass(JSVM::jsvm_context* context, LPCTSTR szClassName)
	{
		JS_CLASS_INFO* pInfo = ExportClasses.Find(szClassName);
		if (pInfo)
		{
			return JSVM::jsvm_get_value_from_persistent_value(context, pInfo->Class);
		}
		return NULL;
	}
};



class CJSVMHelper :public CStaticObject4<CJSVMHelper>
{
protected:
	CEasyTimer										m_GCTimer;
	JSVM::jsvm_vm*									m_pMainVM;
	CEasyArray<JSVM_VM_INFO*>						m_AllVMList;
	CEasyCriticalSection							m_CriticalSection;

	static thread_local CEasyArray<JSVM_VM_INFO>	m_VMInfoList;
public:
	CJSVMHelper();
	~CJSVMHelper();

	bool Init();
	void Release();
	int Update(int ProcessLimit = DEFAULT_SERVER_PROCESS_PACKET_LIMIT);

	JSVM::jsvm_vm* GetMainVM()
	{
		return m_pMainVM;
	}
	JSVM_VM_INFO* CreateJSVM(UINT PluginID, UINT LogChannel, int GroupIndex, LPCTSTR szMainMoudulePath, LPCTSTR szConfigDir, LPCTSTR szLogDir);
	JSVM_VM_INFO* FindJSVM(UINT PluginID, int GroupIndex);
	JSVM_VM_INFO* GetCurJSVM(UINT PluginID)
	{
		return m_VMInfoList.Find([PluginID](const JSVM_VM_INFO& Info)->bool {return Info.PluginID == PluginID; });
	}
	JSVM_VM_INFO* GetCurJSVM(JSVM::jsvm_context* context);

	bool RunPlugin(UINT PluginID, UINT LogChannel, LPCTSTR ConfigDir, LPCTSTR LogDir);
	bool QueryReleasePlugin(UINT PluginID);

	static int OnDOSGroupUpdate(int GroupIndex, int ProcessLimit);
	static void OnDOSGroupRelease(int GroupIndex);
	static bool JSDOSPrintLog(UINT LogChannel, int Level, LPCTSTR Tag, LPCTSTR Msg);
protected:
	void RegisterVMInfo(JSVM_VM_INFO* pVMInfo);
	void UnegisterVMInfo(JSVM_VM_INFO* pVMInfo);
	
};

class JSIDOSMessage :public CDOSMessage
{
public:
	static void JSConstructor();

	JSVM::jsvm_value* get_Data(JSVM::jsvm_context* context);

	JSVM::jsvm_value* get_DataPacket(JSVM::jsvm_context* context);
};

class JSIDOSMessagePacket :public CDOSMessagePacket
{
public:
	static void JSConstructor();

	UINT get_MsgID()
	{
		return GetMessage().GetMsgID();
	}
	void set_MsgID(UINT value)
	{
		GetMessage().SetMsgID(value);
	}

	UINT get_MsgFlag()
	{
		return GetMessage().GetMsgFlag();
	}
	void set_MsgFlag(UINT value)
	{
		GetMessage().SetMsgFlag(value);
	}
	MSG_LEN_TYPE get_DataLength()
	{
		return GetMessage().GetDataLength();
	}
	void set_DataLength(MSG_LEN_TYPE value) 
	{
		GetMessage().SetDataLength(value);
	}

	JSVM::jsvm_value* get_Data(JSVM::jsvm_context* context);
	void set_Data(JSVM::jsvm_context* context, JSVM::jsvm_value* value);

	JSVM::jsvm_value* get_DataPacket(JSVM::jsvm_context* context);
	void set_DataPacket(JSVM::jsvm_context* context, JSVM::jsvm_value* value);

	OBJECT_ID get_SenderID()
	{
		return GetMessage().GetSenderID();
	}
	void set_SenderID(OBJECT_ID value)
	{
		GetMessage().SetSenderID(value);
	}

	JSVM::jsvm_value* get_TargetIDs(JSVM::jsvm_context* context);
	void set_TargetIDs(JSVM::jsvm_context* context, JSVM::jsvm_value* value);

	JSVM::jsvm_value* get_EmptyDataPacket(JSVM::jsvm_context* context);
};

