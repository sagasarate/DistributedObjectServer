#pragma once

class CDistributedObjectManager :
	public IDistributedObjectManager
{
protected:
	CDOSObjectManager* m_pDOSObjectManager;

	CThreadSafeIDStorage<CDistributedObjectOperator>	m_ObjectPool;
	CThreadSafeIDStorage<CDistributedObjectOperatorCS>	m_ObjectPoolCS;
	CThreadSafeIDStorage<CDistributedObjectOperatorJS>	m_ObjectPoolJS;
public:
	CDistributedObjectManager(void);
	~CDistributedObjectManager(void);

	bool Init(CDOSObjectManager* pDOSObjectManager);
	virtual void Destory();


	virtual bool RegisterObject(DOS_OBJ_REGISTER_INFO& ObjectRegisterInfo);
	bool RegisterObject(DOS_OBJECT_REGISTER_INFO_FOR_CS& ObjectRegisterInfo, MONO_DOMAIN_INFO& MonoDomainInfo);
	bool RegisterObject(JSVM::jsvm_context* Context, JSVM::jsvm_value* ObjectRegisterInfo, JSVM_VM_INFO* pParentJSVMInfo);

	bool UnregisterObject(CDistributedObjectOperator* pObjectOperator);
	bool UnregisterObject(CDistributedObjectOperatorCS* pObjectOperator);
	bool UnregisterObject(CDistributedObjectOperatorJS* pObjectOperator);

	CDistributedObjectOperator* CreateObjectOperator(IDistributedObject* pObject);
	bool DeleteObjectOperator(CDistributedObjectOperator* pObjectOperator);

	CDistributedObjectOperatorCS* CreateObjectOperator(MONO_DOMAIN_INFO& MonoDomainInfo, MonoObject* pObject);
	bool DeleteObjectOperator(CDistributedObjectOperatorCS* pObjectOperator);

	CDistributedObjectOperatorJS* CreateObjectOperator(JS_DOS_OBJECT_INIT_INFO& Info);
	bool DeleteObjectOperator(CDistributedObjectOperatorJS* pObjectOperator);

	void PrintObjectCount();

	bool JSRegisterObject(JSVM::jsvm_context* Context, JSVM::jsvm_value* ObjectRegisterInfo);

};

