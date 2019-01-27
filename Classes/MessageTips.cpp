#include "MessageTips.h"
#include "UIText.h"
#include "UIDrawable.h"

#define MT_APPEAR_TIME   500  //出现时间
#define MT_SHOW_TIME		1000  // 保持时间
#define MT_HIDE_TIME		500 // 消失时间
#define MT_START_Y			(SCREEN_SIZE.height/5*3-20)
#define MT_END_Y				(SCREEN_SIZE.height/5*4-20)
#define MT_START_SPEED	200.0f	//开始速度
#define MT_ADD_SPEED		(-40)	 //加速度

MessageTips::MessageTips(void)
{
}

MessageTips::~MessageTips(void)
{
}

bool MessageTipsUpdateCallBack(UI *ui, float dt)
{
	if( !ui || !ui->getUserData() )
		return false;
	MSGTips_Data *pTips = (MSGTips_Data*)ui->getUserData();
	cc_timeval now;
	CCTime::gettimeofdayCocos2d(&now,NULL);
	long long timeNow = now.tv_sec * 1000 + now.tv_usec / 1000;
	if( fabs(pTips->m_pUI->getPositionY()-MT_END_Y) < 5 )
	{
		MessageTips::instance().removeFontUI(ui);
		pTips->m_pUI->setPositionY(MT_END_Y);
	}
	else
	{
		pTips->m_fSpeed = pTips->m_fSpeed+dt*MT_ADD_SPEED;
		if( pTips->m_fSpeed <=0 )
			pTips->m_fSpeed = 0;
		pTips->m_pUI->setPositionY(pTips->m_pUI->getPositionY()+pTips->m_fSpeed*dt);
	}

	if( timeNow - pTips->m_timeBegin <= MT_APPEAR_TIME )
	{
		float f = (timeNow - pTips->m_timeBegin)/(float)MT_APPEAR_TIME;
		if( f >= 1.0f ) f = 1.0f;
		//pTips->m_pUI->setColor(ccc4(255,255,255, f*255));
	}
	else if( timeNow - pTips->m_timeBegin >= MT_SHOW_TIME )
	{
		float f = float((timeNow - pTips->m_timeBegin - MT_SHOW_TIME) / (float)MT_HIDE_TIME );
		if( f >= 1.0f )
		{
			pTips->m_pUI->setVisible(false);
		}
		else
		{
			pTips->m_pUI->setColor(
				ccc4(
					255,255,255, (GLbyte)((1.0f-f)*255)
				)
			);
		}
	}
	return false;
}

void MessageTips::unInit()
{
	m_listMSGTips.clear();
}

void MessageTips::removeFontUI(UI *ui)
{
	while(true)
	{
		list<MSGTips_Data>::iterator it = m_listMSGTips.begin();
		if( it == m_listMSGTips.end() )
			return;
		if( it->m_pUI == ui )
		{
			return;
		}
		else
		{
			it->m_pUI->remove();
			it->m_pUI->setVisible(false);
			m_listMSGTips.pop_front();
		}
	}
}

void MessageTips::showMessageTips( string strText)
{
    ccColor3B c = {255,255,255};
	showMessageTips(strText, c);
}

void MessageTips::showMessageTips( string strText, ccColor3B &color )
{
	if( strText.size() == 0 )
		return;

	MessageTips::instance().m_iTipsCounter++;
	char sz[512];
	sprintf( sz, "<C=%d,%d,%d>%s</C>", color.r, color.g, color.b, strText.c_str());
	MSGTips_Data tips;
	tips.m_iIndex = MessageTips::instance().m_iTipsCounter;
	tips.m_pUI = UIManager::Instance()->loadUIWithIndex("MessageTipsUI.ui", MessageTips::instance().m_iTipsCounter, false);
	tips.m_pText = UIText::initUITextWithString(sz, 20, CCSize(0, 40), tAlignCenterX | tAlignCenterY) ;
	tips.m_pUI->setIgnoreTouch(true);
	UI *pBack = tips.m_pUI->findUI("Massagetips1");
	pBack->setImage("chat_gonggao.png");
	//pBack->m_width = tips.m_pText->m_width+20;
	pBack->addChild(tips.m_pText);
	tips.m_pText->setPosition((pBack->m_width-tips.m_pText->m_width)/2, 0);
	tips.m_pUI->Top();
	cc_timeval now;
	CCTime::gettimeofdayCocos2d( &now, NULL );
	tips.m_timeBegin = now.tv_sec * 1000 + now.tv_usec / 1000;
	tips.m_pUI->setUpdateFunc(MessageTipsUpdateCallBack);
	tips.m_pUI->setPosition(SCREEN_SIZE.width/2, MT_START_Y);
	tips.m_fSpeed = MT_START_SPEED;
	MessageTips::instance().m_listMSGTips.push_back(tips);
	tips.m_pUI->setUserData(&(MessageTips::instance().m_listMSGTips.back()));
}