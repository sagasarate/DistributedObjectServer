#include "stdafx.h"


CDistributedObjectManager::CDistributedObjectManager(void)
{
	m_pDOSObjectManager = NULL;
	m_ObjectPool.SetTag(_T("CDistributedObjectManager"));
}

CDistributedObjectManager::~CDistributedObjectManager(void)
{
}

bool CDistributedObjectManager::Init(CDOSObjectManager* pDOSObjectManager)
{
	m_pDOSObjectManager = pDOSObjectManager;

	const STORAGE_POOL_SETTING& PoolConfig = CDOSConfig::GetInstance()->GetPluginObjectPoolConfig();
	if (m_ObjectPool.Create(PoolConfig))
	{
		Log("原生插件对象池已创建[%u,%u,%u]", PoolConfig.StartSize, PoolConfig.GrowSize, PoolConfig.GrowLimit);
	}
	else
	{
		Log("无法创建[%u,%u,%u]大小的原生插件对象池", PoolConfig.StartSize, PoolConfig.GrowSize, PoolConfig.GrowLimit);
		return false;
	}

	if (CDOSConfig::GetInstance()->GetMonoConfig().Enable)
	{
		const STORAGE_POOL_SETTING& PoolConfig = CDOSConfig::GetInstance()->GetMonoConfig().PluginObjectPoolConfig;
		if (m_ObjectPoolCS.Create(PoolConfig))
		{
			Log("CS插件对象池已创建[%u,%u,%u]", PoolConfig.StartSize, PoolConfig.GrowSize, PoolConfig.GrowLimit);
		}
		else
		{
			Log("无法创建[%u,%u,%u]大小的CS插件对象池", PoolConfig.StartSize, PoolConfig.GrowSize, PoolConfig.GrowLimit);
			return false;
		}
	}

	if (CDOSConfig::GetInstance()->GetJSVMConfig().Enable)
	{
		const STORAGE_POOL_SETTING& PoolConfig = CDOSConfig::GetInstance()->GetJSVMConfig().PluginObjectPoolConfig;
		if (m_ObjectPoolJS.Create(PoolConfig))
		{
			Log("JS插件对象池已创建[%u,%u,%u]", PoolConfig.StartSize, PoolConfig.GrowSize, PoolConfig.GrowLimit);
		}
		else
		{
			Log("无法创建[%u,%u,%u]大小的JS插件对象池", PoolConfig.StartSize, PoolConfig.GrowSize, PoolConfig.GrowLimit);
			return false;
		}
	}
	return true;
}

void CDistributedObjectManager::Destory()
{
	m_ObjectPool.Destory();
	m_pDOSObjectManager = NULL;
}

bool CDistributedObjectManager::RegisterObject(DOS_OBJ_REGISTER_INFO& ObjectRegisterInfo)
{
	if (m_pDOSObjectManager && ObjectRegisterInfo.pObject)
	{
		CDistributedObjectOperator* pObjectOperator = CreateObjectOperator(ObjectRegisterInfo.pObject);
		if (pObjectOperator)
		{
			DOS_OBJECT_REGISTER_INFO RegisterInfo;

			RegisterInfo.ObjectID = ObjectRegisterInfo.ObjectID;
			strncpy_s(RegisterInfo.ObjectTypeName, MAX_OBJECT_TYPE_NAME_LEN, ObjectRegisterInfo.szObjectTypeName, MAX_OBJECT_TYPE_NAME_LEN);
			RegisterInfo.ObjectTypeName[MAX_OBJECT_TYPE_NAME_LEN] = 0;
			RegisterInfo.pObject = pObjectOperator;
			RegisterInfo.Weight = ObjectRegisterInfo.Weight;
			RegisterInfo.ObjectGroupIndex = ObjectRegisterInfo.ObjectGroupIndex;
			RegisterInfo.MsgQueueSize = ObjectRegisterInfo.MsgQueueSize;
			RegisterInfo.MsgProcessLimit = ObjectRegisterInfo.MsgProcessLimit;
			RegisterInfo.Flag = ObjectRegisterInfo.Flag;

			if (m_pDOSObjectManager->RegisterObject(RegisterInfo))
			{
				//LogDebug("成功注册对象0x%llX", pObjectOperator->GetObjectID().ID);
				return true;
			}
			else
			{
				Log("RegisterObject:注册对象0x%llX失败", ObjectRegisterInfo.ObjectID.ID);
			}
		}

	}
	return false;
}

bool CDistributedObjectManager::RegisterObject(DOS_OBJECT_REGISTER_INFO_FOR_CS& ObjectRegisterInfo, MONO_DOMAIN_INFO& MonoDomainInfo)
{
	if (m_pDOSObjectManager && ObjectRegisterInfo.pObject)
	{
		CDistributedObjectOperatorCS* pObjectOperator = CreateObjectOperator(MonoDomainInfo, ObjectRegisterInfo.pObject);
		if (pObjectOperator)
		{
			DOS_OBJECT_REGISTER_INFO RegisterInfo;

			RegisterInfo.ObjectID = ObjectRegisterInfo.ObjectID;
			strncpy_s(RegisterInfo.ObjectTypeName, MAX_OBJECT_TYPE_NAME_LEN, ObjectRegisterInfo.ObjectTypeName, MAX_OBJECT_TYPE_NAME_LEN);
			RegisterInfo.ObjectTypeName[MAX_OBJECT_TYPE_NAME_LEN] = 0;
			RegisterInfo.pObject = pObjectOperator;
			RegisterInfo.Weight = ObjectRegisterInfo.Weight;
			RegisterInfo.ObjectGroupIndex = ObjectRegisterInfo.ObjectGroupIndex;
			RegisterInfo.MsgQueueSize = ObjectRegisterInfo.MsgQueueSize;
			RegisterInfo.MsgProcessLimit = ObjectRegisterInfo.MsgProcessLimit;
			RegisterInfo.Flag = ObjectRegisterInfo.Flag;

			if (m_pDOSObjectManager->RegisterObject(RegisterInfo))
			{
				//LogDebug("成功注册对象0x%llX", pObjectOperator->GetObjectID().ID);
				return true;
			}
			else
			{
				Log("注册对象0x%llX失败", ObjectRegisterInfo.ObjectID.ID);
			}
		}

	}
	return false;
}

bool CDistributedObjectManager::RegisterObject(JSVM::jsvm_context* Context, JSVM::jsvm_value* ObjectRegisterInfo, JSVM_VM_INFO* pParentJSVMInfo)
{
	DOS_OBJECT_REGISTER_INFO RegisterInfo;

	JSVM::jsvm_value* Member = JSVM::jsvm_obj_get(Context, ObjectRegisterInfo, "ObjectID");
	if (Member && JSVM::jsvm_is_object(Member))
		RegisterInfo.ObjectID = JSVM::TypeConvertor<OBJECT_ID>::FromJS(Context, Member);
	if (RegisterInfo.ObjectID.ObjectTypeID == 0)
	{
		LogJSVM("注册信息的ObjectID必须包含ObjectTypeID");
		return false;
	}

	Member = JSVM::jsvm_obj_get(Context, ObjectRegisterInfo, "ObjectTypeName");
	if (Member && JSVM::jsvm_is_string(Member))
		JSVM::jsvm_get_value_string_utf8(Context, Member, RegisterInfo.ObjectTypeName, MAX_OBJECT_TYPE_NAME_LEN);

	Member = JSVM::jsvm_obj_get(Context, ObjectRegisterInfo, "Weight");
	if (Member && JSVM::jsvm_is_int32(Member))
		RegisterInfo.Weight = JSVM::jsvm_get_value_int32(Context, Member);

	Member = JSVM::jsvm_obj_get(Context, ObjectRegisterInfo, "ObjectGroupIndex");
	if (Member && JSVM::jsvm_is_int32(Member))
		RegisterInfo.ObjectGroupIndex = JSVM::jsvm_get_value_int32(Context, Member);

	Member = JSVM::jsvm_obj_get(Context, ObjectRegisterInfo, "MsgQueueSize");
	if (Member && JSVM::jsvm_is_uint32(Member))
		RegisterInfo.MsgQueueSize = JSVM::jsvm_get_value_uint32(Context, Member);

	Member = JSVM::jsvm_obj_get(Context, ObjectRegisterInfo, "MsgProcessLimit");
	if (Member && JSVM::jsvm_is_uint32(Member))
		RegisterInfo.MsgProcessLimit = JSVM::jsvm_get_value_uint32(Context, Member);

	Member = JSVM::jsvm_obj_get(Context, ObjectRegisterInfo, "Flag");
	if (Member && JSVM::jsvm_is_uint32(Member))
		RegisterInfo.Flag = JSVM::jsvm_get_value_uint32(Context, Member);

	Member = JSVM::jsvm_obj_get(Context, ObjectRegisterInfo, "DOSObject");
	if (Member == NULL)
	{
		LogJSVM("注册信息不存在DOSObject成员");
		return false;
	}
	JS_DOS_OBJECT_INIT_INFO Info;
	Info.PluginID = pParentJSVMInfo->PluginID;
	Info.LogChannel = pParentJSVMInfo->LogChannel;
	Info.MainMoudulePath = pParentJSVMInfo->MainMoudulePath;
	Info.ConfigDir = pParentJSVMInfo->ConfigDir;
	Info.LogDir = pParentJSVMInfo->LogDir;

	if (JSVM::jsvm_is_function(Member))
	{
		const char* szClassName = NULL;
		JSVM::jsvm_value* ClassName = JSVM::jsvm_class_get_name(Context, Member);
		if (ClassName)
			szClassName = JSVM::jsvm_get_value_string_utf8_no_buff(Context, ClassName);
		if (szClassName && strlen(szClassName) > 0)
		{
			Info.DOSObjectClassName = szClassName;
		}
		else
		{
			LogJSVM("无法获得要注册的类名");
			return false;
		}
	}
	else if (JSVM::jsvm_is_object(Member))
	{
		if (pParentJSVMInfo->GroupIndex < 0)
		{
			LogJSVM("插件初始化时只能以类来注册DOSObject");
			return false;
		}
		if (RegisterInfo.ObjectGroupIndex != pParentJSVMInfo->GroupIndex)
		{
			LogJSVM("注册信息的ObjectGroupIndex与父对象所在Group不一致，以提供对象方式注册必须一致");
			return false;
		}
		Info.DOSObject = JSVM::jsvm_create_persistent_value(Context, Member);
		if (Info.DOSObject == NULL)
		{
			LogJSVM("持久化DOSObject失败");
			return false;
		}
	}
	else
	{
		LogJSVM("注册信息不存在DOSObject成员类型非法，必须为类或者对象");
		return false;
	}
	

	CDistributedObjectOperatorJS* pObjectOperator = CreateObjectOperator(Info);
	if (pObjectOperator)
	{
		RegisterInfo.pObject = pObjectOperator;
		if (m_pDOSObjectManager->RegisterObject(RegisterInfo))
		{
			LogDebug("成功注册对象(%s)0x%llX", RegisterInfo.ObjectTypeName, RegisterInfo.ObjectID.ID);
			return true;
		}
		else
		{
			Log("注册对象0x%llX失败", RegisterInfo.ObjectID.ID);
		}
	}

	return false;
}

bool CDistributedObjectManager::UnregisterObject(CDistributedObjectOperator* pObjectOperator)
{
	if (m_pDOSObjectManager && pObjectOperator)
	{

		if (m_pDOSObjectManager->UnregisterObject(pObjectOperator->GetObjectID()))
		{
			//LogDebug("成功注销对象0x%llX", pObjectOperator->GetObjectID().ID);
			return true;
		}
		else
		{
			Log("注销对象0x%llX失败",
				pObjectOperator->GetObjectID().ID);
		}
	}
	return false;
}

bool CDistributedObjectManager::UnregisterObject(CDistributedObjectOperatorCS* pObjectOperator)
{
	if (m_pDOSObjectManager && pObjectOperator)
	{

		if (m_pDOSObjectManager->UnregisterObject(pObjectOperator->GetObjectID()))
		{
			//LogDebug("成功注销对象0x%llX", pObjectOperator->GetObjectID().ID);
			return true;
		}
		else
		{
			Log("注销对象0x%llX失败",
				pObjectOperator->GetObjectID().ID);
		}
	}
	return false;
}

bool CDistributedObjectManager::UnregisterObject(CDistributedObjectOperatorJS* pObjectOperator)
{
	if (m_pDOSObjectManager && pObjectOperator)
	{

		if (m_pDOSObjectManager->UnregisterObject(pObjectOperator->GetObjectID()))
		{
			//LogDebug("成功注销对象0x%llX", pObjectOperator->GetObjectID().ID);
			return true;
		}
		else
		{
			Log("注销对象0x%llX失败",
				pObjectOperator->GetObjectID().ID);
		}
	}
	return false;
}

CDistributedObjectOperator* CDistributedObjectManager::CreateObjectOperator(IDistributedObject* pObject)
{
	CDistributedObjectOperator* pObjectOperator = NULL;
	UINT ID = m_ObjectPool.NewObject(&pObjectOperator);
	if (pObjectOperator)
	{
		if (pObjectOperator->Init(this, ID, pObject))
		{
			//成功新建对象%u[%llX],现有对象%u",ID,ObjectID.ID,m_ObjectPool.GetObjectCount());
			return pObjectOperator;
		}
		else
		{
			m_ObjectPool.DeleteObject(ID);
		}
	}
	LogDebug("新建对象失败,现有对象%u", m_ObjectPool.GetObjectCount());
	return NULL;
}
bool CDistributedObjectManager::DeleteObjectOperator(CDistributedObjectOperator* pObjectOperator)
{
	UINT ID = pObjectOperator->GetID();

	if (m_ObjectPool.DeleteObject(ID))
	{
		//LogDebug("删除对象%u,现有对象%u",ID,m_ObjectPool.GetObjectCount());
		return true;
	}
	else
	{
		LogDebug("删除对象%u失败,现有对象%u", ID, m_ObjectPool.GetObjectCount());
	}
	return false;
}

CDistributedObjectOperatorCS* CDistributedObjectManager::CreateObjectOperator(MONO_DOMAIN_INFO& MonoDomainInfo, MonoObject* pObject)
{
	CDistributedObjectOperatorCS* pObjectOperator = NULL;
	UINT ID = m_ObjectPoolCS.NewObject(&pObjectOperator);
	if (pObjectOperator)
	{
		if (pObjectOperator->Init(this, ID, MonoDomainInfo, pObject))
		{
			//LogDebug("成功新建对象%u[%llX],现有对象%u",ID,ObjectID.ID,m_ObjectPool.GetObjectCount());
			return pObjectOperator;
		}
		else
		{
			m_ObjectPool.DeleteObject(ID);
		}
	}
	LogDebug("新建对象失败,现有对象%u", m_ObjectPool.GetObjectCount());
	return NULL;
}
bool CDistributedObjectManager::DeleteObjectOperator(CDistributedObjectOperatorCS* pObjectOperator)
{
	UINT ID = pObjectOperator->GetID();

	if (m_ObjectPoolCS.DeleteObject(ID))
	{
		//LogDebug("删除对象%u,现有对象%u",ID,m_ObjectPool.GetObjectCount());
		return true;
	}
	else
	{
		LogDebug("删除对象%u失败,现有对象%u", ID, m_ObjectPoolCS.GetObjectCount());
	}
	return false;
}

CDistributedObjectOperatorJS* CDistributedObjectManager::CreateObjectOperator(JS_DOS_OBJECT_INIT_INFO& Info)
{
	CDistributedObjectOperatorJS* pObjectOperator = NULL;
	UINT ID = m_ObjectPoolJS.NewObject(&pObjectOperator);
	if (pObjectOperator)
	{
		if (pObjectOperator->Init(this, ID, Info))
		{
			//LogDebug("成功新建对象%u[%llX],现有对象%u",ID,ObjectID.ID,m_ObjectPool.GetObjectCount());
			return pObjectOperator;
		}
		else
		{
			m_ObjectPool.DeleteObject(ID);
		}
	}
	LogDebug("新建对象失败,现有对象%u", m_ObjectPool.GetObjectCount());
	return NULL;
}
bool CDistributedObjectManager::DeleteObjectOperator(CDistributedObjectOperatorJS* pObjectOperator)
{
	UINT ID = pObjectOperator->GetID();

	if (m_ObjectPoolJS.DeleteObject(ID))
	{
		//LogDebug("删除对象%u,现有对象%u",ID,m_ObjectPool.GetObjectCount());
		return true;
	}
	else
	{
		LogDebug("删除对象%u失败,现有对象%u", ID, m_ObjectPoolJS.GetObjectCount());
	}
	return false;
}


void CDistributedObjectManager::PrintObjectCount()
{
	Log("一共有%u个插件对象",
		m_ObjectPool.GetObjectCount());
}

bool CDistributedObjectManager::JSRegisterObject(JSVM::jsvm_context* Context, JSVM::jsvm_value* ObjectRegisterInfo)
{
	JSVM_VM_INFO* pVMInfo = CJSVMHelper::GetInstance()->GetCurJSVM(Context);
	if (pVMInfo)
	{
		return RegisterObject(Context, ObjectRegisterInfo, pVMInfo);
	}
	else
	{
		JSVM::jsvm_throw_exception(Context, "VMInfo not exist on cur thread");
	}
	return false;
}