using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DOSSample;
using DOSSystem;

namespace CSSample
{
    class Ship : DistributedObject, IShip, ISpaceAreaAck
    {
        protected CShipMsgHandler m_ShipMsgHandler = new CShipMsgHandler();
        protected CSpaceAreaAckMsgHandler m_SpaceAreaAckMsgHandler = new CSpaceAreaAckMsgHandler();

        protected OBJECT_ID m_SpaceAreaID;
        protected OBJECT_ID m_TargetID = OBJECT_ID.Empty;
        protected uint m_AttackTimerID = 0;

        protected ShipInfo m_ShipInfo = new ShipInfo();

        public Ship(SHIP_TYPE Type, string Name, OBJECT_ID SpaceAreaID)
        {
            m_ShipInfo.SetShipType(Type);
            m_ShipInfo.SetName(Name);
            m_SpaceAreaID = SpaceAreaID;
        }
        protected override bool Initialize(DistributedObjectOperator Operator)
        {
            m_Operator = Operator;
            m_ShipMsgHandler.InitMsgMap(this, m_MsgMap);
            m_SpaceAreaAckMsgHandler.InitMsgMap(this, m_MsgMap);

            m_ShipInfo.SetShipID(m_Operator.GetObjectID());
            LogDebug($"已创建:Type={m_ShipInfo.GetShipType()} Name={m_ShipInfo.GetName()} SpaceAreaID={m_SpaceAreaID}");

            switch (m_ShipInfo.GetShipType())
            {
                case SHIP_TYPE.SMALL:
                    m_ShipInfo.SetHP(100);
                    m_ShipInfo.SetAtk(10);
                    break;
                case SHIP_TYPE.MEDIUM:
                    m_ShipInfo.SetHP(200);
                    m_ShipInfo.SetAtk(20);
                    break;
                case SHIP_TYPE.LARGE:
                    m_ShipInfo.SetHP(300);
                    m_ShipInfo.SetAtk(10);
                    break;
            }

            CSpaceAreaMsgCaller MsgCaller = new CSpaceAreaMsgCaller(m_Operator, m_Operator.GetObjectID(), m_SpaceAreaID);
            MsgCaller.RegisterShip(m_ShipInfo);
            return true;
        }
        protected override void OnConcernedObjectLost(OBJECT_ID ObjectID)
        {
            //目标消失，置空目标ID
            LogDebug($"对象{ObjectID}已消失");
            if (ObjectID == m_TargetID)
                m_TargetID.ID = 0;
        }
        protected override void Destory()
        {
            if (m_AttackTimerID != 0)
                m_Operator.DeleteTimer(m_AttackTimerID);
            CSpaceAreaMsgCaller MsgCaller = new CSpaceAreaMsgCaller(m_Operator, m_Operator.GetObjectID(), m_SpaceAreaID);
            MsgCaller.UnregisterShip();
        }
        protected override void OnTimer(uint ID, object Param, bool IsRepeat)
        {
            if (m_TargetID.ID != 0)
            {
                //有目标，发起攻击
                CShipMsgCaller MsgCaller = new CShipMsgCaller(m_Operator, m_Operator.GetObjectID(), m_TargetID);
                MsgCaller.Attack((int)m_ShipInfo.GetAtk());
                LogDebug($"向{m_TargetID}发起攻击{m_ShipInfo.GetAtk()}");
            }
            else
            {
                //没有目标，寻找一个目标
                CSpaceAreaMsgCaller MsgCaller = new CSpaceAreaMsgCaller(m_Operator, m_Operator.GetObjectID(), m_SpaceAreaID);
                MsgCaller.FindShip();
            }
        }


        public int Attack(int Damage)
        {
            //受到攻击
            LogDebug($"受到{m_CurMsgSenderID}的攻击{Damage}");
            if (m_ShipInfo.GetHP() > Damage)
            {
                m_ShipInfo.SetHP((uint)(m_ShipInfo.GetHP() - Damage));
                LogDebug($"HP剩余{m_ShipInfo.GetHP()}");
            }
            else
            {
                //HP归零，销毁自身
                LogDebug("HP归零，销毁");
                m_ShipInfo.SetHP(0);
                m_Operator.Release();
            }
            return 0;
        }

        public int RegisterShipAck(short Result)
        {
            if (Result == (short)MSG_RESULT.MSR_SUCEED)
            {
                //注册成功，创建行动定时器
                m_AttackTimerID = m_Operator.AddTimer(3000, null, true);
            }
            else
            {
                m_Operator.Release();
            }
            return 0;
        }
        public int UnregisterShipAck(short Result)
        {
            return 0;
        }
        public int FindShipAck(short Result, OBJECT_ID ShipID)
        {
            if (Result == (short)MSG_RESULT.MSR_SUCEED)
            {
                //找到目标，设置目标ID
                LogDebug($"找到目标：{ShipID}");
                m_TargetID = ShipID;
                m_Operator.AddConcernedObject(m_TargetID, false);
            }
            return 0;
        }
        public int RegisterShipyardAck(short Result)
        {
            return 0;
        }

        protected void Log(string Msg)
        {
            Logger.InternalLog(Logger.LogChannel, LOG_LEVEL.LOG_LEVEL_NORMAL, $"Ship{m_ShipInfo.GetShipID()}", Msg);
        }
        protected void LogDebug(string Msg)
        {
            Logger.InternalLog(Logger.LogChannel, LOG_LEVEL.LOG_LEVEL_DEBUG, $"Ship{m_ShipInfo.GetShipID()}", Msg);
        }
    }
}
