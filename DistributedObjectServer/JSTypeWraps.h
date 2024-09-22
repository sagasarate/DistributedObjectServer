#pragma once

namespace JSVM
{
	template<>
	struct TypeConvertor<OBJECT_ID>
	{
		static bool IsMatch(jsvm_context* context, jsvm_value* Value)
		{
			return jsvm_is_object(Value) || jsvm_is_int64(Value);
		}
		static OBJECT_ID FromJS(jsvm_context* context, jsvm_value* Value)
		{
			OBJECT_ID ObjID;
			if (jsvm_is_int64(Value))
			{
				ObjID.ID = jsvm_get_value_uint64(context, Value);
			}
			else
			{
				jsvm_value* Member = jsvm_obj_get(context, Value, "ID");
				if (Member)
					ObjID.ID = jsvm_get_value_uint64(context, Member);
			}
			return ObjID;
		}
		static jsvm_value* ToJS(jsvm_context* context, OBJECT_ID Value)
		{
			JSVM_VM_INFO* pVMInfo = CJSVMHelper::GetInstance()->GetCurJSVM(context);
			if (pVMInfo)
			{
				if (pVMInfo->PersistentValues[JSVMPersistentValues_OBJECT_ID])
				{
					JSVM::jsvm_value* Class = jsvm_get_value_from_persistent_value(context, pVMInfo->PersistentValues[JSVMPersistentValues_OBJECT_ID]);
					if (Class)
					{
						jsvm_value* Obj = JSVM::jsvm_new_obj(context, Class);
						if (Obj)
						{
							jsvm_value* id_value = jsvm_create_uint64(context, Value.ID);
							if (id_value)
								jsvm_obj_set(context, Obj, "ID", id_value);
							else
								LogJSVM("无法创建uint64z值");
							return Obj;
						}
						else
						{
							LogJSVM("无法创建JS对象:OBJECT_ID");
						}
					}
					else
					{
						LogJSVM("无法获得JS类:OBJECT_ID");
					}
				}
				else
				{
					LogJSVM("JS类:OBJECT_ID未初始化");
				}
			}
			else
			{
				LogJSVM("未能获取到VMInfo");
			}
			return jsvm_create_undefined(context);
		}
	};

	template<>
	struct TypeConvertor<CDOSMessage*>
	{
		static bool IsMatch(jsvm_context* context, jsvm_value* Value)
		{
			return jsvm_is_object(Value);
		}
		static CDOSMessage* FromJS(jsvm_context* context, jsvm_value* Value)
		{
			return (CDOSMessage*)jsvm_obj_get_internal_field(context, Value, (int)JSVM::JSVMInternalFieldIndex::This);
		}
		static jsvm_value* ToJS(jsvm_context* context, CDOSMessage* Value)
		{
			JSVM_VM_INFO* pVMInfo = CJSVMHelper::GetInstance()->GetCurJSVM(context);
			if (pVMInfo)
			{
				if (pVMInfo->PersistentValues[JSVMPersistentValues_IDOSMessage])
				{
					JSVM::jsvm_value* Class = JSVM::jsvm_get_value_from_persistent_value(context, pVMInfo->PersistentValues[JSVMPersistentValues_IDOSMessage]);
					if (Class)
					{
						jsvm_value* Obj = jsvm_new_obj(context, Class);
						if (Obj)
						{
							jsvm_obj_set_internal_field(context, Obj, (int)JSVM::JSVMInternalFieldIndex::This, Value);
							return Obj;
						}
						else
						{
							LogJSVM("无法创建JS对象IDOSMessage");
						}
					}
					else
					{
						LogJSVM("无法获得JS类IDOSMessage");
					}
				}
				else
				{
					LogJSVM("JS类IDOSMessage未初始化");
				}
			}
			else
			{
				LogJSVM("未能获取到VMInfo");
			}
			return jsvm_create_undefined(context);
		}
	};

	template<>
	struct TypeConvertor<CDOSMessagePacket*>
	{
		static bool IsMatch(jsvm_context* context, jsvm_value* Value)
		{
			return jsvm_is_object(Value);
		}
		static CDOSMessagePacket* FromJS(jsvm_context* context, jsvm_value* Value)
		{
			return (CDOSMessagePacket*)jsvm_obj_get_internal_field(context, Value, (int)JSVM::JSVMInternalFieldIndex::This);
		}
		static jsvm_value* ToJS(jsvm_context* context, CDOSMessagePacket* Value)
		{
			JSVM_VM_INFO* pVMInfo = CJSVMHelper::GetInstance()->GetCurJSVM(context);
			if (pVMInfo)
			{
				if (pVMInfo->PersistentValues[JSVMPersistentValues_IDOSMessagePacket])
				{
					JSVM::jsvm_value* Class = JSVM::jsvm_get_value_from_persistent_value(context, pVMInfo->PersistentValues[JSVMPersistentValues_IDOSMessagePacket]);
					if (Class)
					{
						jsvm_value* Obj = jsvm_new_obj(context, Class);
						if (Obj)
						{
							jsvm_obj_set_internal_field(context, Obj, (int)JSVM::JSVMInternalFieldIndex::This, Value);
							return Obj;
						}
						else
						{
							LogJSVM("无法创建JS对象IDOSMessage");
						}
					}
					else
					{
						LogJSVM("无法获得JS类IDOSMessage");
					}
				}
				else
				{
					LogJSVM("JS类IDOSMessage未初始化");
				}
			}
			else
			{
				LogJSVM("未能获取到VMInfo");
			}
			return jsvm_create_undefined(context);
		}
	};
}