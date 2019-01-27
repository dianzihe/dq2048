
#pragma  once

#include <string>
#include "base.h"
#include "ImageCenter.h"
//#include "NetDef.h"

//#define USE_UI_DRAG
using namespace std;

const int ALIGN_LEFT	= (1<<0);
const int ALIGN_HCENTER = (1<<1);
const int ALIGN_RIGHT	= (1<<2);
const int ALIGN_TOP		= (1<<3);
const int ALIGN_VCENTER = (1<<4);
const int ALIGN_BOTTOM	= (1<<5);

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)          { if (p) { delete(p); (p) = NULL;} }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)    { if (p) { delete[] (p); (p) = NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)         { if (p) { (p)->release(); (p) = NULL; } }
#endif

enum TUIType
{
	UI_BASE,
	UI_TEXT,
	UI_BUTTON,	
	UI_UISCROLLVIEW,
	UI_DRAWABLE,
	UI_UIEDIT,
	UI_UISCROLL,
	UI_ANIMATION,
	UI_UISCROLL_BAR,
	UI_TREE,

	UI_BTN_feature,	// not button, just use down effect.
	UI_NOTEITEM,
};

enum TEventType
{
	EVENT_DOWN,
	EVENT_UP,
	EVENT_DRAG,
	EVETN_DRAG_END,
	EVENT_SIZE,
};

enum	eTextType
{
	eTextType_MsFont,
	eTextType_Num
};

class UI;
class UIScrollView;

typedef void (*TEventFun)( UI* ui, void* data );

struct UIData
{
	string name;

	int x;
	int y;
	int width;
	int height;

	short type;
	short tag;
	short isEnlarge;		//是否扩大点击范围
	short isSTensile;  //是否支持拉伸
	short textType;

	int align;
	int posalign;

	string picName;

	string text;

	UIData()
		: x(0)
		, type(UI_BASE)
		, y(0)
		, width(0)
		, height(0)
	{
		textType = eTextType_MsFont;
		isEnlarge = 0;
		isSTensile = 1;
	}

	UI* createUI();
};

struct UIDataGroup
{
	string name;
	int count;
	int align;
	UIData* data;

	UIDataGroup()
	{
		count = 0;
		data  = NULL;
	}

	~UIDataGroup()
	{
		free();	
	}

	void free()
	{
		count = 0;
		SAFE_DELETE_ARRAY(data);
	}


	void Load(const char* fileName);

	UI* createUIBase();

	UI* createUI(int zoder);
};


struct UIImage
{
	RenderBatchData* m_RenderBatch;
	RenderBatchData* m_downRenderBatch;
	Texture2D* m_IconTexture;
	std::string  m_IconName;

	UIImage()
	{
		m_RenderBatch = NULL;
		m_downRenderBatch = NULL;
		m_IconTexture = NULL;
	}

	~UIImage()
	{
		if(m_IconTexture)
		{
			CC_SAFE_RELEASE(m_IconTexture);
		}
	}
};


struct EventFunData
{
	TEventFun funcEvent;
	void*	pData;
};

enum	UIMusicEffect_Type
{
	UIMusicType_Button_Check,
	UIMusicType_Button_Close,
	UIMusicType_Button_Select,

	UIMusicType_Button_Num
};

extern bool gStopEven;

class UIManager;
class UIText;
class UIAnimation;
typedef bool (*UIUpdateFunc)( UI * ui, float t);
typedef bool (*UIScrollView_updat)(UIScrollView* ui, float t);
typedef void (*UIDragFunc)( UI * selfUI, UI * srcUI, void * data );
typedef UI* (*CreateDragUIFunc)( UI * srcUI );

class UI : public Node
{
	friend class UIManager;
public:
	UI();
	virtual ~UI();

public:
	static UI*			node(void);

	bool		init() override;
	UI*					loadChild( const string& name );
	UI*					findUI( const string& name );
	void					Top();

	bool	isHaveUnKnowChar();

	Rect	getGlobalRect();

	virtual void		update(float t);

	void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;
	void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
	//virtual void		visit(void);
	//重写移除child函数
	void					removeChild(UI* child, bool cleanup);				
	void					setRenderBatch(RenderBatchData* pRenderBatch);
	void					setImage(const string& name, bool boundingWH = false);
	void					setImageByFullPath( const string& name, bool boundingWH = false );
	void					setIconTex(Texture2D* tex, bool boundingWH = false);
	//重新计算_down图片的UV和SOURCE SIZE
	void					needReCalDownTex();
	//新整理接口
	Texture2D *           getTexture2D();
	//reverse texture
	void					setReverseTex(bool reverse);
	//围绕X轴旋转
	void                    SetRotateX(float fa);
	//围绕Y轴旋转            
	void                    SetRotateY(float fa);
	//围绕Z轴旋转
	void                    SetRotateZ(float fa);
	//重新设置UI描点,(范围为0到1之间)
	void                    setUIAnchorPoint(float x,float y);

	eTextType			getTextType(){ return m_eTextType; };
	virtual void		setText( const std::string& strText );
	void					setText(int number);
	virtual void		setText( const std::string& strText, int fontSize );
	virtual void		setText( const std::string& strText, int fontSize, int alignType );
	virtual void		setText( const std::string& strText, int fontSize, const Size &size, int alignType , bool isAutoNewLine=true );
	virtual void		setText( const std::string& strText, int fontSize, const Size &size, int alignType , string fontName, bool isAutoNewLine=true );

	//判断UI是否被按下，返回-1表示不被按下，返回大于等于0表示此UI被按下到目前的时间
	float					GetHoldTime();

	void					setEvent( TEventType msg, TEventFun fun , void* data = NULL );	
	void				setOtherEvent(TEventType msg, TEventFun fun , void* data = NULL );
	virtual void		OnEvent( TEventType msg, Touch* touch );

	UI*					setSubImage(int Id,string name);
	void					rermoveImag(int Id);

	UI*					getTouchUI(Touch* touch);
	virtual UI*			getTouchUI(int beginX, int beginY, Touch* touch);

	string&				getText( void )		{ return m_text; }

	short				getAlign( void )const	{ return m_align; }
	void					setAlign( short n )		{ m_align = n; }

	UI*					SetSubValue( string UIName, string Text);		//设置UI子控件值

	Point			getRealPos();
	Point			getCurPos();

	Color4B			getColor(){ return m_color;};
	virtual void		setColor( Color4B color );

	void					setUpdateFunc( UIUpdateFunc fun){ m_pUpdateFun = fun;};

	bool					isEnlarge(){ return m_isEnlarge;};
	void					setIsEnlarge(bool b){ m_isEnlarge=b;};

	bool					isIgnoreTouch(){ return m_bIsIgnoreTouch; }; //设置 是否穿透事件
	void					setIgnoreTouch( bool b){ m_bIsIgnoreTouch =  b;}; 

	bool					isPenetrateTouch() {return m_bIsPenetrateTouch;};
	void					setPenetrateTouch(bool b){ m_bIsPenetrateTouch=b; };

	bool					isRemove(){ return bIsRemove; };
	void					remove(){ bIsRemove = true; };
	void					retainChild(){ bIsRemove = false; };

	void					SetClip(const Rect& rect  ) ;
	Rect*					getClip() {return m_clipRect;}
	virtual bool			isInClipRect( Touch* touch);

	bool					isMonopolizeTouch(){ return bIsMonopolizeTouch; };
	void					setMonopolizeTouch(bool b);

	bool					isMasking(){ return m_bIsMasking; };
	void					setMasking( bool b ) {m_bIsMasking = b; };

	void					setEnable(bool b){ bIsEnable = b; };
	bool					isEnable(){ return bIsEnable; };

	long					getParam64( void )const		{ return m_nParam64; }
	void					setParam64( long n )		{ m_nParam64 = n; }

	UI*						findUsingUI( const string& uiFileName, const string& uiNodeName);
    virtual void			removeAllChildrenWithCleanup(bool cleanup);
    virtual void setVisible(bool visible);
	virtual void recursiveResource(bool visible);
    virtual void onGetTop();
    virtual void onLoseTop();
	virtual void onReleaseResource() {} // Release Useless Resource When Set Visible false
	virtual void onReLoadResource() {} // Reload Resource When Set Visible true

	void setLinePos(int sx, int sy, int ex, int ey);
	
	void	onlyUseVertexColor(bool bUse)
	{
		m_onlyUseVertexColor = bUse;
	}

	bool	isOnlyUseVertexColor() const
	{
		return m_onlyUseVertexColor;
	}

	//当前UI父UI是否显示
	bool isParentVisible();

	const bool isTop()
	{
		return m_isTop;
	}

	//获取UI注册的事件
	EventFunData* getUIEvent(TEventType type);

private:
	bool			m_onlyUseVertexColor;

	bool			m_isLine;			//is drawing line
	int				m_startLineX;		//start point of line
	int				m_startLineY;
	int				m_endLineX;			//end point of line
	int				m_endLineY;
	bool			m_isTop;

	bool			m_isReverseTex;

public:
	string			m_name;
	string			m_text;
	short			m_align;
	short			m_tag;
	TUIType			m_type;
	int				m_width;
	int				m_height;
	Color4B		m_color;
	bool			m_down;
    
//    bool            m_bisTop;
    bool            m_bisRoot;
	UIImage			m_image;
protected:
	
	UIText*			m_pText;
	UIText*			m_pTextDown;
	EventFunData	m_eventData[EVENT_SIZE];
	EventFunData	m_otherEventData[EVENT_SIZE];
	bool			bIsRemove;
	bool			bIsMonopolizeTouch; // 是否独占touch
	bool			m_bIsIgnoreTouch;// 是否忽略touch
	bool			m_bIsPenetrateTouch; //是否穿透touch
	bool			bIsEnable; //enable
	bool			m_bIsMasking; //是否蒙板
	Rect*			m_clipRect;
	Rect*			m_rect9;
	bool			m_isEnlarge;		//是否扩大点击范围;
	//bool         m_isSTensile;    //是否lashen
	static  RenderBatchData * m_MaskingBack;

	eTextType	 m_eTextType;
	static string	m_strTextFontName[eTextType_Num];


	long			m_nParam64;
	//自定义update函数，
	UIUpdateFunc m_pUpdateFun;


public:
	struct	UIRect9Data 
	{
		bool	isHave;
		Rect rect;
	};
private:
	static map<string, UIRect9Data>		m_sRect9Datatable;


	//music effect
private:
	UIMusicEffect_Type	m_eMusicEffectType;
	static string					m_strMusicEffectFileName[UIMusicType_Button_Num];
	bool								m_bIsPlayEffect;

public:
	UIMusicEffect_Type	getUIEffectType(){ return m_eMusicEffectType; }
	void		setUIEffectType( UIMusicEffect_Type type ){ m_eMusicEffectType=type; }

	bool		isPlayEffect(){ return m_bIsPlayEffect; }
	void		setPlayEffect( bool b ){ m_bIsPlayEffect=b; }

#ifdef USE_UI_DRAG
private:
	int		m_iDragSrcTag;	//做为拖动源的标记
	void*	m_pDragSrcData;	//拖动源的附加数据
	CreateDragUIFunc	m_pCreateDragFunc; //创建拖动UI回调函数

	int		m_iDragDestTag;	//做为拖动目标的标记	只有在和正在拖动UI的拖动源标记一样才是有效拖动
	UIDragFunc	m_pDragFunc;

public:
	void		setDragData( int srcTag, CreateDragUIFunc func, void * data ){ m_iDragSrcTag = srcTag; m_pCreateDragFunc = func; m_pDragSrcData = data; }
	void		setDragFunc( int destTag, UIDragFunc func ){ m_iDragDestTag = destTag; m_pDragFunc = func; }
#endif
};

enum TDrawType
{
	DrawType_NOMAL,
	DrawType_SELECTED,
	DrawType_DRAGGING,
};

struct UIEvent 
{
	UI* self;
	EventFunData data;
	unsigned long lEventTime;
};
//add by maminghai	2013-11-20
//use loadchild load Ui file record
struct ChildUiLocateItem
{
	//parent name
	std::vector<std::string> parentNameArr;
};
class UIEditNew;
class UIManager : public Node
{
public:
	typedef std::map<std::string, UI*> UIMap;
	std::map<std::string, ChildUiLocateItem> m_mapChildLocatea;
	UI*			findUI( const string& name );
	UI*			findUIByName( const string name );
	string		findTopRootName(const string name);
    
	UI*			loadUI( const string& name, bool isTop = true, int zoder = 0);
	void		addUI(UI* pUI, bool isTop = true);
	void		addUI(UI* pUI, int id, bool isTop = true);
	UI*			loadUIWithIndex( const string& name, int iIndex, bool isTop = true , int zoder = 0 );
    
	UI*			findUI( const string& name,int iIndex );
	void		removeUI( const string& name,int iIndex );
	void		removeUI( const string& name );
	UI*			findUsingUI( const string& uiFileName, const string& uiNodeName);

	UIManager();
	~UIManager();

	void			clear();

	UIMap&			GetUIMap() { return m_uiMap; }
	virtual void	update(float dt);

	//void			AddTouchDelegate( CCTouchDelegate* pHandler );
	//void			RemoveTouchDelegate( CCTouchDelegate* pHandler );

	bool	onTouchBegan(Touch* touch, Event* event);
	void	onTouchMoved(Touch* touch, Event* event) ;
	void	onTouchEnded(Touch* touch, Event* event) ;

	// optional
	/*
	virtual void	ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void	ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void	ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	*/

	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);

	UI*			getTouchUI( Touch* touch );

	static UIManager* Instance();

	Touch*		getCurTouch( void )		{ return m_curTouch; }
	static void		PushEvent(UI* p, const EventFunData& data );
	static void		RemoveEvent( UI* p );
	static void		PushOtherEvent(UI* p, const EventFunData& data );
	static void		RemoveOtherEvent( UI* p );
	void				setOnlyMessage(UI *p){ m_pOnlyMessage = p; };
	UI*				getOnlyMessage(){ return m_pOnlyMessage; };

    void    topUI(UI* ui);
    void    onLoseTop(UI* ui);
    void    onGetTop(UI* ui);
	//在childmap表查找所需ui
	UI*		findUIInChildMap(const string& name );
	//在使用loadchild时添加新节点到childmap表,name为查找ui的名称,thisUi为使用loadchild的ui指针
	void	addItemToChildMap(const string name,UI* thisUi);
	//在删除节点时检测表中是否存在，存在则删除
	void	removeItemFromChildMap(const string name);
	//清理整个节点表
	void	clearChildMap();

public:
	vector<UI*>		m_selected;
	UIEditNew *		m_NonceEdit; //当前输入框
	float			m_holdTime;
	Point			m_curPos;
	Rect			m_nonceClipRect;
	Point			m_TranslatefPos;
	//std::set<CCTouchDelegate*> m_TouchHandlers;
    
    std::vector<UI*> m_stack;

#ifdef USED_JUMP
	static int m_draged;
#endif
private:
	UIMap			m_uiMap;
	list<UIEvent>	m_eventList;
	list<UIEvent>	m_otherEventList;
	Touch*		m_curTouch;
	UI*				m_pOnlyMessage;
	bool			m_bIsDrag;
	int				m_iTouchNum;

#ifdef USE_UI_DRAG
private:
	UI*				m_pDragUI;
	void				_CreateGragUI( UI* srcUI, CCTouch *touch );
	void				_MoveGragUI( CCTouch *touch );
	void				_HandleGragEvent( UI * destUI, CCTouch *touch );
#endif

};