import { CSmartValue, CSmartArray, CSmartStruct } from "../DOSSystem/SmartStruct";
import { OBJECT_ID } from "../DOSSystem/DOSSystem";
import { ALL_CLASS } from "./DOSSampleDataStructs";
import { SHIP_TYPE } from "./DOSSampleSpaceShipEnums"


//


export const enum POS_MEMBER_IDS
{
	SST_TEST_STRUCT_X = 1,
	SST_TEST_STRUCT_Y = 2,
	
};

export class POS_MEMBER_IDS_UTILS
{
	public static ENUM_COUNT: number = 2;
};

export class POS
{
	public x: number;
	public y: number;
	
	
	constructor()
	{
		this.x=0;
		this.y=0;
		
	}
	public Clear(): void
	{
		this.x=0;
		this.y=0;
		
	}
	public MakePacket(Packet: CSmartStruct): boolean
	{
		let FailCount=0;

		{
			if(!Packet.AddMemberInt(POS_MEMBER_IDS.SST_TEST_STRUCT_X,this.x)) FailCount++;
		}
		{
			if(!Packet.AddMemberInt(POS_MEMBER_IDS.SST_TEST_STRUCT_Y,this.y)) FailCount++;
		}
		

		return FailCount==0;
	}
	public ParsePacket(Packet: CSmartStruct): boolean
	{
		
		let Pos=Packet.GetFirstMemberPosition();
		while(Pos.Pos>0)
		{
			let PacketMember=Packet.GetNextMember(Pos);
			switch(Pos.MemberID)
			{
			case POS_MEMBER_IDS.SST_TEST_STRUCT_X:
				{
					this.x=PacketMember.valueOf();
				}
				break;
			case POS_MEMBER_IDS.SST_TEST_STRUCT_Y:
				{
					this.y=PacketMember.valueOf();
				}
				break;
			
			}
		}
		
		return true;
	}
	public GetSmartStructSize(): number
	{
		let _PackSize = 0;
		_PackSize += CSmartStruct.GetFixMemberSize(4);
		
		_PackSize += CSmartStruct.GetFixMemberSize(4);
		
		
		return _PackSize;
	}
	public CloneFrom(Value: POS): void
	{
		{
			if(Value.x)
			{
				this.x=Value.x;
			}
		}
		{
			if(Value.y)
			{
				this.y=Value.y;
			}
		}
		
	}
	public Clone(): POS
	{
		let CloneObject=new POS();
		CloneObject.CloneFrom(this);
		return CloneObject;
	}
};

ALL_CLASS["POS"] = POS;


