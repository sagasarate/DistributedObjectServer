import { table_cp936 } from "./cp936.js"
import { table_gbk_added } from "./gbk-added.js"
import { table_gb18030 } from "./gb18030-ranges.js"

export namespace Encoding {

    const table_gbk = table_cp936.concat(table_gbk_added);
    const UNASSIGNED = -1;
    const DEF_CHAR = -1;
    const GB18030_CODE = -2;
    const SEQ_START = -10;
    const DEFAULT_CHAR_UNICODE = '�';
    const DEFAULT_CHAR_SINGLE = '?';
    const NODE_START = -1000;
    let UNASSIGNED_NODE = new Array(0x100);
    for (var i = 0; i < 0x100; i++)
        UNASSIGNED_NODE[i] = UNASSIGNED;

    function PushBuffer(Buffer: Uint8Array, Pos: { Pos: number, Len: number }, ...Bytes: number[]): boolean {
        if (Buffer) {
            if (Pos.Pos + Pos.Len > Buffer.length)
                return false;
            if (Pos.Len < Bytes.length)
                return false;
            for (let byte of Bytes) {
                if (Pos.Pos < Buffer.length) {
                    Buffer[Pos.Pos] = byte;
                    Pos.Pos++;
                    Pos.Len--;
                }
            }
            return true;
        }
        else {
            //不提供Buffer时仅计数
            Pos.Pos += Bytes.length;
            Pos.Len -= Bytes.length;
            return true;
        }        
    }

    // Checks the type of a UTF-8 byte, whether it's ASCII, a leading byte, or a
    // continuation byte. If an invalid byte is detected, -2 is returned.
    function utf8CheckByte(byte: number) {
        if (byte <= 0x7F)
            return 1;
        else if (byte >> 5 === 0x06)
            return 2;
        else if (byte >> 4 === 0x0E)
            return 3;
        else if (byte >> 3 === 0x1E)
            return 4;
        return byte >> 6 === 0x02 ? -1 : -2;
    }

    function GetDecodeTrieNode(addr: number, decodeTables: any, encodingName: string) {
        var bytes = [];
        for (; addr > 0; addr >>>= 8)
            bytes.push(addr & 0xFF);
        if (bytes.length == 0)
            bytes.push(0);

        var node = decodeTables[0];
        for (var i = bytes.length - 1; i > 0; i--) { // Traverse nodes deeper into the trie.
            var val = node[bytes[i]];

            if (val == UNASSIGNED) { // Create new node.
                node[bytes[i]] = NODE_START - decodeTables.length;
                decodeTables.push(node = UNASSIGNED_NODE.slice(0));
            }
            else if (val <= NODE_START) { // Existing node.
                node = decodeTables[NODE_START - val];
            }
            else
                throw new Error("Overwrite byte in " + encodingName + ", addr: " + addr.toString(16));
        }
        return node;
    }
    function AddDecodeChunk(chunk: any, decodeTables: any, decodeTableSeq: any, encodingName: string) {
        // First element of chunk is the hex mbcs code where we start.
        var curAddr = parseInt(chunk[0], 16);

        // Choose the decoding node where we'll write our chars.
        var writeTable = GetDecodeTrieNode(curAddr, decodeTables, encodingName);
        curAddr = curAddr & 0xFF;

        // Write all other elements of the chunk to the table.
        for (var k = 1; k < chunk.length; k++) {
            var part = chunk[k];
            if (typeof part === "string") { // String, write as-is.
                for (var l = 0; l < part.length;) {
                    var code = part.charCodeAt(l++);
                    if (0xD800 <= code && code < 0xDC00) { // Decode surrogate
                        var codeTrail = part.charCodeAt(l++);
                        if (0xDC00 <= codeTrail && codeTrail < 0xE000)
                            writeTable[curAddr++] = 0x10000 + (code - 0xD800) * 0x400 + (codeTrail - 0xDC00);
                        else
                            throw new Error("Incorrect surrogate pair in " + encodingName + " at chunk " + chunk[0]);
                    }
                    else if (0x0FF0 < code && code <= 0x0FFF) { // Character sequence (our own encoding used)
                        var len = 0xFFF - code + 2;
                        var seq = [];
                        for (var m = 0; m < len; m++)
                            seq.push(part.charCodeAt(l++)); // Simple variation: don't support surrogates or subsequences in seq.

                        writeTable[curAddr++] = SEQ_START - decodeTableSeq.length;
                        decodeTableSeq.push(seq);
                    }
                    else
                        writeTable[curAddr++] = code; // Basic char
                }
            }
            else if (typeof part === "number") { // Integer, meaning increasing sequence starting with prev character.
                var charCode = writeTable[curAddr - 1] + 1;
                for (var l = 0; l < part; l++)
                    writeTable[curAddr++] = charCode++;
            }
            else
                throw new Error("Incorrect type '" + typeof part + "' given in " + encodingName + " at chunk " + chunk[0]);
        }
        if (curAddr > 0xFF)
            throw new Error("Incorrect chunk in " + encodingName + " at addr " + chunk[0] + ": too long" + curAddr);
    }


    function FindIdx(table, val) {
        if (table[0] > val)
            return -1;

        var l = 0, r = table.length;
        while (l < r - 1) { // always table[l] <= val < table[r]
            var mid = l + ((r - l + 1) >> 1);
            if (table[mid] <= val)
                l = mid;
            else
                r = mid;
        }
        return l;
    }
    function GetEncodeBucket(encodeTable: any, uCode: number): any {
        var high = uCode >> 8; // This could be > 0xFF because of astral characters.
        if (encodeTable[high] === undefined)
            encodeTable[high] = UNASSIGNED_NODE.slice(0); // Create bucket on demand.
        return encodeTable[high];
    }

    function SetEncodeChar(encodeTable: any, encodeTableSeq: any, uCode: number, dbcsCode: number): void {
        var bucket = GetEncodeBucket(encodeTable, uCode);
        var low = uCode & 0xFF;
        if (bucket[low] <= SEQ_START)
            encodeTableSeq[SEQ_START - bucket[low]][DEF_CHAR] = dbcsCode; // There's already a sequence, set a single-char subsequence of it.
        else if (bucket[low] == UNASSIGNED)
            bucket[low] = dbcsCode;
    }

    function SetEncodeSequence(encodeTable: any, encodeTableSeq: any, seq: any, dbcsCode: number) {

        // Get the root of character tree according to first character of the sequence.
        var uCode = seq[0];
        var bucket = GetEncodeBucket(encodeTable, uCode);
        var low = uCode & 0xFF;

        var node;
        if (bucket[low] <= SEQ_START) {
            // There's already a sequence with  - use it.
            node = encodeTableSeq[SEQ_START - bucket[low]];
        }
        else {
            // There was no sequence object - allocate a new one.
            node = {};
            if (bucket[low] !== UNASSIGNED) node[DEF_CHAR] = bucket[low]; // If a char was set before - make it a single-char subsequence.
            bucket[low] = SEQ_START - encodeTableSeq.length;
            encodeTableSeq.push(node);
        }

        // Traverse the character tree, allocating new nodes as needed.
        for (var j = 1; j < seq.length - 1; j++) {
            var oldVal = node[uCode];
            if (typeof oldVal === 'object')
                node = oldVal;
            else {
                node = node[uCode] = {}
                if (oldVal !== undefined)
                    node[DEF_CHAR] = oldVal
            }
        }
        // Set the leaf to given dbcsCode.
        uCode = seq[seq.length - 1];
        node[uCode] = dbcsCode;
    }
    function FillEncodeTable(decodeTables: any, decodeTableSeq: any, encodeTable: any, encodeTableSeq: any, nodeIdx: number, prefix: number, skipEncodeChars): boolean {
        var node = decodeTables[nodeIdx];
        var hasValues = false;
        var subNodeEmpty = {};
        for (var i = 0; i < 0x100; i++) {
            var uCode = node[i];
            var mbCode = prefix + i;
            if (skipEncodeChars[mbCode])
                continue;

            if (uCode >= 0) {
                SetEncodeChar(encodeTable, encodeTableSeq, uCode, mbCode);
                hasValues = true;
            } else if (uCode <= NODE_START) {
                var subNodeIdx = NODE_START - uCode;
                if (!subNodeEmpty[subNodeIdx]) {  // Skip empty subtrees (they are too large in gb18030).
                    var newPrefix = (mbCode << 8) >>> 0;  // NOTE: '>>> 0' keeps 32-bit num positive.
                    if (FillEncodeTable(decodeTables, decodeTableSeq, encodeTable, encodeTableSeq, subNodeIdx, newPrefix, skipEncodeChars))
                        hasValues = true;
                    else
                        subNodeEmpty[subNodeIdx] = true;
                }
            } else if (uCode <= SEQ_START) {
                SetEncodeSequence(encodeTable, encodeTableSeq, decodeTableSeq[SEQ_START - uCode], mbCode);
                hasValues = true;
            }
        }
        return hasValues;
    }

    export class UTF8 {
        //将字符串转换成UTF8字节串，返回字节串长度，如果不提供Buffer则返回预计转换后的字节串长度
        public static GetBytes(Str: string, BytesBuffer?: Uint8Array, StartIndex?: number, BufferLen?: number): number {
            if (BytesBuffer) {
                if (typeof (StartIndex) != "number" || typeof (BufferLen) != "number") {
                    StartIndex = 0;
                    BufferLen = BytesBuffer.length;
                }
            }
            else {
                StartIndex = 0;
                BufferLen = Number.MAX_SAFE_INTEGER;
            }

            if (Str.length) {
                let codePoint
                const length = Str.length
                let leadSurrogate = null
                let Pos = { Pos: StartIndex, Len: BufferLen };

                for (let i = 0; i < length; ++i) {
                    codePoint = Str.charCodeAt(i);

                    // is surrogate component
                    if (codePoint > 0xD7FF && codePoint < 0xE000) {
                        // last char was a lead
                        if (!leadSurrogate) {
                            // no lead yet
                            if (codePoint > 0xDBFF) {
                                // unexpected trail
                                if (!PushBuffer(BytesBuffer, Pos, 0xEF, 0xBF, 0xBD))
                                    break;
                                continue;
                            } else if (i + 1 === length) {
                                // unpaired lead
                                if (!PushBuffer(BytesBuffer, Pos, 0xEF, 0xBF, 0xBD))
                                    break;
                                continue;
                            }

                            // valid lead
                            leadSurrogate = codePoint;

                            continue;
                        }

                        // 2 leads in a row
                        if (codePoint < 0xDC00) {
                            if (!PushBuffer(BytesBuffer, Pos, 0xEF, 0xBF, 0xBD))
                                break;
                            leadSurrogate = codePoint;
                            continue;
                        }

                        // valid surrogate pair
                        codePoint = (leadSurrogate - 0xD800 << 10 | codePoint - 0xDC00) + 0x10000;
                    } else if (leadSurrogate) {
                        // valid bmp char, but last char was a lead
                        if (!PushBuffer(BytesBuffer, Pos, 0xEF, 0xBF, 0xBD))
                            break;
                    }

                    leadSurrogate = null

                    // encode utf8
                    if (codePoint < 0x80) {
                        if (!PushBuffer(BytesBuffer, Pos, codePoint))
                            break;
                    } else if (codePoint < 0x800) {
                        if (!PushBuffer(BytesBuffer, Pos,
                            codePoint >> 0x6 | 0xC0,
                            codePoint & 0x3F | 0x80
                        ))
                            break;
                    } else if (codePoint < 0x10000) {
                        if (!PushBuffer(BytesBuffer, Pos,
                            codePoint >> 0xC | 0xE0,
                            codePoint >> 0x6 & 0x3F | 0x80,
                            codePoint & 0x3F | 0x80
                        ))
                            break;
                    } else if (codePoint < 0x110000) {
                        if (!PushBuffer(BytesBuffer, Pos,
                            codePoint >> 0x12 | 0xF0,
                            codePoint >> 0xC & 0x3F | 0x80,
                            codePoint >> 0x6 & 0x3F | 0x80,
                            codePoint & 0x3F | 0x80
                        ))
                            break;
                    } else {
                        throw new Error('Invalid code point')
                    }
                }
                if (BytesBuffer && Pos.Len >= 1) {
                    BytesBuffer[Pos.Pos] = 0;
                }
                return Pos.Pos - StartIndex;
            }
            return 0;
        }
        public static GetString(BytesBuffer: Uint8Array, StartIndex?: number, BufferLen?: number): string {
            if (typeof (StartIndex) != "number" || typeof (BufferLen) != "number") {
                StartIndex = 0;
                BufferLen = BytesBuffer.length;
            }
            let CharCodes = [];
            let Offset = StartIndex;
            let EndIndex = StartIndex + BufferLen;
            while (Offset < EndIndex) {
                let Byte1 = BytesBuffer[Offset];
                let ByteCount = utf8CheckByte(Byte1);
                let CharCode = null;
                let Byte2, Byte3, Byte4, tempCodePoint;
                switch (ByteCount) {
                    case 1:
                        CharCode = Byte1;
                        Offset++;
                        break;
                    case 2:
                        if (Offset + 2 < EndIndex) {
                            Byte2 = BytesBuffer[Offset + 1];
                            if ((Byte2 & 0xC0) === 0x80) {
                                tempCodePoint = (Byte1 & 0x1F) << 0x6 | (Byte2 & 0x3F);
                                if (tempCodePoint > 0x7F) {
                                    CharCode = tempCodePoint;
                                }
                            }
                            Offset += 2;
                        }
                        else {
                            Offset++;
                        }                        
                        break
                    case 3:
                        if (Offset + 3 < EndIndex) {
                            Byte2 = BytesBuffer[Offset + 1];
                            Byte3 = BytesBuffer[Offset + 2];
                            if ((Byte2 & 0xC0) === 0x80 && (Byte3 & 0xC0) === 0x80) {
                                tempCodePoint = (Byte1 & 0xF) << 0xC | (Byte2 & 0x3F) << 0x6 | (Byte3 & 0x3F);
                                if (tempCodePoint > 0x7FF && (tempCodePoint < 0xD800 || tempCodePoint > 0xDFFF)) {
                                    CharCode = tempCodePoint;
                                }
                            }
                            Offset += 3;
                        }
                        else {
                            Offset = EndIndex;
                        }
                        break
                    case 4:
                        if (Offset + 4 < EndIndex) {
                            Byte2 = BytesBuffer[Offset + 1];
                            Byte3 = BytesBuffer[Offset + 2];
                            Byte4 = BytesBuffer[Offset + 3];
                            if ((Byte2 & 0xC0) === 0x80 && (Byte3 & 0xC0) === 0x80 && (Byte4 & 0xC0) === 0x80) {
                                tempCodePoint = (Byte1 & 0xF) << 0x12 | (Byte2 & 0x3F) << 0xC | (Byte3 & 0x3F) << 0x6 | (Byte4 & 0x3F)
                                if (tempCodePoint > 0xFFFF && tempCodePoint < 0x110000) {
                                    CharCode = tempCodePoint
                                }
                            }
                            Offset += 4;
                        }
                        else {
                            Offset = EndIndex;
                        }
                        break;
                    default:
                        Offset++;
                        break;
                }

                if (CharCode === null) {
                    // we did not generate a valid codePoint so insert a
                    // replacement char (U+FFFD) and advance only 1 byte
                    CharCode = 0xFFFD
                } else if (CharCode > 0xFFFF) {
                    // encode to utf16 (surrogate pair dance)
                    CharCode -= 0x10000
                    CharCodes.push(String.fromCharCode(CharCode >>> 10 & 0x3FF | 0xD800));
                    CharCode = 0xDC00 | CharCode & 0x3FF
                }
                CharCodes.push(String.fromCharCode(CharCode));
            }
            return CharCodes.join('');
        }
    }

    export class UCS2 {
        public static GetBytes(Str: string, BytesBuffer?: Uint8Array, StartIndex?: number, BufferLen?: number): number {

            if (BytesBuffer) {
                if (typeof (StartIndex) != "number" || typeof (BufferLen) != "number") {
                    StartIndex = 0;
                    BufferLen = BytesBuffer.length;
                }
            }
            else {
                StartIndex = 0;
                BufferLen = Number.MAX_SAFE_INTEGER;
            }
            if (Str.length) {
                let c, hi, lo
                let Pos = { Pos: StartIndex, Len: BufferLen };

                for (let i = 0; i < Str.length; ++i) {
                    c = Str.charCodeAt(i);
                    hi = c >> 8;
                    lo = c % 256;
                    if (!PushBuffer(BytesBuffer, Pos, lo, hi))
                        break;;
                }
                if (BytesBuffer && Pos.Len >= 2) {
                    BytesBuffer[Pos.Pos] = 0;
                    BytesBuffer[Pos.Pos + 1] = 0;
                }
                return Pos.Pos - StartIndex;
            }
            return 0;
        }
        public static GetString(BytesBuffer: Uint8Array, StartIndex?: number, BufferLen?: number): string {
            if (typeof (StartIndex) != "number" || typeof (BufferLen) != "number") {
                StartIndex = 0;
                BufferLen = BytesBuffer.length;
            }
            let CharCodes = [];
            while (BufferLen > 0) {
                if (BufferLen >= 2) {
                    CharCodes.push(String.fromCharCode(BytesBuffer[StartIndex] | BytesBuffer[StartIndex + 1] << 8));
                    StartIndex += 2;
                    BufferLen -= 2;
                }
                else {
                    CharCodes.push(String.fromCharCode(0xFFFD));
                    StartIndex++;
                    BufferLen--;
                }
            }
            return CharCodes.join('');
        }
    }

    export class GBK {
        protected static m_DecodeTables: any;
        protected static m_DecodeTableSeq: any;
        protected static m_EncodeTable: any;
        protected static m_EncodeTableSeq: any;
        protected static m_DefaultCharSingleByte: number;

        protected static InitTables(): void {

            // Decode tables: MBCS -> Unicode.

            // decodeTables is a trie, encoded as an array of arrays of integers. Internal arrays are trie nodes and all have len = 256.
            // Trie root is decodeTables[0].
            // Values: >=  0 -> unicode character code. can be > 0xFFFF
            //         == UNASSIGNED -> unknown/unassigned sequence.
            //         == GB18030_CODE -> this is the end of a GB18030 4-byte sequence.
            //         <= NODE_START -> index of the next node in our trie to process next byte.
            //         <= SEQ_START  -> index of the start of a character code sequence, in decodeTableSeq.
            GBK.m_DecodeTables = [];
            GBK.m_DecodeTables[0] = UNASSIGNED_NODE.slice(0); // Create root node.

            // Sometimes a MBCS char corresponds to a sequence of unicode chars. We store them as arrays of integers here. 
            GBK.m_DecodeTableSeq = [];

            // Actual mapping tables consist of chunks. Use them to fill up decode tables.
            for (var i = 0; i < table_gbk.length; i++)
                AddDecodeChunk(table_gbk[i], GBK.m_DecodeTables, GBK.m_DecodeTableSeq, "gbk");

            // Add GB18030 common decode nodes.
            var commonThirdByteNodeIdx = GBK.m_DecodeTables.length;
            GBK.m_DecodeTables.push(UNASSIGNED_NODE.slice(0));

            var commonFourthByteNodeIdx = GBK.m_DecodeTables.length;
            GBK.m_DecodeTables.push(UNASSIGNED_NODE.slice(0));

            // Fill out the tree
            var firstByteNode = GBK.m_DecodeTables[0];
            for (var i = 0x81; i <= 0xFE; i++) {
                var secondByteNode = GBK.m_DecodeTables[NODE_START - firstByteNode[i]];
                for (var j = 0x30; j <= 0x39; j++) {
                    if (secondByteNode[j] === UNASSIGNED) {
                        secondByteNode[j] = NODE_START - commonThirdByteNodeIdx;
                    } else if (secondByteNode[j] > NODE_START) {
                        throw new Error("gb18030 decode tables conflict at byte 2");
                    }

                    var thirdByteNode = GBK.m_DecodeTables[NODE_START - secondByteNode[j]];
                    for (var k = 0x81; k <= 0xFE; k++) {
                        if (thirdByteNode[k] === UNASSIGNED) {
                            thirdByteNode[k] = NODE_START - commonFourthByteNodeIdx;
                        } else if (thirdByteNode[k] === NODE_START - commonFourthByteNodeIdx) {
                            continue;
                        } else if (thirdByteNode[k] > NODE_START) {
                            throw new Error("gb18030 decode tables conflict at byte 3");
                        }

                        var fourthByteNode = GBK.m_DecodeTables[NODE_START - thirdByteNode[k]];
                        for (var l = 0x30; l <= 0x39; l++) {
                            if (fourthByteNode[l] === UNASSIGNED)
                                fourthByteNode[l] = GB18030_CODE;
                        }
                    }
                }
            }

            // Encode tables: Unicode -> DBCS.

            // `encodeTable` is array mapping from unicode char to encoded char. All its values are integers for performance.
            // Because it can be sparse, it is represented as array of buckets by 256 chars each. Bucket can be null.
            // Values: >=  0 -> it is a normal char. Write the value (if <=256 then 1 byte, if <=65536 then 2 bytes, etc.).
            //         == UNASSIGNED -> no conversion found. Output a default char.
            //         <= SEQ_START  -> it's an index in encodeTableSeq, see below. The character starts a sequence.
            GBK.m_EncodeTable = [];

            // `encodeTableSeq` is used when a sequence of unicode characters is encoded as a single code. We use a tree of
            // objects where keys correspond to characters in sequence and leafs are the encoded dbcs values. A special DEF_CHAR key
            // means end of sequence (needed when one sequence is a strict subsequence of another).
            // Objects are kept separately from encodeTable to increase performance.
            GBK.m_EncodeTableSeq = [];

            let skipEncodeChars = {};

            FillEncodeTable(GBK.m_DecodeTables, GBK.m_DecodeTableSeq, GBK.m_EncodeTable, GBK.m_EncodeTableSeq, 0, 0, skipEncodeChars);

            GBK.m_DefaultCharSingleByte = GBK.m_DecodeTables[0][DEFAULT_CHAR_SINGLE.charCodeAt(0)];
            if (GBK.m_DefaultCharSingleByte === UNASSIGNED)
                GBK.m_DefaultCharSingleByte = GBK.m_DecodeTables[0]['?'];
            if (GBK.m_DefaultCharSingleByte === UNASSIGNED)
                GBK.m_DefaultCharSingleByte = "?".charCodeAt(0);
        }
        public static GetBytes(Str: string, BytesBuffer?: Uint8Array, StartIndex?: number, BufferLen?: number): number {

            if (!GBK.m_DecodeTables)
                GBK.InitTables();
            if (BytesBuffer) {
                if (typeof (StartIndex) != "number" || typeof (BufferLen) != "number") {
                    StartIndex = 0;
                    BufferLen = BytesBuffer.length;
                }
            }
            else {
                StartIndex = 0;
                BufferLen = Number.MAX_SAFE_INTEGER;
            }            
            if (Str.length) {
                let CharCode;
                let NextCharCode = -1;
                let leadSurrogate = -1;
                let seqObj = undefined;
                let Index = 0;
                let Pos = { Pos: StartIndex, Len: BufferLen };
                while (true) {
                    // 0. Get next character.
                    if (NextCharCode === -1) {
                        if (Index == Str.length) break;
                        CharCode = Str.charCodeAt(Index++);
                    }
                    else {
                        CharCode = NextCharCode;
                        NextCharCode = -1;
                    }

                    // 1. Handle surrogates.
                    if (0xD800 <= CharCode && CharCode < 0xE000) { // Char is one of surrogates.
                        if (CharCode < 0xDC00) { // We've got lead surrogate.
                            if (leadSurrogate === -1) {
                                leadSurrogate = CharCode;
                                continue;
                            } else {
                                leadSurrogate = CharCode;
                                // Double lead surrogate found.
                                CharCode = UNASSIGNED;
                            }
                        } else { // We've got trail surrogate.
                            if (leadSurrogate !== -1) {
                                CharCode = 0x10000 + (leadSurrogate - 0xD800) * 0x400 + (CharCode - 0xDC00);
                                leadSurrogate = -1;
                            } else {
                                // Incomplete surrogate pair - only trail surrogate found.
                                CharCode = UNASSIGNED;
                            }

                        }
                    }
                    else if (leadSurrogate !== -1) {
                        // Incomplete surrogate pair - only lead surrogate found.
                        NextCharCode = CharCode; CharCode = UNASSIGNED; // Write an error, then current char.
                        leadSurrogate = -1;
                    }

                    // 2. Convert CharCode character.
                    var dbcsCode = UNASSIGNED;
                    if (seqObj !== undefined && CharCode != UNASSIGNED) { // We are in the middle of the sequence
                        var resCode = seqObj[CharCode];
                        if (typeof resCode === 'object') { // Sequence continues.
                            seqObj = resCode;
                            continue;

                        } else if (typeof resCode == 'number') { // Sequence finished. Write it.
                            dbcsCode = resCode;

                        } else if (resCode == undefined) { // Current character is not part of the sequence.

                            // Try default character for this sequence
                            resCode = seqObj[DEF_CHAR];
                            if (resCode !== undefined) {
                                dbcsCode = resCode; // Found. Write it.
                                NextCharCode = CharCode; // Current character will be written too in the next iteration.

                            } else {
                                // TODO: What if we have no default? (resCode == undefined)
                                // Then, we should write first char of the sequence as-is and try the rest recursively.
                                // Didn't do it for now because no encoding has this situation yet.
                                // Currently, just skip the sequence and write current char.
                            }
                        }
                        seqObj = undefined;
                    }
                    else if (CharCode >= 0) {  // Regular character
                        var subtable = GBK.m_EncodeTable[CharCode >> 8];
                        if (subtable !== undefined)
                            dbcsCode = subtable[CharCode & 0xFF];

                        if (dbcsCode <= SEQ_START) { // Sequence start
                            seqObj = GBK.m_EncodeTableSeq[SEQ_START - dbcsCode];
                            continue;
                        }

                        if (dbcsCode == UNASSIGNED && table_gb18030) {
                            // Use GB18030 algorithm to find character(s) to write.
                            var idx = FindIdx(table_gb18030.uChars, CharCode);
                            if (idx != -1) {
                                var dbcsCode = table_gb18030.gbChars[idx] + (CharCode - table_gb18030.uChars[idx]);
                                PushBuffer(BytesBuffer, Pos, 0x81 + Math.floor(dbcsCode / 12600));
                                dbcsCode = dbcsCode % 12600;
                                PushBuffer(BytesBuffer, Pos, 0x30 + Math.floor(dbcsCode / 1260));
                                dbcsCode = dbcsCode % 1260;
                                PushBuffer(BytesBuffer, Pos, 0x81 + Math.floor(dbcsCode / 10));
                                dbcsCode = dbcsCode % 10;
                                PushBuffer(BytesBuffer, Pos, 0x30 + dbcsCode);
                                continue;
                            }
                        }
                    }

                    // 3. Write dbcsCode character.
                    if (dbcsCode === UNASSIGNED)
                        dbcsCode = GBK.m_DefaultCharSingleByte;

                    if (dbcsCode < 0x100) {
                        PushBuffer(BytesBuffer, Pos, dbcsCode);
                    }
                    else if (dbcsCode < 0x10000) {
                        PushBuffer(BytesBuffer, Pos,dbcsCode >> 8);   // high byte
                        PushBuffer(BytesBuffer, Pos,dbcsCode & 0xFF); // low byte
                    }
                    else if (dbcsCode < 0x1000000) {
                        PushBuffer(BytesBuffer, Pos, dbcsCode >> 16);
                        PushBuffer(BytesBuffer, Pos, (dbcsCode >> 8) & 0xFF);
                        PushBuffer(BytesBuffer, Pos, dbcsCode & 0xFF);
                    } else {
                        PushBuffer(BytesBuffer, Pos, dbcsCode >>> 24);
                        PushBuffer(BytesBuffer, Pos, (dbcsCode >>> 16) & 0xFF);
                        PushBuffer(BytesBuffer, Pos, (dbcsCode >>> 8) & 0xFF);
                        PushBuffer(BytesBuffer, Pos, dbcsCode & 0xFF);
                    }
                }

                if (leadSurrogate !== -1 || seqObj !== undefined) {
                    if (seqObj) { // We're in the sequence.
                        dbcsCode = seqObj[DEF_CHAR];
                        if (dbcsCode !== undefined) { // Write beginning of the sequence.
                            if (dbcsCode < 0x100) {
                                PushBuffer(BytesBuffer, Pos, dbcsCode);
                            }
                            else {
                                PushBuffer(BytesBuffer, Pos, dbcsCode >> 8);   // high byte
                                PushBuffer(BytesBuffer, Pos, dbcsCode & 0xFF); // low byte
                            }
                        } else {
                            // See todo above.
                        }
                        seqObj = undefined;
                    }

                    if (leadSurrogate !== -1) {
                        // Incomplete surrogate pair - only lead surrogate found.
                        PushBuffer(BytesBuffer, Pos, GBK.m_DefaultCharSingleByte);
                        leadSurrogate = -1;
                    }
                }

                if (BytesBuffer && Pos.Len >= 1) {
                    BytesBuffer[Pos.Pos] = 0;
                }
                return Pos.Pos - StartIndex;
            }
            return 0;
        }

        public static GetString(BytesBuffer: Uint8Array, StartIndex?: number, BufferLen?: number): string {
            if (!GBK.m_DecodeTables)
                GBK.InitTables();

            if (typeof (StartIndex) != "number" || typeof (BufferLen) != "number") {
                StartIndex = 0;
                BufferLen = BytesBuffer.length;
            }
            let CharCodes = [];
            let Offset = StartIndex;
            let EndIndex = StartIndex + BufferLen;
            let NextIndex = StartIndex;
            let nodeIdx = 0;
            let uCode;

            while (Offset < EndIndex) {

                let curByte = BytesBuffer[Offset++];

                // Lookup in current trie node.
                uCode = GBK.m_DecodeTables[nodeIdx][curByte];

                if (uCode >= 0) {
                    // Normal character, just use it.
                }
                else if (uCode === UNASSIGNED) { // Unknown char.
                    // TODO: Callback with seq.
                    uCode = DEFAULT_CHAR_UNICODE.charCodeAt(0);
                }
                else if (uCode === GB18030_CODE) {
                    if (Offset - 3 >= NextIndex) {
                        var ptr = (BytesBuffer[Offset - 3] - 0x81) * 12600 + (BytesBuffer[Offset - 2] - 0x30) * 1260 + (BytesBuffer[Offset - 1] - 0x81) * 10 + (curByte - 0x30);
                        var idx = FindIdx(table_gb18030.gbChars, ptr);
                        uCode = table_gb18030.uChars[idx] + ptr - table_gb18030.gbChars[idx];
                    } else {
                        for (let i = NextIndex; i < Offset; i++)
                            CharCodes.push(String.fromCharCode(DEFAULT_CHAR_UNICODE.charCodeAt(0)));
                        uCode = DEFAULT_CHAR_UNICODE.charCodeAt(0);
                    }                    
                }
                else if (uCode <= NODE_START) { // Go to next trie node.
                    nodeIdx = NODE_START - uCode;
                    continue;
                }
                else if (uCode <= SEQ_START) { // Output a sequence of chars.
                    var seq = GBK.m_DecodeTableSeq[SEQ_START - uCode];
                    for (var k = 0; k < seq.length - 1; k++) {
                        uCode = seq[k];
                        CharCodes.push(String.fromCharCode(uCode));
                    }
                    uCode = seq[seq.length - 1];
                }
                else
                    throw new Error("iconv-lite internal error: invalid decoding table value " + uCode + " at " + nodeIdx + "/" + curByte);

                // Write the character to buffer, handling higher planes using surrogate pair.
                if (uCode >= 0x10000) {
                    uCode -= 0x10000;
                    var uCodeLead = 0xD800 | (uCode >> 10);
                    CharCodes.push(String.fromCharCode(uCodeLead));

                    uCode = 0xDC00 | (uCode & 0x3FF);
                }
                CharCodes.push(String.fromCharCode(uCode));

                // Reset trie node.
                nodeIdx = 0;
                NextIndex = Offset;
            }

            return CharCodes.join('');
        }
    }
}