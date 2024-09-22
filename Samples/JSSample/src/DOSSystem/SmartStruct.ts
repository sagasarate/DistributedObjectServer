import { Encoding } from "./Encoding/Encoding"

export enum SMART_VALUE_TYPE
{
    VT_NULL,
    VT_CHAR,
    VT_UCHAR,
    VT_SHORT,
    VT_USHORT,
    VT_INT,
    VT_UINT,
    VT_BIGINT,
    VT_UBIGINT,
    VT_FLOAT,
    VT_DOUBLE,
    VT_STRING_UTF8,
    VT_STRING_UCS16,
    VT_STRUCT,
    VT_STRING_ANSI,
    VT_BINARY,
    VT_ARRAY,
    VT_BOOL,
    VT_UNKNOWN,
};

export enum STRING_CODE_PAGE
{
    STRING_CODE_PAGE_ANSI,
    STRING_CODE_PAGE_UTF8,
    STRING_CODE_PAGE_UCS16,
};

export enum SST_PAIR {
    KEY = 101,
    DATA = 102,
};

export class CSmartValue {
    static readonly NULL_DATA: Uint8Array = new Uint8Array([SMART_VALUE_TYPE.VT_NULL]);    
    public static INTERNAL_STRING_CODE_PAGE: number = STRING_CODE_PAGE.STRING_CODE_PAGE_UCS16;

    protected m_pData: Uint8Array = CSmartValue.NULL_DATA;
    protected m_StartIndex: number = 0;
    protected m_DataLen: number = CSmartValue.NULL_DATA.length;

    constructor()
    constructor(Type: SMART_VALUE_TYPE, Len: number);
    constructor(Data: Uint8Array, StartIndex: number, DataLen: number, ClearType: SMART_VALUE_TYPE);
    constructor(Data: CSmartValue);
    constructor(Data: CSmartStruct);
    constructor(Data: CSmartArray);
    constructor(Data?: any, StartIndex: number = 0, DataLen: number = 0, ClearType: SMART_VALUE_TYPE = SMART_VALUE_TYPE.VT_UNKNOWN) {
        if (Data instanceof Uint8Array) {
            this.Attach(Data, StartIndex, DataLen, ClearType);
        }
        else if (Data instanceof CSmartValue) {
            this.Attach(Data.m_pData, Data.m_StartIndex, Data.m_DataLen, SMART_VALUE_TYPE.VT_UNKNOWN);
        }
        else if (Data instanceof CSmartStruct) {
            this.Attach(Data.GetData(), Data.GetDataStartIndex(), Data.GetDataLen(), SMART_VALUE_TYPE.VT_UNKNOWN);
        }
        else if (Data instanceof CSmartArray) {
            this.Attach(Data.GetData(), Data.GetDataStartIndex(), Data.GetDataLen(), SMART_VALUE_TYPE.VT_UNKNOWN);
        }
        else if (typeof (Data) == "number") {
            this.Create(Data, StartIndex);
        }
    }

    public static get NULL(): CSmartValue {
        return new CSmartValue(CSmartValue.NULL_DATA, 0, 1, SMART_VALUE_TYPE.VT_UNKNOWN);
    }

    public Create(Type: SMART_VALUE_TYPE, Len: number): boolean {
        this.Destory();
        switch (Type) {
            case SMART_VALUE_TYPE.VT_NULL:
                this.m_pData = CSmartValue.NULL_DATA;
                this.m_StartIndex = 0;
                this.m_DataLen = CSmartValue.NULL_DATA.length;
                break;
            case SMART_VALUE_TYPE.VT_CHAR:
            case SMART_VALUE_TYPE.VT_UCHAR:
                this.m_DataLen = 2;
                this.m_pData = new Uint8Array(this.m_DataLen);
                this.m_pData[0] = Type;
                break;
            case SMART_VALUE_TYPE.VT_SHORT:
            case SMART_VALUE_TYPE.VT_USHORT:
                this.m_DataLen = 3;
                this.m_pData = new Uint8Array(this.m_DataLen);
                this.m_pData[0] = Type;
                break;
            case SMART_VALUE_TYPE.VT_INT:
            case SMART_VALUE_TYPE.VT_UINT:
                this.m_DataLen = 5;
                this.m_pData = new Uint8Array(this.m_DataLen);
                this.m_pData[0] = Type;
                break;
            case SMART_VALUE_TYPE.VT_BIGINT:
            case SMART_VALUE_TYPE.VT_UBIGINT:
                this.m_DataLen = 9;
                this.m_pData = new Uint8Array(this.m_DataLen);
                this.m_pData[0] = Type;
                break;
            case SMART_VALUE_TYPE.VT_FLOAT:
                this.m_DataLen = 5;
                this.m_pData = new Uint8Array(this.m_DataLen);
                this.m_pData[0] = Type;
                break;
            case SMART_VALUE_TYPE.VT_DOUBLE:
                this.m_DataLen = 9;
                this.m_pData = new Uint8Array(this.m_DataLen);
                this.m_pData[0] = Type;
                break;
            case SMART_VALUE_TYPE.VT_STRING_UTF8:
            case SMART_VALUE_TYPE.VT_STRING_ANSI:
                this.m_DataLen = Len + 6;
                this.m_pData = new Uint8Array(this.m_DataLen);
                this.m_pData[0] = Type;
                this.SetLength(0);
                this.m_pData[5] = 0;
                break;
            case SMART_VALUE_TYPE.VT_STRING_UCS16:
                this.m_DataLen = Len + 7;
                this.m_pData = new Uint8Array(this.m_DataLen);
                this.m_pData[0] = Type;
                this.SetLength(0);
                this.m_pData[5] = 0;
                this.m_pData[6] = 0;
                break;
            case SMART_VALUE_TYPE.VT_STRUCT:
            case SMART_VALUE_TYPE.VT_BINARY:
            case SMART_VALUE_TYPE.VT_ARRAY:
                this.m_DataLen = Len + 5;
                this.m_pData = new Uint8Array(this.m_DataLen);
                this.m_pData[0] = Type;
                this.SetLength(0);
                break;
            case SMART_VALUE_TYPE.VT_BOOL:
                this.m_DataLen = 2;
                this.m_pData = new Uint8Array(this.m_DataLen);
                this.m_pData[0] = Type;
                break;
            default:
                return false;
        }

        return true;
    }

    public Attach(Data: Uint8Array, StartIndex: number, DataLen: number, ClearType: SMART_VALUE_TYPE): boolean {
        if (!(Data instanceof Uint8Array))
            return false;

        this.Destory();
        this.m_pData = Data;
        this.m_StartIndex = StartIndex;
        this.m_DataLen = DataLen;

        let DataType: SMART_VALUE_TYPE = ClearType;
        let RealDataLen: number = 0;

        if (ClearType == SMART_VALUE_TYPE.VT_UNKNOWN) {
            DataType = this.GetValueType();
            RealDataLen = this.GetLength();
        }

        let dv: DataView;

        switch (DataType) {
            case SMART_VALUE_TYPE.VT_NULL:
                this.m_DataLen = CSmartValue.NULL_DATA.length;
                break;
            case SMART_VALUE_TYPE.VT_CHAR:
            case SMART_VALUE_TYPE.VT_UCHAR:
                this.m_DataLen = 2;
                break;
            case SMART_VALUE_TYPE.VT_SHORT:
            case SMART_VALUE_TYPE.VT_USHORT:
                this.m_DataLen = 3;
                break;
            case SMART_VALUE_TYPE.VT_INT:
            case SMART_VALUE_TYPE.VT_UINT:
                this.m_DataLen = 5;
                break;
            case SMART_VALUE_TYPE.VT_BIGINT:
            case SMART_VALUE_TYPE.VT_UBIGINT:
                this.m_DataLen = 9;
                break;
            case SMART_VALUE_TYPE.VT_FLOAT:
                this.m_DataLen = 5;
                break;
            case SMART_VALUE_TYPE.VT_DOUBLE:
                this.m_DataLen = 9;
                break;
            case SMART_VALUE_TYPE.VT_STRING_UTF8:
            case SMART_VALUE_TYPE.VT_STRING_ANSI:
                this.m_DataLen = RealDataLen + 6;
                break;
            case SMART_VALUE_TYPE.VT_STRING_UCS16:
                this.m_DataLen = RealDataLen + 7;
                break;
            case SMART_VALUE_TYPE.VT_STRUCT:
            case SMART_VALUE_TYPE.VT_BINARY:
            case SMART_VALUE_TYPE.VT_ARRAY:
                this.m_DataLen = RealDataLen + 5;
                break;
            case SMART_VALUE_TYPE.VT_BOOL:
                this.m_DataLen = 2;
                break;
            default:
                return false;
        }

        if (DataLen < this.m_DataLen) {
            this.Destory();
            return false;
        }

        this.m_DataLen = DataLen;

        if (ClearType == SMART_VALUE_TYPE.VT_UNKNOWN) {
            switch (DataType) {
                case SMART_VALUE_TYPE.VT_STRING_UTF8:
                case SMART_VALUE_TYPE.VT_STRING_ANSI:
                    dv = this.toDataView();
                    dv.setUint8(RealDataLen + 5, 0);
                    break;
                case SMART_VALUE_TYPE.VT_STRING_UCS16:
                    dv = this.toDataView();
                    dv.setUint16(RealDataLen + 5, 0, true);
                    break;
            }
        }
        else {
            switch (ClearType) {
                case SMART_VALUE_TYPE.VT_NULL:
                    this.m_pData[this.m_StartIndex] = ClearType;
                    break;
                case SMART_VALUE_TYPE.VT_CHAR:
                case SMART_VALUE_TYPE.VT_UCHAR:
                    this.m_pData[this.m_StartIndex] = ClearType;
                    this.m_pData.fill(0, this.m_StartIndex + 1, this.m_StartIndex + 2);
                    break;
                case SMART_VALUE_TYPE.VT_SHORT:
                case SMART_VALUE_TYPE.VT_USHORT:
                    this.m_pData[this.m_StartIndex] = ClearType;
                    this.m_pData.fill(0, this.m_StartIndex + 1, this.m_StartIndex + 3);
                    break;
                case SMART_VALUE_TYPE.VT_INT:
                case SMART_VALUE_TYPE.VT_UINT:
                    this.m_pData[this.m_StartIndex] = ClearType;
                    this.m_pData.fill(0, this.m_StartIndex + 1, this.m_StartIndex + 5);
                    break;
                case SMART_VALUE_TYPE.VT_BIGINT:
                case SMART_VALUE_TYPE.VT_UBIGINT:
                    this.m_pData[this.m_StartIndex] = ClearType;
                    this.m_pData.fill(0, this.m_StartIndex + 1, this.m_StartIndex + 9);
                    break;
                case SMART_VALUE_TYPE.VT_FLOAT:
                    this.m_pData[this.m_StartIndex] = ClearType;
                    this.m_pData.fill(0, this.m_StartIndex + 1, this.m_StartIndex + 5);
                    break;
                case SMART_VALUE_TYPE.VT_DOUBLE:
                    this.m_pData[this.m_StartIndex] = ClearType;
                    this.m_pData.fill(0, this.m_StartIndex + 1, this.m_StartIndex + 9);
                    break;
                case SMART_VALUE_TYPE.VT_STRING_UTF8:
                case SMART_VALUE_TYPE.VT_STRING_ANSI:
                    this.m_pData[this.m_StartIndex] = ClearType;
                    this.m_pData.fill(0, this.m_StartIndex + 1, this.m_StartIndex + 6);
                    break;
                case SMART_VALUE_TYPE.VT_STRING_UCS16:
                    this.m_pData[this.m_StartIndex] = ClearType;
                    this.m_pData.fill(0, this.m_StartIndex + 1, this.m_StartIndex + 7);
                    break;
                case SMART_VALUE_TYPE.VT_STRUCT:
                case SMART_VALUE_TYPE.VT_BINARY:
                    this.m_pData[this.m_StartIndex] = ClearType;
                    this.m_pData.fill(0, this.m_StartIndex + 1, this.m_StartIndex + 5);
                    break;
                case SMART_VALUE_TYPE.VT_BOOL:
                    this.m_pData[this.m_StartIndex] = ClearType;
                    this.m_pData.fill(0, this.m_StartIndex + 1, this.m_StartIndex + 2);
                    break;
            }
        }
        return true;
    }

    public Destory(): void {
        this.m_pData = CSmartValue.NULL_DATA;
        this.m_StartIndex = 0;
        this.m_DataLen = CSmartValue.NULL_DATA.length;
    }
    public GetValueType(): SMART_VALUE_TYPE {
        if (!(this.m_pData instanceof Uint8Array))
            return SMART_VALUE_TYPE.VT_UNKNOWN;
        return this.m_pData[this.m_StartIndex];
    }
    public GetLength(): number {
        switch (this.GetValueType()) {
            case SMART_VALUE_TYPE.VT_NULL:
                return 0;
            case SMART_VALUE_TYPE.VT_CHAR:
            case SMART_VALUE_TYPE.VT_UCHAR:
                return 1;
            case SMART_VALUE_TYPE.VT_SHORT:
            case SMART_VALUE_TYPE.VT_USHORT:
                return 2;
            case SMART_VALUE_TYPE.VT_INT:
            case SMART_VALUE_TYPE.VT_UINT:
                return 4;
            case SMART_VALUE_TYPE.VT_BIGINT:
            case SMART_VALUE_TYPE.VT_UBIGINT:
                return 8;
            case SMART_VALUE_TYPE.VT_FLOAT:
                return 4;
            case SMART_VALUE_TYPE.VT_DOUBLE:
                return 8;
            case SMART_VALUE_TYPE.VT_STRING_UTF8:
            case SMART_VALUE_TYPE.VT_STRING_ANSI:
            case SMART_VALUE_TYPE.VT_STRING_UCS16:
            case SMART_VALUE_TYPE.VT_STRUCT:
            case SMART_VALUE_TYPE.VT_BINARY:
            case SMART_VALUE_TYPE.VT_ARRAY:
                let dv = this.toDataView();
                return dv.getUint32(1, true);
            case SMART_VALUE_TYPE.VT_BOOL:
                return 1;
        }
        return 0;
    }
    public SetLength(Len: number): boolean {
        switch (this.GetValueType()) {
            case SMART_VALUE_TYPE.VT_STRING_UTF8:
            case SMART_VALUE_TYPE.VT_STRING_ANSI:
            case SMART_VALUE_TYPE.VT_STRING_UCS16:
            case SMART_VALUE_TYPE.VT_STRUCT:
            case SMART_VALUE_TYPE.VT_BINARY:
            case SMART_VALUE_TYPE.VT_ARRAY:
                let dv = this.toDataView();
                dv.setUint32(1, Len, true);
                return true;
        }
        return false;
    }

    public GetDataLen(): number {
        let dv: DataView;
        switch (this.GetValueType()) {
            case SMART_VALUE_TYPE.VT_NULL:
                return 1;
            case SMART_VALUE_TYPE.VT_CHAR:
            case SMART_VALUE_TYPE.VT_UCHAR:
                return 2;
            case SMART_VALUE_TYPE.VT_SHORT:
            case SMART_VALUE_TYPE.VT_USHORT:
                return 3;
            case SMART_VALUE_TYPE.VT_INT:
            case SMART_VALUE_TYPE.VT_UINT:
                return 5;
            case SMART_VALUE_TYPE.VT_BIGINT:
            case SMART_VALUE_TYPE.VT_UBIGINT:
                return 9;
            case SMART_VALUE_TYPE.VT_FLOAT:
                return 5;
            case SMART_VALUE_TYPE.VT_DOUBLE:
                return 9;
            case SMART_VALUE_TYPE.VT_STRING_UTF8:
            case SMART_VALUE_TYPE.VT_STRING_ANSI:
                dv = this.toDataView();
                return dv.getUint32(1, true) + 6;
            case SMART_VALUE_TYPE.VT_STRING_UCS16:
                dv = this.toDataView();
                return dv.getUint32(1, true) + 7;
            case SMART_VALUE_TYPE.VT_STRUCT:
            case SMART_VALUE_TYPE.VT_BINARY:
            case SMART_VALUE_TYPE.VT_ARRAY:
                dv = this.toDataView();
                return dv.getUint32(1, true) + 5;
            case SMART_VALUE_TYPE.VT_BOOL:
                return 2;
        }
        return 0;
    }

    public GetBufferLen(): number {
        return this.m_DataLen;
    }

    public GetData(): Uint8Array {
        return this.m_pData;
    }
    public GetDataStartIndex(): number {
        return this.m_StartIndex;
    }

    public valueOf(): number {
        let dv = this.toDataView();
        switch (this.GetValueType()) {
            case SMART_VALUE_TYPE.VT_CHAR:
                return dv.getInt8(1);
            case SMART_VALUE_TYPE.VT_UCHAR:
                return dv.getUint8(1);
            case SMART_VALUE_TYPE.VT_SHORT:
                return dv.getInt16(1, true);
            case SMART_VALUE_TYPE.VT_USHORT:
                return dv.getUint16(1, true);
            case SMART_VALUE_TYPE.VT_INT:
                return dv.getInt32(1, true);
            case SMART_VALUE_TYPE.VT_UINT:
                return dv.getUint32(1, true);
            case SMART_VALUE_TYPE.VT_BIGINT:
                return dv.getUint32(1, true);
            case SMART_VALUE_TYPE.VT_UBIGINT:
                return dv.getUint32(1, true);
            case SMART_VALUE_TYPE.VT_FLOAT:
                return dv.getFloat32(1, true);
            case SMART_VALUE_TYPE.VT_DOUBLE:
                return dv.getFloat64(1, true);
            case SMART_VALUE_TYPE.VT_BOOL:
                return dv.getInt8(1);
        }
        return 0;
    }
    public toLong(): bigint {
        let dv = this.toDataView();
        switch (this.GetValueType()) {
            case SMART_VALUE_TYPE.VT_CHAR:
                return BigInt(dv.getInt8(1));
            case SMART_VALUE_TYPE.VT_UCHAR:
                return BigInt(dv.getUint8(1));
            case SMART_VALUE_TYPE.VT_SHORT:
                return BigInt(dv.getInt16(1, true));
            case SMART_VALUE_TYPE.VT_USHORT:
                return BigInt(dv.getUint16(1, true));
            case SMART_VALUE_TYPE.VT_INT:
                return BigInt(dv.getInt32(1, true));
            case SMART_VALUE_TYPE.VT_UINT:
                return BigInt(dv.getUint32(1, true));
            case SMART_VALUE_TYPE.VT_BIGINT:
                return dv.getBigInt64(1, true);
            case SMART_VALUE_TYPE.VT_UBIGINT:
                return dv.getBigUint64(1, true);
            case SMART_VALUE_TYPE.VT_FLOAT:
                return BigInt(dv.getFloat32(1, true));
            case SMART_VALUE_TYPE.VT_DOUBLE:
                return BigInt(dv.getFloat64(1, true));
            case SMART_VALUE_TYPE.VT_BOOL:
                return BigInt(dv.getInt8(1));
        }
        return 0n;
    }
    public toBoolean(): boolean {
        let dv = this.toDataView();
        switch (this.GetValueType()) {
            case SMART_VALUE_TYPE.VT_CHAR:
                return dv.getInt8(1) != 0;
            case SMART_VALUE_TYPE.VT_UCHAR:
                return dv.getUint8(1) != 0;
            case SMART_VALUE_TYPE.VT_SHORT:
                return dv.getInt16(1, true) != 0;
            case SMART_VALUE_TYPE.VT_USHORT:
                return dv.getUint16(1, true) != 0;
            case SMART_VALUE_TYPE.VT_INT:
                return dv.getInt32(1, true) != 0;
            case SMART_VALUE_TYPE.VT_UINT:
                return dv.getUint32(1, true) != 0;
            case SMART_VALUE_TYPE.VT_BIGINT:
                return (dv.getUint32(1, true) != 0) && (dv.getUint32(5, true) != 0);
            case SMART_VALUE_TYPE.VT_UBIGINT:
                return (dv.getUint32(1, true) != 0) && (dv.getUint32(5, true) != 0);
            case SMART_VALUE_TYPE.VT_FLOAT:
                return dv.getFloat32(1, true) != 0;
            case SMART_VALUE_TYPE.VT_DOUBLE:
                return dv.getFloat64(1, true) != 0;
            case SMART_VALUE_TYPE.VT_BOOL:
                return dv.getInt8(1) != 0;
        }
        return false;
    }
       
    public toString(): string {
        switch (this.GetValueType()) {
            case SMART_VALUE_TYPE.VT_NULL:
                return "NULL";
            case SMART_VALUE_TYPE.VT_CHAR:
            case SMART_VALUE_TYPE.VT_UCHAR:
            case SMART_VALUE_TYPE.VT_SHORT:
            case SMART_VALUE_TYPE.VT_USHORT:
            case SMART_VALUE_TYPE.VT_INT:
            case SMART_VALUE_TYPE.VT_UINT:            
            case SMART_VALUE_TYPE.VT_FLOAT:
            case SMART_VALUE_TYPE.VT_DOUBLE:
                return this.valueOf().toString();                
            case SMART_VALUE_TYPE.VT_BIGINT:
            case SMART_VALUE_TYPE.VT_UBIGINT:
                return this.toLong().toString();
            case SMART_VALUE_TYPE.VT_STRING_ANSI:
                return Encoding.GBK.GetString(this.m_pData, this.m_StartIndex + 5, this.GetLength());
            case SMART_VALUE_TYPE.VT_STRING_UTF8:
                return Encoding.UTF8.GetString(this.m_pData, this.m_StartIndex + 5, this.GetLength());
            case SMART_VALUE_TYPE.VT_STRING_UCS16:
                return Encoding.UCS2.GetString(this.m_pData, this.m_StartIndex + 5, this.GetLength());
            case SMART_VALUE_TYPE.VT_STRUCT:
                return "SmartStruct";
            case SMART_VALUE_TYPE.VT_BINARY:
                return this.toBinary().toString();
            case SMART_VALUE_TYPE.VT_ARRAY:
                return "SmartArray";
            case SMART_VALUE_TYPE.VT_BOOL:
                return this.toBoolean()?"true":"false";
            default:
                return "Unknow";
        }
        return null;
    }
    public toBinary(): Uint8Array {
        switch (this.GetValueType()) {
            case SMART_VALUE_TYPE.VT_STRING_ANSI:
            case SMART_VALUE_TYPE.VT_STRING_UTF8:
            case SMART_VALUE_TYPE.VT_STRING_UCS16:
            case SMART_VALUE_TYPE.VT_BINARY:
                return this.m_pData.subarray(this.m_StartIndex + 5, this.m_StartIndex + 5 + this.GetLength());
        }
        return null;
    }
    public toSmartStruct(): CSmartStruct {
        return new CSmartStruct(this);
    }

    public toSmartArray(): CSmartArray {
        return new CSmartArray(this);
    }
    public toDataView(): DataView {
        return new DataView(this.m_pData.buffer, this.m_pData.byteOffset + this.m_StartIndex, this.m_DataLen);
    }
    public toAny(): any {
        switch (this.GetValueType()) {
            case SMART_VALUE_TYPE.VT_CHAR:
            case SMART_VALUE_TYPE.VT_UCHAR:
            case SMART_VALUE_TYPE.VT_SHORT:
            case SMART_VALUE_TYPE.VT_USHORT:
            case SMART_VALUE_TYPE.VT_INT:
            case SMART_VALUE_TYPE.VT_UINT:
            case SMART_VALUE_TYPE.VT_FLOAT:
            case SMART_VALUE_TYPE.VT_DOUBLE:
                return this.valueOf();
            case SMART_VALUE_TYPE.VT_BIGINT:
            case SMART_VALUE_TYPE.VT_UBIGINT:
                return this.toLong();
            case SMART_VALUE_TYPE.VT_STRING_ANSI:
                return Encoding.GBK.GetString(this.m_pData, this.m_StartIndex + 5, this.GetLength());
            case SMART_VALUE_TYPE.VT_STRING_UTF8:
                return Encoding.UTF8.GetString(this.m_pData, this.m_StartIndex + 5, this.GetLength());
            case SMART_VALUE_TYPE.VT_STRING_UCS16:
                return Encoding.UCS2.GetString(this.m_pData, this.m_StartIndex + 5, this.GetLength());
            case SMART_VALUE_TYPE.VT_STRUCT:
                {
                    let Struct = this.toSmartStruct();
                    let Value = {};
                    let Pos = Struct.GetFirstMemberPosition();
                    while (Pos.Pos > 0) {
                        let Member = Struct.GetNextMember(Pos).toSmartStruct();
                        if (Member.IsMemberExist(SST_PAIR.KEY) && Member.IsMemberExist(SST_PAIR.DATA)) {
                            let key = Member.GetMember(SST_PAIR.KEY).toAny();
                            let data = Member.GetMember(SST_PAIR.DATA).toAny();
                            Value[key] = data;
                        }
                    }
                    return Value;
                }
                break;
            case SMART_VALUE_TYPE.VT_BINARY:
                return this.toBinary();
            case SMART_VALUE_TYPE.VT_ARRAY:
                {
                    let Array = new CSmartArray(this);
                    let Value = [];
                    let Pos = Array.GetFirstMemberPosition();
                    while (Pos.Pos > 0) {
                        let Member = Array.GetNextMember(Pos);
                        Value.push(Member.toAny());
                    }
                    return Value;
                }
                break;
            case SMART_VALUE_TYPE.VT_BOOL:
                return this.toBoolean();
            default:
                return null;
        }
    }
    public static fromSbyte(Value: number): CSmartValue {
        let SmartValue = new CSmartValue(SMART_VALUE_TYPE.VT_CHAR, 0);
        SmartValue.SetValue(Value);
        return SmartValue;
    }
    public static fromByte(Value: number): CSmartValue {
        let SmartValue = new CSmartValue(SMART_VALUE_TYPE.VT_UCHAR, 0);
        SmartValue.SetValue(Value);
        return SmartValue;
    }
    public static fromShort(Value: number): CSmartValue {
        let SmartValue = new CSmartValue(SMART_VALUE_TYPE.VT_SHORT, 0);
        SmartValue.SetValue(Value);
        return SmartValue;
    }
    public static fromUshort(Value: number): CSmartValue {
        let SmartValue = new CSmartValue(SMART_VALUE_TYPE.VT_USHORT, 0);
        SmartValue.SetValue(Value);
        return SmartValue;
    }
    public static fromInt(Value: number): CSmartValue {
        let SmartValue = new CSmartValue(SMART_VALUE_TYPE.VT_INT, 0);
        SmartValue.SetValue(Value);
        return SmartValue;
    }
    public static fromUint(Value: number): CSmartValue {
        let SmartValue = new CSmartValue(SMART_VALUE_TYPE.VT_UINT, 0);
        SmartValue.SetValue(Value);
        return SmartValue;
    }
    public static fromLong(Value: bigint): CSmartValue {
        let SmartValue = new CSmartValue(SMART_VALUE_TYPE.VT_BIGINT, 0);
        SmartValue.SetValue(Value);
        return SmartValue;
    }
    public static fromFloat(Value: number): CSmartValue {
        let SmartValue = new CSmartValue(SMART_VALUE_TYPE.VT_FLOAT, 0);
        SmartValue.SetValue(Value);
        return SmartValue;
    }
    public static fromDouble(Value: number): CSmartValue {
        let SmartValue = new CSmartValue(SMART_VALUE_TYPE.VT_DOUBLE, 0);
        SmartValue.SetValue(Value);
        return SmartValue;
    }
    public static fromString(Value: string): CSmartValue {
        let SmartValue: CSmartValue;
        let BufferLen: number;
        switch (CSmartValue.INTERNAL_STRING_CODE_PAGE) {
            case STRING_CODE_PAGE.STRING_CODE_PAGE_ANSI:
                BufferLen = Encoding.GBK.GetBytes(Value);
                SmartValue = new CSmartValue(SMART_VALUE_TYPE.VT_STRING_ANSI, BufferLen);
                if (SmartValue.m_DataLen >= BufferLen + 6) {
                    Encoding.GBK.GetBytes(Value, SmartValue.m_pData, SmartValue.m_StartIndex + 5, SmartValue.m_DataLen - 6);
                    SmartValue.SetLength(BufferLen);
                }
                break;
            case STRING_CODE_PAGE.STRING_CODE_PAGE_UTF8:
                BufferLen = Encoding.UTF8.GetBytes(Value);
                SmartValue = new CSmartValue(SMART_VALUE_TYPE.VT_STRING_UTF8, BufferLen);
                if (SmartValue.m_DataLen >= BufferLen + 6) {
                    Encoding.UTF8.GetBytes(Value, SmartValue.m_pData, SmartValue.m_StartIndex + 5, SmartValue.m_DataLen - 6);
                    SmartValue.SetLength(BufferLen);
                }
                break;
            case STRING_CODE_PAGE.STRING_CODE_PAGE_UCS16:
                BufferLen = Encoding.UCS2.GetBytes(Value);
                SmartValue = new CSmartValue(SMART_VALUE_TYPE.VT_STRING_UCS16, BufferLen);
                if (SmartValue.m_DataLen >= BufferLen + 7) {
                    Encoding.UCS2.GetBytes(Value, SmartValue.m_pData, SmartValue.m_StartIndex + 5, SmartValue.m_DataLen - 7);
                    SmartValue.SetLength(BufferLen);
                }
                break;
        }
        return SmartValue;
    }
    public static fromBinary(Value: Uint8Array): CSmartValue {
        let SmartValue: CSmartValue;

        SmartValue = new CSmartValue(SMART_VALUE_TYPE.VT_BINARY, Value.length);
        if (SmartValue.m_pData.length < Value.length + 5)
            Value = Value.subarray(0, SmartValue.m_pData.length - 5);
        SmartValue.m_pData.set(Value, 5);
                
        return SmartValue;
    }
    public static fromBoolean(Value: boolean): CSmartValue {
        let SmartValue = new CSmartValue(SMART_VALUE_TYPE.VT_CHAR, 0);
        SmartValue.SetValue(Value);
        return SmartValue;
    }
    public static fromSmartStruct(Value: CSmartStruct): CSmartValue {
        return new CSmartValue(Value);
    }
    public static fromSmartArray(Value: CSmartArray): CSmartValue {
        return new CSmartValue(Value);
    }
    public static fromAny(Value: any): CSmartValue {
        if (Value == null || Value == undefined) {
            return CSmartValue.NULL;
        }
        else if (typeof Value == "boolean") {
            return CSmartValue.fromBoolean(Value);
        }
        else if (typeof Value == "number") {
            return CSmartValue.fromDouble(Value);
        }
        else if (typeof Value == "bigint") {
            return CSmartValue.fromLong(Value);
        }
        else if (typeof Value == "string") {
            return CSmartValue.fromString(Value);
        }
        else if (typeof Value == "object") {
            if (Value instanceof Uint8Array) {
                return CSmartValue.fromBinary(Value);
            }
            else if (Value instanceof CSmartValue) {
                return new CSmartValue(Value);
            }
            else if (Value instanceof CSmartStruct) {
                return new CSmartValue(Value);
            }
            else if (Value instanceof CSmartArray) {
                return new CSmartValue(Value);
            } 
            else if (Value instanceof Array) {
                let NeedSize = CSmartValue.GetValueSize(Value);
                let SmartValue = new CSmartValue(SMART_VALUE_TYPE.VT_ARRAY, NeedSize);
                let SmartArray = new CSmartArray(SmartValue);
                for (let Child of Value) {
                    if (!SmartArray.AddMember(Child))
                        return null;
                }
                return SmartValue;
            }
            else {
                let NeedSize = CSmartValue.GetValueSize(Value);
                let SmartValue = new CSmartValue(SMART_VALUE_TYPE.VT_STRUCT, NeedSize);
                let SmartStruct = new CSmartStruct(SmartValue);
                for (const [key, value] of Object.entries(Value)) {
                    let SubStruct = SmartStruct.PrepareSubStruct();
                    if (!SubStruct.AddMember(SST_PAIR.KEY, key))
                        return null;
                    if (!SubStruct.AddMember(SST_PAIR.DATA, value))
                        return null;
                    if (!SmartStruct.FinishMember(1, SubStruct.GetDataLen()))
                        return null;
                }
                return SmartValue;
            }            
        }
        else {
            return CSmartValue.NULL;
        }
    }
    public get IsNull(): boolean {
        return this.GetValueType() == SMART_VALUE_TYPE.VT_NULL;
    }
    public SetValue(Value: any): boolean {
        if (this.m_pData instanceof Uint8Array) {
            if (typeof (Value) == "number") {
                let dv = this.toDataView();
                switch (this.GetValueType()) {
                    case SMART_VALUE_TYPE.VT_CHAR:
                        dv.setInt8(1, Value);
                        return true;
                    case SMART_VALUE_TYPE.VT_UCHAR:
                        dv.setUint8(1, Value);
                        return true;
                    case SMART_VALUE_TYPE.VT_SHORT:
                        dv.setInt16(1, Value, true);
                        return true;
                    case SMART_VALUE_TYPE.VT_USHORT:
                        dv.setUint16(1, Value, true);
                        return true;
                    case SMART_VALUE_TYPE.VT_INT:
                        dv.setInt32(1, Value, true);
                        return true;
                    case SMART_VALUE_TYPE.VT_UINT:
                        dv.setUint32(1, Value, true);
                        return true;
                    case SMART_VALUE_TYPE.VT_BIGINT:
                        dv.setUint32(1, Value, true);
                        dv.setUint32(5, 0, true);
                        return true;
                    case SMART_VALUE_TYPE.VT_UBIGINT:
                        dv.setUint32(1, Value, true);
                        dv.setUint32(5, 0, true);
                        return true;
                    case SMART_VALUE_TYPE.VT_FLOAT:
                        dv.setFloat32(1, Value, true);
                        return true;
                    case SMART_VALUE_TYPE.VT_DOUBLE:
                        dv.setFloat64(1, Value, true);
                        return true;
                    case SMART_VALUE_TYPE.VT_BOOL:
                        dv.setInt8(1, Value ? 1 : 0);
                        return true;
                }
            }
            else if (typeof (Value) == "string") {
                let BufferLen: number;
                switch (this.GetValueType()) {
                    case SMART_VALUE_TYPE.VT_STRING_ANSI:
                        BufferLen = Encoding.GBK.GetBytes(Value);
                        if (this.m_DataLen >= BufferLen + 6) {
                            Encoding.GBK.GetBytes(Value, this.m_pData, this.m_StartIndex + 5, this.m_DataLen - 6);
                            this.SetLength(BufferLen);
                            this.m_pData[this.m_StartIndex + 5 + BufferLen] = 0;
                            return true;
                        }
                        break;
                    case SMART_VALUE_TYPE.VT_STRING_UTF8:
                        BufferLen = Encoding.UTF8.GetBytes(Value);
                        if (this.m_DataLen >= BufferLen + 6) {
                            Encoding.UTF8.GetBytes(Value, this.m_pData, this.m_StartIndex + 5, this.m_DataLen - 6);
                            this.SetLength(BufferLen);
                            this.m_pData[this.m_StartIndex + 5 + BufferLen] = 0;
                            return true
                        }
                        break;
                    case SMART_VALUE_TYPE.VT_STRING_UCS16:
                        BufferLen = Encoding.UCS2.GetBytes(Value);
                        if (this.m_DataLen >= BufferLen + 7) {
                            Encoding.UCS2.GetBytes(Value, this.m_pData, this.m_StartIndex + 5, this.m_DataLen - 7);
                            this.SetLength(BufferLen);
                            this.m_pData[this.m_StartIndex + 5 + BufferLen] = 0;
                            this.m_pData[this.m_StartIndex + 5 + BufferLen + 1] = 0;
                            return true
                        }
                        break;
                }
            }
            else if (typeof (Value) == "boolean") {
                let dv = this.toDataView();
                switch (this.GetValueType()) {
                    case SMART_VALUE_TYPE.VT_CHAR:
                        dv.setInt8(1, Value ? 1 : 0);
                        return true;
                    case SMART_VALUE_TYPE.VT_UCHAR:
                        dv.setUint8(1, Value ? 1 : 0);
                        return true;
                    case SMART_VALUE_TYPE.VT_SHORT:
                        dv.setInt16(1, Value ? 1 : 0, true);
                        return true;
                    case SMART_VALUE_TYPE.VT_USHORT:
                        dv.setUint16(1, Value ? 1 : 0, true);
                        return true;
                    case SMART_VALUE_TYPE.VT_INT:
                        dv.setInt32(1, Value ? 1 : 0, true);
                        return true;
                    case SMART_VALUE_TYPE.VT_UINT:
                        dv.setUint32(1, Value ? 1 : 0, true);
                        return true;
                    case SMART_VALUE_TYPE.VT_BIGINT:
                        dv.setUint32(1, Value ? 1 : 0, true);
                        dv.setUint32(5, 0, true);
                        return true;
                    case SMART_VALUE_TYPE.VT_UBIGINT:
                        dv.setUint32(1, Value ? 1 : 0, true);
                        dv.setUint32(5, 0, true);
                        return true;
                    case SMART_VALUE_TYPE.VT_FLOAT:
                        dv.setFloat32(1, Value ? 1 : 0, true);
                        return true;
                    case SMART_VALUE_TYPE.VT_DOUBLE:
                        dv.setFloat64(1, Value ? 1 : 0, true);
                        return true;
                    case SMART_VALUE_TYPE.VT_BOOL:
                        dv.setInt8(1, Value ? 1 : 0);
                        return true;
                }
            }
            else if (typeof (Value) == "bigint") {
                let dv = this.toDataView();
                switch (this.GetValueType()) {
                    case SMART_VALUE_TYPE.VT_CHAR:
                        dv.setInt8(1, Number(Value));
                        return true;
                    case SMART_VALUE_TYPE.VT_UCHAR:
                        dv.setUint8(1, Number(Value));
                        return true;
                    case SMART_VALUE_TYPE.VT_SHORT:
                        dv.setInt16(1, Number(Value), true);
                        return true;
                    case SMART_VALUE_TYPE.VT_USHORT:
                        dv.setUint16(1, Number(Value), true);
                        return true;
                    case SMART_VALUE_TYPE.VT_INT:
                        dv.setInt32(1, Number(Value), true);
                        return true;
                    case SMART_VALUE_TYPE.VT_UINT:
                        dv.setUint32(1, Number(Value), true);
                        return true;
                    case SMART_VALUE_TYPE.VT_BIGINT:
                        dv.setBigInt64(1, Value, true);
                        return true;
                    case SMART_VALUE_TYPE.VT_UBIGINT:
                        dv.setBigUint64(1, Value, true);
                        return true;
                    case SMART_VALUE_TYPE.VT_FLOAT:
                        dv.setFloat32(1, Number(Value), true);
                        return true;
                    case SMART_VALUE_TYPE.VT_DOUBLE:
                        dv.setFloat64(1, Number(Value), true);
                        return true;
                    case SMART_VALUE_TYPE.VT_BOOL:
                        dv.setInt8(1, Value ? 1 : 0);
                        return true;
                }
            }
            else if (Value instanceof Uint8Array) {
                switch (this.GetValueType()) {
                    case SMART_VALUE_TYPE.VT_BINARY:
                        if (this.m_pData.length < Value.length + 5) {
                            Value = Value.subarray(0, this.m_pData.length - 5);
                            this.m_pData.set(Value, 5);
                            return true;
                        }
                        break;
                }
            }
            else if (Value instanceof CSmartValue) {
                if (Value.m_DataLen > 0) {
                    if (this.m_DataLen >= Value.m_DataLen) {
                        this.m_pData.set(Value.m_pData.subarray(Value.m_StartIndex, Value.m_StartIndex + Value.m_DataLen), this.m_StartIndex);
                        return true;
                    }
                }
                else {
                    return true;
                }
            }
            else if (Value instanceof CSmartStruct) {
                if (Value.GetDataLen() > 0) {
                    if (this.GetValueType() == SMART_VALUE_TYPE.VT_STRUCT) {
                        if (this.m_DataLen >= Value.GetDataLen()) {
                            this.m_pData.set(Value.GetData().subarray(Value.GetDataStartIndex(), Value.GetDataStartIndex() + Value.GetDataLen()), this.m_StartIndex);
                            return true;
                        }
                    }
                }
                else {
                    return true;
                }
            }
            else if (Value instanceof CSmartArray) {
                if (Value.GetDataLen() > 0) {
                    if (this.GetValueType() == SMART_VALUE_TYPE.VT_ARRAY) {
                        if (this.m_DataLen >= Value.GetDataLen()) {
                            this.m_pData.set(Value.GetData().subarray(Value.GetDataStartIndex(), Value.GetDataStartIndex() + Value.GetDataLen()), this.m_StartIndex);
                            return true;
                        }
                    }
                }
                else {
                    return true;
                }
            }
            else if (Value instanceof Array) {
                let Size = CSmartValue.GetValueSize(Value);
                if (Size + CSmartArray.HEAD_SIZE <= this.GetBufferLen()) {
                    let Array = new CSmartArray(this);
                    Array.Clear();
                    for (let Child of Value) {
                        if (!Array.AddMember(Child))
                            return false;
                    }
                    return true;
                }
            }
            else {
                let Size = CSmartValue.GetValueSize(Value);
                if (Size + CSmartStruct.HEAD_SIZE <= this.GetBufferLen()) {
                    let Struct = new CSmartStruct(this);
                    Struct.Clear();
                    for (const [key, value] of Object.entries(Value)) {
                        let SubStruct = Struct.PrepareSubStruct();
                        if (!SubStruct.AddMember(SST_PAIR.KEY, key))
                            return false;
                        if (!SubStruct.AddMember(SST_PAIR.DATA, value))
                            return false;
                        if (!Struct.FinishMember(1, SubStruct.GetDataLen()))
                            return false;
                    }                    
                    return true;
                }
            }
        }
        return false;
    }
    public static GetValueSize(Value: any): number {
        switch (typeof (Value)) {
            case "number":
                return 8;
            case "string":
                return Value.length;
            case "boolean":
                return 1;
            case "bigint":
                return 8;
            case "object":
                if (Value instanceof Uint8Array) {
                    return Value.length;
                }
                else if (Value instanceof CSmartValue) {
                    return Value.GetLength();
                }
                else if (Value instanceof CSmartArray) {
                    return Value.GetLength();
                }
                else if (Value instanceof CSmartStruct) {
                    return Value.GetLength();
                }
                else if (Value instanceof Array) {
                    let Size = 0;
                    for (let Child of Value) {
                        Size += CSmartArray.GetVariedMemberSize(Child);
                    }
                    return Size;
                }
                else {
                    let Size = 0;
                    for (const [key, value] of Object.entries(Value)) {
                        let SubSize = CSmartStruct.GetVariedMemberSize(key) + CSmartStruct.GetVariedMemberSize(value);
                        Size += CSmartStruct.GetStructMemberSize(SubSize);
                    }
                    return Size;
                }
                break;
        }
        return 0;
    }
}
export class CSmartStruct {
    protected m_pData: Uint8Array;
    protected m_StartIndex: number = 0;
    protected m_DataLen: number = 0;
    public static readonly HEAD_SIZE: number = 5;
    public static readonly INVALID_MEMBER_ID: number = 0xFFFF;

    constructor()
    constructor(BufferLen: number);
    constructor(Struct: CSmartValue);
    constructor(Data: Uint8Array, StartIndex: number, DataLen: number, IsEmpty: boolean)
    constructor(Data?: any, StartIndex: number = 0, DataLen: number = 0, IsEmpty: boolean = true) {        
        if (Data instanceof Uint8Array) {
            this.Attach(Data, StartIndex, DataLen, IsEmpty);
        }
        else if (Data instanceof ArrayBuffer) {
            this.Attach(new Uint8Array(Data), StartIndex, DataLen, IsEmpty);
        }
        else if (Data instanceof CSmartValue) {
            this.Attach(Data.GetData(), Data.GetDataStartIndex(), Data.GetDataLen(), false);
        }
        else if (typeof (Data) == "number") {
            this.Create(Data);
        }
    }
    public Destory(): void {
        this.m_pData = null;
        this.m_StartIndex = 0;
        this.m_DataLen = 0;
    }

    public Clear(): void {
        this.SetLength(0);
    }

    public Create(BufferLen: number): boolean {
        this.Destory();
        this.m_StartIndex = 0;
        this.m_DataLen = BufferLen + 5;
        this.m_pData = new Uint8Array(this.m_DataLen);
        this.m_pData[0] = SMART_VALUE_TYPE.VT_STRUCT;
        this.SetLength(0);
        return true;
    }

    public Attach(Data: Uint8Array, StartIndex: number, DataLen: number, IsEmpty: boolean): boolean {
        
        if (!(Data instanceof Uint8Array))
            return false;

        if (DataLen == 0)
            DataLen = Data.length;

        if (StartIndex + DataLen > Data.length)
            return false;

        this.Destory();

        this.m_pData = Data;
        this.m_StartIndex = StartIndex;
        this.m_DataLen = DataLen;
        if (IsEmpty) {
            if (DataLen >= 5) {
                this.m_pData[this.m_StartIndex] = SMART_VALUE_TYPE.VT_STRUCT;
                this.SetLength(0);
            }
            else {
                this.Destory();
                return false;
            }
        }
        else {
            if (this.m_pData[this.m_StartIndex] != SMART_VALUE_TYPE.VT_STRUCT) {
                this.Destory();
                return false;
            }
        }
        return true;
    }
    public CloneFrom(Value: CSmartStruct): boolean {
        this.Destory();
        if (!(Value.GetData() instanceof Uint8Array))
            return false;

        this.m_StartIndex = 0;
        this.m_DataLen = Value.GetDataLen();
        this.m_pData = Value.GetData().slice(Value.GetDataStartIndex(), Value.GetDataStartIndex() + this.m_DataLen);

        return true;
    }

    public GetLength(): number {
        if (!(this.m_pData instanceof Uint8Array))
            return 0;
        let dv = this.toDataView();
        return dv.getUint32(1, true);
    }
    protected SetLength(Len: number): void {
        let dv = this.toDataView();
        dv.setUint32(1, Len, true);
    }
    public GetDataLen(): number {
        if (!(this.m_pData instanceof Uint8Array))
            return 0;
        let DataLen = this.GetLength() + 5;
        if (DataLen > this.m_DataLen)
            DataLen = this.m_DataLen;
        return DataLen;
    }
    public GetBufferLen(): number {
        return this.m_DataLen;
    }
    public GetFreeLen(): number {
        return this.m_DataLen - this.GetDataLen();
    }
    public GetData(): Uint8Array {
        return this.m_pData;
    }
    public GetDataStartIndex(): number {
        return this.m_StartIndex;
    }
    public toDataView(): DataView {
        return new DataView(this.m_pData.buffer, this.m_pData.byteOffset + this.m_StartIndex, this.m_DataLen);
    }

    public PrepareMember(Type: SMART_VALUE_TYPE): CSmartValue {
        if (!(this.m_pData instanceof Uint8Array))
            return null;
        if (this.GetFreeLen() < 3)
            return null;
        let Value = new CSmartValue();
        if (Value.Attach(this.m_pData, this.m_StartIndex + CSmartStruct.HEAD_SIZE + this.GetLength() + 2,
            this.GetFreeLen() - 2, Type)) {
            return Value;
        }
        return null;
    }

    public PrepareSubStruct(): CSmartStruct {
        if ((this.m_pData instanceof Uint8Array) && this.GetFreeLen() >= 7) {
            let SubStruct = new CSmartStruct();
            let StartIndex = this.m_StartIndex + this.GetLength() + 7;
            let DataLen = this.m_DataLen - (StartIndex - this.m_StartIndex);
            SubStruct.Attach(this.m_pData, StartIndex, DataLen, true);
            return SubStruct;
        }
        else {
            return null;
        }
    }
    public PrepareSubArray(): CSmartArray {
        if ((this.m_pData instanceof Uint8Array) && this.GetFreeLen() >= 7) {
            let SubArray = new CSmartArray();
            let StartIndex = this.m_StartIndex + this.GetLength() + 7;
            let DataLen = this.m_DataLen - (StartIndex - this.m_StartIndex);
            SubArray.Attach(this.m_pData, StartIndex, DataLen, true);
            return SubArray;
        }
        else {
            return null;
        }
    }
    public FinishMember(ID: number, MemberSize: number): boolean {
        if (!(this.m_pData instanceof Uint8Array))
            return false;
        if (ID == 0)
            return false;
        let NeedSize = 2 + MemberSize;
        if (this.GetFreeLen() < NeedSize) {
            return false;
        }
        let StartIndex = 5 + this.GetLength();
        let dv = this.toDataView();
        dv.setUint16(StartIndex, ID, true);
        dv.setUint32(1, this.GetLength() + NeedSize, true);
        return true;
    }
    public AddMemberNull(ID: number): boolean {
        let SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_NULL);
        if (SmartValue) {
            return this.FinishMember(ID, SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberSbyte(ID: number, Value: number): boolean {
        let SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_CHAR);
        if (SmartValue) {
            if (SmartValue.SetValue(Value))
                return this.FinishMember(ID, SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberByte(ID: number, Value: number): boolean {
        let SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_UCHAR);
        if (SmartValue) {
            if (SmartValue.SetValue(Value))
                return this.FinishMember(ID, SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberShort(ID: number, Value: number): boolean {
        let SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_SHORT);
        if (SmartValue) {
            if (SmartValue.SetValue(Value))
                return this.FinishMember(ID, SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberUshort(ID: number, Value: number): boolean {
        let SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_USHORT);
        if (SmartValue) {
            if (SmartValue.SetValue(Value))
                return this.FinishMember(ID, SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberInt(ID: number, Value: number): boolean {
        let SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_INT);
        if (SmartValue) {
            if (SmartValue.SetValue(Value))
                return this.FinishMember(ID, SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberUint(ID: number, Value: number): boolean {
        let SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_UINT);
        if (SmartValue) {
            if (SmartValue.SetValue(Value))
                return this.FinishMember(ID, SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberLong(ID: number, Value: bigint): boolean {
        let Type = SMART_VALUE_TYPE.VT_BIGINT;
        let SmartValue = this.PrepareMember(Type);
        if (SmartValue) {
            if (SmartValue.SetValue(Value))
                return this.FinishMember(ID, SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberFloat(ID: number, Value: number): boolean {
        let SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_FLOAT);
        if (SmartValue) {
            if (SmartValue.SetValue(Value))
                return this.FinishMember(ID, SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberDouble(ID: number, Value: number): boolean {
        let SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_DOUBLE);
        if (SmartValue) {
            if (SmartValue.SetValue(Value))
                return this.FinishMember(ID, SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberBoolean(ID: number, Value: boolean): boolean {
        let SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_BOOL);
        if (SmartValue) {
            if (SmartValue.SetValue(Value))
                return this.FinishMember(ID, SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberString(ID: number, Value: string): boolean {
        let SmartValue: CSmartValue = null;
        switch (CSmartValue.INTERNAL_STRING_CODE_PAGE) {
            case STRING_CODE_PAGE.STRING_CODE_PAGE_ANSI:
                SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_STRING_ANSI);
                break;
            case STRING_CODE_PAGE.STRING_CODE_PAGE_UTF8:
                SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_STRING_UTF8);
                break;
            case STRING_CODE_PAGE.STRING_CODE_PAGE_UCS16:
                SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_STRING_UCS16);
                break;
        }
        if (SmartValue) {
            if (SmartValue.SetValue(Value))
                return this.FinishMember(ID, SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberBinary(ID: number, Value: Uint8Array): boolean {
        let SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_BINARY);
        if (SmartValue) {
            if (SmartValue.SetValue(Value)) {
                    return this.FinishMember(ID, SmartValue.GetDataLen());
            }
        }
        return false;
    }
    public AddMemberValue(ID: number, Value: CSmartValue): boolean {
        if (Value.GetDataLen() > 0) {
            let SmartValue = this.PrepareMember(Value.GetValueType());
            if (SmartValue) {
                if (SmartValue.SetValue(Value)) {
                    return this.FinishMember(ID, Value.GetDataLen());
                }
            }
            return false;
        }
        else {
            return true;
        }
    }
    public AddMemberStruct(ID: number, Value: CSmartStruct): boolean {
        if (Value.GetDataLen() > 0) {
            let SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_STRUCT);
            if (SmartValue) {
                if (SmartValue.SetValue(Value)) {
                    return this.FinishMember(ID, Value.GetDataLen());
                }
            }
            return false;
        }
        else {
            return true;
        }
    }
    public AddMemberArray(ID: number, Value: CSmartArray): boolean {
        if (Value.GetDataLen() > 0) {
            let SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_ARRAY);
            if (SmartValue) {
                if (SmartValue.SetValue(Value)) {
                    return this.FinishMember(ID, Value.GetDataLen());
                }
            }
            return false;
        }
        else {
            return true;
        }
    }
    public AddMember(ID: number, Value: any): boolean {
        if (Value == null || Value == undefined) {
            return this.AddMemberNull(ID);
        }
        else if (typeof Value == "boolean") {
            return this.AddMemberBoolean(ID, Value);
        }
        else if (typeof Value == "number") {
            return this.AddMemberDouble(ID, Value);
        }
        else if (typeof Value == "string") {
            return this.AddMemberString(ID, Value);
        }
        else if (typeof Value == "bigint") {
            return this.AddMemberLong(ID, Value);
        }
        else if (typeof Value == "object") {
            if (Value instanceof Uint8Array) {
                return this.AddMemberBinary(ID, Value);
            }
            else if (Value instanceof CSmartValue) {
                return this.AddMemberValue(ID, Value);
            }
            else if (Value instanceof CSmartStruct) {
                return this.AddMemberStruct(ID, Value);
            }
            else if (Value instanceof CSmartArray) {
                return this.AddMemberArray(ID, Value);
            }
            else if (Value instanceof Array) {
                let array = this.PrepareSubArray();
                if (array) {
                    for (let Child of Value) {
                        if (!array.AddMember(Child))
                            return false;
                    }
                    return this.FinishMember(ID, array.GetDataLen());
                }
            }
            else {
                let struct = this.PrepareSubStruct();
                if (struct) {
                    for (const [key, value] of Object.entries(Value)) {
                        let SubStruct = struct.PrepareSubStruct();
                        if (!SubStruct.AddMember(SST_PAIR.KEY, key))
                            return null;
                        if (!SubStruct.AddMember(SST_PAIR.DATA, value))
                            return null;
                        if (!struct.FinishMember(1, SubStruct.GetDataLen()))
                            return null;
                    }
                    return this.FinishMember(ID, struct.GetDataLen());
                }
            }
        }
        return false;
    }
    public GetMemberCount(): number {
        if (!(this.m_pData instanceof Uint8Array))
            return 0;
        let HeadIndex = this.m_StartIndex;
        let TailIndex = this.m_StartIndex + this.GetDataLen();
        let MemberCount = 0;
        HeadIndex += 5;
        while (HeadIndex + 2 < TailIndex) {
            HeadIndex += 2;

            let Value = new CSmartValue(this.m_pData, HeadIndex, TailIndex - HeadIndex, SMART_VALUE_TYPE.VT_UNKNOWN);
            HeadIndex += Value.GetDataLen();
            MemberCount++;
        }
        return MemberCount;
    }
    public GetMemberByIndex(Index: number): CSmartValue {
        let Value = new CSmartValue();

        if (!(this.m_pData instanceof Uint8Array))
            return Value;
        let HeadIndex = this.m_StartIndex;
        let TailIndex = this.m_StartIndex + this.GetDataLen();

        HeadIndex += 5;
        while (HeadIndex + 2 < TailIndex) {
            HeadIndex += 2;
            if (!Value.Attach(this.m_pData, HeadIndex, TailIndex - HeadIndex, SMART_VALUE_TYPE.VT_UNKNOWN))
                break;
            HeadIndex += Value.GetDataLen();
            if (Index == 0)
                return Value;
            Index--;
        }
        Value.Destory();
        return Value;
    }

    public GetMember(ID: number): CSmartValue {

        let Value = new CSmartValue();

        if (!(this.m_pData instanceof Uint8Array))
            return Value;                

        let dv = this.toDataView();
        let HeadIndex = 5;
        let TailIndex = dv.byteLength;
        while (HeadIndex + 2 < TailIndex) {
            let MemberID = dv.getUint16(HeadIndex, true);
            HeadIndex += 2;
            if (!Value.Attach(this.m_pData, this.m_StartIndex + HeadIndex, TailIndex - HeadIndex, SMART_VALUE_TYPE.VT_UNKNOWN))
                break;
            if (MemberID == ID)
                return Value;
            HeadIndex += Value.GetDataLen();
        }
        Value.Destory();
        return Value;
    }
    public IsMemberExist(ID: number): boolean {
        if (this.IDToIndex(ID) == CSmartStruct.INVALID_MEMBER_ID) {
            return false;
        }
        return true;
    }
    public IDToIndex(ID: number): number {
        let Value = new CSmartValue();

        if (!(this.m_pData instanceof Uint8Array))
            return CSmartStruct.INVALID_MEMBER_ID;
        
        let Index = 0;
        let dv = this.toDataView();
        let HeadIndex = 5;
        let TailIndex = dv.byteLength;
        while (HeadIndex + 2 < TailIndex) {
            let MemberID = dv.getUint16(HeadIndex, true);
            HeadIndex += 2;
            if (!Value.Attach(this.m_pData, this.m_StartIndex + HeadIndex, TailIndex - HeadIndex, SMART_VALUE_TYPE.VT_UNKNOWN))
                break;
            if (MemberID == ID)
                return Index;
            HeadIndex += Value.GetDataLen();
            Index++;
        }
        return CSmartStruct.INVALID_MEMBER_ID;
    }
    public IndexToID(Index: number): number {
        let Value = new CSmartValue();

        if (!(this.m_pData instanceof Uint8Array))
            return 0;
        
        let MemberID = 0;

        let dv = this.toDataView();
        let HeadIndex = 5;
        let TailIndex = dv.byteLength;
        while (HeadIndex + 2 < TailIndex) {
            let MemberID = dv.getUint16(HeadIndex, true);
            if (Index == 0)
                return MemberID;
            HeadIndex += 2;
            if (!Value.Attach(this.m_pData, this.m_StartIndex + HeadIndex, TailIndex - HeadIndex, SMART_VALUE_TYPE.VT_UNKNOWN))
                break;
            HeadIndex += Value.GetDataLen();
            Index--;
        }
        return 0;
    }

    public GetFirstMemberPosition(): { Pos: number, MemberID: number } {
        if (!(this.m_pData instanceof Uint8Array))
            return { Pos: 0, MemberID: 0 };


        let HeadIndex = this.m_StartIndex;
        let TailIndex = this.m_StartIndex + this.GetDataLen();
        HeadIndex += 5;

        if (HeadIndex < TailIndex)
            return { Pos: HeadIndex, MemberID: 0 };
        else
            return { Pos: 0, MemberID: 0 };
    }
    public GetNextMember(Pos: { Pos: number, MemberID: number }): CSmartValue {
        let Value = new CSmartValue();

        if (!(this.m_pData instanceof Uint8Array)) {
            Pos.Pos = 0;
            return Value;
        }

        let dv = this.toDataView();
        if (Pos.Pos != 0) {
            let HeadIndex = Pos.Pos;
            let TailIndex = this.m_StartIndex + this.GetDataLen();
            Pos.MemberID = dv.getUint16(HeadIndex - this.m_StartIndex, true);

            HeadIndex += 2;

            if (HeadIndex >= TailIndex) {
                Pos.Pos = 0;
            }
            else {
                if (!Value.Attach(this.m_pData, HeadIndex, TailIndex - HeadIndex, SMART_VALUE_TYPE.VT_UNKNOWN)) {
                    Pos.Pos = 0;
                    return Value;
                }
                Pos.Pos = HeadIndex + Value.GetDataLen();
                if (Pos.Pos >= TailIndex) {
                    Pos.Pos = 0;
                }
            }

        }
        return Value;
    }
    public static GetDataLenFromData(Data: Uint8Array, Index: number): number {
        if (!(Data instanceof Uint8Array) || Data.length - Index < CSmartStruct.HEAD_SIZE)
            return 0;
        let dv = new DataView(Data.buffer, Index);
        return dv.getUint32(1, true);
    }

    public static GetEmptyStructSize(): number {
        return 5;
    }

    public static GetFixMemberSize(TypeLen: number): number {
        return 3 + TypeLen;
    }
    public static GetStructMemberSize(StructSize: number): number {
        return 7 + StructSize;
    }
    public static GetArrayMemberSize(StructSize: number): number {
        return 7 + StructSize;
    }
    public static GetBinaryMemberSize(Len: number): number {
        return 7 + Len;
    }
    public static GetStringMemberSize(Str: string): number {
        let DataLen = 0;
        switch (CSmartValue.INTERNAL_STRING_CODE_PAGE) {
            case STRING_CODE_PAGE.STRING_CODE_PAGE_ANSI:
                DataLen = 1;
                if (Str != null && Str != undefined && (Str.length > 0)) {
                    let StrLen = Encoding.GBK.GetBytes(Str);
                    DataLen += StrLen;
                }
                break;
            case STRING_CODE_PAGE.STRING_CODE_PAGE_UTF8:
                DataLen = 1;
                if (Str != null && Str != undefined && (Str.length > 0)) {
                    let StrLen = Encoding.UTF8.GetBytes(Str);
                    DataLen += StrLen;
                }
                break;
            case STRING_CODE_PAGE.STRING_CODE_PAGE_UCS16:
                DataLen = 2;
                if (Str != null && Str != undefined && (Str.length > 0)) {
                    let StrLen = Encoding.UCS2.GetBytes(Str);
                    DataLen += StrLen;
                }
                break;
        }
        return 7 + DataLen;
    }
    public static GetVariedMemberSize(Value: any): number {
        if (typeof Value == "boolean") {
            return CSmartStruct.GetFixMemberSize(1);
        }
        else if (typeof Value == "number") {
            return CSmartStruct.GetFixMemberSize(4);
        }
        else if (typeof Value == "string") {
            return CSmartStruct.GetStringMemberSize(Value);
        }
        else if (typeof Value == "bigint") {
            return CSmartStruct.GetFixMemberSize(8);
        }
        else if (typeof Value == "object") {
            if (Value instanceof Uint8Array) {
                return CSmartStruct.GetBinaryMemberSize(Value.length);
            }
            else if (Value instanceof CSmartValue) {
                return 2 + Value.GetDataLen();
            }
            else if (Value instanceof CSmartStruct) {
                return CSmartStruct.GetStructMemberSize(Value.GetLength());
            }
            else if (Value instanceof CSmartArray) {
                return CSmartStruct.GetArrayMemberSize(Value.GetLength());
            }
            else if (Value instanceof Array) {
                let Size = 0;
                for (let Element of Value)
                    Size += CSmartArray.GetVariedMemberSize(Element);
                return CSmartStruct.GetArrayMemberSize(Size);
            }
            else {
                let Size = 0;
                for (const [key, value] of Object.entries(Value)) {
                    let SubSize = CSmartStruct.GetVariedMemberSize(key) + CSmartStruct.GetVariedMemberSize(value);
                    Size += CSmartStruct.GetStructMemberSize(SubSize);
                }
                return CSmartStruct.GetStructMemberSize(Size);
            }
        }
        return 3;
    }
}

export class CSmartArray {
    protected m_pData: Uint8Array;
    protected m_StartIndex: number = 0;
    protected m_DataLen: number = 0;
    protected m_ElementSize: number = 0;
    public static readonly HEAD_SIZE: number = 5;


    constructor()
    constructor(BufferLen: number);
    constructor(Struct: CSmartValue);
    constructor(Data: Uint8Array, StartIndex: number, DataLen: number, IsEmpty: boolean)
    constructor(Data?: any, StartIndex: number = 0, DataLen: number = 0, IsEmpty: boolean = true) {
        if (Data instanceof Uint8Array) {
            this.Attach(Data, StartIndex, DataLen, IsEmpty);
        }
        else if (Data instanceof CSmartValue) {
            this.Attach(Data.GetData(), Data.GetDataStartIndex(), Data.GetDataLen(), false);
        }
        else if (typeof (Data) == "number") {
            this.Create(Data);
        }
    }
    public Destory(): void {
        this.m_pData = null;
        this.m_StartIndex = 0;
        this.m_DataLen = 0;
        this.m_ElementSize = 0;
    }

    public Clear(): void {
        this.SetLength(0);
    }

    public Create(BufferLen: number): boolean {
        this.Destory();
        this.m_StartIndex = 0;
        this.m_DataLen = BufferLen + 5;
        this.m_ElementSize = 0;
        this.m_pData = new Uint8Array(this.m_DataLen);
        this.m_pData[0] = SMART_VALUE_TYPE.VT_ARRAY;
        this.SetLength(0);
        return true;
    }
    public Attach(Data: Uint8Array, StartIndex: number, DataLen: number, IsEmpty: boolean): boolean {
        if (!(Data instanceof Uint8Array))
            return false;
        if (StartIndex + DataLen > Data.length)
            return false;

        this.Destory();

        this.m_pData = Data;
        this.m_StartIndex = StartIndex;
        this.m_DataLen = DataLen;
        if (IsEmpty) {
            if (DataLen >= 5) {
                this.m_pData[this.m_StartIndex] = SMART_VALUE_TYPE.VT_ARRAY;
                this.SetLength(0);
            }
            else {
                this.Destory();
                return false;
            }
        }
        else {
            if (this.m_pData[this.m_StartIndex] != SMART_VALUE_TYPE.VT_ARRAY) {
                this.Destory();
                return false;
            }
        }
        this.CheckElements();
        return true;
    }
    public CloneFrom(Value: CSmartArray): boolean {
        this.Destory();
        if (!(Value.GetData() instanceof Uint8Array))
            return false;

        this.m_StartIndex = 0;
        this.m_DataLen = Value.GetDataLen();
        this.m_ElementSize = Value.m_ElementSize;
        this.m_pData = Value.GetData().slice(Value.GetDataStartIndex(), Value.GetDataStartIndex() + this.m_DataLen);

        return true;
    }

    public GetLength(): number {
        if (!(this.m_pData instanceof Uint8Array))
            return 0;
        let dv = this.toDataView();
        return dv.getUint32(1, true);
    }
    protected SetLength(Len: number): void {
        let dv = this.toDataView();
        dv.setUint32(1, Len, true);
    }
    public GetDataLen(): number {
        if (!(this.m_pData instanceof Uint8Array))
            return 0;
        let DataLen = this.GetLength() + 5;
        if (DataLen > this.m_DataLen)
            DataLen = this.m_DataLen;
        return DataLen;
    }
    public GetBufferLen(): number {
        return this.m_DataLen;
    }
    public GetFreeLen(): number {
        return this.m_DataLen - this.GetDataLen();
    }
    public GetData(): Uint8Array {
        return this.m_pData;
    }
    public GetDataStartIndex(): number {
        return this.m_StartIndex;
    }
    public toDataView(): DataView {
        return new DataView(this.m_pData.buffer, this.m_pData.byteOffset + this.m_StartIndex, this.m_DataLen);
    }
    public GetArrayLength(): number {
        if ((!(this.m_pData instanceof Uint8Array)) || (this.m_DataLen < CSmartArray.HEAD_SIZE))
            return 0;
        let ArrayDataStart = this.m_StartIndex + CSmartArray.HEAD_SIZE;
        let ArrayDataLen = this.GetLength();
        if (ArrayDataLen + CSmartArray.HEAD_SIZE > this.m_DataLen)
            ArrayDataLen = this.m_DataLen - CSmartArray.HEAD_SIZE;
        if (ArrayDataLen > 0) {            
            if (this.m_ElementSize > 0) {
                //定长元素
                return ArrayDataLen / this.m_ElementSize;
            }
            else {
                //变长元素
                let Value = CSmartValue.NULL;
                let Count = 0;
                let HeadIndex = ArrayDataStart;
                let TailIndex = HeadIndex + ArrayDataLen;

                while (HeadIndex + 1 < TailIndex) {
                    if (!Value.Attach(this.m_pData, HeadIndex, TailIndex - HeadIndex, SMART_VALUE_TYPE.VT_UNKNOWN))
                        break;
                    Count++;
                    HeadIndex += Value.GetDataLen();
                }
                return Count;
            }
        }
        return 0;
    }

    public GetElementType(): SMART_VALUE_TYPE {
        if (this.m_ElementSize > 0 && this.GetLength() > 0) {
            return this.m_pData[this.m_StartIndex + CSmartArray.HEAD_SIZE];
        }
        return SMART_VALUE_TYPE.VT_UNKNOWN;
    }

    public GetElementSize(): number {
        return this.m_ElementSize;        
    }

    public GetMember(Index: number): CSmartValue {
        let Value = CSmartValue.NULL;

        if (this.m_ElementSize > 0) {
            //定长元素
            let ArrayLen = Math.floor(this.GetLength() / this.m_ElementSize);
            if (Index < ArrayLen) {

                let StartIndex = this.m_StartIndex + CSmartArray.HEAD_SIZE + this.m_ElementSize * Index;
                Value.Attach(this.m_pData, StartIndex, this.m_ElementSize, SMART_VALUE_TYPE.VT_UNKNOWN);                
                return Value;
            }
        }
        else {
            //变长元素
            let HeadIndex = this.m_StartIndex + CSmartArray.HEAD_SIZE;
            let TailIndex = HeadIndex + this.GetLength();

            while (HeadIndex + 1 < TailIndex) {
                if (!Value.Attach(this.m_pData, HeadIndex, TailIndex - HeadIndex, SMART_VALUE_TYPE.VT_UNKNOWN))
                    break;
                if (Index == 0)
                    return Value;
                HeadIndex += Value.GetDataLen();
            }
        }
        return Value;
    }
    public GetFirstMemberPosition(): { Pos: number } {
        if (!(this.m_pData instanceof Uint8Array))
            return { Pos: 0 };


        let HeadIndex = this.m_StartIndex;
        let TailIndex = this.m_StartIndex + this.GetDataLen();
        HeadIndex += CSmartArray.HEAD_SIZE;

        if (HeadIndex < TailIndex)
            return { Pos: HeadIndex };
        else
            return { Pos: 0 };
    }
    public GetNextMember(Pos: { Pos: number }): CSmartValue {
        let Value = CSmartValue.NULL;

        if (!(this.m_pData instanceof Uint8Array)) {
            Pos.Pos = 0;
            return Value;
        }

        if (Pos.Pos != 0) {
            let HeadIndex = Pos.Pos;
            let TailIndex = this.m_StartIndex + this.GetDataLen();

            if (HeadIndex >= TailIndex) {
                Pos.Pos = 0;
            }
            else {
                if (!Value.Attach(this.m_pData, HeadIndex, TailIndex - HeadIndex, SMART_VALUE_TYPE.VT_UNKNOWN)) {
                    Pos.Pos = 0;
                    return Value;
                }
                Pos.Pos = HeadIndex + Value.GetDataLen();
                if (Pos.Pos >= TailIndex) {
                    Pos.Pos = 0;
                }
            }

        }
        return Value;
    }
    public GetNextArrayMember(Pos: { Pos: number }): CSmartArray {
        let Value = new CSmartArray();

        if (!(this.m_pData instanceof Uint8Array)) {
            Pos.Pos = 0;
            return Value;
        }


        if (Pos.Pos != 0) {
            let HeadIndex = Pos.Pos;
            let TailIndex = this.m_StartIndex + this.GetDataLen();

            if (HeadIndex >= TailIndex) {
                Pos.Pos = 0;
            }
            else {
                if (!Value.Attach(this.m_pData, HeadIndex, TailIndex - HeadIndex, false)) {
                    Pos.Pos = 0;
                    return Value;
                }
                Pos.Pos = HeadIndex + Value.GetDataLen();
                if (Pos.Pos >= TailIndex) {
                    Pos.Pos = 0;
                }
            }

        }
        return Value;
    }
    public GetNextStructMember(Pos: { Pos: number }): CSmartStruct {
        let Value = new CSmartStruct();

        if (!(this.m_pData instanceof Uint8Array)) {
            Pos.Pos = 0;
            return Value;
        }


        if (Pos.Pos != 0) {
            let HeadIndex = Pos.Pos;
            let TailIndex = this.m_StartIndex + this.GetDataLen();

            if (HeadIndex >= TailIndex) {
                Pos.Pos = 0;
            }
            else {
                if (!Value.Attach(this.m_pData, HeadIndex, TailIndex - HeadIndex, false)) {
                    Pos.Pos = 0;
                    return Value;
                }
                Pos.Pos = HeadIndex + Value.GetDataLen();
                if (Pos.Pos >= TailIndex) {
                    Pos.Pos = 0;
                }
            }

        }
        return Value;
    }
    public GetArray(Array: any[]): boolean {
        if (this.GetLength() > 0) {
            if (this.m_ElementSize > 0) {
                let Value = CSmartValue.NULL;

                let ArrayLen = Math.floor(this.GetLength() / this.m_ElementSize);
                for (let i = 0; i < ArrayLen; i++) {
                    if (!Value.Attach(this.m_pData, this.m_StartIndex + CSmartArray.HEAD_SIZE + this.m_ElementSize * i, this.m_ElementSize, SMART_VALUE_TYPE.VT_UNKNOWN))
                        return false;
                    Array.push(Value.toAny());
                }
                return true;
            }
            else {
                let HeadIndex = this.m_StartIndex + CSmartArray.HEAD_SIZE;
                let TailIndex = HeadIndex + this.GetLength();
                let Value = CSmartValue.NULL;

                while (HeadIndex + 1 < TailIndex) {
                    if (!Value.Attach(this.m_pData, HeadIndex, TailIndex - HeadIndex, SMART_VALUE_TYPE.VT_UNKNOWN))
                        return false;
                    Array.push(Value.toAny());
                    HeadIndex += Value.GetDataLen();
                }
            }            
            return true;
        }
        else {
            return true;
        }
    }
    public GetArrayBoolean(Array: boolean[]): boolean {
        if (this.GetLength() > 0) {
            if (this.m_ElementSize > 0) {
                let Value = CSmartValue.NULL;

                let ArrayLen = Math.floor(this.GetLength() / this.m_ElementSize);
                for (let i = 0; i < ArrayLen; i++) {
                    if (!Value.Attach(this.m_pData, this.m_StartIndex + CSmartArray.HEAD_SIZE + this.m_ElementSize * i, this.m_ElementSize, SMART_VALUE_TYPE.VT_UNKNOWN))
                        return false;
                    Array.push(Value.toBoolean());
                }
                return true;
            }
            else {
                let HeadIndex = this.m_StartIndex + CSmartArray.HEAD_SIZE;
                let TailIndex = HeadIndex + this.GetLength();
                let Value = CSmartValue.NULL;

                while (HeadIndex + 1 < TailIndex) {
                    if (!Value.Attach(this.m_pData, HeadIndex, TailIndex - HeadIndex, SMART_VALUE_TYPE.VT_UNKNOWN))
                        return false;
                    Array.push(Value.toBoolean());
                    HeadIndex += Value.GetDataLen();
                }
            }
        }
        else {
            return true;
        }
    }
    public GetArrayNumber(Array: number[]): boolean {
        if (this.GetLength() > 0) {            
            if (this.m_ElementSize > 0) {
                let Value = CSmartValue.NULL;

                let ArrayLen = Math.floor(this.GetLength() / this.m_ElementSize);
                for (let i = 0; i < ArrayLen; i++) {
                    if (!Value.Attach(this.m_pData, this.m_StartIndex + CSmartArray.HEAD_SIZE + this.m_ElementSize * i, this.m_ElementSize, SMART_VALUE_TYPE.VT_UNKNOWN))
                        return false;
                    Array.push(Value.valueOf());
                }
                return true;
            }
            else {
                let HeadIndex = this.m_StartIndex + CSmartArray.HEAD_SIZE;
                let TailIndex = HeadIndex + this.GetLength();
                let Value = CSmartValue.NULL;

                while (HeadIndex + 1 < TailIndex) {
                    if (!Value.Attach(this.m_pData, HeadIndex, TailIndex - HeadIndex, SMART_VALUE_TYPE.VT_UNKNOWN))
                        return false;
                    Array.push(Value.valueOf());
                    HeadIndex += Value.GetDataLen();
                }
            }
        }
        else {
            return true;
        }
    }
    public GetArrayLong(Array: bigint[]): boolean {
        if (this.GetLength() > 0) {            
            if (this.m_ElementSize > 0) {
                let Value = CSmartValue.NULL;

                let ArrayLen = Math.floor(this.GetLength() / this.m_ElementSize);
                for (let i = 0; i < ArrayLen; i++) {
                    if (!Value.Attach(this.m_pData, this.m_StartIndex + CSmartArray.HEAD_SIZE + this.m_ElementSize * i, this.m_ElementSize, SMART_VALUE_TYPE.VT_UNKNOWN))
                        return false;
                    Array.push(Value.toLong());
                }
                return true;
            }
            else {
                let HeadIndex = this.m_StartIndex + CSmartArray.HEAD_SIZE;
                let TailIndex = HeadIndex + this.GetLength();
                let Value = CSmartValue.NULL;

                while (HeadIndex + 1 < TailIndex) {
                    if (!Value.Attach(this.m_pData, HeadIndex, TailIndex - HeadIndex, SMART_VALUE_TYPE.VT_UNKNOWN))
                        return false;
                    Array.push(Value.toLong());
                    HeadIndex += Value.GetDataLen();
                }
            }
        }
        else {
            return true;
        }
    }
    public GetArrayString(Array: string[]): boolean {
        if (this.GetLength() > 0) {
            let HeadIndex = this.m_StartIndex + CSmartArray.HEAD_SIZE;
            let TailIndex = HeadIndex + this.GetLength();
            let Value = CSmartValue.NULL;

            while (HeadIndex + 1 < TailIndex) {
                if (!Value.Attach(this.m_pData, HeadIndex, TailIndex - HeadIndex, SMART_VALUE_TYPE.VT_UNKNOWN))
                    return false;
                Array.push(Value.toString());
                HeadIndex += Value.GetDataLen();
            }
            return true;
        }
        else {
            return true;
        }
    }
    public GetArrayBinary(Array: Uint8Array[]): boolean {
        if (this.GetLength() > 0) {
            let HeadIndex = this.m_StartIndex + CSmartArray.HEAD_SIZE;
            let TailIndex = HeadIndex + this.GetLength();
            let Value = CSmartValue.NULL;

            while (HeadIndex + 1 < TailIndex) {
                if (!Value.Attach(this.m_pData, HeadIndex, TailIndex - HeadIndex, SMART_VALUE_TYPE.VT_UNKNOWN))
                    return false;
                Array.push(Value.toBinary());
                HeadIndex += Value.GetDataLen();
            }
            return true;
        }
        else {
            return true;
        }
    }
    public GetArrayStruct(Array: CSmartStruct[]): boolean {
        if (this.GetLength() > 0) {
            let HeadIndex = this.m_StartIndex + CSmartArray.HEAD_SIZE;
            let TailIndex = HeadIndex + this.GetLength();
            let Value = new CSmartStruct();

            while (HeadIndex + 1 < TailIndex) {
                if (!Value.Attach(this.m_pData, HeadIndex, TailIndex - HeadIndex, false))
                    return false;
                Array.push(Value);
                HeadIndex += Value.GetDataLen();
            }
            return true;
        }
        else {
            return true;
        }
    }
    public GetArrayArray(Array: CSmartArray[]): boolean {
        if (this.GetLength() > 0) {
            let HeadIndex = this.m_StartIndex + CSmartArray.HEAD_SIZE;
            let TailIndex = HeadIndex + this.GetLength();
            let Value = new CSmartArray();

            while (HeadIndex + 1 < TailIndex) {
                if (!Value.Attach(this.m_pData, HeadIndex, TailIndex - HeadIndex, false))
                    return false;
                Array.push(Value);
                HeadIndex += Value.GetDataLen();
            }
            return true;
        }
        else {
            return true;
        }
    }
    public PrepareMember(Type: SMART_VALUE_TYPE): CSmartValue {        
        if (!(this.m_pData instanceof Uint8Array))
            return null;        
        if (this.GetFreeLen() < 1)
            return null;
        let Value = new CSmartValue();
        if (Value.Attach(this.m_pData, this.m_StartIndex + CSmartArray.HEAD_SIZE + this.GetLength(),
            this.GetFreeLen(), Type)) {
            return Value;
        }
        return null;
    }

    public PrepareSubStruct(): CSmartStruct {
        let SubStruct: CSmartStruct;
        if (!(this.m_pData instanceof Uint8Array))
            return SubStruct;
        if (this.m_pData != null && this.GetFreeLen() >= CSmartStruct.HEAD_SIZE) {
            SubStruct = new CSmartStruct();
            let StartIndex = this.m_StartIndex + CSmartArray.HEAD_SIZE + this.GetLength();
            let DataLen = this.m_DataLen - (StartIndex - this.m_StartIndex);
            SubStruct.Attach(this.m_pData, StartIndex, DataLen, true);
        }

        return SubStruct;
    }
    public PrepareSubArray(): CSmartArray {
        let SubStruct: CSmartArray;
        if (!(this.m_pData instanceof Uint8Array))
            return SubStruct;
        if (this.m_pData != null && this.GetFreeLen() >= CSmartArray.HEAD_SIZE) {
            SubStruct = new CSmartArray();
            let StartIndex = this.m_StartIndex + CSmartArray.HEAD_SIZE + this.GetLength();
            let DataLen = this.m_DataLen - (StartIndex - this.m_StartIndex);
            SubStruct.Attach(this.m_pData, StartIndex, DataLen, true);
        }

        return SubStruct;
    }
    public FinishMember(MemberSize: number): boolean {
        if (!(this.m_pData instanceof Uint8Array))
            return false;

        if (this.GetFreeLen() < MemberSize) {
            return false;
        }
        let FreeBufferIndex = this.m_StartIndex + CSmartArray.HEAD_SIZE + this.GetLength();
        let NewElementSize = this.GetElementSizeByType(this.m_pData[FreeBufferIndex]);
        if (this.GetLength()) {
            //如果新元素和已有元素大小不同，ElementSize设置为0代表改数组不是规则数组			
            if (this.m_ElementSize != NewElementSize)
                this.m_ElementSize = 0;
        }
        else {
            this.m_ElementSize = NewElementSize;
        }
        this.SetLength(this.GetLength() + MemberSize);
        return true;
    }
    public AddMemberNull(): boolean {
        let SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_NULL);
        if (SmartValue) {
            return this.FinishMember(SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberSbyte(Value: number): boolean {
        let SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_CHAR);
        if (SmartValue) {
            if (SmartValue.SetValue(Value))
                return this.FinishMember(SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberByte(Value: number): boolean {
        let SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_UCHAR);
        if (SmartValue) {
            if (SmartValue.SetValue(Value))
                return this.FinishMember(SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberShort(Value: number): boolean {
        let SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_SHORT);
        if (SmartValue) {
            if (SmartValue.SetValue(Value))
                return this.FinishMember(SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberUshort(Value: number): boolean {
        let SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_USHORT);
        if (SmartValue) {
            if (SmartValue.SetValue(Value))
                return this.FinishMember(SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberInt(Value: number): boolean {
        let SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_INT);
        if (SmartValue) {
            if (SmartValue.SetValue(Value))
                return this.FinishMember(SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberUint(Value: number): boolean {
        let SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_UINT);
        if (SmartValue) {
            if (SmartValue.SetValue(Value))
                return this.FinishMember(SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberLong(Value: bigint): boolean {
        let ValueType = SMART_VALUE_TYPE.VT_BIGINT;
        let SmartValue = this.PrepareMember(ValueType);
        if (SmartValue) {
            if (SmartValue.SetValue(Value))
                return this.FinishMember(SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberFloat(Value: number): boolean {
        let SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_FLOAT);
        if (SmartValue) {
            if (SmartValue.SetValue(Value))
                return this.FinishMember(SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberDouble(Value: number): boolean {
        let SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_DOUBLE);
        if (SmartValue) {
            if (SmartValue.SetValue(Value))
                return this.FinishMember(SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberBoolean(Value: boolean): boolean {
        let SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_BOOL);
        if (SmartValue) {
            if (SmartValue.SetValue(Value))
                return this.FinishMember(SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberString(Value: string): boolean {
        let SmartValue: CSmartValue = null;
        switch (CSmartValue.INTERNAL_STRING_CODE_PAGE) {
            case STRING_CODE_PAGE.STRING_CODE_PAGE_ANSI:
                SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_STRING_ANSI);
                break;
            case STRING_CODE_PAGE.STRING_CODE_PAGE_UTF8:
                SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_STRING_UTF8);
                break;
            case STRING_CODE_PAGE.STRING_CODE_PAGE_UCS16:
                SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_STRING_UCS16);
                break;
        }
        if (SmartValue) {
            if (SmartValue.SetValue(Value))
                return this.FinishMember(SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberBinary(Value: Uint8Array): boolean {
        let SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_BINARY);
        if (SmartValue) {
            if (SmartValue.SetValue(Value))
                return this.FinishMember(SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberValue(Value: CSmartValue): boolean {
        let SmartValue = this.PrepareMember(Value.GetValueType());
        if (SmartValue) {
            if (SmartValue.SetValue(Value))
                return this.FinishMember(SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberStruct(Value: CSmartStruct): boolean {
        let SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_STRUCT);
        if (SmartValue) {
            if (SmartValue.SetValue(Value))
                return this.FinishMember(SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMemberArray(Value: CSmartArray): boolean {
        let SmartValue = this.PrepareMember(SMART_VALUE_TYPE.VT_ARRAY);
        if (SmartValue) {
            if (SmartValue.SetValue(Value))
                return this.FinishMember(SmartValue.GetDataLen());
        }
        return false;
    }
    public AddMember(Value: any): boolean {
        if (Value == null || Value == undefined) {
            return this.AddMemberNull();
        }
        else if (typeof Value == "boolean") {
            return this.AddMemberBoolean(Value);
        }
        else if (typeof Value == "number") {
            return this.AddMemberDouble(Value);
        }
        else if (typeof Value == "string") {
            return this.AddMemberString(Value);
        }
        else if (typeof Value == "bigint") {
            return this.AddMemberLong(Value);
        }
        else if (typeof Value == "object") {
            if (Value instanceof Uint8Array) {
                return this.AddMemberBinary(Value);
            }
            else if (Value instanceof CSmartValue) {
                return this.AddMemberValue(Value);
            }
            else if (Value instanceof CSmartStruct) {
                return this.AddMemberStruct(Value);
            }
            else if (Value instanceof CSmartArray) {
                return this.AddMemberArray(Value);
            }
            else if (Value instanceof Array) {
                let array = this.PrepareSubArray();
                if (array) {
                    for (let Child of Value) {
                        if (!array.AddMember(Child))
                            return false;
                    }
                    return this.FinishMember(array.GetDataLen());
                }
            }
            else {
                let struct = this.PrepareSubStruct();
                if (struct) {
                    for (const [key, value] of Object.entries(Value)) {
                        let SubStruct = struct.PrepareSubStruct();
                        if (!SubStruct.AddMember(SST_PAIR.KEY, key))
                            return null;
                        if (!SubStruct.AddMember(SST_PAIR.DATA, value))
                            return null;
                        if (!struct.FinishMember(1, SubStruct.GetDataLen()))
                            return null;
                    }
                    return this.FinishMember(struct.GetDataLen());
                }
            }
        }
        return false;
    }
    public AddArray(Array: any[], ValueType: string): boolean {
        switch (ValueType) {
            case "bool[]":
                return this.AddArrayBoolean(Array);
            case "char[]":
                return this.AddArrayNumber(Array, SMART_VALUE_TYPE.VT_CHAR);
            case "BYTE[]":
                return this.AddArrayNumber(Array, SMART_VALUE_TYPE.VT_UCHAR);
            case "short[]":
                return this.AddArrayNumber(Array, SMART_VALUE_TYPE.VT_SHORT);
            case "WORD[]":
                return this.AddArrayNumber(Array, SMART_VALUE_TYPE.VT_USHORT);
            case "int[]":
                return this.AddArrayNumber(Array, SMART_VALUE_TYPE.VT_INT);
            case "UINT[]":
                return this.AddArrayNumber(Array, SMART_VALUE_TYPE.VT_UINT);
            case "TimeStamp[]":
                return this.AddArrayNumber(Array, SMART_VALUE_TYPE.VT_UINT);
            case "INT64[]":
                return this.AddArrayLong(Array);
            case "UINT64[]":
                return this.AddArrayLong(Array);
            case "OBJECT_ID[]":
                return this.AddArrayLong(Array);
            case "float[]":
                return this.AddArrayNumber(Array, SMART_VALUE_TYPE.VT_FLOAT);
            case "double[]":
                return this.AddArrayNumber(Array, SMART_VALUE_TYPE.VT_DOUBLE);
            case "String[]":
                return this.AddArrayString(Array);            
            case "ByteArray[]":
                return this.AddArrayBinary(Array);
            case "Variant[]":
                return this.AddArrayVaried(Array);
        }
        if (ValueType.endsWith("[][]"))
            return this.AddArrayArray(Array);
        else
            return this.AddArrayStruct(Array);
    }
    public AddArrayNumber(Array: number[], ArrayType: SMART_VALUE_TYPE): boolean {
        if (Array.length > 0) {
            let ArrayElementSize = 0;
            switch (ArrayType) {
                case SMART_VALUE_TYPE.VT_CHAR:
                case SMART_VALUE_TYPE.VT_UCHAR:
                    ArrayElementSize = 1 + 1;
                    break;
                case SMART_VALUE_TYPE.VT_SHORT:
                case SMART_VALUE_TYPE.VT_USHORT:
                    ArrayElementSize = 1 + 2;
                    break;
                case SMART_VALUE_TYPE.VT_INT:
                case SMART_VALUE_TYPE.VT_UINT:
                    ArrayElementSize = 1 + 4;
                    break;
                case SMART_VALUE_TYPE.VT_FLOAT:
                    ArrayElementSize = 1 + 4;
                    break;
                case SMART_VALUE_TYPE.VT_DOUBLE:
                    ArrayElementSize = 1 + 8;
                    break;
                case SMART_VALUE_TYPE.VT_BOOL:
                    ArrayElementSize = 1 + 1;
                    break;
                default:
                    return false;
            }

            let NeedSize = ArrayElementSize * Array.length;
            if (this.GetFreeLen() >= NeedSize) {
                let StartIndex = this.m_StartIndex + CSmartArray.HEAD_SIZE + this.GetLength();
                let DataLen = this.m_DataLen - (StartIndex - this.m_StartIndex);
                let Element = new CSmartValue();
                for (let Value of Array) {
                    if (Element.Attach(this.m_pData, StartIndex, ArrayElementSize, ArrayType))
                        Element.SetValue(Value);
                    else
                        return false;
                    StartIndex += ArrayElementSize;
                }
                return this.FinishMember(NeedSize);
            }
            else {
                return false;
            }
        }
        else {
            return true;
        }        
    }
    public AddArrayLong(Array: bigint[]): boolean {
        if (Array.length > 0) {
            let NeedSize = CSmartArray.GetFixMemberSize(8) * Array.length;
            if (this.GetFreeLen() >= NeedSize) {
                for (let Value of Array) {
                    if (!this.AddMemberLong(Value))
                        return false;
                }
            }
            else {
                return false;
            }
        }
        return true;
    }
    public AddArrayBoolean(Array: boolean[]): boolean {
        if (Array.length > 0) {
            let NeedSize = CSmartArray.GetFixMemberSize(1) * Array.length;
            if (this.GetFreeLen() >= NeedSize) {
                for (let Value of Array) {
                    if (!this.AddMemberBoolean(Value))
                        return false;
                }
            }
            else {
                return false;
            }
        }
        return true;
    }
    public AddArrayString(Array: string[]): boolean {
        if (Array.length > 0) {
            let NeedSize = 0;
            for (let Value of Array)
                NeedSize += CSmartArray.GetStringMemberSize(Value);
            if (this.GetFreeLen() >= NeedSize) {
                for(let Value of Array)
                {
                    if (!this.AddMemberString(Value))
                        return false;
                }
            }
            else {
                return false;
            }
        }
        return true;
    }
    public AddArrayBinary(Array: Uint8Array[]): boolean {
        if (Array.length > 0) {
            let NeedSize = 0;
            for (let Value of Array)
                NeedSize += CSmartArray.GetBinaryMemberSize(Value.length);
            if (this.GetFreeLen() >= NeedSize) {
                for (let Value of Array) {
                    if (!this.AddMemberBinary(Value))
                        return false;
                }
            }
            else {
                return false;
            }
        }
        return true;
    }
    public AddArrayStruct(Array: CSmartStruct[]): boolean {
        if (Array.length > 0) {
            let NeedSize = 0;
            for (let Value of Array)
                NeedSize += CSmartArray.GetStructMemberSize(Value.GetLength());
            if (this.GetFreeLen() >= NeedSize) {
                for (let Value of Array) {
                    if (!this.AddMemberStruct(Value))
                        return false;
                }
            }
            else {
                return false;
            }
        }
        return true;
    }
    public AddArrayArray(Array: CSmartArray[]): boolean {
        if (Array.length > 0) {
            let NeedSize = 0;
            for (let Value of Array)
                NeedSize += CSmartArray.GetArrayMemberSize(Value.GetLength());
            if (this.GetFreeLen() >= NeedSize) {
                for (let Value of Array) {
                    if (!this.AddMemberArray(Value))
                        return false;
                }
            }
            else {
                return false;
            }
        }
        return true;
    }
    public AddArrayVaried(Array: any[]): boolean {
        if (Array.length > 0) {
            let NeedSize = 0;
            for (let Value of Array)
                NeedSize += CSmartArray.GetVariedMemberSize(Value);
            if (this.GetFreeLen() >= NeedSize) {
                for (let Value of Array) {
                    if (!this.AddMember(Value))
                        return false;
                }
            }
            else {
                return false;
            }
        }
        return true;
    }
    public static GetFixMemberSize(TypeLen: number):number {
        return 1 + TypeLen;
    }
    public static GetArrayMemberSize(ArraySize: number): number {
        return CSmartArray.HEAD_SIZE + ArraySize;
    }
    public static GetStructMemberSize(StructSize: number): number {
        return CSmartArray.HEAD_SIZE + StructSize;
    }
    public static GetBinaryMemberSize(Len: number): number {
        return CSmartArray.HEAD_SIZE + Len;
    }
    public static GetStringMemberSize(Str: string): number {
        let DataLen = 0;
        switch (CSmartValue.INTERNAL_STRING_CODE_PAGE) {
            case STRING_CODE_PAGE.STRING_CODE_PAGE_ANSI:
                DataLen = 1;
                if (Str != null && Str != undefined && (Str.length > 0)) {
                    let StrLen = Encoding.GBK.GetBytes(Str);
                    DataLen += StrLen;
                }
                break;
            case STRING_CODE_PAGE.STRING_CODE_PAGE_UTF8:
                DataLen = 1;
                if (Str != null && Str != undefined && (Str.length > 0)) {
                    let StrLen = Encoding.UTF8.GetBytes(Str);
                    DataLen += StrLen;
                }
                break;
            case STRING_CODE_PAGE.STRING_CODE_PAGE_UCS16:
                DataLen = 2;
                if (Str != null && Str != undefined && (Str.length > 0)) {
                    let StrLen = Encoding.UCS2.GetBytes(Str);
                    DataLen += StrLen;
                }
                break;
        }
        return CSmartArray.HEAD_SIZE + DataLen;
    }

    public static GetVariedMemberSize(Value: any): number {
        if (typeof Value == "boolean") {
            return CSmartArray.GetFixMemberSize(1);
        }
        else if (typeof Value == "number") {
            return CSmartArray.GetFixMemberSize(4);
        }
        else if (typeof Value == "string") {
            return CSmartArray.GetStringMemberSize(Value);
        }
        else if (typeof Value == "bigint") {
            return CSmartArray.GetFixMemberSize(8);
        }
        else if (typeof Value == "object") {
            if (Value instanceof Uint8Array) {
                return CSmartArray.GetBinaryMemberSize(Value.length);
            }
            else if (Value instanceof CSmartValue) {
                return Value.GetDataLen();
            }
            else if (Value instanceof CSmartStruct) {
                return CSmartArray.GetStructMemberSize(Value.GetLength());
            }
            else if (Value instanceof CSmartArray) {
                return CSmartArray.GetArrayMemberSize(Value.GetLength());
            }
            else if (Value instanceof Array) {
                let Size = 0;
                for (let Element of Value)
                    Size += CSmartArray.GetVariedMemberSize(Element);
                return CSmartArray.GetArrayMemberSize(Size);
            }
            else {
                let Size = 0;
                for (const [key, value] of Object.entries(Value)) {
                    let SubSize = CSmartStruct.GetVariedMemberSize(key) + CSmartStruct.GetVariedMemberSize(value);
                    Size += CSmartStruct.GetStructMemberSize(SubSize);
                }
                return CSmartArray.GetStructMemberSize(Size);
            }
        }
        return 1;
    }
    protected CheckElements(): void {
        this.m_ElementSize = 0;
        let Pos = this.GetFirstMemberPosition();
        while (Pos.Pos > 0) {
            let Value = this.GetNextMember(Pos);
            let Size = this.GetElementSizeByType(Value.GetValueType());
            if (Size) {
                if (this.m_ElementSize == 0) {
                    this.m_ElementSize = Size;
                }
                else if (this.m_ElementSize != Size) {
                    this.m_ElementSize = 0;
                    break;
                }
            }
            else {
                this.m_ElementSize = 0;
                break;
            }
        }       
    }
    protected GetElementSizeByType(Type: SMART_VALUE_TYPE): number {
        switch (Type) {
            case SMART_VALUE_TYPE.VT_NULL:
                return 1;
            case SMART_VALUE_TYPE.VT_CHAR:
            case SMART_VALUE_TYPE.VT_UCHAR:
                return 2;
            case SMART_VALUE_TYPE.VT_SHORT:
            case SMART_VALUE_TYPE.VT_USHORT:
                return 3;
            case SMART_VALUE_TYPE.VT_INT:
            case SMART_VALUE_TYPE.VT_UINT:
                return 5;
            case SMART_VALUE_TYPE.VT_BIGINT:
            case SMART_VALUE_TYPE.VT_UBIGINT:
                return 9;
            case SMART_VALUE_TYPE.VT_FLOAT:
                return 5;
            case SMART_VALUE_TYPE.VT_DOUBLE:
                return 9;
            case SMART_VALUE_TYPE.VT_BOOL:
                return 2;
        }
        return 0;
    }
}