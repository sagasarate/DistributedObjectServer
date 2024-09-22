import { CDistributedObject, COMMON_RESULT_CODE, IDistributedObjectOperator, IDOSMessage, Log, LOG_LEVEL, LogDebug, LogTrace, OBJECT_ID } from "./DOSSystem/DOSSystem";
import { MSG_RESULT, SHIP_TYPE} from "./Protocol/DOSSampleSpaceShipEnums";
import { ShipInfo } from "./Protocol/ShipInfo";
import { IShip } from "./Protocol/ShipInterface";
import { CShipMsgCaller } from "./Protocol/ShipMsgCaller";
import { CShipMsgHandler } from "./Protocol/ShipMsgHandler";
import { ISpaceAreaAck } from "./Protocol/SpaceAreaAckInterface";
import { CSpaceAreaAckMsgHandler } from "./Protocol/SpaceAreaAckMsgHandler";
import { CSpaceAreaMsgCaller } from "./Protocol/SpaceAreaMsgCaller";

declare function DOSPrintLog(LogChannel: number, Level: LOG_LEVEL, Tag: string, Msg: string): boolean;
export class Ship extends CDistributedObject implements IShip, ISpaceAreaAck {
    protected m_ShipInfo = new ShipInfo;
    protected m_SpaceAreaID: OBJECT_ID;
    protected m_TargetID: OBJECT_ID;

    protected m_AttackTimerID = 0;

    protected m_ShipMsgHandler = new CShipMsgHandler;
    protected m_SpaceAreaAckMsgHandler = new CSpaceAreaAckMsgHandler;
    constructor(Type: SHIP_TYPE, Name: string, SpaceAreaID: OBJECT_ID) {
        super();
        this.m_ShipMsgHandler.InitMsgMap(this, this.m_MsgMap);
        this.m_SpaceAreaAckMsgHandler.InitMsgMap(this, this.m_MsgMap);
        this.m_ShipInfo.ShipType = Type;
        this.m_ShipInfo.Name = Name;
        this.m_SpaceAreaID = SpaceAreaID;
    }
    Initialize(Operator: IDistributedObjectOperator): boolean {
        this.m_Operator = Operator;
        this.m_ShipInfo.ShipID = this.m_Operator.GetObjectID();

        this.LogDebug(`已创建:Type=${this.m_ShipInfo.ShipType} Name=${this.m_ShipInfo.Name} SpaceAreaID=${this.m_SpaceAreaID}`);
        switch (this.m_ShipInfo.ShipType) {
            case SHIP_TYPE.SMALL:
                this.m_ShipInfo.HP = 100;
                this.m_ShipInfo.Atk = 10;
                break;
            case SHIP_TYPE.MEDIUM:
                this.m_ShipInfo.HP = 200;
                this.m_ShipInfo.Atk = 20;
                break;
            case SHIP_TYPE.LARGE:
                this.m_ShipInfo.HP = 300;
                this.m_ShipInfo.Atk = 10;
                break;
        }

        let MsgCaller = new CSpaceAreaMsgCaller(this.m_Operator, this.m_Operator.GetObjectID(), this.m_SpaceAreaID);       
        MsgCaller.RegisterShip(this.m_ShipInfo);
        return true;
    }
    Destory(): void {
        if (this.m_AttackTimerID) {
            this.m_Operator.DeleteTimer(this.m_AttackTimerID);
            this.m_AttackTimerID = 0;
        }            
        let MsgCaller = new CSpaceAreaMsgCaller(this.m_Operator, this.m_Operator.GetObjectID(), this.m_SpaceAreaID);
        MsgCaller.UnregisterShip();
    }

   
    Update(ProcessPacketLimit: number): number {
        let ProcessCount = 0;
        return ProcessCount;
    }
    protected OnConcernedObjectLost(ObjectID: OBJECT_ID): void {
        //目标消失，置空目标ID
        this.LogDebug(`对象${ObjectID}已消失`);
        if (ObjectID.ID == this.m_TargetID.ID)
            this.m_TargetID = null;
    }

    Attack(Damage: number): number {
        //受到攻击
        this.LogDebug(`受到${this.m_CurMsgSenderID}的攻击${Damage}`);
        if (this.m_ShipInfo.HP > Damage) {
            this.m_ShipInfo.HP -= Damage;
            this.LogDebug(`HP剩余${this.m_ShipInfo.HP}`);
        }
        else {
            //HP归零，销毁自身
            this.LogDebug(`HP归零，销毁`);
            this.m_ShipInfo.HP = 0;
            this.m_Operator.Release();
        }
        return COMMON_RESULT_CODE.COMMON_RESULT_SUCCEED;
    }

    RegisterShipAck(Result: number): number {
        if (Result == MSG_RESULT.MSR_SUCEED) {
            //注册成功，创建行动定时器

            this.m_AttackTimerID = this.AddTimer(3000n, true, (ID: number, IsRepeat: boolean) => {
                if (this.m_TargetID) {
                    //有目标，发起攻击
                    let MsgCaller = new CShipMsgCaller(this.m_Operator, this.m_Operator.GetObjectID(), this.m_TargetID);
                    MsgCaller.Attack(this.m_ShipInfo.Atk);
                    this.LogDebug(`向${this.m_TargetID}发起攻击${this.m_ShipInfo.Atk}`);
                }
                else {
                    //没有目标，寻找一个目标
                    let MsgCaller = new CSpaceAreaMsgCaller(this.m_Operator, this.m_Operator.GetObjectID(), this.m_SpaceAreaID);
                    MsgCaller.FindShip();
                }
            });
        }
        else {
            this.m_Operator.Release();
        }
        return COMMON_RESULT_CODE.COMMON_RESULT_SUCCEED;
    }
    
    UnregisterShipAck(Result: number): number {
        
        return COMMON_RESULT_CODE.COMMON_RESULT_SUCCEED;
    }

    FindShipAck(Result: number, ShipID: OBJECT_ID): number {
        if (Result == MSG_RESULT.MSR_SUCEED) {
            //找到目标，设置目标ID
            this.LogDebug(`找到目标：${ShipID}`);
            this.m_TargetID = ShipID;
            this.m_Operator.AddConcernedObject(this.m_TargetID, false);
        }
        return COMMON_RESULT_CODE.COMMON_RESULT_SUCCEED;
    }
    RegisterShipyardAck(Result: number): number {
        return COMMON_RESULT_CODE.COMMON_RESULT_SUCCEED;
    }

    protected LogDebug(Msg: string) {
        DOSPrintLog(globalThis.$PLUGIN_INFO.LogChannel, LOG_LEVEL.DEBUG, `Ship(${this.m_ShipInfo.ShipID})`, Msg);
    }
}