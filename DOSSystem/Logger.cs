using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace DOSSystem
{
    public class Logger
    {
        protected static uint m_LogChannel = 0;
        public static uint LogChannel
        {
            get { return m_LogChannel; }
            set { m_LogChannel = value; }
        }
        public static void Log(string Msg)
        {
            InternalLog(m_LogChannel, LOG_LEVEL.LOG_LEVEL_DEBUG, string.Empty, Msg);
        }
        public static void LogDebug(string Msg)
        {
            InternalLog(m_LogChannel, LOG_LEVEL.LOG_LEVEL_NORMAL, string.Empty, Msg);
        }
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public void InternalLog(uint LogChannel, LOG_LEVEL LogLevel, string Tag, string Msg);
    }
}
