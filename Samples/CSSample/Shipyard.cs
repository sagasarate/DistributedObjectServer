using DOSSystem;
using DOSSample;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSSample
{
    class Shipyard : DistributedObject, IShipyard
    {
        protected OBJECT_ID m_SpaceAreaID = OBJECT_ID.Empty;
        protected uint m_ObjectFindTimerID = 0;

        protected CShipyardMsgHandler m_ShipyardMsgHandler = new CShipyardMsgHandler();

        public Shipyard()
        {
            m_ShipyardMsgHandler.InitMsgMap(this, m_MsgMap);
        }

        protected override bool Initialize(DistributedObjectOperator Operator)
        {
            m_Operator = Operator;
            LogDebug("造船厂已创建");

            m_ObjectFindTimerID = m_Operator.AddTimer(5000, null, true);

            return true;
        }

        protected override void OnObjectReport(OBJECT_ID ObjectID, byte[] Data)
        {
            m_SpaceAreaID = ObjectID;
            LogDebug($"已找到空间区域{m_SpaceAreaID}");
            m_Operator.DeleteTimer(m_ObjectFindTimerID);
            CSpaceAreaMsgCaller MsgCaller = new CSpaceAreaMsgCaller(m_Operator, m_Operator.GetObjectID(), m_SpaceAreaID);
            MsgCaller.RegisterShipyard();
            LogDebug($"已向空间区域{m_SpaceAreaID}注册船厂");
        }
        protected override void OnTimer(uint ID, object Param, bool IsRepeat)
        {
            LogDebug("正在查找空间区域");
            m_Operator.FindObject((uint)OBJECT_TYPES.OBJECT_TYPE_SPACE_AREA, false);
        }

        protected void Log(string Msg)
        {
            Logger.InternalLog(Logger.LogChannel, LOG_LEVEL.LOG_LEVEL_NORMAL, $"Shipyard{m_Operator.GetObjectID()}", Msg);
        }
        protected void LogDebug(string Msg)
        {
            Logger.InternalLog(Logger.LogChannel, LOG_LEVEL.LOG_LEVEL_DEBUG, $"Shipyard{m_Operator.GetObjectID()}", Msg);
        }

        public int CreateShip(SHIP_TYPE ShipType, string ShipName)
        {
            LogDebug($"收到建船请求：Type ={ShipType},Name ={ShipName}");
            CShipyardAckMsgCaller MsgCaller = new CShipyardAckMsgCaller(m_Operator, m_Operator.GetObjectID(), m_CurMsgSenderID);
            if (!m_SpaceAreaID.IsEmpty())
            {
                var Info = new DOS_OBJ_REGISTER_INFO();
                Info.ObjectID.ObjectTypeID = (ushort)OBJECT_TYPES.OBJECT_TYPE_SHIP;
                Info.ObjectTypeName = "Ship";
                Info.Flag = (uint)DOS_OBJECT_REGISTER_FLAG.DOS_OBJECT_REGISTER_FLAG_SEND_DESTORY_NOTIFY;
                //Info.ObjectGroupIndex = m_Operator.GetObjectID().GroupIndex;
                Info.Object = new Ship(ShipType, ShipName, m_SpaceAreaID);
                m_Operator.RegisterObject(Info);
                MsgCaller.CreateShipAck((short)MSG_RESULT.MSR_SUCEED);
            }
            else
            {
                MsgCaller.CreateShipAck((short)MSG_RESULT.MSR_FAILED);
            }
            return 0;
        }
    }
}
