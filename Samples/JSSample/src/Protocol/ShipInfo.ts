//<GenerateArea1Start>
import { CSmartValue, CSmartArray, CSmartStruct } from "../DOSSystem/SmartStruct";
import { OBJECT_ID } from "../DOSSystem/DOSSystem";
import { ALL_CLASS } from "./DOSSampleDataStructs";
import { DATA_OBJECT_FLAGS, GET_MODIFY_FLAG_TYPE, DOMF_ALL_FULL } from "./DOSSampleSystemEnums";
import { SHIP_TYPE } from "./DOSSampleSpaceShipEnums"
import { POS } from "./DOSSampleSpaceShipStructs"



//
//<GenerateArea1End>

//<GenerateArea2Start>

export const enum SHIP_INFO_MEMBER_IDS
{
	SST_SI_SHIP_ID = 101,
	SST_SI_SHIP_TYPE = 106,
	SST_SI_NAME = 102,
	SST_SI_POS = 103,
	SST_SI_HP = 104,
	SST_SI_ATK = 105,
	
};

export class SHIP_INFO_MEMBER_IDS_UTILS
{
	public static ENUM_COUNT: number = 6;
};

export const SHIP_INFO_MODIFY_FLAGS =
{
	MF_SHIP_ID : 1n << 0n,
	MF_SHIP_TYPE : 1n << 1n,
	MF_NAME : 1n << 2n,
	MF_POS : 1n << 3n,
	MF_HP : 1n << 4n,
	MF_ATK : 1n << 5n,
	
};

export class SHIP_INFO_MODIFY_FLAGS_UTILS
{
	public static ENUM_COUNT : number = 6;
};
//<GenerateArea2End>

export class ShipInfo
{

	protected m_ModifyFlag: bigint = 0n;
	
//<GenerateArea3Start>	   
	protected m_ShipID: OBJECT_ID = new OBJECT_ID;
	protected m_ShipType: SHIP_TYPE;
	protected m_Name: string;
	protected m_Pos: POS=new POS();
	protected m_HP: number;
	protected m_Atk: number;
	

	constructor()
	{
		this.m_ModifyFlag = 0n;
		this.m_ShipID.ID = 0n;
		this.m_ShipType=0;
		this.m_Name="";
		this.m_Pos.Clear();
		this.m_HP=0;
		this.m_Atk=0;
		
//<GenerateArea3End>
	}
	
	public Clear(): void
	{
//<GenerateArea4Start>
		this.m_ModifyFlag = 0n;
	   
		this.m_ShipID.ID = 0n;
		this.m_ShipType=0;
		this.m_Name="";
		this.m_Pos.Clear();
		this.m_HP=0;
		this.m_Atk=0;
		
//<GenerateArea4End>

	}
	 
//<GenerateArea5Start>

	public SetModifyFlag(RemoveFlags: bigint, AddFlags: bigint): void
	{
		this.m_ModifyFlag = this.m_ModifyFlag & (~RemoveFlags);
		this.m_ModifyFlag = this.m_ModifyFlag | AddFlags;
	}


	public set ShipID(Value: OBJECT_ID)
	{
		this.m_ShipID.ID = Value.ID;
		this.m_ModifyFlag = this.m_ModifyFlag | SHIP_INFO_MODIFY_FLAGS.MF_SHIP_ID;
	}
	public set ShipType(Value: SHIP_TYPE)
	{
		this.m_ShipType=Value;
		this.m_ModifyFlag = this.m_ModifyFlag | SHIP_INFO_MODIFY_FLAGS.MF_SHIP_TYPE;
	}
	public set Name(Value: string)
	{
		this.m_Name=Value;
		this.m_ModifyFlag = this.m_ModifyFlag | SHIP_INFO_MODIFY_FLAGS.MF_NAME;
	}
	public set Pos(Value: POS)
	{
		this.m_Pos=Value.Clone();
		this.m_ModifyFlag = this.m_ModifyFlag | SHIP_INFO_MODIFY_FLAGS.MF_POS;
	}
	public set HP(Value: number)
	{
		this.m_HP=Value;
		this.m_ModifyFlag = this.m_ModifyFlag | SHIP_INFO_MODIFY_FLAGS.MF_HP;
	}
	public set Atk(Value: number)
	{
		this.m_Atk=Value;
		this.m_ModifyFlag = this.m_ModifyFlag | SHIP_INFO_MODIFY_FLAGS.MF_ATK;
	}
	
			   
	public get ShipID(): OBJECT_ID
	{
		return this.m_ShipID;
	}
	public get ShipType(): SHIP_TYPE
	{
		return this.m_ShipType;
	}
	public get Name(): string
	{
		return this.m_Name;
	}
	public get Pos(): POS
	{
		return this.m_Pos;
	}
	public get HP(): number
	{
		return this.m_HP;
	}
	public get Atk(): number
	{
		return this.m_Atk;
	}
	

//<GenerateArea5End>

	public GetModifyFlag(ModifyFlags: bigint[], GetType: number, MemberFlags: bigint[]): void
	{
//<GenerateArea6Start>
		switch(GetType)
		{
		case GET_MODIFY_FLAG_TYPE.GMFT_COPY:
			ModifyFlags[DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_SHIP_INFO] = this.m_ModifyFlag & MemberFlags[DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_SHIP_INFO];
			break;
		case GET_MODIFY_FLAG_TYPE.GMFT_AND:
			ModifyFlags[DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_SHIP_INFO] = ModifyFlags[DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_SHIP_INFO] & (this.m_ModifyFlag & MemberFlags[DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_SHIP_INFO]);
			break;
		case GET_MODIFY_FLAG_TYPE.GMFT_OR:
			ModifyFlags[DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_SHIP_INFO] = ModifyFlags[DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_SHIP_INFO] | (this.m_ModifyFlag & MemberFlags[DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_SHIP_INFO]);
			break;
		}
		
//<GenerateArea6End>
	}
	
	public IsModified(MemberFlags: bigint[]): boolean
	{
		let IsModified = false;
//<GenerateArea7Start>
		
		IsModified = IsModified || ((this.m_ModifyFlag & MemberFlags[DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_SHIP_INFO]) != 0n);
		IsModified = IsModified
			;
//<GenerateArea7End>
		return IsModified;

	}
	
	public ClearModifyFlag(MemberFlags: bigint[]): void
	{
//<GenerateArea8Start>
		this.m_ModifyFlag = this.m_ModifyFlag | (~MemberFlags[DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_SHIP_INFO]);
		
//<GenerateArea8End>
	}

	public MakeUpdatePacket(Packet: CSmartStruct, MemberFlags: bigint[]): boolean
	{
//<GenerateArea9Start>
		let ModifyFlags: bigint[] = [];
		for(let i=0;i<DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_MAX;i++)
			ModifyFlags.push(0n);
		this.GetModifyFlag(ModifyFlags,GET_MODIFY_FLAG_TYPE.GMFT_OR,MemberFlags);	
//<GenerateArea9End>
		return this.MakePacket(Packet,ModifyFlags);
	}
	
	public MakePacket(Packet: CSmartStruct, MemberFlags: bigint[]): boolean
	{
//<GenerateArea10Start>
		let FailCount: number = 0;
		
		let Flag = MemberFlags[DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_SHIP_INFO];

		if((Flag & SHIP_INFO_MODIFY_FLAGS.MF_SHIP_ID) != 0n)
		{
			if(!Packet.AddMemberLong(SHIP_INFO_MEMBER_IDS.SST_SI_SHIP_ID,this.m_ShipID.ID)) FailCount++;
		}
		if((Flag & SHIP_INFO_MODIFY_FLAGS.MF_SHIP_TYPE) != 0n)
		{
			if(!Packet.AddMemberInt(SHIP_INFO_MEMBER_IDS.SST_SI_SHIP_TYPE,this.m_ShipType)) FailCount++;
		}
		if((Flag & SHIP_INFO_MODIFY_FLAGS.MF_NAME) != 0n)
		{
			if(!Packet.AddMemberString(SHIP_INFO_MEMBER_IDS.SST_SI_NAME,this.m_Name)) FailCount++;
		}
		if((Flag & SHIP_INFO_MODIFY_FLAGS.MF_POS) != 0n)
		{
			let SubPacket=Packet.PrepareSubStruct();
			if(!this.m_Pos.MakePacket(SubPacket))
				FailCount++;
			Packet.FinishMember(SHIP_INFO_MEMBER_IDS.SST_SI_POS,SubPacket.GetDataLen());
		}
		if((Flag & SHIP_INFO_MODIFY_FLAGS.MF_HP) != 0n)
		{
			if(!Packet.AddMemberUint(SHIP_INFO_MEMBER_IDS.SST_SI_HP,this.m_HP)) FailCount++;
		}
		if((Flag & SHIP_INFO_MODIFY_FLAGS.MF_ATK) != 0n)
		{
			if(!Packet.AddMemberUint(SHIP_INFO_MEMBER_IDS.SST_SI_ATK,this.m_Atk)) FailCount++;
		}
		
//<GenerateArea10End>
		return FailCount == 0;
	}
		
	public ParsePacket(Packet: CSmartStruct, MemberFlags: bigint[]): void
	{
//<GenerateArea11Start>
		
		let Flag = MemberFlags[DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_SHIP_INFO];
		let UpdateFlag = 0n;
		
		
		let Pos = Packet.GetFirstMemberPosition();
		while(Pos.Pos > 0)
		{
			let PacketMember = Packet.GetNextMember(Pos);
			switch(Pos.MemberID)
			{
			case SHIP_INFO_MEMBER_IDS.SST_SI_SHIP_ID:
				if((Flag & SHIP_INFO_MODIFY_FLAGS.MF_SHIP_ID) != 0n)
				{
					this.m_ShipID.ID = PacketMember.toLong();
					UpdateFlag = UpdateFlag | SHIP_INFO_MODIFY_FLAGS.MF_SHIP_ID;
				}
				break;
			case SHIP_INFO_MEMBER_IDS.SST_SI_SHIP_TYPE:
				if((Flag & SHIP_INFO_MODIFY_FLAGS.MF_SHIP_TYPE) != 0n)
				{
					this.m_ShipType=PacketMember.valueOf();
					UpdateFlag = UpdateFlag | SHIP_INFO_MODIFY_FLAGS.MF_SHIP_TYPE;
				}
				break;
			case SHIP_INFO_MEMBER_IDS.SST_SI_NAME:
				if((Flag & SHIP_INFO_MODIFY_FLAGS.MF_NAME) != 0n)
				{
					this.m_Name=PacketMember.toString();
					UpdateFlag = UpdateFlag | SHIP_INFO_MODIFY_FLAGS.MF_NAME;
				}
				break;
			case SHIP_INFO_MEMBER_IDS.SST_SI_POS:
				if((Flag & SHIP_INFO_MODIFY_FLAGS.MF_POS) != 0n)
				{
					this.m_Pos.ParsePacket(PacketMember.toSmartStruct());
					
					UpdateFlag = UpdateFlag | SHIP_INFO_MODIFY_FLAGS.MF_POS;
				}
				break;
			case SHIP_INFO_MEMBER_IDS.SST_SI_HP:
				if((Flag & SHIP_INFO_MODIFY_FLAGS.MF_HP) != 0n)
				{
					this.m_HP=PacketMember.valueOf();
					UpdateFlag = UpdateFlag | SHIP_INFO_MODIFY_FLAGS.MF_HP;
				}
				break;
			case SHIP_INFO_MEMBER_IDS.SST_SI_ATK:
				if((Flag & SHIP_INFO_MODIFY_FLAGS.MF_ATK) != 0n)
				{
					this.m_Atk=PacketMember.valueOf();
					UpdateFlag = UpdateFlag | SHIP_INFO_MODIFY_FLAGS.MF_ATK;
				}
				break;
			
			}
		}
		
//<GenerateArea11End>
		this.m_ModifyFlag = this.m_ModifyFlag | UpdateFlag;
	}
	
	public CloneFrom(DataObject: ShipInfo, MemberFlags: bigint[]): void
	{
	//<GenerateArea12Start>
		
		let Flag = MemberFlags[DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_SHIP_INFO];
		let UpdateFlag = 0n;
		
		if((Flag & SHIP_INFO_MODIFY_FLAGS.MF_SHIP_ID) != 0n)
		{
			if(DataObject.m_ShipID)
			{
				this.m_ShipID.ID = DataObject.m_ShipID.ID;
				UpdateFlag = UpdateFlag | SHIP_INFO_MODIFY_FLAGS.MF_SHIP_ID;
			}
		}
		if((Flag & SHIP_INFO_MODIFY_FLAGS.MF_SHIP_TYPE) != 0n)
		{
			if(DataObject.m_ShipType)
			{
				this.m_ShipType=DataObject.m_ShipType;
				UpdateFlag = UpdateFlag | SHIP_INFO_MODIFY_FLAGS.MF_SHIP_TYPE;
			}
		}
		if((Flag & SHIP_INFO_MODIFY_FLAGS.MF_NAME) != 0n)
		{
			if(DataObject.m_Name)
			{
				this.m_Name=DataObject.m_Name;
				UpdateFlag = UpdateFlag | SHIP_INFO_MODIFY_FLAGS.MF_NAME;
			}
		}
		if((Flag & SHIP_INFO_MODIFY_FLAGS.MF_POS) != 0n)
		{
			if(DataObject.m_Pos)
			{
				this.m_Pos.CloneFrom(DataObject.m_Pos);
				UpdateFlag = UpdateFlag | SHIP_INFO_MODIFY_FLAGS.MF_POS;
			}
		}
		if((Flag & SHIP_INFO_MODIFY_FLAGS.MF_HP) != 0n)
		{
			if(DataObject.m_HP)
			{
				this.m_HP=DataObject.m_HP;
				UpdateFlag = UpdateFlag | SHIP_INFO_MODIFY_FLAGS.MF_HP;
			}
		}
		if((Flag & SHIP_INFO_MODIFY_FLAGS.MF_ATK) != 0n)
		{
			if(DataObject.m_Atk)
			{
				this.m_Atk=DataObject.m_Atk;
				UpdateFlag = UpdateFlag | SHIP_INFO_MODIFY_FLAGS.MF_ATK;
			}
		}
		
		
	//<GenerateArea12End>

		this.m_ModifyFlag = this.m_ModifyFlag | UpdateFlag;
	}
	public GetSmartStructSize(MemberFlags: bigint[]): number
	{
//<GenerateArea13Start>
		let _PackSize = 0;
		let Flag = MemberFlags[DATA_OBJECT_FLAGS.DATA_OBJECT_FLAG_SHIP_INFO];
		if((Flag & SHIP_INFO_MODIFY_FLAGS.MF_SHIP_ID) != 0n)
		{
			_PackSize += CSmartStruct.GetFixMemberSize(8);
		}
		if((Flag & SHIP_INFO_MODIFY_FLAGS.MF_SHIP_TYPE) != 0n)
		{
			_PackSize += CSmartStruct.GetFixMemberSize(4);
		}
		if((Flag & SHIP_INFO_MODIFY_FLAGS.MF_NAME) != 0n)
		{
			_PackSize += CSmartStruct.GetStringMemberSize(this.m_Name);
		}
		if((Flag & SHIP_INFO_MODIFY_FLAGS.MF_POS) != 0n)
		{
			_PackSize += CSmartStruct.GetStructMemberSize(this.m_Pos.GetSmartStructSize());
		}
		if((Flag & SHIP_INFO_MODIFY_FLAGS.MF_HP) != 0n)
		{
			_PackSize += CSmartStruct.GetFixMemberSize(4);
		}
		if((Flag & SHIP_INFO_MODIFY_FLAGS.MF_ATK) != 0n)
		{
			_PackSize += CSmartStruct.GetFixMemberSize(4);
		}
		
			
//<GenerateArea13End>	
		return _PackSize;
	}
	public Clone(): any
	{
//<GenerateArea14Start>
		let DataObject = new ShipInfo();
		DataObject.CloneFrom(this, DOMF_ALL_FULL);
		return DataObject;
//<GenerateArea14End>	
	}
//<GenerateArea15Start>
//<GenerateArea15End>
};

ALL_CLASS["ShipInfo"] = ShipInfo;