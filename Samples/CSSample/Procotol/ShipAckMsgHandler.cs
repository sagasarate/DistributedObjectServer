using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using DOSSystem;
using DOSSample;


namespace DOSSample
{
	
	public class CShipAckMsgHandler
	{
		IShipAck	m_pMsgHandler;
		public void InitMsgMap(IShipAck pMsgHandler,Dictionary<uint, DistributedObject.MsgHandler> MsgMap)
		{
			m_pMsgHandler = pMsgHandler;
			
		}

		
	};
};