using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DOSSystem;
using DOSSample;

namespace CSSample
{
    public class SampleMain
    {
        static uint m_PluginID;
        bool InitPlugin(uint PluginID, uint LogChannel, string ConfigDir, string LogDir)
        {

            m_PluginID = PluginID;
            Logger.LogChannel = LogChannel;

            {
                SpaceArea Obj = new SpaceArea();
                DOS_OBJ_REGISTER_INFO RegisterInfo = new DOS_OBJ_REGISTER_INFO();

                RegisterInfo.ObjectID.ObjectTypeID = (ushort)OBJECT_TYPES.OBJECT_TYPE_SPACE_AREA;
                RegisterInfo.Object = Obj;
                RegisterInfo.Weight = 100;
                RegisterInfo.ObjectTypeName = "SpaceArea";

                DistributedObjectOperator.RegisterObject(m_PluginID, RegisterInfo);
            }

            {
                Shipyard Obj = new Shipyard();
                DOS_OBJ_REGISTER_INFO RegisterInfo = new DOS_OBJ_REGISTER_INFO();

                RegisterInfo.ObjectID.ObjectTypeID = (ushort)OBJECT_TYPES.OBJECT_TYPE_SHIPYARD;
                RegisterInfo.Object = Obj;
                RegisterInfo.Weight = 100;
                RegisterInfo.ObjectTypeName = "Shipyard";

                DistributedObjectOperator.RegisterObject(m_PluginID, RegisterInfo);
            }

            return true;
        }
       
        public bool CheckPluginRelease()
        {
            return true;
        }
        
    };
};