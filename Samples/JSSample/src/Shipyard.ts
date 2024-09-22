import { CDistributedObject, COMMON_RESULT_CODE, DOS_OBJ_REGISTER_INFO, DOS_OBJECT_REGISTER_FLAG, IDistributedObjectOperator, IDOSMessage, LogDebug, LogTrace, OBJECT_ID } from "./DOSSystem/DOSSystem";
import { IShipyard } from "./Protocol/ShipyardInterface";
import { OBJECT_TYPES, SHIP_TYPE } from "./Protocol/DOSSampleSpaceShipEnums"
import { CShipyardAckMsgCaller } from "./Protocol/ShipyardAckMsgCaller";
import { Ship } from "./Ship";
import { CShipyardMsgHandler } from "./Protocol/ShipyardMsgHandler";
import { CSpaceAreaMsgCaller } from "./Protocol/SpaceAreaMsgCaller";

export class Shipyard extends CDistributedObject implements IShipyard {
    protected m_SpaceAreaID: OBJECT_ID;
    protected m_ObjectFindTimer: NodeJS.Timeout;

    protected m_ShipyardMsgHandler = new CShipyardMsgHandler;

    constructor() {
        super();
        this.m_ShipyardMsgHandler.InitMsgMap(this, this.m_MsgMap);
    }

    Initialize(Operator: IDistributedObjectOperator): boolean {
        this.m_Operator = Operator;
        LogTrace("造船厂已创建");

        this.m_ObjectFindTimer = setInterval(() => {
            LogDebug("正在查找空间区域");
            this.m_Operator.FindObject(OBJECT_TYPES.OBJECT_TYPE_SPACE_AREA, false);
        }, 5000);

        return true;
    }
    Destory(): void {

    }

    
    Update(ProcessPacketLimit: number): number {
        let ProcessCount = 0;
        return ProcessCount;
    }

    protected OnObjectReport(ObjectID: OBJECT_ID, ObjectInfoData: ArrayBuffer): void {
        this.m_SpaceAreaID = ObjectID;
        LogDebug(`已找到空间区域${this.m_SpaceAreaID}`)
        clearInterval(this.m_ObjectFindTimer);
        let MsgCaller = new CSpaceAreaMsgCaller(this.m_Operator, this.m_Operator.GetObjectID(), this.m_SpaceAreaID);
        MsgCaller.RegisterShipyard();
        LogDebug(`已向空间区域${this.m_SpaceAreaID}注册船厂`);
    }
    

    CreateShip(ShipType: SHIP_TYPE, ShipName: string): number {
        LogDebug(`收到建船请求：Type=${ShipType},Name=${ShipName}`);
        let MsgCaller = new CShipyardAckMsgCaller(this.m_Operator, this.m_Operator.GetObjectID(), this.m_CurMsgSenderID);
        if (this.m_SpaceAreaID.ID) {
            let Info = new DOS_OBJ_REGISTER_INFO;
            Info.ObjectID.ObjectTypeID = OBJECT_TYPES.OBJECT_TYPE_SHIP;
            Info.ObjectTypeName = "Ship";
            Info.Flag = DOS_OBJECT_REGISTER_FLAG.DOS_OBJECT_REGISTER_FLAG_SEND_DESTORY_NOTIFY;
            Info.ObjectGroupIndex = this.m_Operator.GetObjectID().GroupIndex;
            Info.DOSObject = new Ship(ShipType, ShipName, this.m_SpaceAreaID);
            this.m_Operator.RegisterObject(Info);
            MsgCaller.CreateShipAck(COMMON_RESULT_CODE.COMMON_RESULT_SUCCEED);
        }
        else {
            MsgCaller.CreateShipAck(COMMON_RESULT_CODE.COMMON_RESULT_FAILED);
        }
        return COMMON_RESULT_CODE.COMMON_RESULT_SUCCEED;
    }
}