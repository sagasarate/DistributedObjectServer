#pragma once

class CDistributedObjectManager;

class CDistributedObjectOperator :
	public IDistributedObjectOperator,
	public IConsoleCommandReceiver,
	public CDOSBaseObject
{
protected:
	CDistributedObjectManager*		m_pManager;
	IDistributedObject*				m_pDistributedObject;
	bool							m_IsCommandReceiver;
public:
	CDistributedObjectOperator();
	~CDistributedObjectOperator();

	bool Init(CDistributedObjectManager * pManager,UINT PoolID,IDistributedObject * pDistributedObject);
	virtual bool Initialize();
	virtual void Destory();

	virtual bool OnConsoleCommand(LPCTSTR szCommand) override;
	virtual UINT64 GetReceiverID() override;

	virtual UINT GetRouterID() override;
	virtual OBJECT_ID GetObjectID() override;
	virtual int GetGroupIndex() override;
	virtual bool SendMessage(OBJECT_ID ReceiverID, OBJECT_ID SenderID, MSG_ID_TYPE MsgID, WORD MsgFlag = 0, LPCVOID pData = 0, UINT DataSize = 0) override;
	virtual bool SendMessageMulti(OBJECT_ID * pReceiverIDList, UINT ReceiverCount, bool IsSorted, OBJECT_ID SenderID, MSG_ID_TYPE MsgID, WORD MsgFlag = 0, LPCVOID pData = 0, UINT DataSize = 0) override;
	virtual bool BroadcastMessageToProxyByMask(WORD RouterID, BYTE ProxyType, UINT64 Mask, OBJECT_ID SenderID, MSG_ID_TYPE MsgID, WORD MsgFlag, LPCVOID pData, UINT DataSize) override;
	virtual bool BroadcastMessageToProxyByGroup(WORD RouterID, BYTE ProxyType, UINT64 GroupID, OBJECT_ID SenderID, MSG_ID_TYPE MsgID, WORD MsgFlag, LPCVOID pData, UINT DataSize) override;

	virtual CDOSMessagePacket * NewMessagePacket(UINT DataSize, UINT ReceiverCount) override;
	virtual bool ReleaseMessagePacket(CDOSMessagePacket * pPacket) override;
	virtual bool SendMessagePacket(CDOSMessagePacket * pPacket) override;

	virtual bool RegisterMsgMap(OBJECT_ID ProxyObjectID, MSG_ID_TYPE * pMsgIDList, int CmdCount) override;
	virtual bool UnregisterMsgMap(OBJECT_ID ProxyObjectID, MSG_ID_TYPE * pMsgIDList, int CmdCount) override;
	virtual bool RegisterGlobalMsgMap(ROUTE_ID_TYPE ProxyRouterID, BYTE ProxyType, MSG_ID_TYPE MsgID, int MapType) override;
	virtual bool UnregisterGlobalMsgMap(ROUTE_ID_TYPE ProxyRouterID, BYTE ProxyType, MSG_ID_TYPE MsgID) override;
	virtual bool SetUnhanleMsgReceiver(ROUTE_ID_TYPE ProxyRouterID, BYTE ProxyType) override;

	virtual bool AddConcernedObject(OBJECT_ID ObjectID, bool NeedTest) override;
	virtual bool DeleteConcernedObject(OBJECT_ID ObjectID) override;

	virtual bool FindObject(UINT ObjectType, bool OnlyLocal) override;
	virtual bool ReportObject(OBJECT_ID TargetID, const void * pObjectInfoData, UINT DataSize) override;
	virtual bool CloseProxyObject(OBJECT_ID ProxyObjectID, UINT Delay) override;
	virtual bool RequestProxyObjectIP(OBJECT_ID ProxyObjectID) override;

	virtual bool RegisterObject(DOS_OBJ_REGISTER_INFO& ObjectRegisterInfo) override;
	virtual void Release() override;

	virtual bool QueryShutDown(OBJECT_ID TargetID, BYTE Level, UINT Param) override;
	virtual void ShutDown(UINT PluginID) override;
	virtual bool RegisterCommandReceiver() override;
	virtual bool UnregisterCommandReceiver() override;
	

	virtual bool RegisterLogger(UINT LogChannel, LPCTSTR FileName) override;
	virtual bool RegisterCSVLogger(UINT LogChannel, LPCTSTR FileName, LPCTSTR CSVLogHeader) override;

	virtual void SetServerWorkStatus(BYTE WorkStatus) override;

	virtual UINT AddTimer(UINT64 TimeOut, UINT64 Param, bool IsRepeat) override;
	virtual bool DeleteTimer(UINT ID) override;

	virtual bool SetBroadcastMask(OBJECT_ID ProxyObjectID, UINT64 Mask) override;
	virtual bool AddBroadcastMask(OBJECT_ID ProxyObjectID, UINT64 Mask) override;
	virtual bool RemoveBroadcastMask(OBJECT_ID ProxyObjectID, UINT64 Mask) override;
	virtual bool AddBroadcastGroup(OBJECT_ID ProxyObjectID, UINT64 GroupID) override;
	virtual bool RemoveBroadcastGroup(OBJECT_ID ProxyObjectID, UINT64 GroupID) override;
protected:
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
	
};
