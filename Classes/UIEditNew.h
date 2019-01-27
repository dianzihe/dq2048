#pragma once

#include "UI.h"
//#include "CCTextFieldTTF.h"
#include "UIText.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#define USE_EXTENSION_EDITBOX

#endif

#ifdef USE_EXTENSION_EDITBOX
#include "CCEditBox.h"
using namespace cocos2d::extension;
#endif

//2012,9,19 by yqf 重新设计的Edit

//插入字符监控函数
typedef bool (*InsertTextCallBack)(CCTextFieldTTF * sender, const char * text, int nLen);

//删除字符监控函数
typedef bool (*DelTextCallBack)(CCTextFieldTTF * sender, const char * delText, int nLen);

//呼出输入框监控函数
typedef bool (*AttachWithIMECallBack)(CCTextFieldTTF * sender);

//隐藏输入框监控函数
typedef bool (*DetachWithIMECallBack)(CCTextFieldTTF * sender);



#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
typedef void (*KeyboardWillShowFUnc)(CCIMEKeyboardNotificationInfo& info);

typedef void (*KeyboardDidShowFUnc)(CCIMEKeyboardNotificationInfo& info);

typedef void (*KeyboardWillHideFUnc)(CCIMEKeyboardNotificationInfo& info);

typedef void (*KeyboardDidHideFUnc)(CCIMEKeyboardNotificationInfo& info);
#endif

class UIEditNew :  public UI,
#ifdef USE_EXTENSION_EDITBOX
                    public cocos2d::extension::CCEditBoxDelegate
#else
                    public CCTextFieldDelegate
#endif

{
public:
	UIEditNew(void);
	virtual ~UIEditNew(void);
    
    virtual void onExit();
    virtual void onEnter();

	virtual bool init();
	virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
    virtual void		update(float t);
	//自定义初始化
	void userDefinedInit( int nW, int nH, float fontSize, const char *defaultStr, CCTextAlignment agin, bool isMultiLine = false);

	void setIsMultiline(bool isMultiLine = false);

	void setMaxCharCount( int count );//{ m_iMaxCharCount = count; };
	int getMaxCharCount(){ return m_iMaxCharCount; };

	int getCharCount();//{ if(m_pField) return m_pField->getCharCount(); return 0; }

	void setHideChar( bool b );//{ m_bIsHideChar = b;};
	bool isHideChar(){ return m_bIsHideChar; };
    
    void setIsPassword(bool is);
    bool getIsPassword() { return m_bIsPwd;}

	void setNotChiniese( bool b ){ m_bIsNotChiniese = b; };
	bool isNotChiniese(){ return m_bIsNotChiniese; };

	void setOnlyNumber( bool b );//{ m_bIsOnlyNumber = b; };
	bool isOnlyNumber(){ return m_bIsOnlyNumber; };

	void setMaxNumber( int max ){ m_iMaxNumber = max; };
	int getMaxNumber(){ return m_iMaxNumber; };

	void setInsertTextCallBack( InsertTextCallBack p){ InsertText =p; };
	void setDelTextCallBack( DelTextCallBack p ){ DelText = p; };
	void setAttachIMECallBack( AttachWithIMECallBack p ){ AttachIME = p; };
	void setDetachIMECallBack( DetachWithIMECallBack p ){ DetachIME=p; };

	std::string getText();
	virtual void setText( const std::string& strText );

	void attachWithIME();
	void detachWithIME();

	//是否拥有屏蔽字符
	bool	isHaveUnKnowChar();//{ return m_pField->isHaveUnKnowChar(); };

	// CCTextFieldDelegate
    virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * sender);
    virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * sender);
    virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen);
    virtual bool onDraw(CCTextFieldTTF * sender);
	void	setMaxLineNum( int num ); 
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)    
    virtual void keyboardWillShow(CCIMEKeyboardNotificationInfo& info);
    virtual void keyboardDidShow(CCIMEKeyboardNotificationInfo& info);
    virtual void keyboardWillHide(CCIMEKeyboardNotificationInfo& info);
    virtual void keyboardDidHide(CCIMEKeyboardNotificationInfo& info);
    
    void   SetKeyboardWillShowFunc(KeyboardWillShowFUnc func) { WillShow = func; };
    void   SetKeyboardDidSHowFunc(KeyboardDidShowFUnc func) { DidShow = func;};
    void   SetKeyboardWillHideFUnc(KeyboardWillHideFUnc func) { WillHide = func; };
    void   SetkeyboardDidHideFunc(KeyboardDidHideFUnc func) { DidHide = func; };
#endif


	virtual void setColor( ccColor4B color );
private:
	CCTextFieldTTF *    m_pField;
	CCTextAlignment     m_Alignment;

	int			m_iMaxCharCount;
	bool		m_bIsHideChar;    //是否隐藏字符 （替换为*号）
	bool		m_bIsNotChiniese;  //是否不支持中文
	bool		m_bIsOnlyNumber; //是否只支持数字
	int			m_iMaxNumber; //输入数字的最大值（只能输入数字模式下有效）
    int			m_iMaxLineNum; //最大每行字符数
	int			m_iNowNum;
	bool		m_bMuiltiline;
    bool        m_bIsPwd;
    
	InsertTextCallBack      InsertText;
	DelTextCallBack         DelText;
	AttachWithIMECallBack   AttachIME;
	DetachWithIMECallBack	DetachIME;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
public:
    enum
    {
        MoveMainUI_Hide,
        MoveMainUI_UP,
        MoveMainUI_Show,
        MoveMainUI_Down
    };
    
    void   SetByKeyboardMoveUI( UI* ui )
    {
        m_pMoveMainUI = ui;
        m_fMoveUIStartPosY = ui->getPositionY();
        m_fMoveUIEndPosY = m_fMoveUIStartPosY+30;
        m_eUIState = MoveMainUI_Hide;
    };
private:
    KeyboardWillShowFUnc   WillShow;
    KeyboardDidShowFUnc    DidShow;
    KeyboardWillHideFUnc   WillHide;
    KeyboardDidHideFUnc    DidHide;
    
    UI*      m_pMoveMainUI;
    float    m_fMoveUIStartPosY;
    float    m_fMoveUIEndPosY;
    int8     m_eUIState;
    
    float   m_fWindMove;
#endif
    
#ifdef USE_EXTENSION_EDITBOX
protected:
    
    //static
    CCEditBox*      m_pExtensionBox;
    
    UIText*         m_pExtText;
    
    bool            m_bisMultiLine;
    
public:
    static bool m_isUseEditer;
    
    virtual void setVisible(bool visible);
    
    virtual void onGetTop();
    virtual void onLoseTop();
    
    void createExtBox();
    
    void removeExtBox();
    
    /**
     * This method is called when an edit box gains focus after keyboard is shown.
     * @param editBox The edit box object that generated the event.
     */
    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    
    
    /**
     * This method is called when an edit box loses focus after keyboard is hidden.
     * @param editBox The edit box object that generated the event.
     */
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    
    /**
     * This method is called when the edit box text was changed.
     * @param editBox The edit box object that generated the event.
     * @param text The new text.
     */
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    
    /**
     * This method is called when the return button was pressed or the outside area of keyboard was touched.
     * @param editBox The edit box object that generated the event.
     */
    virtual void editBoxReturn(CCEditBox* editBox);
    
#endif
    
};
