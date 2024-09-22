using System;
using System.Text;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace DOSSystem
{
    public struct OBJECT_ID
    {
        public static OBJECT_ID Empty = new OBJECT_ID();
        public ulong ID;

        public OBJECT_ID(ulong id = 0)
        {
            ID = id;
            //Logger.Log("Create OBJECT_ID=0x{0:X}", ID);
        }
        public OBJECT_ID(ushort RouterID, ushort ObjectTypeID, ushort GroupIndex, ushort ObjectIndex)
        {
            ID = (((ulong)RouterID) << 48) | (((ulong)ObjectTypeID) << 32) | (((ulong)GroupIndex) << 16) | ((ulong)ObjectIndex);
        }
        public void MakeID(ushort RouterID, ushort ObjectTypeID, ushort GroupIndex, ushort ObjectIndex)
        {
            ID = (((ulong)RouterID) << 48) | (((ulong)ObjectTypeID) << 32) | (((ulong)GroupIndex) << 16) | ((ulong)ObjectIndex);
        }
        public ushort RouterID
        {
            get { return (ushort)((ID >> 48) & 0xFFFF); }
            set { ID = (ID & ((ulong)0x0000FFFFFFFFFFFFL) | ((ulong)value) << 48); }
        }
        public ushort ObjectTypeID
        {
            get { return (ushort)((ID >> 32) & 0xFFFF); }
            set { ID = (ID & ((ulong)0xFFFF0000FFFFFFFFL) | ((ulong)value) << 32); }
        }
        public ushort GroupIndex
        {
            get { return (ushort)((ID >> 16) & 0xFFFF); }
            set { ID = (ID & ((ulong)0xFFFFFFFF0000FFFFL) | ((ulong)value) << 16); }
        }
        public ushort ObjectIndex
        {
            get { return (ushort)(ID & 0xFFFF); }
            set { ID = (ID & ((ulong)0xFFFFFFFFFFFF0000L) | ((ulong)value)); }
        }
        public override string ToString()
        {
            return string.Format("0x{0:X}", ID);
        }
        public static implicit operator ulong(OBJECT_ID ObjectID)
        {
            return ObjectID.ID;
        }
        public static implicit operator OBJECT_ID(ulong ID)
        {
            return new OBJECT_ID(ID);
        }
        public static bool operator ==(OBJECT_ID ObjectID1, OBJECT_ID ObjectID2)
        {
            return ObjectID1.ID == ObjectID2.ID;
        }
        public static bool operator !=(OBJECT_ID ObjectID1, OBJECT_ID ObjectID2)
        {
            return ObjectID1.ID != ObjectID2.ID;
        }
        public override int GetHashCode()
        {
            return ID.GetHashCode();
        }
        public override bool Equals(object Other)
        {
            if (Other is OBJECT_ID)
            {
                return ID == ((OBJECT_ID)Other).ID;
            }
            return false;
        }
        public bool IsEmpty()
        {
            return ID == 0;
        }
    }
    public enum LOG_LEVEL
    {
        LOG_LEVEL_NORMAL = (1),
        LOG_LEVEL_DEBUG = (1 << 1),
    };
    public enum COMMON_RESULT_CODE
    {
        COMMON_RESULT_SUCCEED = 0,
        COMMON_RESULT_FAILED = -2,
        COMMON_RESULT_MSG_PACK_ERROR = -3,
        COMMON_RESULT_MSG_UNPACK_ERROR = -4,
        COMMON_RESULT_MSG_ALLOC_ERROR = -5,
        COMMON_RESULT_MSG_SEND_ERROR = -6,
        COMMON_RESULT_MSG_NO_HANDLER = -7,
    };
    public enum DOS_MESSAGE_FLAG
    {
        DOS_MESSAGE_FLAG_SYSTEM_MESSAGE = 1,
        DOS_MESSAGE_FLAG_COMPRESSED = (1 << 1),
        DOS_MESSAGE_FLAG_CAN_CACHE = (1 << 2),
        DOS_MESSAGE_FLAG_ENCRYPT = (1 << 3),
        DOS_MESSAGE_FLAG_NO_COMPRESS = (1 << 4),
    };

    public enum BROAD_CAST_IDS
    {
        BROAD_CAST_ROUTER_ID = 0xFFFF,
        BROAD_CAST_OBJECT_TYPE_ID = 0xFFFF,
        BROAD_CAST_GROUP_INDEX = 0xFFFF,
        BROAD_CAST_OBJECT_INDEX = 0xFFFF,
        BROAD_CAST_PROXY_TYPE = 0xFF,
        BROAD_CAST_PROXY_ID = 0xFF,
    }

    public enum DOS_SYSTEM_MESSAGE
    {
        DSM_NONE = 0,

        DSM_PROXY_REGISTER_MSG_MAP,
        DSM_PROXY_REGISTER_MSG_MAP_RESULT,
        DSM_PROXY_UNREGISTER_MSG_MAP,
        DSM_PROXY_UNREGISTER_MSG_MAP_RESULT,
        DSM_PROXY_REGISTER_GLOBAL_MSG_MAP,
        DSM_PROXY_REGISTER_GLOBAL_MSG_MAP_RESULT,
        DSM_PROXY_UNREGISTER_GLOBAL_MSG_MAP,
        DSM_PROXY_UNREGISTER_GLOBAL_MSG_MAP_RESULT,
        DSM_PROXY_DISCONNECT,
        DSM_PROXY_KEEP_ALIVE_PING,
        DSM_PROXY_KEEP_ALIVE_PONG,
        DSM_PROXY_GET_IP,
        DSM_PROXY_IP_REPORT,
        DSM_PROXY_SET_UNHANDLE_MSG_RECEIVER,
        DSM_PROXY_SET_BROADCAST_MASK,
        DSM_PROXY_ADD_BROADCAST_MASK,
        DSM_PROXY_REMOVE_BROADCAST_MASK,
        DSM_PROXY_ADD_BROADCAST_GROUP,
        DSM_PROXY_REMOVE_BROADCAST_GROUP,
        DSM_PROXY_BROADCAST_BY_MASK,
        DSM_PROXY_BROADCAST_BY_GROUP,
        DSM_ROUTE_LINK_LOST = 100,
        DSM_ROUTE_LINK_START,
        DSM_OBJECT_ALIVE_TEST = 200,
        DSM_OBJECT_FIND,
        DSM_OBJECT_REPORT,
        DSM_OBJECT_BE_CONCERN_NOTIFY,
        DSM_OBJECT_DESTORY_NOTIFY,
        DSM_SYSTEM_SHUTDOWN = 300,
        DSM_PROTOCOL_OPTION = 400,
        DSM_MAX,
    };

    public enum DOS_OBJECT_REGISTER_FLAG
    {
        DOS_OBJECT_REGISTER_FLAG_USE_PRIVATE_OBJECT_GROUP = 1,
        DOS_OBJECT_REGISTER_FLAG_SEND_DESTORY_NOTIFY = (1 << 1),
        DOS_OBJECT_REGISTER_FLAG_BROADCAST_DESTORY_NOTIFY = (1 << 2),
    };

}
