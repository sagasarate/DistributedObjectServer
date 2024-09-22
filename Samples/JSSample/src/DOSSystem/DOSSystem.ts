import { CSmartStruct } from "./SmartStruct"
export enum DOS_SYSTEM_MESSAGE {
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

export enum DOS_MESSAGE_FLAG {
    DOS_MESSAGE_FLAG_SYSTEM_MESSAGE = 1,
    DOS_MESSAGE_FLAG_COMPRESSED = (1 << 1),
    DOS_MESSAGE_FLAG_CAN_CACHE = (1 << 2),
    DOS_MESSAGE_FLAG_ENCRYPT = (1 << 3),
    DOS_MESSAGE_FLAG_NO_COMPRESS = (1 << 4),
};

export enum GLOBAL_MSG_MAP_TYPE {
    GLOBAL_MSG_MAP_TYPE_OVERLAP,
    GLOBAL_MSG_MAP_TYPE_RANDOM,
    GLOBAL_MSG_MAP_TYPE_BROADCAST,
};

export enum PROTOCOL_OPTION_FLAG {
    PROTOCOL_OPTION_FLAG_UP_MSG_USE_ENCRYPT = 1,
};

export enum COMMON_RESULT_CODE {
    COMMON_RESULT_SUCCEED = 0,
    COMMON_RESULT_FAILED = -2,
    COMMON_RESULT_MSG_PACK_ERROR = -3,
    COMMON_RESULT_MSG_UNPACK_ERROR = -4,
    COMMON_RESULT_MSG_ALLOC_ERROR = -5,
    COMMON_RESULT_MSG_SEND_ERROR = -6,
    COMMON_RESULT_MSG_NO_HANDLER = -7,
};

export enum DOS_OBJECT_REGISTER_FLAG {
    DOS_OBJECT_REGISTER_FLAG_USE_PRIVATE_OBJECT_GROUP = 1,
    DOS_OBJECT_REGISTER_FLAG_SEND_DESTORY_NOTIFY = (1 << 1),
    DOS_OBJECT_REGISTER_FLAG_BROADCAST_DESTORY_NOTIFY = (1 << 2),
};

export enum LOG_LEVEL {
    NORMAL = (1),
    DEBUG = (1 << 1),
}
export class OBJECT_ID {
    public ID: bigint = 0n;

    constructor(RouterID?: number | bigint, ObjectTypeID?: number, GroupIndex?: number, ObjectIndex?: number) {
        if (RouterID !== undefined) {
            if (typeof RouterID == "bigint") {
                this.ID = RouterID;
                return;
            }
            else {
                this.RouterID = RouterID;
            }
        }
        if (ObjectTypeID !== undefined)
            this.ObjectTypeID = ObjectTypeID;
        if (GroupIndex !== undefined)
            this.GroupIndex = GroupIndex;
        if (ObjectIndex !== undefined)
            this.ObjectIndex = ObjectIndex;
    }
    public get RouterID(): number {
        return Number((this.ID >> 48n) & 0xFFFFn);
    }
    public set RouterID(value: number) {
        this.ID = (this.ID & 0x0000FFFFFFFFFFFFn) | (BigInt(value) << 48n);
    }

    public get ObjectTypeID(): number {
        return Number((this.ID >> 32n) & 0xFFFFn);
    }
    public set ObjectTypeID(value: number) {
        this.ID = (this.ID & 0xFFFF0000FFFFFFFFn) | (BigInt(value) << 32n);
    }

    public get GroupIndex(): number {
        return Number((this.ID >> 16n) & 0xFFFFn);
    }
    public set GroupIndex(value: number) {
        this.ID = (this.ID & 0xFFFFFFFF0000FFFFn) | (BigInt(value) << 16n);
    }

    public get ObjectIndex(): number {
        return Number(this.ID & 0xFFFFn);
    }
    public set ObjectIndex(value: number) {
        this.ID = (this.ID & 0xFFFFFFFFFFFF0000n) | BigInt(value);
    }
    public toString(): string {
        return `0x${this.ID.toString(16)}`;
    }
};

export declare class IDOSMessage {
    Init(): void;
    get MsgID(): number;
    set MsgID(value: number);
    get MsgFlag(): number;
    set MsgFlag(value: number);
    get DataLength(): number;
    get Data(): ArrayBuffer;
    get DataPacket(): CSmartStruct;
    get SenderID(): OBJECT_ID;
    set SenderID(value: OBJECT_ID);

};
export declare class IDOSMessagePacket extends IDOSMessage {
    set DataLength(value: number);
    get TargetIDs(): OBJECT_ID[];
    set TargetIDs(value: OBJECT_ID[]);
    get EmptyDataPacket(): CSmartStruct;
}

export class DOS_OBJ_REGISTER_INFO {
    public ObjectID: OBJECT_ID = new OBJECT_ID();
    public ObjectTypeName: string = "";
    public Weight: number = 1;
    public ObjectGroupIndex: number = -1;
    public MsgQueueSize: number = 0;
    public MsgProcessLimit: number = 0;
    public Flag: number = 0;
    public DOSObject: object | (new () => object);

};

export interface IDistributedObjectOperator {
    GetRouterID(): number;
    GetObjectID(): OBJECT_ID;
    GetGroupIndex(): number;
    SendMessage(ReceiverID: OBJECT_ID | bigint, SenderID: OBJECT_ID | bigint, MsgID: number, MsgFlag: number, Data: ArrayBufferLike): boolean;
    SendMessageMulti(ReceiverIDList: OBJECT_ID[] | bigint[], IsSorted: boolean, SenderID: OBJECT_ID | bigint, MsgID: number, MsgFlag: number, Data: ArrayBufferLike): boolean;
    BroadcastMessageToProxyByMask(RouterID: number, ProxyType: number, Mask: bigint, SenderID: OBJECT_ID | bigint, MsgID: number, MsgFlag: number, Data: ArrayBufferLike): boolean;
    BroadcastMessageToProxyByGroup(RouterID: number, ProxyType: number, GroupID: bigint, SenderID: OBJECT_ID | bigint, MsgID: number, MsgFlag: number, Data: ArrayBufferLike): boolean;

    NewMessagePacket(DataSize: number, ReceiverCount: number): IDOSMessagePacket;
    ReleaseMessagePacket(Packet: IDOSMessagePacket): boolean;
    SendMessagePacket(Packet: IDOSMessagePacket): boolean;

    RegisterMsgMap(ProxyObjectID: OBJECT_ID | bigint, MsgIDList: number[]): boolean;
    UnregisterMsgMap(ProxyObjectID: OBJECT_ID | bigint, MsgIDList: number[]): boolean;
    RegisterGlobalMsgMap(ProxyRouterID: number, ProxyType: number, MsgID: number, MapType: number): boolean;
    UnregisterGlobalMsgMap(ProxyRouterID: number, ProxyType: number, MsgIDList: number[]): boolean;
    SetUnhanleMsgReceiver(ProxyRouterID: number, ProxyType: number): boolean;

    AddConcernedObject(ObjectID: OBJECT_ID | bigint, NeedTest: boolean): boolean;
    DeleteConcernedObject(ObjectID: OBJECT_ID | bigint): boolean;

    FindObject(ObjectType: number, OnlyLocal: boolean): boolean;
    ReportObject(TargetID: OBJECT_ID | bigint, ObjectInfoData: ArrayBufferLike): boolean;
    CloseProxyObject(ProxyObjectID: OBJECT_ID | bigint, Delay: number): boolean;
    RequestProxyObjectIP(ProxyObjectID: OBJECT_ID | bigint): boolean;

    RegisterObject(ObjectRegisterInfo: DOS_OBJ_REGISTER_INFO): boolean;
    Release(): void;

    QueryShutDown(TargetID: OBJECT_ID | bigint, Level: number, Param: number): boolean;
    ShutDown(PluginID: number): void;
    RegisterCommandReceiver(): boolean;
    UnregisterCommandReceiver(): boolean;

    RegisterLogger(LogChannel: number, FileName: string): boolean;
    RegisterCSVLogger(LogChannel: number, FileName: string, CSVLogHeader: string): boolean;

    SetServerWorkStatus(WorkStatus: number): void;
    AddTimer(Timeout: bigint, Param: any, IsRepeat: boolean): number;
    DeleteTimer(ID: number): boolean;

    SetBroadcastMask(ProxyObjectID: OBJECT_ID | bigint, Mask: bigint): boolean;
    AddBroadcastMask(ProxyObjectID: OBJECT_ID | bigint, Mask: bigint): boolean;
    RemoveBroadcastMask(ProxyObjectID: OBJECT_ID | bigint, Mask: bigint): boolean;
    AddBroadcastGroup(ProxyObjectID: OBJECT_ID | bigint, GroupID: bigint): boolean;
    RemoveBroadcastGroup(ProxyObjectID: OBJECT_ID | bigint, GroupID: bigint): boolean;
};

declare function DOSPrintLog(LogChannel: number, Level: LOG_LEVEL, Tag: string, Msg: string): boolean;

export function Log(Msg: string) {
    DOSPrintLog(globalThis.$PLUGIN_INFO.LogChannel, LOG_LEVEL.NORMAL, "", Msg);
}

export function LogDebug(Msg: string) {
    DOSPrintLog(globalThis.$PLUGIN_INFO.LogChannel, LOG_LEVEL.DEBUG, "", Msg);
}

export function LogTrace(Msg: string) {
    let err = new Error;
    err.name = Msg;
    DOSPrintLog(globalThis.$PLUGIN_INFO.LogChannel, LOG_LEVEL.NORMAL, "", err.stack);
}

export class DOS_MSG_HANDLER_INFO {
    ThisPtr: any;
    HandlerFN: (MsgPacket: CSmartStruct) => number;
}

export type DOS_MSG_MAP = Map<number, DOS_MSG_HANDLER_INFO>;
export class CDistributedObject {
    protected m_Operator: IDistributedObjectOperator;
    protected m_CurMsgSenderID: OBJECT_ID;
    protected m_MsgMap = new Map<number, DOS_MSG_HANDLER_INFO>();

    protected Initialize(Operator: IDistributedObjectOperator): boolean {
        return true;
    }
    protected Destory(): void {

    }

    protected OnPreTranslateMessage(Message: IDOSMessage): number {
        return COMMON_RESULT_CODE.COMMON_RESULT_MSG_NO_HANDLER;
    }
    protected OnMessage(Message: IDOSMessage): number {
        this.m_CurMsgSenderID = Message.SenderID;
        let Handler = this.m_MsgMap.get(Message.MsgID);
        if (Handler) {
            return Handler.HandlerFN.call(Handler.ThisPtr, Message.DataPacket);
        }
        return COMMON_RESULT_CODE.COMMON_RESULT_MSG_NO_HANDLER;
    }
    protected OnSystemMessage(Message: IDOSMessage): number {
        return COMMON_RESULT_CODE.COMMON_RESULT_MSG_NO_HANDLER;
    }
    protected OnConcernedObjectLost(ObjectID: OBJECT_ID): void {

    }
    protected OnFindObject(CallerID: OBJECT_ID): boolean {
        return false;
    }
    protected OnObjectReport(ObjectID: OBJECT_ID, ObjectInfoData: ArrayBuffer): void {

    }
    protected OnProxyObjectIPReport(ProxyObjectID: OBJECT_ID, Port: number, IPString: string): void {

    }
    protected OnShutDown(Level: number, Param: number): void {

    }
    protected Update(ProcessPacketLimit: number): number {
        let ProcessCount = 0;
        return ProcessCount;
    }
    protected OnConsoleCommand(Command: string): boolean {
        return false;
    }
    protected OnTimer(ID: number, Param: any, IsRepeat: boolean): void {
        if (typeof Param == "function") {
            Param(ID, IsRepeat);
        }
    }
    protected OnTimerRelease(ID, Param: any): void {

    }
    protected AddTimer(Timeout: bigint, IsRepeat: boolean, Callback: (ID: number, IsRepeat: boolean) => void): number {
        return this.m_Operator.AddTimer(Timeout, Callback, IsRepeat);
    }
};

export interface IDistributedObjectManager {
    RegisterObject(ObjectRegisterInfo: DOS_OBJ_REGISTER_INFO): boolean;
}

export function DOSMakeMsgID(ModuleID: number, InterfaceID: number, MethodID: number, IsAck: boolean): number {
    return (
        ((ModuleID & 0x7f) << 24) |
        ((InterfaceID & 0xff) << 16) |
        (MethodID & 0xffff) |
        (IsAck ? 0x80000000 : 0)
    ) >>> 0;
}



export function GetRandom(min: number, max: number) {
    return Math.floor(Math.random() * (max - min + 1)) + min;
}