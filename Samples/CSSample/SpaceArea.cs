using DOSSystem;
using DOSSample;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSSample
{
    class SpaceArea : DistributedObject, ISpaceArea
    {
        protected List<OBJECT_ID> m_ShipyardList = new List<OBJECT_ID>();
        protected Dictionary<OBJECT_ID, ShipInfo> m_Ships = new Dictionary<OBJECT_ID, ShipInfo>();
        protected int m_ShipCreateIndex = 0;

        protected CSpaceAreaMsgHandler m_SpaceAreaMsgHandler = new CSpaceAreaMsgHandler();

        protected TickTimer m_CreateTimer = new TickTimer();
        public SpaceArea()
        {
            m_SpaceAreaMsgHandler.InitMsgMap(this, this.m_MsgMap);
        }
        protected override bool Initialize(DistributedObjectOperator Operator)
        {
            m_Operator = Operator;
            Logger.LogDebug("SpaceArea created");

            m_CreateTimer.SetTimeOut(5000);
            return true;
        }

        protected override int Update(int ProcessPacketLimit)
        {
            int ProcessCount = 0;
            if (m_CreateTimer.IsTimeOut())
            {
                m_CreateTimer.SaveTime();
                if (m_ShipyardList.Count > 0 && m_Ships.Count < 10)
                {
                    int Index = DOSUtils.GetRandom(0, m_ShipyardList.Count - 1);
                    OBJECT_ID ShipyardID = m_ShipyardList[Index];
                    Logger.LogDebug($"现有{m_Ships.Count},向{ShipyardID}请求建船");
                    CShipyardMsgCaller MsgCaller = new CShipyardMsgCaller(m_Operator, m_Operator.GetObjectID(), ShipyardID);
                    SHIP_TYPE Type = (SHIP_TYPE)DOSUtils.GetRandom((int)SHIP_TYPE.SMALL, (int)SHIP_TYPE.LARGE);
                    MsgCaller.CreateShip(Type, $"Ship{m_ShipCreateIndex++}");
                }
            }
            return ProcessCount;
        }
        protected override void OnConcernedObjectLost(OBJECT_ID ObjectID)
        {
            //目标消失，移除目标ID
            if (m_ShipyardList.RemoveAll((ID) => { return ID == ObjectID; }) > 0)
                Logger.LogDebug($"船厂{ObjectID}已移除");
        }

        public int RegisterShip(ShipInfo RegisterShipInfo)
        {
            CSpaceAreaAckMsgCaller MsgCaller = new CSpaceAreaAckMsgCaller(m_Operator, m_Operator.GetObjectID(), m_CurMsgSenderID);
            RegisterShipInfo.SetShipID(this.m_CurMsgSenderID);
            m_Ships.Add(RegisterShipInfo.GetShipID(), RegisterShipInfo);
            MsgCaller.RegisterShipAck((short)MSG_RESULT.MSR_SUCEED);
            Logger.LogDebug($"ship {RegisterShipInfo.GetShipID()} registered");
            return 0;
        }
        public int UnregisterShip()
        {
            CSpaceAreaAckMsgCaller MsgCaller = new CSpaceAreaAckMsgCaller(m_Operator, m_Operator.GetObjectID(), m_CurMsgSenderID);
            if (m_Ships.Remove(m_CurMsgSenderID))
                Logger.LogDebug($"ship {m_CurMsgSenderID} unregistered");
            MsgCaller.UnregisterShipAck((short)MSG_RESULT.MSR_SUCEED);
            return 0;
        }
        public int FindShip()
        {
            Logger.LogDebug($"{m_CurMsgSenderID}请求查找目标");
            CSpaceAreaAckMsgCaller MsgCaller = new CSpaceAreaAckMsgCaller(m_Operator, m_Operator.GetObjectID(), m_CurMsgSenderID);
            OBJECT_ID ShipID = OBJECT_ID.Empty;
            if (m_Ships.Count >= 2)
            {
                int Index = DOSUtils.GetRandom(0, m_Ships.Count - 2);
                foreach (var Ship in m_Ships.Values)
                {
                    if (Ship.GetShipID() != m_CurMsgSenderID)
                    {
                        if (Index <= 0)
                        {
                            ShipID = Ship.GetShipID();
                            break;
                        }
                        else
                        {
                            Index--;
                        }
                    }
                }
            }
            if (!ShipID.IsEmpty())
            {
                MsgCaller.FindShipAck((short)MSG_RESULT.MSR_SUCEED, ShipID);
            }
            else
            {
                MsgCaller.FindShipAck((short)MSG_RESULT.MSR_NOT_EXIST, new OBJECT_ID(0));
            }
            return 0;
        }
        public int RegisterShipyard()
        {
            if (m_ShipyardList.FindIndex((ID) => { return ID.ID == m_CurMsgSenderID.ID; }) >= 0)
            {
                Logger.LogDebug($"船厂{m_CurMsgSenderID}已经注册过了");
            }
            else
            {
                Logger.LogDebug($"收到船厂{m_CurMsgSenderID}的注册");
                m_ShipyardList.Add(m_CurMsgSenderID);
                m_Operator.AddConcernedObject(m_CurMsgSenderID, true);
            }
            return 0;
        }
    }
}
