using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using DOSSystem;
using DOSSample;


namespace DOSSample
{
	public class CShipAckMsgCaller :
		IShipAck
	{
		DistributedObjectOperator						m_Operator;		
		OBJECT_ID										m_SenderID;
		OBJECT_ID[]										m_TargetIDList;
		
		public CShipAckMsgCaller(DistributedObjectOperator Operator, OBJECT_ID SenderID, OBJECT_ID TargetID)
		{
			m_Operator = Operator;
			m_SenderID = SenderID;
			m_TargetIDList = new OBJECT_ID[1]{TargetID};
		}
		public CShipAckMsgCaller(DistributedObjectOperator Operator, OBJECT_ID SenderID, OBJECT_ID[] TargetIDList)
		{
			m_Operator = Operator;
			m_SenderID = SenderID;
			m_TargetIDList = TargetIDList;
		}
		
		
		
		
	};
};
