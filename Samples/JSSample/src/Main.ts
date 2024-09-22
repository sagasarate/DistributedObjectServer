import { DOS_OBJ_REGISTER_INFO, OBJECT_ID, IDistributedObjectManager, IDistributedObjectOperator} from "./DOSSystem/DOSSystem";
import { Ship } from "./Ship";
import { Shipyard } from "./Shipyard";
import { CSmartStruct } from './DOSSystem/SmartStruct'
import { SpaceArea } from "./SpaceArea";
import { OBJECT_TYPES } from "./Protocol/DOSSampleSpaceShipEnums";
//插件主入口函数
function InitPlugin(ObjectManager: IDistributedObjectManager, PluginID: number, LogChannel: number, ConfigDir: string, LogDir: string): boolean {

    let Info = new DOS_OBJ_REGISTER_INFO;

    Info.ObjectID.ObjectTypeID = OBJECT_TYPES.OBJECT_TYPE_SHIPYARD;
    Info.ObjectTypeName = "Shipyard";
    Info.Weight = 100;
    Info.DOSObject = Shipyard;
    ObjectManager.RegisterObject(Info);

    //Info = new DOS_OBJ_REGISTER_INFO;

    //Info.ObjectID.ObjectTypeID = OBJECT_TYPES.OBJECT_TYPE_SPACE_AREA;
    //Info.ObjectTypeName = "SpaceArea";
    //Info.Weight = 100;
    //Info.DOSObject = SpaceArea;
    //ObjectManager.RegisterObject(Info);


    return true;
}

//插件卸载确认函数，返回true插件将被卸载
function CheckPluginRelease(): boolean {
    return true;
}


globalThis.$PLUGIN_INFO = {
    PluginID: 0,
    LogChannel: 0,
    GroupIndex: -1,
    ConfigDir: "",
    LogDir: "",
    InitPlugin: InitPlugin,
    CheckPluginRelease: CheckPluginRelease,
    ExportClasses: [OBJECT_ID, DOS_OBJ_REGISTER_INFO, CSmartStruct, SpaceArea, Shipyard, Ship]
}


