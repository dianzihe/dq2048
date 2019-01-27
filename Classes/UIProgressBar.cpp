#include "UIProgressBar.h"
#include "UI.h"
//#include "UIBatchRenderer.h"

CProgressBar::CProgressBar()
{

}

CProgressBar::CProgressBar(const char *baseFrameName)
{
}

CProgressBar::~CProgressBar()
{
}

//��ʼ��һ��������������--�ļ�������С���������ǵݼ����Ƿ����֡���Զ����½���
CProgressBar* CProgressBar::initPB( const char *fileName, CCSize baseSize, int changeType, bool autoUpdate )
{
	CProgressBar *pProgressBar = new CProgressBar();
	pProgressBar->m_type = UI_BASE;
	pProgressBar->m_width = (int)baseSize.width;
	pProgressBar->m_height = (int)baseSize.height;
	pProgressBar->autorelease();
	pProgressBar->setIgnoreTouch(true);
	
	pProgressBar->m_progressTimer = new CCProgressTimer;
	pProgressBar->m_progressTimer->autorelease();
	pProgressBar->m_progressTimer->initWithSprite(Sprite::createWithSpriteFrameName(fileName));
	if(pProgressBar->m_progressTimer){
		CCSize progressSize = pProgressBar->m_progressTimer->getContentSize();
		float xScale = ((float)baseSize.width)/progressSize.width;
		float yScale = ((float)baseSize.height)/progressSize.height;
		pProgressBar->m_progressTimer->setScaleX(xScale);
		pProgressBar->m_progressTimer->setScaleY(yScale);

		pProgressBar->m_progressTimer->setPosition(pProgressBar->m_width/2, pProgressBar->m_height/2);
		pProgressBar->m_progressTimer->setType(kCCProgressTimerTypeBar);
		pProgressBar->addChild(pProgressBar->m_progressTimer);
	}
	pProgressBar->m_changeType = changeType;
	pProgressBar->m_autoUpdate = autoUpdate;

	return pProgressBar;
}

//��ʼ��һ��������������--�������������ļ������������ļ������������ǵݼ����Ƿ����֡���Զ����½���
CProgressBar* CProgressBar::initPBHasBaseFrame( const char *baseFrameName, const char *pszFileName, int changeType, bool autoUpdate )
{
	CProgressBar *pProgressBar = new CProgressBar();
	pProgressBar->m_type = UI_BASE;
	pProgressBar->setImageByFullPath(baseFrameName, true);
	pProgressBar->autorelease();
	pProgressBar->setIgnoreTouch(true);
	pProgressBar->m_progressTimer = new CCProgressTimer;
	pProgressBar->m_progressTimer->autorelease();
	pProgressBar->m_progressTimer->initWithSprite(Sprite::createWithSpriteFrameName(pszFileName));
	if(pProgressBar->m_progressTimer){
		pProgressBar->m_progressTimer->setAnchorPoint(ccp(0, 1));
		pProgressBar->m_progressTimer->setPosition(0,pProgressBar->m_height);
		pProgressBar->m_progressTimer->setType(kCCProgressTimerTypeBar);
		pProgressBar->addChild(pProgressBar->m_progressTimer);
	}
	pProgressBar->m_changeType = changeType;
	pProgressBar->m_autoUpdate = autoUpdate;

	return pProgressBar;
}

void CProgressBar::updatePercentage(float dt, int flag)
{
	//m_speed -= dt;
	if(flag == 0)
	{
		m_progressTimer->setPercentage(m_progressTimer->getPercentage() + (dt*100)/m_speed);
	}
	else
	{
		m_progressTimer->setPercentage(m_progressTimer->getPercentage() - (dt*100)/m_speed);
	}
	
}

void CProgressBar::startProgress(float speed, float percentage)	//��ʼ����������������ʾ
{
	setVisible(true);
	m_speed = speed;
	m_progressTimer->setPercentage(percentage);
}

void CProgressBar::completeProgress(ActorID objDI, int dataID)	//��������ɣ�����������
{
	setVisible(false);
	m_speed = 0;
}

void CProgressBar::interruptProgress()			//��Ͻ�����������������
{
	setVisible(false);
	m_speed = 0;
}

void CProgressBar::update(float t)		//��֡����
{
	if(m_autoUpdate){
		if(m_changeType == ChangeType_Increase && m_progressTimer->getPercentage() >= 100){
			//completeProgress();
		}
		else if(m_changeType == ChangeType_Reduce && m_progressTimer->getPercentage() <= 0){
			//completeProgress();
		}

		if(m_changeType == ChangeType_Increase && m_progressTimer->getPercentage() < 100 ){
			updatePercentage(t, m_changeType);
		}
		else if(m_changeType == ChangeType_Reduce && m_progressTimer->getPercentage()> 0){
			updatePercentage(t, m_changeType);
		}
	}
}

void CProgressBar::setPercentage(float percentage)		//���ý������ٷֱ�1f~100f
{
	m_progressTimer->setPercentage(percentage);
}

float CProgressBar::getPercentage()						//��ȡ�������ٷֱ�
{
	return m_progressTimer->getPercentage();
}

void CProgressBar::setType(CCProgressTimerType type)		//���ý���������
{
	m_progressTimer->setType(type);
}

void CProgressBar::setDisplayText(const char *text)		//���ý���������ʾ������
{
	this->setText(text);
}

void CProgressBar::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
#ifdef UI_BATCH_RENDERER
	if (m_progressTimer->getPercentage() != 0)		
	{
		UIBatchRenderer::instance()->flush();
	}
#endif
	UI::draw(renderer, transform, flags);
}