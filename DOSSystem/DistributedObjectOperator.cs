using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace DOSSystem
{


    public class DOS_OBJ_REGISTER_INFO
    {
        public OBJECT_ID ObjectID;
        public string ObjectTypeName;
        public int Weight = 1;
        public int ObjectGroupIndex = -1;
        public uint MsgQueueSize = 1024;
        public uint MsgProcessLimit = 32;
        public uint Flag = 0;
        public object Object = null;
    };



    public class DistributedObjectOperator
    {
        IntPtr m_ObjectHandle;

        DistributedObjectOperator(IntPtr ObjectHandle)
        {
            m_ObjectHandle = ObjectHandle;
        }

        public static uint GetRouterID()
        {
            return InternalCallGetRouterID();
        }
        public OBJECT_ID GetObjectID()
        {
            return InternalCallGetObjectID(m_ObjectHandle);
        }
        public int GetGroupIndex()
        {
            return InternalCallGetGroupIndex(m_ObjectHandle);
        }
        public bool SendMessage(OBJECT_ID ReceiverID, OBJECT_ID SenderID, uint MsgID, ushort MsgFlag, byte[] Data, int StartIndex, int DataLen)
        {
            return InternalCallSendMessage(m_ObjectHandle, ReceiverID, SenderID, MsgID, MsgFlag, Data, StartIndex, DataLen);
        }
        public bool SendMessageMulti(OBJECT_ID[] ReceiverIDList, bool IsSorted, OBJECT_ID SenderID, uint MsgID, ushort MsgFlag, byte[] Data, int StartIndex, int DataLen)
        {
            return InternalCallSendMessageMulti(m_ObjectHandle, ReceiverIDList, IsSorted, SenderID, MsgID, MsgFlag, Data, StartIndex, DataLen);
        }
        public bool BroadcastMessageToProxyByMask(ushort RouterID, byte ProxyType, ulong Mask, OBJECT_ID SenderID, uint MsgID, ushort MsgFlag, byte[] Data, int StartIndex, int DataLen)
        {
            return InternalCallBroadcastMessageToProxyByMask(m_ObjectHandle, RouterID, ProxyType, Mask, SenderID, MsgID, MsgFlag, Data, StartIndex, DataLen);
        }
        public bool BroadcastMessageToProxyByGroup(ushort RouterID, byte ProxyType, ulong GroupID, OBJECT_ID SenderID, uint MsgID, ushort MsgFlag, byte[] Data, int StartIndex, int DataLen)
        {
            return InternalCallBroadcastMessageToProxyByGroup(m_ObjectHandle, RouterID, ProxyType, GroupID, SenderID, MsgID, MsgFlag, Data, StartIndex, DataLen);
        }
        public bool RegisterMsgMap(OBJECT_ID ProxyObjectID, uint[] MsgIDList)
        {
            return InternalCallRegisterMsgMap(m_ObjectHandle, ProxyObjectID, MsgIDList);
        }
        public bool UnregisterMsgMap(OBJECT_ID ProxyObjectID, uint[] MsgIDList)
        {
            return InternalCallUnregisterMsgMap(m_ObjectHandle, ProxyObjectID, MsgIDList);
        }
        public bool RegisterGlobalMsgMap(ushort ProxyRouterID, byte ProxyType, uint MsgID, int MapType)
        {
            return InternalCallRegisterGlobalMsgMap(m_ObjectHandle, ProxyRouterID, ProxyType, MsgID, MapType);
        }
        public bool UnregisterGlobalMsgMap(ushort ProxyRouterID, byte ProxyType, uint MsgID)
        {
            return InternalCallUnregisterGlobalMsgMap(m_ObjectHandle, ProxyRouterID, ProxyType, MsgID);
        }

        public bool SetUnhanleMsgReceiver(ushort ProxyRouterID, byte ProxyType)
        {
            return InternalCallSetUnhanleMsgReceiver(m_ObjectHandle, ProxyRouterID, ProxyType);
        }

        public bool AddConcernedObject(OBJECT_ID ObjectID, bool NeedTest)
        {
            return InternalCallAddConcernedObject(m_ObjectHandle, ObjectID, NeedTest);
        }
        public bool DeleteConcernedObject(OBJECT_ID ObjectID)
        {
            return InternalCallDeleteConcernedObject(m_ObjectHandle, ObjectID);
        }

        public bool FindObject(uint ObjectType, bool OnlyLocal)
        {
            return InternalCallFindObject(m_ObjectHandle, ObjectType, OnlyLocal);
        }
        public bool ReportObject(OBJECT_ID TargetID, byte[] Data, int StartIndex, int DataLen)
        {
            return InternalCallReportObject(m_ObjectHandle, TargetID, Data, StartIndex, DataLen);
        }
        public bool CloseProxyObject(OBJECT_ID ProxyObjectID, uint Delay)
        {
            return InternalCallCloseProxyObject(m_ObjectHandle, ProxyObjectID, Delay);
        }
        public bool RequestProxyObjectIP(OBJECT_ID ProxyObjectID)
        {
            return InternalCallRequestProxyObjectIP(m_ObjectHandle, ProxyObjectID);
        }

        public static bool RegisterObject(uint PluginID, DOS_OBJ_REGISTER_INFO ObjectRegisterInfo)
        {
            return InternalCallRegisterObject(PluginID, ObjectRegisterInfo);
        }

        public bool RegisterObject(DOS_OBJ_REGISTER_INFO ObjectRegisterInfo)
        {
            return InternalCallRegisterObject(m_ObjectHandle, ObjectRegisterInfo);
        }
        public void Release()
        {
            InternalCallRelease(m_ObjectHandle);
        }

        public bool QueryShutDown(OBJECT_ID TargetID, byte Level, uint Param)
        {
            return InternalCallQueryShutDown(m_ObjectHandle, TargetID, Level, Param);
        }
        public void ShutDown(uint PluginID)
        {
            InternalCallShutDown(m_ObjectHandle, PluginID);
        }

        public static bool RegisterLogger(uint LogChannel, string FileName)
        {
            return InternalCallRegisterLogger(LogChannel, FileName);
        }
        public static bool RegisterCSVLogger(uint LogChannel, string FileName, string CSVLogHeader)
        {
            return InternalCallRegisterCSVLogger(LogChannel, FileName, CSVLogHeader);
        }

        public bool RegisterCommandReceiver()
        {
            return InternalCallRegisterCommandReceiver(m_ObjectHandle);
        }
        public bool UnregisterCommandReceiver()
        {
            return InternalCallUnregisterCommandReceiver(m_ObjectHandle);
        }

        public void SetServerWorkStatus(byte WorkStatus)
        {
            InternalCallSetServerWorkStatus(m_ObjectHandle, WorkStatus);
        }
        public uint AddTimer(ulong TimeOut, object Param, bool IsRepeat)
        {
            return InternalCallAddTimer(m_ObjectHandle, TimeOut, Param, IsRepeat);
        }
        public bool DeleteTimer(uint ID)
        {
            return InternalCallDeleteTimer(m_ObjectHandle, ID);
        }
        public bool SetBroadcastMask(OBJECT_ID ProxyObjectID, ulong Mask)
        {
            return InternalCallSetBroadcastMask(m_ObjectHandle, ProxyObjectID, Mask);
        }
        public bool AddBroadcastMask(OBJECT_ID ProxyObjectID, ulong Mask)
        {
            return InternalCallAddBroadcastMask(m_ObjectHandle, ProxyObjectID, Mask);
        }
        public bool RemoveBroadcastMask(OBJECT_ID ProxyObjectID, ulong Mask)
        {
            return InternalCallRemoveBroadcastMask(m_ObjectHandle, ProxyObjectID, Mask);
        }
        public bool AddBroadcastGroup(OBJECT_ID ProxyObjectID, ulong GroupID)
        {
            return InternalCallAddBroadcastGroup(m_ObjectHandle, ProxyObjectID, GroupID);
        }
        public bool RemoveBroadcastGroup(OBJECT_ID ProxyObjectID, ulong GroupID)
        {
            return InternalCallRemoveBroadcastGroup(m_ObjectHandle, ProxyObjectID, GroupID);
        }
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static uint InternalCallGetRouterID();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static OBJECT_ID InternalCallGetObjectID(IntPtr ObjectHandle);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static int InternalCallGetGroupIndex(IntPtr ObjectHandle);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallSendMessage(IntPtr ObjectHandle, OBJECT_ID ReceiverID, OBJECT_ID SenderID, uint MsgID, ushort MsgFlag, byte[] Data, int StartIndex, int DataLen);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallSendMessageMulti(IntPtr ObjectHandle, OBJECT_ID[] ReceiverIDList, bool IsSorted, OBJECT_ID SenderID, uint MsgID, ushort MsgFlag, byte[] Data, int StartIndex, int DataLen);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallBroadcastMessageToProxyByMask(IntPtr ObjectHandle, ushort RouterID, byte ProxyType, ulong Mask, OBJECT_ID SenderID, uint MsgID, ushort MsgFlag, byte[] Data, int StartIndex, int DataLen);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallBroadcastMessageToProxyByGroup(IntPtr ObjectHandle, ushort RouterID, byte ProxyType, ulong GroupID, OBJECT_ID SenderID, uint MsgID, ushort MsgFlag, byte[] Data, int StartIndex, int DataLen);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallRegisterMsgMap(IntPtr ObjectHandle, OBJECT_ID ProxyObjectID, uint[] MsgIDList);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallUnregisterMsgMap(IntPtr ObjectHandle, OBJECT_ID ProxyObjectID, uint[] MsgIDList);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallRegisterGlobalMsgMap(IntPtr ObjectHandle, ushort ProxyRouterID, byte ProxyType, uint MsgID, int MapType);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallUnregisterGlobalMsgMap(IntPtr ObjectHandle, ushort ProxyRouterID, byte ProxyType, uint MsgID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallSetUnhanleMsgReceiver(IntPtr ObjectHandle, ushort ProxyRouterID, byte ProxyType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallAddConcernedObject(IntPtr ObjectHandle, OBJECT_ID ObjectID, bool NeedTest);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallDeleteConcernedObject(IntPtr ObjectHandle, OBJECT_ID ObjectID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallFindObject(IntPtr ObjectHandle, uint ObjectType, bool OnlyLocal);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallReportObject(IntPtr ObjectHandle, OBJECT_ID TargetID, byte[] Data, int StartIndex, int DataLen);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallCloseProxyObject(IntPtr ObjectHandle, OBJECT_ID ProxyObjectID, uint Delay);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallRequestProxyObjectIP(IntPtr ObjectHandle, OBJECT_ID ProxyObjectID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallRegisterObject(uint PluginID, DOS_OBJ_REGISTER_INFO ObjectRegisterInfo);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallRegisterObject(IntPtr ObjectHandle, DOS_OBJ_REGISTER_INFO ObjectRegisterInfo);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void InternalCallRelease(IntPtr ObjectHandle);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallQueryShutDown(IntPtr ObjectHandle, OBJECT_ID TargetID, byte Level, uint Param);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void InternalCallShutDown(IntPtr ObjectHandle, uint PluginID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallRegisterLogger(uint LogChannel, string FileName);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallRegisterCSVLogger(uint LogChannel, string FileName, string CSVLogHeader);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallRegisterCommandReceiver(IntPtr ObjectHandle);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallUnregisterCommandReceiver(IntPtr ObjectHandle);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallSetServerWorkStatus(IntPtr ObjectHandle, byte WorkStatus);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static uint InternalCallAddTimer(IntPtr ObjectHandle, ulong TimeOut, object Param, bool IsRepea);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallDeleteTimer(IntPtr ObjectHandle, uint ID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallSetBroadcastMask(IntPtr ObjectHandle, OBJECT_ID ProxyObjectID, ulong Mask);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallAddBroadcastMask(IntPtr ObjectHandle, OBJECT_ID ProxyObjectID, ulong Mask);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallRemoveBroadcastMask(IntPtr ObjectHandle, OBJECT_ID ProxyObjectID, ulong Mask);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallAddBroadcastGroup(IntPtr ObjectHandle, OBJECT_ID ProxyObjectID, ulong GroupID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool InternalCallRemoveBroadcastGroup(IntPtr ObjectHandle, OBJECT_ID ProxyObjectID, ulong GroupID);

    };


    public class DistributedObject
    {
        public static uint MakeMsgID(uint ModuleID, uint InterfaceID, uint MethodID, bool IsAck)
        {
            return ((((uint)ModuleID & 0x7F) << 24) | (((uint)InterfaceID & 0xFF) << 16) | (((uint)MethodID) & 0xFFFF) | (IsAck ? 0x80000000 : 0));
        }

        public delegate int MsgHandler(CSmartStruct MsgPacket);

        protected DistributedObjectOperator m_Operator;
        protected OBJECT_ID m_CurMsgSenderID = new OBJECT_ID();
        protected Dictionary<uint, MsgHandler> m_MsgMap = new Dictionary<uint, MsgHandler>();
        protected virtual bool Initialize(DistributedObjectOperator Operator)
        {
            return false;
        }
        protected virtual void Destory()
        {

        }
        protected virtual int OnPreTranslateMessage(uint MsgID, ushort MsgFlag, OBJECT_ID SenderID, byte[] Data)
        {
            return (int)COMMON_RESULT_CODE.COMMON_RESULT_MSG_NO_HANDLER;
        }
        protected virtual int OnMessage(uint MsgID, ushort MsgFlag, OBJECT_ID SenderID, byte[] Data)
        {
            MsgHandler Handler;
            if (m_MsgMap.TryGetValue(MsgID, out Handler))
            {
                m_CurMsgSenderID = SenderID;
                CSmartStruct Packet = new CSmartStruct(Data, 0, (uint)Data.Length, false);
                return Handler(Packet);
            }
            return (int)COMMON_RESULT_CODE.COMMON_RESULT_MSG_NO_HANDLER;
        }
        protected virtual int OnSystemMessage(uint MsgID, ushort MsgFlag, OBJECT_ID SenderID, byte[] Data)
        {
            return (int)COMMON_RESULT_CODE.COMMON_RESULT_MSG_NO_HANDLER;
        }
        protected virtual void OnConcernedObjectLost(OBJECT_ID ObjectID)
        {

        }
        protected virtual bool OnFindObject(OBJECT_ID CallerID)
        {
            return false;
        }
        protected virtual void OnObjectReport(OBJECT_ID ObjectID, byte[] Data)
        {

        }
        protected virtual void OnProxyObjectIPReport(OBJECT_ID ProxyObjectID, uint Port, string IPStr)
        {

        }
        protected virtual void OnShutDown(byte Level, uint Param)
        {

        }
        protected virtual int Update(int ProcessPacketLimit)
        {
            return 0;
        }

        protected virtual void OnException(System.Exception Exp)
        {

        }

        protected virtual bool OnConsoleCommand(string Command)
        {
            return false;
        }

        protected virtual void OnTimer(uint ID, object Param, bool IsRepeat)
        {

        }
        protected virtual void OnTimerRelease(uint ID, object Param)
        {

        }
    };

    public class DistributedObjectPlugin
    {
        public virtual bool InitPlugin(uint PluginID, uint LogChannel, string ConfigDir, string LogDir)
        {
            return true;
        }
        public virtual bool CheckPluginRelease()
        {
            return true;
        }
    };
}