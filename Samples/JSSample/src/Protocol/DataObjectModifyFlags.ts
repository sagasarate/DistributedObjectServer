﻿import { SHIP_INFO_MODIFY_FLAGS } from "./ShipInfo"
 

export const DOMF_SHIP_INFO_FULL: bigint[]=[
		SHIP_INFO_MODIFY_FLAGS.MF_SHIP_ID|SHIP_INFO_MODIFY_FLAGS.MF_SHIP_TYPE|SHIP_INFO_MODIFY_FLAGS.MF_NAME|SHIP_INFO_MODIFY_FLAGS.MF_POS|SHIP_INFO_MODIFY_FLAGS.MF_HP|SHIP_INFO_MODIFY_FLAGS.MF_ATK];
export const DOMF_SHIP_INFO_FOR_REGISTER: bigint[]=[
		SHIP_INFO_MODIFY_FLAGS.MF_NAME|SHIP_INFO_MODIFY_FLAGS.MF_POS|SHIP_INFO_MODIFY_FLAGS.MF_HP];

