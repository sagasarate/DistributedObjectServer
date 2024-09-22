#include "pch.h"

extern "C" UTILS_DLL_EXPORT bool InitPlugin(IDistributedObjectManager* pObjectManager, UINT PluginID, UINT LogChannel, LPCTSTR ConfigDir, LPCTSTR LogDir);
extern "C" UTILS_DLL_EXPORT bool CheckPluginRelease();

CSpaceArea* g_pObject1;
CShipyard* g_pObject2;
UINT g_LogChannel = 0;
UINT g_PluginID = 0;

bool InitPlugin(IDistributedObjectManager* pObjectManager, UINT PluginID, UINT LogChannel, LPCTSTR ConfigDir, LPCTSTR LogDir)
{
	//_CrtSetBreakAlloc(21390);
	g_LogChannel = LogChannel;
	g_PluginID = PluginID;

	


	DOS_OBJ_REGISTER_INFO RegisterInfo;

	{
		g_pObject1 = MONITORED_NEW(_T("CSpaceArea"), CSpaceArea);

		RegisterInfo.pObject = g_pObject1;
		RegisterInfo.ObjectID.ObjectTypeID = OBJECT_TYPE_SPACE_AREA;
		RegisterInfo.szObjectTypeName = "SpaceArea";
		RegisterInfo.Weight = 100;
		pObjectManager->RegisterObject(RegisterInfo);
	}

	{
		g_pObject2 = MONITORED_NEW(_T("Shipyard"), CShipyard);

		RegisterInfo.pObject = g_pObject2;
		RegisterInfo.ObjectID.ObjectTypeID = OBJECT_TYPE_SHIPYARD;
		RegisterInfo.szObjectTypeName = "Shipyard";
		RegisterInfo.Weight = 100;
		pObjectManager->RegisterObject(RegisterInfo);
	}

	return true;
}

bool CheckPluginRelease()
{
	Log("Check Release");
	if(g_pObject1)
	{
		if (g_pObject1->GetUseRef() > 1)
		{
			return false;
		}
		else
		{
			SAFE_RELEASE(g_pObject1);
		}
	}
	return true;
}