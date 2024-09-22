import { COMMON_RESULT_CODE, IDistributedObjectOperator, IDOSMessage, OBJECT_ID, LogDebug, LogTrace, DOS_OBJ_REGISTER_INFO, CDistributedObject, GetRandom } from "./DOSSystem/DOSSystem";
import { ShipInfo } from "./Protocol/ShipInfo";
import { ISpaceArea } from "./Protocol/SpaceAreaInterface";
import { MSG_RESULT, OBJECT_TYPES} from "./Protocol/DOSSampleSpaceShipEnums";
import { Shipyard } from "./Shipyard";
import { CShipyardMsgCaller } from "./Protocol/ShipyardMsgCaller";
import { CSpaceAreaAckMsgCaller } from "./Protocol/SpaceAreaAckMsgCaller";
import { CSpaceAreaMsgHandler } from "./Protocol/SpaceAreaMsgHandler";

export class SpaceArea extends CDistributedObject implements ISpaceArea {
    protected m_ShipyardList = new Array<OBJECT_ID>();
    protected m_Ships = new Map<bigint, ShipInfo>();
    protected m_ShipCreateIndex = 0;

    protected m_SpaceAreaMsgHandler = new CSpaceAreaMsgHandler;

    constructor() {
        super();
        this.m_SpaceAreaMsgHandler.InitMsgMap(this, this.m_MsgMap);
    }

    Initialize(Operator: IDistributedObjectOperator): boolean {
        this.m_Operator = Operator;
        console.trace("SpaceArea created");
       
        setInterval(() => {
            if (this.m_ShipyardList.length > 0 && this.m_Ships.size < 10) {
                let Index = GetRandom(0, this.m_ShipyardList.length - 1);
                let ShipyardID = this.m_ShipyardList[Index];
                LogDebug(`现有${this.m_Ships.size},向${ShipyardID}请求建船`);
                let MsgCaller = new CShipyardMsgCaller(this.m_Operator, this.m_Operator.GetObjectID(), ShipyardID);                
                MsgCaller.CreateShip((Math.random() * 10) % 3, `Ship${this.m_ShipCreateIndex++}`);
            }
        }, 5000);

        return true;
    }
    Destory(): void {

    }

    Update(ProcessPacketLimit: number): number {
        let ProcessCount = 0;


        return ProcessCount;
    }

    protected OnConcernedObjectLost(ObjectID: OBJECT_ID): void {
        //目标消失，移除目标ID
        let Index = this.m_ShipyardList.findIndex((ID) => { return ID.ID == ObjectID.ID; });
        if (Index >= 0) {
            this.m_ShipyardList.splice(Index, 1);
            LogDebug(`船厂${ObjectID}已移除`);
        }
    }

    RegisterShip(ShipInfo: ShipInfo): number {
        let MsgCaller = new CSpaceAreaAckMsgCaller(this.m_Operator, this.m_Operator.GetObjectID(), this.m_CurMsgSenderID);
        ShipInfo.ShipID = this.m_CurMsgSenderID;
        this.m_Ships.set(ShipInfo.ShipID.ID, ShipInfo);
        MsgCaller.RegisterShipAck(MSG_RESULT.MSR_SUCEED);
        LogDebug(`ship 0x${ShipInfo.ShipID} registered`);
        return COMMON_RESULT_CODE.COMMON_RESULT_SUCCEED;
    }



    UnregisterShip(): number {
        let MsgCaller = new CSpaceAreaAckMsgCaller(this.m_Operator, this.m_Operator.GetObjectID(), this.m_CurMsgSenderID);
        this.m_Ships.delete(this.m_CurMsgSenderID.ID);
        LogDebug(`ship 0x${this.m_CurMsgSenderID} unregistered`);
        MsgCaller.UnregisterShipAck(MSG_RESULT.MSR_SUCEED);
        return COMMON_RESULT_CODE.COMMON_RESULT_SUCCEED;
    }



    FindShip(): number {
        LogDebug(`${this.m_CurMsgSenderID}请求查找目标`);
        let MsgCaller = new CSpaceAreaAckMsgCaller(this.m_Operator, this.m_Operator.GetObjectID(), this.m_CurMsgSenderID);
        let ShipID: OBJECT_ID;
        if (this.m_Ships.size >= 2) {
            let Index = GetRandom(0, this.m_Ships.size - 2);
            for (let Ship of this.m_Ships.values()) {
                if (Ship.ShipID.ID != this.m_CurMsgSenderID.ID) {
                    if (Index <= 0) {
                        ShipID = Ship.ShipID;
                        break;
                    }
                    else {
                        Index--;
                    }
                }
            }
        }
        if (ShipID) {
            MsgCaller.FindShipAck(MSG_RESULT.MSR_SUCEED, ShipID);
        }
        else {
            MsgCaller.FindShipAck(MSG_RESULT.MSR_NOT_EXIST, new OBJECT_ID(0));
        }
        return COMMON_RESULT_CODE.COMMON_RESULT_SUCCEED;
    }
    RegisterShipyard(): number {
        if (this.m_ShipyardList.find((ID) => { return ID.ID == this.m_CurMsgSenderID.ID; })) {
            LogDebug(`船厂${this.m_CurMsgSenderID}已经注册过了`);
        }
        else {
            LogDebug(`收到船厂${this.m_CurMsgSenderID}的注册`);
            this.m_ShipyardList.push(this.m_CurMsgSenderID);
            this.m_Operator.AddConcernedObject(this.m_CurMsgSenderID, true);
        }
        return COMMON_RESULT_CODE.COMMON_RESULT_SUCCEED;
    }
}