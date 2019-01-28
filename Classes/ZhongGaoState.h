#pragma once

#include "base.h"
#include "GameState.h"
#include "UI.h"

class CZhongGaoState : public CRunState
{
public:
	CZhongGaoState(void);
	~CZhongGaoState(void);

	virtual void EnterState();
	virtual void OnUpdate(float dt);
	virtual void OnDraw();
	virtual void OnDestroy();

	void	OpenSelectLanguageTypeUI();
private:
	float		m_fTime;
	UI	*		m_pMainUI;
};
