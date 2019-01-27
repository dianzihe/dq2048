#include "UIEditNew.h"

#ifdef USE_EXTENSION_EDITBOX

#endif

const int MAX_LINE_CHAR_NUM = 64;

UIEditNew::UIEditNew(void):
m_pField(NULL),
m_iMaxCharCount(-1),
m_bIsHideChar(false),
m_bIsNotChiniese(false),
InsertText(NULL),
DelText(NULL),
m_bIsOnlyNumber(false),
m_bMuiltiline(false),
m_iMaxNumber(0),
AttachIME(NULL),
DetachIME(NULL),
m_iNowNum(0),
m_iMaxLineNum(MAX_LINE_CHAR_NUM),
m_bIsPwd(false)
#ifdef USE_EXTENSION_EDITBOX
,m_pExtensionBox(NULL),
m_pExtText(NULL),
m_bisMultiLine(false)
#endif
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    m_eUIState = MoveMainUI_Hide;
    m_pMoveMainUI = NULL;
    WillShow = NULL;
    DidShow = NULL;
    WillHide = NULL;
    DidHide = NULL;
#endif
}

int __calcCharCount(const char * pszText)
{
	int n = 0;
	char ch = 0;
	while ((ch = *pszText))
	{
		CC_BREAK_IF(! ch);

		if (0x80 != (0xC0 & ch))
		{
			++n;
		}
		++pszText;
	}
	return n;
}

void cutCharByCount( const char * pszText, int count )
{
	int n = 0;
	char ch = 0;
	int i=0;
	while ((ch = pszText[i]))
	{
		CC_BREAK_IF(! ch);

		if (0x80 != (0xC0 & ch))
		{
			++n;
			if( n > count )
			{
				((char*)pszText)[i] = '\0';
				return;
			}
		}
		++i;
	}
}


UIEditNew::~UIEditNew(void)
{

}

void UIEditNew::onEnter()
{
    UI::onEnter();
//    
//#ifdef USE_EXTENSION_EDITBOX
//    this->createExtBox();    
//#endif
    
}

void UIEditNew::onExit()
{
    UI::onExit();
    
//#ifdef USE_EXTENSION_EDITBOX
//    
//    if (m_pExtensionBox)
//    {
//        m_pExtensionBox->removeFromParentAndCleanup(true);
//        m_pExtensionBox = NULL;
//    }
//#endif
}

bool UIEditNew::init()
{
	if( m_pField ) m_pField->removeFromParentAndCleanup(true);

	m_Alignment = kCCTextAlignmentCenter;
	float posx = 0.0f;
	switch( m_align&0x7 )
	{
	case ALIGN_LEFT:
		m_Alignment = kCCTextAlignmentLeft;
		posx = 0.0f;
		break;
	case ALIGN_HCENTER:
		m_Alignment = kCCTextAlignmentCenter;
		posx = m_width/2.0f;
		break;
	case ALIGN_RIGHT:
		m_Alignment = kCCTextAlignmentRight;
		break;
	}

#ifdef USE_EXTENSION_EDITBOX
   this->setIsMultiline(m_bisMultiLine);

#else
	m_pField =  CCTextFieldTTF::textFieldWithPlaceHolder( m_text.c_str(), CCSize( m_width, m_height), m_Alignment, FONT_BMP_24, 24);
	m_pField->setPosition( ccp(posx, m_height/2) );
	m_pField->setDelegate(this);
	if( m_Alignment == kCCTextAlignmentLeft )
		m_pField->setAnchorPoint(ccp(0.0f, 0.5f));
	addChild(m_pField);
#endif
    
	SetClip(CCRectMake(0,0, m_width, m_height));
	return true;
}

void UIEditNew::setIsMultiline(bool isMultiLine /* = false */)
{
	m_bMuiltiline = true;
#ifdef USE_EXTENSION_EDITBOX

	if (m_bisMultiLine != isMultiLine)
	{
		this->removeExtBox();
	}

	m_bisMultiLine = isMultiLine;
	this->createExtBox();

#endif
}

void UIEditNew::userDefinedInit( int nW, int nH, float fontSize, const char *defaultStr, CCTextAlignment agin , bool isMultiLine )
{
	if( m_pField ) m_pField->removeFromParentAndCleanup(true);

	m_Alignment = agin;
	float posx = 0.0f;
	switch( m_Alignment )
	{
	case kCCTextAlignmentLeft:
		posx = 0.0f;
		break;
	case kCCTextAlignmentCenter:
		posx = m_width/2.0f;
		break;
	case kCCTextAlignmentRight:
		posx = m_width/2.0f;
		break;
	}

#ifdef USE_EXTENSION_EDITBOX
    this->setIsMultiline(isMultiLine);    
#else
	m_pField = CCTextFieldTTF::textFieldWithPlaceHolder(defaultStr, CCSize( nW, nH), agin, FONT_BMP_24, fontSize);
	m_pField->setPosition( ccp(posx, m_height/2) );
	m_pField->setDelegate(this);
	if( m_Alignment == kCCTextAlignmentLeft )
		m_pField->setAnchorPoint(ccp(0.0f, 0.5f));
	addChild(m_pField);
	SetClip(CCRectMake(0,0, m_width, m_height));
#endif
}


std::string UIEditNew::getText()
{
    
#ifdef USE_EXTENSION_EDITBOX
    
    if (m_pExtensionBox)
    {
//        CCLOG("GET:%s", m_text.c_str());
        return m_pExtensionBox->getText();
    }
    else
    {
//        CCLOG("GET:%s", m_text.c_str());
        return m_text;
    }
#else
	if (m_pField)
		return m_pField->getString();
	else
		return "";
#endif
    
}

void UIEditNew::setText( const std::string& strText )
{
//     CCLOG("SET:%s-->%s", m_text.c_str(), strText.c_str());

		m_text = strText;
    
#ifdef USE_EXTENSION_EDITBOX
    if (m_pExtensionBox)
        m_pExtensionBox->setText(strText.c_str());
    
    if (m_pExtText)
        m_pExtText->setText(strText);
#else
	if( m_pField)
		m_pField->setString(strText.c_str());
#endif
}

void UIEditNew::attachWithIME()
{
#ifndef USE_EXTENSION_EDITBOX  
	if( m_pField)
		m_pField->attachWithIME();
#endif
}

void UIEditNew::detachWithIME()
{
#ifdef USE_EXTENSION_EDITBOX
    if (m_pExtensionBox)
        m_pExtensionBox->closeKeyboard();
#else
	if( m_pField)
		m_pField->detachWithIME();
#endif
}


bool UIEditNew::onTextFieldAttachWithIME(CCTextFieldTTF * sender)
{
	if(AttachIME && AttachIME(sender))
	{
		return true;
	}
	return false;
}

bool UIEditNew::onTextFieldDetachWithIME(CCTextFieldTTF * sender)
{
	if(DetachIME && DetachIME(sender))
	{
		return true;
	}
	return false;
}


bool UIEditNew::onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen)
{
#ifndef USE_EXTENSION_EDITBOX
	if( m_iMaxCharCount >0 && sender->getCharCount() > m_iMaxCharCount)
		return true;
#endif

	if( InsertText && InsertText(sender, text, nLen))
	{
		return true;
	}

#if	CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

	if( m_bIsOnlyNumber )
	{
		for( int i=0; i<strlen(text); ++i )
		{
			if( text[i]<'0' || text[i]> '9' )
			{
				return true;
			}
		}
	}
	else if( m_bIsNotChiniese )
	{
		for( int i=0; i<strlen(text); ++i )
		{
			if( text[i] < 0 )
			{
				return true;
			}
		}
	}
	
	int charCount = __calcCharCount(text);
	if (  m_iMaxCharCount >0 && charCount > m_iMaxCharCount )
	{
		cutCharByCount(text, m_iMaxCharCount);
		return false;
	}
#else 

	if( m_bIsOnlyNumber )
	{
		if(nLen == 1 && text[0]>='0' && text[0]<= '9' )
		{
			string strNumber = this->getText();
			if( strNumber == "0"  && (strcmp(text, "0") == 0) )
				return true;

			if( m_iMaxNumber )
			{
				string str2 = strNumber + text;
				if( atoi(str2.c_str()) > m_iMaxNumber)
					return true;
			}
			if( strNumber == "0")
				sender->setString("");
		}
		else
			return true;
	}
	else if( m_bIsNotChiniese )
	{
		if( text[0] < 0 )
			return true;
	}
#endif
	if ( m_bMuiltiline && sender )
	{
		std::string _str = sender->getString();
		int _nCount = 0;
		int _nLongth = _str.length();
		int _iLongth = _nLongth;
		while ( _nCount<_nLongth )
		{
			_nCount++;
			if ( _str[_iLongth--] == '\n' )
			{
				if ( _nCount == m_iMaxLineNum )
				{
					_str = _str + '\n';
					sender->setString( _str.c_str() );
				}
				break;
			}
			else if ( _nCount == m_iMaxLineNum )
			{
				_str = _str + '\n';
				sender->setString( _str.c_str() );
				break;
			}
		}

	}
	return false;
}

bool UIEditNew::onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen)
{

	if(DelText && DelText(sender, delText, nLen))
		return true;

	if( m_bIsOnlyNumber )
	{
		string strNumber = this->getText();
		if(sender && strNumber.length() == 1)
		{
			sender->setString("0");
			return true;
		}
	}
	return false;
}

bool UIEditNew::onDraw(CCTextFieldTTF * sender)
{
	/*
	if( m_bIsHideChar )
	{
		if(strlen(sender->getString()))
		{
			string str = (char*)sender->getString();
			str.replace(0, str.length(), str.length(), '*');
			if(str != sender->CCLabelBMFont::getString())
				sender->CCLabelBMFont::setString(str.c_str());
		}
	}
	*/
	return false;
}

void UIEditNew::setColor( ccColor4B color )
{
#ifdef USE_EXTENSION_EDITBOX
    if (m_pExtensionBox)
    {
        m_pExtensionBox->setFontColor( ccc3(color.r, color.g, color.b) );
        m_pExtensionBox->setOpacity(color.a);
    }
#else
	if (m_pField)
	{
		m_pField->setColor(ccc3(color.r, color.g, color.b));
		m_pField->setOpacity(color.a);
	}
	
#endif
}

void UIEditNew::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
	//UI::draw();
	UI::draw(renderer, transform, flags);
}

bool    g_bIsShowKeyBoard = false;

void UIEditNew::update(float t)
{
    
#ifdef USE_EXTENSION_EDITBOX
    //global pos
//    if (m_pExtensionBox)
//    {
//        CCPoint g_pos(m_width /2, m_height / 2);
//        CCNode* parent = this;
//        
//        while (parent)
//        {
//            g_pos = ccpAdd(parent->getPosition(), g_pos);
//            
//            
//            parent = parent->getParent();
//        }
//        
//        m_pExtensionBox->setPosition(g_pos);
//    }
#else
    if( m_Alignment != kCCTextAlignmentCenter && m_pField)
	{
		m_pField->setPositionX( (m_width-m_pField->getContentSize().width > 0) ? 0 : (m_width-m_pField->getContentSize().width));
	}
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    const float speed = 1400.0f;
    if( m_pMoveMainUI )
    {
        switch (m_eUIState)
        {
            case MoveMainUI_Hide:
                break;
            case MoveMainUI_UP:
                {
                    if(m_pMoveMainUI->getPositionY() >= m_fMoveUIEndPosY )
                    {
                        m_pMoveMainUI->setPositionY(m_fMoveUIEndPosY);
                        m_eUIState = MoveMainUI_Show;
                    }
                    else
                    {
                        m_pMoveMainUI->setPositionY(m_pMoveMainUI->getPositionY()+speed*t);
                    }
                }
                break;
            case MoveMainUI_Show:
                break;
            case MoveMainUI_Down:
                {
                    if(m_pMoveMainUI->getPositionY() <= m_fMoveUIStartPosY )
                    {
                        m_pMoveMainUI->setPositionY(m_fMoveUIStartPosY);
                        m_eUIState = MoveMainUI_Hide;
                    }
                    else
                    {
                        m_pMoveMainUI->setPositionY(m_pMoveMainUI->getPositionY()-speed*t);
                    }
                }
                break;
        }
    }
#endif
}

void UIEditNew::setMaxCharCount( int count )
{
    m_iMaxCharCount = count;
    
#ifdef USE_EXTENSION_EDITBOX
    if (m_pExtensionBox)
        m_pExtensionBox->setMaxLength(count);
#endif    
    
};


int UIEditNew::getCharCount()
{ 
#ifdef USE_EXTENSION_EDITBOX
	return __calcCharCount(m_text.c_str());

#else
	if(m_pField)
		return m_pField->getCharCount(); 
	return 0;
#endif
}

void UIEditNew::setHideChar( bool b )
{
    m_bIsHideChar = b;
    
    this->setIsPassword(b);
}


void UIEditNew::setOnlyNumber( bool b )
{
    m_bIsOnlyNumber = b;

#ifdef USE_EXTENSION_EDITBOX
    if (m_pExtensionBox)
        m_pExtensionBox->setInputMode(b ? kEditBoxInputModeNumeric : kEditBoxInputModeAny);
#endif
}

void UIEditNew::setIsPassword(bool is)
{
    m_bIsPwd = is;
    
#ifdef USE_EXTENSION_EDITBOX
    if (m_pExtensionBox)
    {
        m_pExtensionBox->setInputFlag(is ? kEditBoxInputFlagPassword : kEditBoxInputFlagSensitive);
    }
#endif
    
}

bool UIEditNew::isHaveUnKnowChar()
{
	return false;
	/*
    if (m_pField)
        return m_pField->isHaveUnKnowChar();
    else
        return false;
		*/
}

void UIEditNew::setMaxLineNum( int num )
{
	m_iMaxLineNum = num;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

void UIEditNew::keyboardWillShow(CCIMEKeyboardNotificationInfo& info)
{
    g_bIsShowKeyBoard = true;
    if( UIManager::Instance()->m_NonceEdit != this )
        return;
    if(WillShow) WillShow(info);
    m_eUIState = MoveMainUI_UP;
    if(m_pMoveMainUI)
    {
        m_pMoveMainUI->setPositionY(m_fMoveUIStartPosY);
        CCPoint pos = getRealPos();
        if( pos.y >= info.end.size.height+10)
        {
            m_fMoveUIEndPosY = m_fMoveUIStartPosY;
        }
        else
        {
            m_fMoveUIEndPosY = m_fMoveUIStartPosY+info.end.size.height+10-pos.y;
        }
    }
}

void UIEditNew::keyboardDidShow(CCIMEKeyboardNotificationInfo& info)
{
    if( UIManager::Instance()->m_NonceEdit != this )
        return;
    if(DidShow) DidShow(info);
    m_eUIState = MoveMainUI_Show;
    if(m_pMoveMainUI) m_pMoveMainUI->setPositionY(m_fMoveUIEndPosY);
}

void UIEditNew::keyboardWillHide(CCIMEKeyboardNotificationInfo& info)
{
    if( UIManager::Instance()->m_NonceEdit != this )
        return;
    if(WillHide) WillHide(info);
    m_eUIState = MoveMainUI_Down;
    if(m_pMoveMainUI) m_pMoveMainUI->setPositionY(m_fMoveUIEndPosY);
}

void UIEditNew::keyboardDidHide(CCIMEKeyboardNotificationInfo& info)
{
    g_bIsShowKeyBoard = false;
    if( UIManager::Instance()->m_NonceEdit != this )
        return;
    if(DidHide) DidHide(info);
    m_eUIState = MoveMainUI_Hide;
    if(m_pMoveMainUI) m_pMoveMainUI->setPositionY(m_fMoveUIStartPosY);
}

#ifdef USE_EXTENSION_EDITBOX
bool UIEditNew::m_isUseEditer = false;

void UIEditNew::setVisible(bool visible)
{
    UI::setVisible(visible);
    
//    if (visible)
//    {
//        this->createExtBox();
//    }
//    else
//    {
//        this->removeExtBox();
//    }
}

void UIEditNew::onGetTop()
{
    UI::onGetTop();
    UIEditNew::m_isUseEditer = true;
    this->createExtBox();
    
}

void UIEditNew::onLoseTop()
{
    UI::onLoseTop();
    UIEditNew::m_isUseEditer = false;
    this->removeExtBox();
    
}

void UIEditNew::createExtBox()
{
//    this->removeExtBox();
//    
////    if (!m_pExtensionBox)
    if (!m_pExtensionBox)
    {
        CCEGLView* eglView = CCEGLView::sharedOpenGLView();
        float scale_x = eglView->getScaleX();
        float scale_y = eglView->getScaleY();
        float scale = scale_x > scale_y ? scale_x : scale_y;
        scale = scale < 1.0f ? 1.0f : scale;
        
        int height = 0;
        if (m_bisMultiLine)
            height = (m_height < 50 ? 50 : m_height);
        else
            height = 46;
//        
//#ifdef DEBUG
//        CCScale9Sprite* sp9 = CCScale9Sprite::create("null.png");
//#else
        CCScale9Sprite* sp9 = CCScale9Sprite::create("Origin/null1.pvr.ccz");
//#endif
        m_pExtensionBox = CCEditBox::create(CCSize( m_width, height), sp9, NULL, NULL, m_bisMultiLine);
        m_pExtensionBox->setText(m_text.c_str());
        m_pExtensionBox->setDelegate(this);
		m_pExtensionBox->setMaxLength(m_iMaxCharCount);
        m_pExtensionBox->setInputFlag(m_bIsPwd ? kEditBoxInputFlagPassword : kEditBoxInputFlagSensitive);
        m_pExtensionBox->setInputMode(m_bIsOnlyNumber ? kEditBoxInputModeNumeric : kEditBoxInputModeAny);
        
        if (CCApplication::sharedApplication()->isIpad2())
            m_pExtensionBox->setFontSize( 36 * scale);
        else
            m_pExtensionBox->setFontSize( 18 * scale);
        
        m_pExtensionBox->setReturnType(kKeyboardReturnTypeDone);
        
        //global pos
        CCPoint g_pos(m_width /2, m_height /2);
//        CCNode* parent = this;
//
//        while (parent)
//        {
//            g_pos = ccpAdd(parent->getPosition(), g_pos);
//
//
//            parent = parent->getParent();
//        }
        
        m_pExtensionBox->setPosition(g_pos);
        this->addChild(m_pExtensionBox, 10000, 10000);
    }
    
    if (!m_pExtText)
    {
        m_pExtText = UIText::initUITextWithString(m_text, 30, CCSize(m_width, m_height), tAlignLeft | tAlignTop);
        this->addChild(m_pExtText, 100);
        
        m_pExtText->setPosition(ccp(0, 0));//ccp(m_width /2, m_height /2));
    }
    
    m_pExtText->setVisible(false);
}

void UIEditNew::removeExtBox()
{
    if (!m_pExtText)
    {
        m_pExtText = UIText::initUITextWithString(m_text, 30, CCSize(m_width, m_height), tAlignLeft | tAlignTop);
        this->addChild(m_pExtText, 100);
        
        m_pExtText->setPosition(ccp(0, 0));
    }
    
    
    
    if (m_pExtensionBox)
    {
        m_text = m_pExtensionBox->getText();
        
        m_pExtensionBox->removeFromParentAndCleanup(true);
        m_pExtensionBox = NULL;
    }
    
    m_pExtText->setVisible(true);
    m_pExtText->setText(m_text);
    
    CCLOG("%s", m_text.c_str());
//    this->setText(m_text);
}

/**
 * This method is called when an edit box gains focus after keyboard is shown.
 * @param editBox The edit box object that generated the event.
 */
void UIEditNew::editBoxEditingDidBegin(CCEditBox* editBox)
{
    UIManager::Instance()->m_NonceEdit = this;
    
}


/**
 * This method is called when an edit box loses focus after keyboard is hidden.
 * @param editBox The edit box object that generated the event.
 */
void UIEditNew::editBoxEditingDidEnd(CCEditBox* editBox)
{
}

/**
 * This method is called when the edit box text was changed.
 * @param editBox The edit box object that generated the event.
 * @param text The new text.
 */
void UIEditNew::editBoxTextChanged(CCEditBox* editBox, const std::string& text)
{
    CCLOG("CHYANGED:%s-->%s", m_text.c_str(), text.c_str());
    
    std::string ch = "";
    int flag = 0;
    
    if (text.size() > m_text.size())
    {
        // insert
        ch = text.substr(m_text.length(), 1);
        
        flag = 1;
        
    }
    else if (text.size() < m_text.size())
    {
        ch = m_text.substr(text.length(), m_text.length() - text.length());
        
        flag = -1;
    }
    
    // save fisrt.
    m_text = text;
    
    // next call may reset m_text;
    if (flag == 1)
        this->onTextFieldInsertText(NULL, ch.c_str(), 1);// text.c_str(), text.length());
    else if (flag == -1)
        this->onTextFieldDeleteBackward(NULL, ch.c_str(), 1);//)(NULL, text.c_str(), text.length());
}

/**
 * This method is called when the return button was pressed or the outside area of keyboard was touched.
 * @param editBox The edit box object that generated the event.
 */
void UIEditNew::editBoxReturn(CCEditBox* editBox)
{
    // '\n'
    
    std::string ch = "\n";
    this->onTextFieldInsertText(NULL, ch.c_str(), 2);// text.c_str(), text.length());
    
}



#endif
#endif
