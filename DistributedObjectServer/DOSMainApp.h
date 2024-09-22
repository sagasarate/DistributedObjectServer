﻿#pragma once



// CMainApp:
// 有关此类的实现，请参阅 DBProxy.cpp
//

class CDOSMainApp : public CServerApp
{
protected:
	bool		m_IsConfigLoaded;
public:
	CDOSMainApp();

	// 重写
public:
	//virtual bool InitInstance();
	//virtual int ExitInstance();

	virtual bool OnStartUp() override;
	virtual void OnShutDown() override;

	// 实现
	virtual void OnSetServiceName();

	void ReInitSignals()
	{
#ifndef WIN32
		InitSignals();
#endif
	}

};

extern CDOSMainApp theApp;
