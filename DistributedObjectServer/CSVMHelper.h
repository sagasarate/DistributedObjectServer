#pragma once

#define MONO_ASSEMBLY_DOSSYSTEM							"DOSSystem.dll"
#define MONO_NAME_SPACE_DOSSYSTEM						"DOSSystem"
#define MONO_CLASS_NAME_OBJECT_ID						"OBJECT_ID"
#define MONO_CLASS_NAME_DISTRIBUTED_OBJECT_OPERATOR		"DistributedObjectOperator"
#define MONO_CLASS_NAME_DOS_OBJ_REGISTER_INFO			"DOS_OBJ_REGISTER_INFO"

#define MONO_CLASS_FIELD_NAME_OBJECT_ID_ID				"ID"

#define MONO_CLASS_FIELD_NAME_DORI_OBJECT_ID			"ObjectID"
#define MONO_CLASS_FIELD_NAME_DORI_OBJECT_TYPE_NAME		"ObjectTypeName"
#define MONO_CLASS_FIELD_NAME_DORI_WEIGHT				"Weight"
#define MONO_CLASS_FIELD_NAME_DORI_OBJECT_GROUP_INDEX	"ObjectGroupIndex"
#define MONO_CLASS_FIELD_NAME_DORI_MSG_QUEUE_SIZE		"MsgQueueSize"
#define MONO_CLASS_FIELD_NAME_DORI_MSG_PROCESS_LIMIT	"MsgProcessLimit"
#define MONO_CLASS_FIELD_NAME_DORI_FLAG					"Flag"
#define MONO_CLASS_FIELD_NAME_DORI_OBJECT				"Object"


#define MONO_CLASS_METHOD_NAME_DO_INITIALIZE				"Initialize"
#define MONO_CLASS_METHOD_NAME_DO_DESTORY					"Destory"
#define MONO_CLASS_METHOD_NAME_DO_ONPRETRANSLATEMESSAGE		"OnPreTranslateMessage"
#define MONO_CLASS_METHOD_NAME_DO_ONMESSAGE					"OnMessage"
#define MONO_CLASS_METHOD_NAME_DO_ONSYSTEMMESSAGE			"OnSystemMessage"
#define MONO_CLASS_METHOD_NAME_DO_ONCONCERNEDOBJECTLOST		"OnConcernedObjectLost"
#define MONO_CLASS_METHOD_NAME_DO_ONFINDOBJECT				"OnFindObject"
#define MONO_CLASS_METHOD_NAME_DO_ONOBJECTREPORT			"OnObjectReport"
#define MONO_CLASS_METHOD_NAME_DO_ONPROXYOBJECTIPREPORT		"OnProxyObjectIPReport"
#define MONO_CLASS_METHOD_NAME_DO_ONSHUTDOWN				"OnShutDown"
#define MONO_CLASS_METHOD_NAME_DO_UPDATE					"Update"
#define MONO_CLASS_METHOD_NAME_DO_ONEXCEPTION				"OnException"
#define MONO_CLASS_METHOD_NAME_DO_ONCONSOLECOMMAND			"OnConsoleCommand"
#define MONO_CLASS_METHOD_NAME_DO_ONTIMER					"OnTimer"
#define MONO_CLASS_METHOD_NAME_DO_ONTIMERRELEASE			"OnTimerRelease"

#define MONO_CLASS_METHOD_PARAM_DO_INITIALIZE				1
#define MONO_CLASS_METHOD_PARAM_DO_DESTORY					0
#define MONO_CLASS_METHOD_PARAM_DO_ONPRETRANSLATEMESSAGE	4
#define MONO_CLASS_METHOD_PARAM_DO_ONMESSAGE				4
#define MONO_CLASS_METHOD_PARAM_DO_ONSYSTEMMESSAGE			4
#define MONO_CLASS_METHOD_PARAM_DO_ONCONCERNEDOBJECTLOST	1
#define MONO_CLASS_METHOD_PARAM_DO_ONFINDOBJECT				1
#define MONO_CLASS_METHOD_PARAM_DO_ONOBJECTREPORT			2
#define MONO_CLASS_METHOD_PARAM_DO_ONPROXYOBJECTIPREPORT	3
#define MONO_CLASS_METHOD_PARAM_DO_ONSHUTDOWN				2
#define MONO_CLASS_METHOD_PARAM_DO_UPDATE					1
#define MONO_CLASS_METHOD_PARAM_DO_ONEXCEPTION				1
#define MONO_CLASS_METHOD_PARAM_DO_ONCONSOLECOMMAND			1
#define MONO_CLASS_METHOD_PARAM_DO_ONTIMER					3
#define MONO_CLASS_METHOD_PARAM_DO_ONTIMERRELEASE			2


#define MONO_CLASS_METHOD_NAME_OBJECT_ID_CTOR			".ctor"
#define MONO_CLASS_METHOD_PARAM_OBJECT_ID_CTOR			1
#define MONO_CLASS_METHOD_NAME_DOO_CTOR					".ctor"
#define MONO_CLASS_METHOD_PARAM_DOO_CTOR				1

#define MONO_DOMAIN_FINALIZE_TIMEOUT					(5*60*1000)

struct MONO_CLASS_INFO_DORI
{
	MonoClass* pClass;
	MonoClassField* pFeildObjectID;
	MonoClassField* pFeildObjectTypeName;
	MonoClassField* pFeildWeight;
	MonoClassField* pFeildObjectGroupIndex;
	MonoClassField* pFeildMsgQueueSize;
	MonoClassField* pFeildMsgProcessLimit;
	MonoClassField* pFeildFlag;
	MonoClassField* pFeildObject;
	MONO_CLASS_INFO_DORI()
	{
		pClass = NULL;
		pFeildObjectID = NULL;
		pFeildObjectTypeName = NULL;
		pFeildWeight = NULL;
		pFeildObjectGroupIndex = NULL;
		pFeildMsgQueueSize = NULL;
		pFeildMsgProcessLimit = NULL;
		pFeildFlag = NULL;
		pFeildObject = NULL;
	}
	bool IsValid()
	{
		return (pClass != NULL) && (pFeildObjectID != NULL) && (pFeildObjectTypeName != NULL) && (pFeildWeight != NULL) &&
			(pFeildObjectGroupIndex != NULL) && (pFeildMsgQueueSize != NULL) &&
			(pFeildMsgProcessLimit != NULL) && (pFeildFlag != NULL) && (pFeildObject != NULL);
	}
};

struct MONO_DOMAIN_INFO
{
	MonoDomain*				pMonoDomain;
	MonoAssembly*			pCSPluginAssembly;
	MonoAssembly*			pMonoAssembly_DOSSystem;
	MonoClass*				pMonoClass_ObjectID;
	MonoClassField*			pMonoClassField_ObjectID_ID;
	MonoMethod*				pMonoClassMethod_ObjectID_Ctor;
	MonoClass*				pMonoClass_DistributedObjectOperator;
	MonoMethod*				pMonoClassMethod_DistributedObjectOperator_Ctor;
	MONO_CLASS_INFO_DORI	MonoClassInfo_DORI;

	MONO_DOMAIN_INFO()
	{
		pMonoDomain = NULL;
		pCSPluginAssembly = NULL;
		pMonoAssembly_DOSSystem = NULL;
		pMonoClass_ObjectID = NULL;
		pMonoClassField_ObjectID_ID = NULL;
		pMonoClassMethod_ObjectID_Ctor = NULL;
		pMonoClass_DistributedObjectOperator = NULL;
		pMonoClassMethod_DistributedObjectOperator_Ctor = NULL;
	}
};

struct DOS_OBJECT_REGISTER_INFO_FOR_CS
{
	OBJECT_ID				ObjectID;
	CEasyString				ObjectTypeName;
	int						Weight;
	int						ObjectGroupIndex;
	UINT					MsgQueueSize;
	UINT					MsgProcessLimit;
	UINT					Flag;
	MonoObject* pObject;
};



struct MONO_CLASS_INFO_DO
{
	MonoClass* pClass;
	MonoMethod* pInitializeMethod;
	MonoMethod* pDestoryMethod;
	MonoMethod* pOnPreTranslateMessageMethod;
	MonoMethod* pOnMessageMethod;
	MonoMethod* pOnSystemMessageMethod;
	MonoMethod* pOnConcernedObjectLostMethod;
	MonoMethod* pOnFindObjectMethod;
	MonoMethod* pOnObjectReportMethod;
	MonoMethod* pOnProxyObjectIPReportMethod;
	MonoMethod* pOnShutDownMethod;
	MonoMethod* pUpdateMethod;
	MonoMethod* pOnExceptionMethod;
	MonoMethod* pOnConsoleCommandMethod;
	MonoMethod* pOnTimerMethod;
	MonoMethod* pOnTimerReleaseMethod;
	MONO_CLASS_INFO_DO()
	{
		pClass = NULL;
		pInitializeMethod = NULL;
		pDestoryMethod = NULL;
		pOnPreTranslateMessageMethod = NULL;
		pOnMessageMethod = NULL;
		pOnSystemMessageMethod = NULL;
		pOnConcernedObjectLostMethod = NULL;
		pOnFindObjectMethod = NULL;
		pOnObjectReportMethod = NULL;
		pOnProxyObjectIPReportMethod = NULL;
		pOnShutDownMethod = NULL;
		pUpdateMethod = NULL;
		pOnExceptionMethod = NULL;
		pOnConsoleCommandMethod = NULL;
		pOnTimerMethod = NULL;
		pOnTimerReleaseMethod = NULL;
	}
	//	bool IsValid()
	//	{
	//		return pClass != NULL&&pInitializeMethod != NULL&&pDestoryMethod != NULL&&
	//			pOnPreTranslateMessageMethod != NULL&&pOnMessageMethod != NULL&&
	//			pOnSystemMessageMethod != NULL&&pOnConcernedObjectLostMethod != NULL&&
	//			pOnFindObjectMethod != NULL&&pOnObjectReportMethod != NULL&&
	//			pOnProxyObjectIPReportMethod != NULL&&pOnShutDownMethod != NULL&&
	//			pUpdateMethod != NULL;
	//	}
};

class CDistributedObjectOperatorCS;

class CCSVMHelper :public CStaticObject4<CCSVMHelper>
{
protected:
	MonoDomain* m_pMonoMainDomain;

	CEasyTimer	m_MonoBaseGCTimer;
	CEasyTimer	m_MonoAdvanceGCTimer;
	CEasyTimer	m_MonoFullGCTimer;
public:
	CCSVMHelper();
	~CCSVMHelper();
	bool Init();
	void Release();
	int Update(int ProcessLimit = DEFAULT_SERVER_PROCESS_PACKET_LIMIT);

	bool InitPluginDomain(MONO_DOMAIN_INFO& MonoDomainInfo, LPCTSTR szName);
	bool ReleasePluginDomain(MONO_DOMAIN_INFO& MonoDomainInfo);

	bool IsMonoArrayElementTypeMatch(MonoArray* pArray, MonoClass* pClass);
	MonoArray* MonoCreateByteArray(MONO_DOMAIN_INFO& DomainInfo, const void* pData, size_t DataSize);
	BYTE* MonoGetByteArray(MonoArray* pArray, size_t& DataSize);
	MSG_ID_TYPE* MonoGetMsgIDArray(MonoArray* pArray, size_t& DataSize);
	//MonoObject * MonoCreateDOSObjectID(MONO_DOMAIN_INFO& DomainInfo, OBJECT_ID ObjectID);
	OBJECT_ID MonoGetObjectID(MONO_DOMAIN_INFO& DomainInfo, MonoObject* pObjectID);
	MonoString* MonoCreateString(MONO_DOMAIN_INFO& DomainInfo, LPCTSTR szStr, size_t StrLen);
	MonoObject* MonoCreateDistributedObjectOperator(MONO_DOMAIN_INFO& DomainInfo, CDistributedObjectOperatorCS* pOperator);
	bool MonoGetDORI(MONO_DOMAIN_INFO& DomainInfo, MonoObject* pObject, DOS_OBJECT_REGISTER_INFO_FOR_CS& RegisterInfo);
	bool MonoGetObjectIDList(MONO_DOMAIN_INFO& DomainInfo, MonoArray* pArray, CEasyArray<OBJECT_ID>& ObjectIDList);

	void DumpMonoClass(MonoImage* pMonoImage, LPCTSTR szNameSpace, LPCTSTR szClassName);
	void DumpAssemblys();
	MonoClass* MonoGetClass(MonoImage* pMonoImage, LPCTSTR szNameSpace, LPCTSTR szClassName);
	MonoClassField* MonoGetClassField(MonoClass* pMonoClass, LPCTSTR szFieldName, bool beRecursive = true);
	MonoMethod* MonoGetClassMethod(MonoClass* pMonoClass, LPCTSTR szMethodName, int ParamCount, bool beRecursive = true);

	void ProcessMonoException(MonoObject* pException);

protected:
	static void MonoLog(const char* log_domain, const char* log_level, const char* message, mono_bool fatal, void* user_data);
	static void MonoPrint(const char* string, mono_bool is_stdout);
	static void MonoPrintErr(const char* string, mono_bool is_stdout);
	
	void RegisterMonoFunctions();


	static void MonoInternalCallLog(UINT LogChannel, int LogLevel, MonoString* pTag, MonoString* pMsg);

	
};

