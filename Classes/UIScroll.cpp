#include "UIScroll.h"
#include "UIManager.h"

#ifdef _MSC_VER
	#pragma warning( disable: 4244 )
#endif
void OnUIScrollAddBtnUP( UI* ui, void* data )
{
	if( !ui )
		return;
	UIScroll *p = (UIScroll*)ui->getUserData();
	p->Add();
}

void OnUIScrollReduceBtnUP( UI* ui, void* data )
{
	if(!ui)
		return;
	UIScroll *p = (UIScroll*)ui->getUserData();
	p->Reduce();
}

void onScrollBarDrag(UI *ui, void *data)
{
	UIScroll *pScroll = (UIScroll*)data;
	pScroll->onBarDrag();
}

void onScrollBarDown(UI *ui, void *data)
{
	UIManager::getInstance()->setOnlyMessage(ui);
	UIScroll *pScroll = (UIScroll*)data;
	pScroll->m_isDrag = false;
}

void onScrollBarUp(UI *ui, void *data)
{
	UIManager::getInstance()->setOnlyMessage(NULL);
	UIScroll *pScroll = (UIScroll*)data;
	pScroll->m_isDrag = false;
}



UIScroll::UIScroll(void):
m_ScrollType(SCROLL_TYPE_X),
m_iMaxNum(20),
m_iCurrentNum(0),
m_pBarBtn(NULL),
m_pAddBtn(NULL),
m_pReduceBtn(NULL),
m_fAddBtnTime(0.0f),
m_fReduceBtnTime(0.0f),
m_isDrag(false)
{
}

UIScroll::~UIScroll(void)
{
}

bool UIScroll::init()
{
	if( m_pBarBtn == NULL )
	{
		UIData stData;
		stData.align = 4;
		stData.tag = 0;
		stData.name = "bar_btn";
		stData.picName = "Bar11.png";
		stData.text = "";
		stData.type = UI_BUTTON;
		stData.x = 0;
		stData.y = 0;
		m_pBarBtn = stData.createUI();
		m_pBarBtn->setImage("Bar11.png",true);
		switch(m_ScrollType)
		{
		case SCROLL_TYPE_X:
			m_pBarBtn->setPosition(ccp( m_width/2-m_pBarBtn->m_width/2, -2 ));
			break;
		case SCROLL_TYPE_Y:
			m_pBarBtn->setPosition(ccp( 0, m_height/2-m_pBarBtn->m_width/2 ));
			m_pBarBtn->setRotation( 90 );
			break;
		};
		addChild(m_pBarBtn);
		m_pBarBtn->setEvent(EVENT_DOWN, onScrollBarDown, this);
		m_pBarBtn->setEvent(EVENT_UP, onScrollBarUp, this);
		m_pBarBtn->setEvent(EVENT_DRAG, onScrollBarDrag, this);
	}

	if( m_pAddBtn == NULL )
	{
		UIData stData;
		stData.align = 4;
		stData.tag = 0;
		stData.name = "add_btn";
		stData.picName = "AddButton1.png";
		stData.text = "";
		stData.type = UI_BUTTON;
		stData.x = 0;
		stData.y = 0;
		m_pAddBtn = stData.createUI();
		m_pAddBtn->setImage("AddButton1.png",true);
		switch(m_ScrollType)
		{
		case SCROLL_TYPE_X:
			m_pAddBtn->setPosition(ccp( m_width-m_pAddBtn->m_width, 0 ));
			break;
		case SCROLL_TYPE_Y:
			m_pAddBtn->setPosition(ccp( 0, 0 ));
			break;
		};
		m_pAddBtn->setUserData(this);
		m_pAddBtn->setEvent(EVENT_UP, OnUIScrollAddBtnUP);
		addChild(m_pAddBtn);
	}

	if( m_pReduceBtn == NULL )
	{
		UIData stData;
		stData.align = 4;
		stData.tag = 0;
		stData.name = "reduce_btn";
		stData.picName = "ReduceButton1.png";
		stData.text = "";
		stData.type = UI_BUTTON;
		stData.x = 0;
		stData.y = 0;
		m_pReduceBtn = stData.createUI();
		m_pReduceBtn->setImage("ReduceButton1.png",true);
		switch(m_ScrollType)
		{
		case SCROLL_TYPE_X:
			m_pReduceBtn->setPosition(ccp( 0, 0 ));
			break;
		case SCROLL_TYPE_Y:
			m_pReduceBtn->setPosition(ccp( 0, m_height/2 ));
			break;
		};
		m_pReduceBtn->setUserData(this);
		m_pReduceBtn->setEvent(EVENT_UP, OnUIScrollReduceBtnUP);
		addChild(m_pReduceBtn);
	}
	return true;
}

void UIScroll::setShowAddAndReduce( bool b )
{
	m_bIsShowAddAndReduce = b;
	if(m_pAddBtn)
		m_pAddBtn->setVisible( m_bIsShowAddAndReduce );
	if(m_pReduceBtn)
		m_pReduceBtn->setVisible( m_bIsShowAddAndReduce );
}


void UIScroll::Add()
{
	m_iCurrentNum = ((m_iCurrentNum+1) > m_iMaxNum) ?  m_iMaxNum : m_iCurrentNum+1;
}

void UIScroll::Reduce()
{
	m_iCurrentNum = ((m_iCurrentNum-1) < 0 ) ? 0 : m_iCurrentNum-1;
}

void UIScroll::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
	if(m_pAddBtn->GetHoldTime() > -1)
	{
		if( m_pAddBtn->GetHoldTime() - m_fAddBtnTime >= 0.1f )
		{
			m_fAddBtnTime = m_pAddBtn->GetHoldTime();
			Add();
		}
	}
	else
	{
		m_fAddBtnTime = 0.0f;
	}
	if(m_pReduceBtn->GetHoldTime() > -1)
	{
		if( m_pReduceBtn->GetHoldTime() - m_fReduceBtnTime >= 0.1f )
		{
			m_fReduceBtnTime = m_pReduceBtn->GetHoldTime();
			Reduce();
		}
	}
	else
	{
		m_fReduceBtnTime = 0.0f;
	}

	if( !m_isDrag && m_bIsShowAddAndReduce )
	{
		float posX,posY;
		float current = 0;
		if( m_iMaxNum > 0 )
			current = (float)m_iCurrentNum/m_iMaxNum;
		switch(m_ScrollType)
		{
		case SCROLL_TYPE_X:
			{
				posY = -2.0f;
				if( m_bIsShowAddAndReduce )
					posX = (m_width - m_pBarBtn->m_width - m_pAddBtn->m_width - m_pReduceBtn->m_width)*current+m_pReduceBtn->m_width;
				else
					posX = (m_width - m_pBarBtn->m_width)*current;
			}
			break;
		case SCROLL_TYPE_Y:
			{
				posX = -2.0f;
				if( m_bIsShowAddAndReduce )
					posY = -(m_height-m_pBarBtn->m_height-m_pAddBtn->m_height-m_pReduceBtn->m_height)*current-m_pReduceBtn->m_height;
				else
					posY = -(m_height-m_pBarBtn->m_width)*current;
			}
			break;
		};
		m_pBarBtn->setPosition(ccp(posX,posY));
	}
	UI::draw(renderer, transform, flags);
}


void UIScroll::onBarDrag()
{
	m_isDrag = true;
	CCTouch *touch = UIManager::getInstance()->getCurTouch();
	switch( m_ScrollType )
	{
	case SCROLL_TYPE_X:
		{
			int x = touch->getLocationInView().x - touch->getPreviousLocationInView().x + m_pBarBtn->getPositionX();
			float current = 0.0f;
			if(m_bIsShowAddAndReduce)
			{
				if( x > m_width-m_pBarBtn->m_width-m_pAddBtn->m_width)
					x = m_width-m_pBarBtn->m_width-m_pAddBtn->m_width;
				if( x< m_pReduceBtn->m_width )
					x = m_pReduceBtn->m_width;
				m_pBarBtn->setPositionX(x);
				current = (float)(x-m_pReduceBtn->m_width)/(m_width-m_pBarBtn->m_width-m_pAddBtn->m_width-m_pReduceBtn->m_width);
			}
			else
			{
				if( x >  m_width-m_pBarBtn->m_width )
					x =  m_width-m_pBarBtn->m_width;
				if( x < 0 )
					x = 0;
				m_pBarBtn->setPositionX(x);
				current = (float)x / (m_width-m_pBarBtn->m_width);
			}
			m_iCurrentNum = m_iMaxNum * current;
		}
		break;
	case SCROLL_TYPE_Y:
		{
			int y = touch->getLocationInView().y - touch->getPreviousLocationInView().y + m_pBarBtn->getPositionY();
			if( y < -(m_height-m_pBarBtn->m_width-m_pAddBtn->m_height) )
				y = -(m_height-m_pBarBtn->m_width-m_pAddBtn->m_height);
			if( y > -m_pReduceBtn->m_height )
				y = -m_pReduceBtn->m_height;
			m_pBarBtn->setPositionY(y);
			float current = (float)(y-m_pReduceBtn->m_height)/(m_height-m_pBarBtn->m_height-m_pAddBtn->m_height-m_pReduceBtn->m_height);
			m_iCurrentNum = m_iMaxNum * current;
		}
		break;
	}
}
