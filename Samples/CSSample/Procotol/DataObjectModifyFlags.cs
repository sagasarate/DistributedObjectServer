using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using DOSSystem;
using DOSSample;

public class DATA_OBJECT_MODIFY_FLAGS
{
	public static readonly ulong[] DOMF_SHIP_INFO_FULL={
			(ulong)ShipInfo.SHIP_INFO_MODIFY_FLAGS.MF_SHIP_ID|(ulong)ShipInfo.SHIP_INFO_MODIFY_FLAGS.MF_SHIP_TYPE|(ulong)ShipInfo.SHIP_INFO_MODIFY_FLAGS.MF_NAME|(ulong)ShipInfo.SHIP_INFO_MODIFY_FLAGS.MF_POS|(ulong)ShipInfo.SHIP_INFO_MODIFY_FLAGS.MF_HP|(ulong)ShipInfo.SHIP_INFO_MODIFY_FLAGS.MF_ATK};
	public static readonly ulong[] DOMF_SHIP_INFO_FOR_REGISTER={
			(ulong)ShipInfo.SHIP_INFO_MODIFY_FLAGS.MF_NAME|(ulong)ShipInfo.SHIP_INFO_MODIFY_FLAGS.MF_POS|(ulong)ShipInfo.SHIP_INFO_MODIFY_FLAGS.MF_HP};
	
};