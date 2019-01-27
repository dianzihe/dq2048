#pragma once

#include "base.h"
#include "UI.h"

enum ChangeType
{
	ChangeType_Increase = 0,
	ChangeType_Reduce,
};

enum CompleteEvent
{
	EVENT_INCREASE = 0,
	EVENT_REDUCE,
};

typedef void (*TBarCompleteFun)( UI* ui, void* data );

class CProgressBar : public UI
{
public:
	CProgressBar();
	CProgressBar(const char *baseFrameName);
	virtual ~CProgressBar();

	void updatePercentage(float dt, int flag);

	static CProgressBar* initPB(const char *fileName, CCSize baseSize, int changeType, bool autoUpdate);
	static CProgressBar* initPBHasBaseFrame(const char *baseFrameName, const char *pszFileName, int changeType, bool autoUpdate);

	void startProgress(float collectTime, float percentage);
	void completeProgress(ActorID objID, int dataID);
	void interruptProgress();

	virtual void update(float t);

	void setPercentage(float percentage);
	float getPercentage();
	void setType(CCProgressTimerType type);
	void setDisplayText(const char *text);

	virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;

	//void setCompleteEvent(CompleteEvent type, TBarCompleteFun fun, void* data=NULL);

	//void SetPROFun( TBarCompleteFun pFun){ PROFun = pFun; };
	//TBarCompleteFun GetPROFun(){ return PROFun; };

public:
	CCProgressTimer *m_progressTimer;

private:
	
	float m_speed;
	string m_text;
	int m_changeType;
	bool m_autoUpdate;
	//TBarCompleteFun PROFun;
};