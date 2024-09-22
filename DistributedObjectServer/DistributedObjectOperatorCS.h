#pragma once

class CDistributedObjectManager;

class CDistributedObjectOperatorCS :
	public CDOSBaseObject,
	public IConsoleCommandReceiver	
{
protected:
	CDistributedObjectManager*		m_pManager;
	bool							m_IsCommandReceiver;

	MONO_DOMAIN_INFO				m_MonoDomainInfo;
	UINT							m_hCSOperatorObject;
	UINT							m_hCSObject;
	
	MONO_CLASS_INFO_DO				m_MonoClassInfo_DO;
public:
	CDistributedObjectOperatorCS();
	~CDistributedObjectOperatorCS();

	bool Init(CDistributedObjectManager * pManager, UINT PoolID, MONO_DOMAIN_INFO& MonoDomainInfo, MonoObject * pDistributedObject);
	virtual bool Initialize() override;
	virtual void Destory() override;
	virtual void Release() override;
	
	virtual bool OnConsoleCommand(LPCTSTR szCommand) override;
	virtual UINT64 GetReceiverID() override;
	void ShutDown(UINT PluginID);
	
protected:

	bool RegisterCommandReceiver();
	bool UnregisterCommandReceiver();


	virtual bool OnMessage(CDOSMessage * pMessage) override;
	virtual bool OnSystemMessage(CDOSMessage * pMessage) override;
	virtual void OnConcernedObjectLost(OBJECT_ID ObjectID) override;
	virtual void OnFindObject(OBJECT_ID CallerID) override;
	virtual void OnObjectReport(OBJECT_ID ObjectID, const void * pObjectInfoData, UINT DataSize) override;
	virtual void OnProxyObjectIPReport(OBJECT_ID ProxyObjectID, UINT Port, LPCSTR szIPString) override;
	virtual void OnShutDown(BYTE Level, UINT Param) override;
	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT) override;
	virtual void OnTimer(UINT ID, UINT64 Param, bool IsRepeat) override;
	virtual void OnTimerRelease(UINT ID, UINT64 Param) override;

	bool CallCSInitialize();
	void CallCSDestory();
	int CallCSOnPreTranslateMessage(MSG_ID_TYPE MsgID, WORD MsgFlag, OBJECT_ID SenderID, const BYTE * pData, UINT DataSize);
	int CallCSOnMessage(MSG_ID_TYPE MsgID, WORD MsgFlag, OBJECT_ID SenderID, const BYTE * pData, UINT DataSize);
	int CallCSOnSystemMessage(MSG_ID_TYPE MsgID, WORD MsgFlag, OBJECT_ID SenderID, const BYTE * pData, UINT DataSize);
	void CallCSOnConcernedObjectLost(OBJECT_ID ObjectID);
	bool CallCSOnFindObject(OBJECT_ID CallerID);
	void CallCSOnObjectReport(OBJECT_ID ObjectID, const void * pObjectInfoData, UINT DataSize);
	void CallCSOnProxyObjectIPReport(OBJECT_ID ProxyObjectID, UINT Port, LPCSTR szIPString);
	void CallCSOnShutDown(BYTE Level, UINT Param);
	int CallCSUpdate(int ProcessPacketLimit);
	void CallCSOnException(MonoObject * pPostException);
	bool CallOnConsoleCommand(LPCTSTR szCommand);
	void CallCSOnTimer(UINT ID, UINT64 Param, bool IsRepeat);
	void CallCSOnTimerRelease(UINT ID, UINT64 Param);

public:
	static UINT InternalCallGetRouterID();
	static UINT64 InternalCallGetObjectID(CDistributedObjectOperatorCS * pOperator);
	static int InternalCallGetGroupIndex(CDistributedObjectOperatorCS * pOperator);
	static bool InternalCallSendMessage(CDistributedObjectOperatorCS * pOperator, OBJECT_ID ReceiverID, OBJECT_ID SenderID, UINT MsgID, WORD MsgFlag, MonoArray * Data, int StartIndex, int DataLen);
	static bool InternalCallSendMessageMulti(CDistributedObjectOperatorCS * pOperator, MonoArray * ReceiverIDList, bool IsSorted, OBJECT_ID SenderID, UINT MsgID, WORD MsgFlag, MonoArray * Data, int StartIndex, int DataLen);
	static bool InternalCallBroadcastMessageToProxyByMask(CDistributedObjectOperatorCS * pOperator, WORD RouterID, BYTE ProxyType, UINT64 Mask, OBJECT_ID SenderID, UINT MsgID, WORD MsgFlag, MonoArray * Data, int StartIndex, int DataLen);
	static bool InternalCallBroadcastMessageToProxyByGroup(CDistributedObjectOperatorCS* pOperator, WORD RouterID, BYTE ProxyType, UINT64 GroupID, OBJECT_ID SenderID, UINT MsgID, WORD MsgFlag, MonoArray* Data, int StartIndex, int DataLen);
	static bool InternalCallRegisterMsgMap(CDistributedObjectOperatorCS * pOperator, OBJECT_ID ProxyObjectID, MonoArray * MsgIDList);
	static bool InternalCallUnregisterMsgMap(CDistributedObjectOperatorCS * pOperator, OBJECT_ID ProxyObjectID, MonoArray * MsgIDList);
	static bool InternalCallRegisterGlobalMsgMap(CDistributedObjectOperatorCS * pOperator, WORD ProxyRouterID, BYTE ProxyType, UINT MsgID, int MapType);
	static bool InternalCallUnregisterGlobalMsgMap(CDistributedObjectOperatorCS * pOperator, WORD ProxyRouterID, BYTE ProxyType, UINT MsgID);
	static bool InternalCallSetUnhanleMsgReceiver(CDistributedObjectOperatorCS * pOperator, WORD ProxyRouterID, BYTE ProxyType);
	static bool InternalCallAddConcernedObject(CDistributedObjectOperatorCS * pOperator, OBJECT_ID ObjectID, bool NeedTest);
	static bool InternalCallDeleteConcernedObject(CDistributedObjectOperatorCS * pOperator, OBJECT_ID ObjectID);
	static bool InternalCallFindObject(CDistributedObjectOperatorCS * pOperator, UINT ObjectType, bool OnlyLocal);
	static bool InternalCallReportObject(CDistributedObjectOperatorCS * pOperator, OBJECT_ID TargetID, MonoArray * Data, int StartIndex, int DataLen);
	static bool InternalCallCloseProxyObject(CDistributedObjectOperatorCS * pOperator, OBJECT_ID ProxyObjectID, UINT Delay);
	static bool InternalCallRequestProxyObjectIP(CDistributedObjectOperatorCS * pOperator, OBJECT_ID ProxyObjectID);
	static bool InternalCallRegisterObjectStatic(UINT PluginID, MonoObject * ObjectRegisterInfo);
	static bool InternalCallRegisterObject(CDistributedObjectOperatorCS * pOperator, MonoObject * ObjectRegisterInfo);
	static void InternalCallRelease(CDistributedObjectOperatorCS * pOperator);
	static bool InternalCallQueryShutDown(CDistributedObjectOperatorCS * pOperator, OBJECT_ID TargetID, BYTE Level, UINT Param);
	static void InternalCallShutDown(CDistributedObjectOperatorCS * pOperator, UINT PluginID);
	static bool InternalCallRegisterLogger(UINT LogChannel, MonoString * FileName);
	static bool InternalCallRegisterCSVLogger(UINT LogChannel, MonoString * FileName, MonoString * CSVLogHeader);
	static bool InternalCallRegisterCommandReceiver(CDistributedObjectOperatorCS * pOperator);
	static bool InternalCallUnregisterCommandReceiver(CDistributedObjectOperatorCS * pOperator);
	static void InternalCallSetServerWorkStatus(CDistributedObjectOperatorCS * pOperator, BYTE WorkStatus);
	static UINT InternalCallAddTimer(CDistributedObjectOperatorCS * pOperator, UINT64 TimeOut, MonoObject * pParam, bool IsRepeat);
	static bool InternalCallDeleteTimer(CDistributedObjectOperatorCS * pOperator, UINT ID);
	static bool InternalCallSetBroadcastMask(CDistributedObjectOperatorCS * pOperator, OBJECT_ID ProxyObjectID, UINT64 Mask);
	static bool InternalCallAddBroadcastMask(CDistributedObjectOperatorCS* pOperator, OBJECT_ID ProxyObjectID, UINT64 Mask);
	static bool InternalCallRemoveBroadcastMask(CDistributedObjectOperatorCS* pOperator, OBJECT_ID ProxyObjectID, UINT64 Mask);
	static bool InternalCallAddBroadcastGroup(CDistributedObjectOperatorCS* pOperator, OBJECT_ID ProxyObjectID, UINT64 GroupID);
	static bool InternalCallRemoveBroadcastGroup(CDistributedObjectOperatorCS* pOperator, OBJECT_ID ProxyObjectID, UINT64 GroupID);
};
