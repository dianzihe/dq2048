#include "UI.h"
#include "GameScene.h"
#include "UIText.h"
#include "UIAnimation.h"
#include "UIEditNew.h"
#include "UIDrawable.h"
#include "UIScroll.h"


/*
#include "GameMainUI.h"
#include "UIScrollView.h"
#include "GraphicsExtension.h"
#include "MainUIMove.h"
#include "../BaseModule/Guide/GuideModule.h"
#include "CCEGLView.h"
#include "UIBatchRenderer.h"
#include "../../CocosDenshion/include/SimpleAudioEngine.h"
#include "Map.h"
#include "MainUIOpenCloseHandle.h"
#include "../gameui/DirectionHandleUI.h"
#include "../gameui/ShortPage.h"
#include "../gameui/Shortcut.h"
#include "Misc.h"
#include "Camera.h"
#include "UINoteBook.h"
*/

const float _LIMITED_MAX_SCREEN_SCALE = 1.8f;
const float _STANDARD_MAX_SCREEN_SCALE = 1.7f;
const float _LIMITED_MIN_SCREEN_SCALE = 0.7f;
const float _STANDARD_MIN_SCREEN_SCALE = 0.8f;
const float _SPEED_PER_TOUCHES = 3000.0f;

static float sDistance = 0.0f;
//fix the scale changes is too big, it makes the screen so bad.
static float sNowScale = 1.0f;
//some times, we need to rescale the screen.
static float sFixScreen = 0.0f;

static bool sIsFinishChange = false;

bool gCanTouchForZoom = true;

float gAntoScale = 1.0f;

static std::string _GetLocaledUIFileFullPath(Language type)
{
	const char* postFix = GetLanguageTypeString(type);
	std::string filePath("UI/UI_utf8str");
	if (postFix[0] != '\0') {
		filePath.push_back('_');
		filePath.append(postFix);
	}
	filePath.append(".bin");

	return FileUtils::getInstance()->fullPathForFilename(filePath.c_str());
}

bool _IsLanguageEnable_LocaledUI(Language type)
{
	return FileUtils::getInstance()->isFileExist(_GetLocaledUIFileFullPath(type).c_str());
}

bool _ReloadLocaledUI();

static bool loaded = false;
static vector<string> table;
static const vector<string>& _GetLocalTextFile()
{
	if (loaded) {
		return table;
	}
	loaded = true;
	_ReloadLocaledUI();
	return table;
}

bool _ReloadLocaledUI()
{
	table.clear();
	std::string fullPath = _GetLocaledUIFileFullPath(GetCurrentLanguage());

	ssize_t lenth;
	unsigned char* buffer = FileUtils::getInstance()->getFileData(fullPath.c_str(), "rb", &lenth);

	unsigned pos = 0;
	while (pos < lenth)
	{
		int size = 0;
		memcpy(&size, buffer + pos, sizeof(int));
		pos += sizeof(int);
		if (size > 0)
		{
			char *pStr = new char[size + 1];
			memcpy(pStr, buffer + pos, sizeof(char)*size);
			pStr[size] = '\0';
			table.push_back(pStr);
			SAFE_DELETE_ARRAY(pStr);
		}
		else
		{
			table.push_back("");
		}
		pos += size;
	}
	SAFE_DELETE_ARRAY(buffer);
	loaded = true;
	return true;
}


//#define DEBUG_DRAW

const int LABLE_TEXT_TAG = 999999;

bool gStopEven = false;

UIManager gUIManager;

#ifdef USED_JUMP
int UIManager::m_draged = 0;
#endif

#ifdef _MSC_VER
#pragma warning( disable: 4244 )
#endif


void DrawFillRect(int x = 0, int y = 0, const CCRect* pRc = NULL)
{}

void UIDataGroup::Load(const char* fileName)
{
	UIDataGroup& g = *this;

	g.name = fileName;
	g.free();

	char filePath[256];
	sprintf(filePath, "UI/%s", fileName);
	READ_BEGIN(filePath);

	READ_INT(g.count);
	READ_INT(g.align);

	//CCLog("--------------------------------------%s\n",fileName);

	g.data = new UIData[g.count];

	for (int i = 0; i < g.count; i++)
	{
		READ_STRING(g.data[i].name);
		READ_INT(g.data[i].x);
		READ_INT(g.data[i].y);
		READ_INT(g.data[i].width);
		READ_INT(g.data[i].height);
		READ_INT(g.data[i].type);

		READ_INT(g.data[i].textType);

		READ_INT16(g.data[i].align);
		READ_INT16(g.data[i].posalign);
		READ_INT16(g.data[i].tag);

		READ_STRING(g.data[i].picName);
		READ_STRING(g.data[i].text);

		if (!_GetLocalTextFile().empty()) {
			// convert index to text here
			bool succeeded = false;
			int index = atoi(g.data[i].text.c_str());
			if (index < (int)_GetLocalTextFile().size())
			{
				g.data[i].text = _GetLocalTextFile()[index];
				succeeded = true;
			}
			if (!succeeded) {
				log("read localized text failed");
			}
		}
		READ_INT16(g.data[i].isEnlarge);
		READ_INT16(g.data[i].isSTensile);
		g.data[i].isEnlarge &= 0x00000001;
		g.data[i].isSTensile &= 0x00000001;
	}

	READ_END();
}

UI* UIData::createUI()
{
	UI* p = NULL;
	if (type == UI_UISCROLLVIEW)
	{
		//p = new UIScrollView();
	}
	else if (type == UI_DRAWABLE)
	{
		//p = new UIDrawable();
	}
	else if (type == UI_TEXT)
	{
		p = new UIText();
	}
	else if (type == UI_UISCROLL)
	{
		//p = new UIScroll();
	}
	else if (type == UI_UIEDIT)
	{
		//p = new UIEditNew();
	}
	else if (type == UI_ANIMATION)
	{
		p = new UIAnimation();
	}
	else if (type == UI_NOTEITEM)
	{
		//p = new UINodeItem();
	}
	else
	{
		p = new UI();
	}
	p->autorelease();
	p->m_name = name;
	p->setPosition(ccp(x, -y - height));
	p->m_type = (TUIType)type;
	p->m_width = width;
	p->m_height = height;
	p->m_align = (1 << ((align / 3) + 3)) | (1 << (align % 3));
	p->m_tag = tag;
	p->setImage(picName, !isSTensile);
	// utf-8 text
	if (_isTextUTF8(text.c_str(), text.length())) {
		p->setText(text);
	}
	else {
		p->setText(ANSI2UTF8(text));
	}
	p->setIsEnlarge(!!isEnlarge);
	if (type == UI_BUTTON)
	{
		p->setUIEffectType(UIMusicType_Button_Check);
		p->setPlayEffect(true);
	}
	else
	{
		p->setUIEffectType(UIMusicType_Button_Select);
		p->setPlayEffect(false);
	}
	p->init();

	return p;
}

UI* UIDataGroup::createUI(int zoder)
{
	UI* p = createUIBase();

	GameScene* scene = GameScene::GetScene();
	scene->m_uiNode->addChild(p, zoder);

	return p;
}

#define BASEWIDTH 960
#define BASEHIGHT 640

UI* UIDataGroup::createUIBase()
{
	UI* p = new UI();
	p->autorelease();

	p->m_name = name;
	p->m_align = align;

	//	int minX = 10000;
	//	int maxX = -10000;
	//
	//	int minY = 10000;
	//	int maxY = -10000;


	UIData* ui = data;



	for (int i = 0; i < count; i++)
	{

		if (ui->name == "SelectPlayerBtn0")
		{
			int a = 0;
			a++;
		}
		//支持多分辨率
		//int uialign = (1<<((ui->align/3)+3)) | (1<<(ui->align%3));
		int uialign = (1 << ((ui->posalign / 3) + 3)) | (1 << (ui->posalign % 3));
		int offsetx = (SCREEN_SIZE.width - BASEWIDTH) / 2;

		if ((uialign & ALIGN_LEFT) != 0 && (align & ALIGN_LEFT) == 0)
		{
			ui->x -= offsetx;
		}

		if ((uialign & ALIGN_RIGHT) != 0 && (align & ALIGN_RIGHT) == 0)
		{
			ui->x += offsetx;
		}

		int offsety = (SCREEN_SIZE.height - BASEHIGHT) / 2;
		if ((uialign & ALIGN_TOP) != 0 && (align & ALIGN_TOP) == 0)
		{
			ui->y -= offsety;
		}

		if ((uialign & ALIGN_BOTTOM) != 0 && (align & ALIGN_BOTTOM) == 0)
		{
			ui->y += offsety;
		}


		UI* pChild = ui->createUI();

		p->addChild(pChild);

		ui++;
	}

	p->init();
	return p;
}


UIManager::UIManager()
{
	//支持多点触控
	//CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);	
	//CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);	

	m_curTouch = NULL;
	m_NonceEdit = NULL;
	m_pOnlyMessage = NULL;
	m_bIsDrag = false;

	m_iTouchNum = 0;

	m_TranslatefPos = ccp(0, 0);

	/*
	// Adds Touch Event Listener
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(UIManager::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(UIManager::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(UIManager::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithFixedPriority(listener, -10);
	_touchListener = listener;
	*/
#ifdef USE_UI_DRAG
	m_pDragUI = NULL;
#endif
}

UIManager::~UIManager()
{
	//for ( map<string,UI*>::iterator i=m_uiMap.begin(); i!=m_uiMap.end(); i++ )
	//{
	//	i->second->release();		
	//}
}

UIManager* UIManager::Instance()
{
	return &gUIManager;
}

bool g_DoTouch = true;
bool UIManager::onTouchBegan(Touch* touch, Event* event)
{
#ifdef USED_JUMP
	m_draged = 0;
#endif

	if (m_NonceEdit)
	{
		m_NonceEdit->detachWithIME();
		m_NonceEdit = NULL;
		g_DoTouch = false;
		return true;
	}

#ifdef USE_UI_DRAG
	if( m_pDragUI )
	{
		m_pDragUI->removeFromParentAndCleanup( true );
		m_pDragUI = NULL;
	}
#endif

	g_DoTouch = true;


	m_curPos = touch->getLocationInView();
	m_holdTime = 0;
	m_curTouch = touch;

	bool isHaveMonopolizeTouchUI = false;
	if (m_pOnlyMessage && !m_pOnlyMessage->isVisible()) setOnlyMessage(NULL);
	if (m_pOnlyMessage)
	{
		UI *p = m_pOnlyMessage->getTouchUI(touch);
		if (p) {
			m_selected.push_back(p);
		}
		else {
			m_selected.push_back(m_pOnlyMessage);
		}
		if (m_NonceEdit)
			m_NonceEdit->detachWithIME();
		m_NonceEdit = NULL;
	}
	else
	{
		GameScene* scene = GameScene::GetScene();
		if (scene == NULL)
			return false;
		CCObject* layer;
		UI * pMonopolizeTouch = NULL;

		for (const auto &layer : scene->m_uiNode->getChildren()) {
			UI* layer1 = (UI*)layer;
			UI* pUI = NULL;

			if (!layer1->isVisible() || layer1->isIgnoreTouch())
				continue;

			if (layer1->isMonopolizeTouch())
				isHaveMonopolizeTouchUI = true;

			if (isHaveMonopolizeTouchUI && !layer1->isMonopolizeTouch())
				continue;
			if (!layer1->isInClipRect(touch))
				continue;

			pUI = layer1->getTouchUI(touch);
			if (pUI == NULL)
			{
				// 如果当前是独占，单无任何控件响应事件，则忽略本次触摸事件
				if (isHaveMonopolizeTouchUI)
					break;
				else
					continue;
			}

			if (pUI->m_type == UI_UIEDIT)
			{
				if (m_NonceEdit) m_NonceEdit->detachWithIME();

				m_NonceEdit = (UIEditNew*)pUI;
				m_NonceEdit->attachWithIME();
			}

			if (((pUI->m_tag == 0) || (!pUI->m_down)) && pUI->isEnable())
			{
				m_selected.push_back(pUI);
				if (pUI->m_tag != 0)
				{
					Node *pParent = pUI->getParent();
					if (pParent)
					{
						CCObject* child;
						for (const auto &child : pParent->getChildren())
						{
							UI* p = (UI*)child;
							if ((p != pUI) && (p->m_tag == pUI->m_tag))
							{
								p->m_down = false;
							}
						}
						pUI->m_down = true;
					}
					break;
				}
			}

			if (pUI->isPenetrateTouch())
			{
				continue;
			}
			else
			{
				break;
			}
		}
	}
	if (m_selected.size())
	{
		for (int i = 0; i < (int)m_selected.size(); ++i)
		{
			m_selected[i]->m_down = true;
			m_selected[i]->OnEvent(EVENT_DOWN, touch);
		}
		return true;
	}

	/*
	//窗口独占情况下，地图不能点击
	if (!isHaveMonopolizeTouchUI) {
		if (GameScene::GetScene()->ccTouchBegan(touch, event)) {
			return true;
		}
	}
	*/
	return true;
}

void UIManager::onTouchMoved(Touch* touch, Event* event)
{
#ifdef USED_JUMP
	m_draged++;
#endif

	m_holdTime = -1;
	if (!g_DoTouch) return;

#ifdef USE_UI_DRAG
	if( m_pDragUI )
	{
		_MoveGragUI( touch );
		return;
	}
#endif

	Vec2 pos = touch->getLocationInView();
	if (!(fabs(pos.x - m_curPos.x) >= 5 || fabs(pos.y - m_curPos.y) >= 5))
	{
		return;
	}

	m_curPos = pos;
	m_curTouch = touch;
	m_bIsDrag = true;
	bool isHaveMonopolizeTouchUI = false;

	if (m_pOnlyMessage && !m_pOnlyMessage->isVisible()) setOnlyMessage(NULL);
	if (m_pOnlyMessage)
	{
		UI *p = m_pOnlyMessage->getTouchUI(touch);
		if (p)
		{
			p->OnEvent(EVENT_DRAG, touch);
		}
		else
		{
			m_pOnlyMessage->OnEvent(EVENT_DRAG, touch);
		}
	}
	else
	{
		bool	isHold = false;
		for (int i = 0; i < (int)m_selected.size(); ++i)
		{
			UI* p = m_selected[i];
			UI* pUI = NULL;

			if (!p->isVisible() || p->isIgnoreTouch())
				continue;

			if (p->isMonopolizeTouch())
				isHaveMonopolizeTouchUI = true;

			if (isHaveMonopolizeTouchUI && !p->isMonopolizeTouch())
				continue;

			if (!p->isInClipRect(touch))
				continue;

			p->OnEvent(EVENT_DRAG, touch);

			CCRect rect;
			if (p->isEnlarge())
			{
				rect = CCRectMake(p->m_width / 4, p->m_height / 4, p->m_width + p->m_width / 2, p->m_height + p->m_height / 2);
			}
			else
			{
				rect = CCRectMake(0, 0, p->m_width, p->m_height);
			}

			/*
			if (CCRect::CCRectContainsPoint(rect, p->convertTouchToNodeSpaceAR(touch)))
			{
				isHold = true;
			}
			*/
		}

		if (!isHold)
		{
			m_holdTime = -1;
		}
		if (!m_selected.size())
		{
			if (!isHaveMonopolizeTouchUI)
			{
				//GameScene::GetScene()->ccTouchMoved(touch, event);
			}
		}
	}
}

void UIManager::onTouchEnded(CCTouch* touch, CCEvent* event)
{
	if (!g_DoTouch) return;
	m_curPos = touch->getLocationInView();
	m_curTouch = touch;

	if (m_pOnlyMessage && !m_pOnlyMessage->isVisible()) setOnlyMessage(NULL);
	if (m_pOnlyMessage)
	{
		UI *p = m_pOnlyMessage->getTouchUI(touch);
		if (p)
		{
#ifdef USE_UI_DRAG
			_HandleGragEvent( p, touch );
#endif
			p->m_down = false;
			p->OnEvent(EVENT_UP, touch);
		}
		else if (m_pOnlyMessage)
		{
#ifdef USE_UI_DRAG
			_HandleGragEvent( m_pOnlyMessage, touch );
#endif
			m_pOnlyMessage->m_down = false;
			m_pOnlyMessage->OnEvent(EVENT_UP, touch);
		}
		/*
		if(!GuideModule::instance().IsInGuide())
		{
		for( int i=0; i<(int)m_selected.size(); ++i )
		{
		if( m_selected[i]->m_tag == 0 )
		m_selected[i]->m_down = false;
		}
		m_selected.clear();
		#ifdef USE_UI_DRAG
		_HandleGragEvent( NULL, touch );
		#endif
		return;
		}
		*/
		m_bIsDrag = false;
	}

	vector<UI*> upUIList;
	GameScene* scene = GameScene::GetScene();
	CCObject* layer;
	bool isHaveMonopolizeTouchUI = false;
	for (const auto &layer : scene->m_uiNode->getChildren())
	{
		UI* layer1 = (UI*)layer;
		UI* pUI = NULL;
		if (!layer1->isVisible() || layer1->isIgnoreTouch())
			continue;

		if (layer1->isMonopolizeTouch())
			isHaveMonopolizeTouchUI = true;

		if (isHaveMonopolizeTouchUI && !layer1->isMonopolizeTouch())
			continue;

		if (!layer1->isInClipRect(touch))
			continue;

		pUI = layer1->getTouchUI(touch);

		if (pUI != NULL)
		{
#ifdef USE_UI_DRAG
			_HandleGragEvent( pUI, touch );
#endif
			for (int i = 0; i < (int)m_selected.size(); ++i)
			{
				if (pUI == m_selected[i])
				{
					upUIList.push_back(pUI);
					break;
				}
			}
			if (pUI->isPenetrateTouch())
				continue;
			break;
		}
	}

#ifdef USE_UI_DRAG
	_HandleGragEvent( NULL, touch );
#endif

	for (int i = 0; i < (int)upUIList.size(); ++i) {
		upUIList[i]->OnEvent(EVENT_UP, touch);
	}

	if (m_selected.size()) {
		for (int i = 0; i < (int)m_selected.size(); ++i) {
			if (m_selected[i]->m_tag == 0)
				m_selected[i]->m_down = false;
			if (m_bIsDrag)  {
				m_selected[i]->OnEvent(EVETN_DRAG_END, touch);
			}
		}
		m_selected.clear();
	}
	else {
		/*
		if( !isHaveMonopolizeTouchUI )
		{
		GameScene::GetScene()->ccTouchEnd(touch, event);
		}
		*/
	}
	m_bIsDrag = false;
}



RenderBatchData *UI::m_MaskingBack = NULL;
map<string, UI::UIRect9Data>	UI::m_sRect9Datatable;
string	UI::m_strMusicEffectFileName[UIMusicType_Button_Num] = { "Music/button1.spd", "Music/closebutton.spd", "Music/select.spd" };
string	UI::m_strTextFontName[eTextType_Num] = { FONT_BMP_24 };


UI::UI() :
bIsRemove(false),
m_pText(NULL),
m_pTextDown(NULL),
m_down(false),
m_bIsIgnoreTouch(false),
bIsMonopolizeTouch(false),
m_bIsPenetrateTouch(false),
m_tag(0),
m_pUpdateFun(NULL),
m_bIsMasking(false),
bIsEnable(true),
m_isEnlarge(false),
m_eMusicEffectType(UIMusicType_Button_Check),
m_bIsPlayEffect(false),
m_bisRoot(false),
m_eTextType(eTextType_MsFont),
m_onlyUseVertexColor(false)
#ifdef USE_UI_DRAG
,m_iDragSrcTag( -1 ),
m_pDragSrcData( NULL ),
m_iDragDestTag( NULL ),
m_pDragFunc( NULL ),
m_pCreateDragFunc( NULL )
#endif
{
	memset(m_eventData, 0, sizeof(m_eventData));
	memset(m_otherEventData, 0, sizeof(m_otherEventData));

	m_clipRect = NULL;

	m_color = ccc4(255, 255, 255, 255);

	m_rect9 = NULL;

	m_width = 0;
	m_height = 0;
	m_nParam64 = 0;

	m_startLineX = 0;		//start point of line
	m_startLineY = 0;
	m_endLineX = 0;			//end point of line
	m_endLineY = 0;
	m_isReverseTex = false;

	m_isLine = false;
	m_isTop = false;
}

UI::~UI()
{
	if (m_clipRect)
	{
		delete m_clipRect;
	}

	if (m_rect9) delete m_rect9;

	UIManager::RemoveEvent(this);
	UIManager::RemoveOtherEvent(this);

	if (UIManager::Instance()->getOnlyMessage() == this)
	{
		UIManager::Instance()->setOnlyMessage(NULL);
	}

	m_text = "";
}

bool UI::init()
{
	return true;
}

bool UI::isInClipRect(CCTouch* touch)
{
	if (!m_clipRect)
		return true;

	// 	Vec2  pos( touch->locationInView().x, touch->locationInView().y );
	// 	pos = CCDirector::sharedDirector()->convertToGL(pos);

	Vec2 r_pos = this->getRealPos();
	CCRect rect;
	rect.origin = ccpAdd(r_pos, m_clipRect->origin);
	rect.size = m_clipRect->size;

	return rect.containsPoint(touch->getLocation());
}

void UI::setMonopolizeTouch(bool b)
{
	bIsMonopolizeTouch = b;
	if (bIsMonopolizeTouch)
	{
		Top();
		//m_bIsMasking = true;
	}
}



UI* UI::getTouchUI(CCTouch* touch)
{
	return getTouchUI(0, 0, touch);
}

UI* UI::getTouchUI(int beginX, int beginY, CCTouch* touch)
{
	CCObject* child;

	auto& children = getChildren();
	for (const auto &child : children) {

		UI* p = dynamic_cast<UI*> (child);

		if (!p) continue;

		if (p->isVisible() && !p->isIgnoreTouch())
		{
			UI* pSel = p->getTouchUI(beginX, beginY, touch);

			if (pSel) return pSel;

			CCRect rect;
			if (p->m_isEnlarge)
			{
				float w = p->m_width / 4;
				w = w > 32 ? 32 : w;

				float h = p->m_height / 4;
				h = h > 32 ? 32 : h;

				rect = CCRectMake(beginX - w, beginY - h, p->m_width + (w * 2), p->m_height + (h * 2));
			}
			else
			{
				rect = CCRectMake(beginX, beginY, p->m_width, p->m_height);
			}


			if (rect.containsPoint( p->convertTouchToNodeSpaceAR(touch)))
			{
				return p;
			}
			
		}
	}

	return NULL;
}

float UI::GetHoldTime()
{
	for (int i = 0; i < (int)gUIManager.m_selected.size(); ++i)
	{
		if (this == gUIManager.m_selected[i])
		{
			return gUIManager.m_holdTime;
		}
	}
	return -1;
}

void UI::update(float t)
{
	if (m_pUpdateFun && m_pUpdateFun(this, t))
	{
		return;
	}

	CCObject* child;
	auto& children = getChildren();
	for (const auto &child : children) {
		Node* p = dynamic_cast<Node*>(child);
		if (p && p->isVisible())
		{
			p->update(t);
		}
	}
}

void UI::setLinePos(int sx, int sy, int ex, int ey)
{
	m_startLineX = sx;
	m_startLineY = sy;
	m_endLineX = ex;
	m_endLineY = ey;
	m_isLine = true;
}

CCRect UI::getGlobalRect()
{
	// 	Node* parent = this->getParent();
	// 	Vec2 g_pos = this->getPosition();
	// 
	// 	while (parent)
	// 	{
	// 		g_pos = ccpAdd(g_pos, parent->getPosition());
	// 
	// 		parent = parent->getParent();
	// 	}

	Vec2 g_pos = this->getRealPos();

	CCRect rect;
	rect.origin = g_pos;
	rect.size.width = m_width;
	rect.size.height = m_height;

	return rect;
}

void UI::needReCalDownTex()
{
	RenderBatchData* downRenderBatch = m_image.m_downRenderBatch;
	RenderBatchData* nRenderBatch = m_image.m_RenderBatch;
	//source size
	int SX = nRenderBatch->_SourceSizeX;
	int SY = nRenderBatch->_SourceSizeY;
	int DX = downRenderBatch->_SourceSizeX;
	int DY = downRenderBatch->_SourceSizeY;
	//uv value
	int SU = nRenderBatch->_ConvertU;
	int SV = nRenderBatch->_ConvertV;
	int DU = downRenderBatch->_ConvertU;
	int DV = downRenderBatch->_ConvertV;
	//change the down uv to fit the normal one
	int CX = DX - SX;
	int CY = DY - SY;

	if (CX != 0 && CY != 0)
	{
		downRenderBatch->_SourceSizeX = SX;
		downRenderBatch->_SourceSizeY = SY;
		downRenderBatch->_ConvertU += CY / 2;
		downRenderBatch->_ConvertV += CX / 2;
	}
}

void UI::setReverseTex(bool reverse)
{
	m_isReverseTex = reverse;
}

void UI::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
	int offY = 0;

	if (m_bIsMasking)
	{
		if (!m_MaskingBack)
		{
			m_MaskingBack = ImageCenter::instance().GetRenderBatch("UI/LRes/NPCCover2.png");
		}
		//if (m_MaskingBack)
		//	DrawImage(m_MaskingBack, -SCREEN_SIZE.width / 2, -SCREEN_SIZE.height / 2 - 100, SCREEN_SIZE.width, SCREEN_SIZE.height + 100);
	}

	RenderBatchData* nRenderBatch = m_image.m_RenderBatch;

	if (m_type == UI_BUTTON || m_type == UI_BTN_feature)
	{
		if (m_down)
		{
			if (m_image.m_downRenderBatch != NULL)
			{
				//don't use it if no need
				if (nRenderBatch != NULL)
				{
					needReCalDownTex();
				}

				nRenderBatch = m_image.m_downRenderBatch;
			}
			else
			{
				offY -= 2;
			}

			if (m_pText) m_pText->setVisible(false);
			if (m_pTextDown) m_pTextDown->setVisible(true);
		}
		else
		{
			if (m_pText) m_pText->setVisible(true);
			if (m_pTextDown) m_pTextDown->setVisible(false);
		}
	}

	// 	CCRect rect = this->getGlobalRect();
	// 
	// 	rect.origin = Vec2Zero;
	// 	Vec2 rt = rect.origin;
	// 	rt.x += rect.size.width;
	// 	rt.y += rect.size.height;
	// 
	// 	ccDrawRect(rect.origin, rt);

	if (nRenderBatch != NULL)
	{
		GLProgram* p = NULL;
		if (m_onlyUseVertexColor)
		{
			p = ShaderCache::getInstance()->programForKey(GLProgram::SHADER_NAME_POSITION_U_COLOR);
		}
		else
		{

			p = ShaderCache::getInstance()->programForKey(GLProgram::SHADER_NAME_POSITION_TEXTURE_U_COLOR);
		}

		//UIBatchRenderer::instance()->setShader(p);

		//add line flag
		if (!m_isReverseTex)
		{
			/*
			if (m_rect9)
				DrawImage9(nRenderBatch, 0, offY, m_width, m_height, m_color, *m_rect9);
			else
				DrawImage(nRenderBatch, 0, offY, m_width, m_height, m_color);
				*/
		}
		else
		{
			//reverse texture.add by wcc.
			//DrawImage_Reverse(nRenderBatch, 0, offY, m_width, m_height, m_color);
			//calculate vertex and uv value
			// 			float left,right,top,bottom;
			// 			CCTexture2D *tex = m_image.m_IconTexture;
			// 
			// 			float width = tex->getContentSize().width;
			// 			float height =  tex->getContentSize().height;
			// 			float pixw =  tex->getPixelsWide();
			// 			float pixh =  tex->getPixelsHigh();
			// 
			// 			left = (2 * 0.0f + 1) / (2 * pixw);
			// 			right = left + (width * 2 - 2) /(2 * pixw);
			// 			top = (2 * 0.0f + 1)/(2 * pixh);
			// 			bottom = top+(height * 2 - 2) /(2 * pixh);
			// 
			// 			Vec2 dir(m_endLineX-m_startLineX, m_endLineY-m_startLineY);
			// 			float len = sqrt(dir.x*dir.x + dir.y*dir.y);
			// 			dir.x = dir.x / len;
			// 			dir.y = dir.y / len;
			// 
			// 			//draw line part
			// 			DrawImage2(m_image.m_IconTexture, 0.4f, bottom, 0.6f, top,m_startLineX, m_startLineY, m_endLineX, m_endLineY, m_color);
			// 			//draw dot part
			// 			DrawImage2(m_image.m_IconTexture, 0.6f, bottom, right, top,m_startLineX - dir.x * width / 2, m_startLineY - dir.y * height / 2, m_startLineX, m_startLineY, m_color);
			// 			DrawImage2(m_image.m_IconTexture, left, bottom, 0.4f, top,m_endLineX, m_endLineY, m_endLineX + dir.x * width / 2, m_endLineY + dir.y * height / 2, m_color);

		}

	}



	if (m_image.m_IconTexture != NULL)
	{
		//DrawImage(m_image.m_IconTexture, 0, offY, m_width, m_height, m_color);
	}


	if (m_clipRect)
	{
#ifdef  UI_BATCH_RENDERER
		UIBatchRenderer::instance()->flush();
#endif
	}


#ifdef DEBUG_DRAW
	Vec2 pos = getPosition();
	if( m_width > 0 && m_height >0 )
		FillRect( CCRectMake( 0, 0, m_width, m_height), ccc4(255,255,255,255), true);
#endif
}


void UI::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags)
{
	/*
	//PROFILE("UI::visit");
	if (m_clipRect)
	{
#ifdef  UI_BATCH_RENDERER
		UIBatchRenderer::instance()->flush();
#endif
		//glEnable(GL_SCISSOR_TEST);
		myGLEnableScissorTest();

		Vec2 realPos = getRealPos();

		float xScale = CCEGLView::sharedOpenGLView()->getFrameSize().width / CCEGLView::sharedOpenGLView()->getDesignResolutionSize().width;
		float yScale = CCEGLView::sharedOpenGLView()->getFrameSize().height / CCEGLView::sharedOpenGLView()->getDesignResolutionSize().height;

		UIManager::Instance()->m_nonceClipRect.origin.x = (realPos.x + m_clipRect->origin.x) * xScale;
		UIManager::Instance()->m_nonceClipRect.origin.y = (realPos.y + m_clipRect->origin.y) * yScale;
		UIManager::Instance()->m_nonceClipRect.size.width = m_clipRect->size.width * xScale;
		UIManager::Instance()->m_nonceClipRect.size.height = m_clipRect->size.height * yScale;

		glScissor((realPos.x + m_clipRect->origin.x) * xScale, (realPos.y + m_clipRect->origin.y) * yScale, m_clipRect->size.width * xScale, m_clipRect->size.height * yScale);
	}

	if ( myIsScissorTestEnabled())
	{
		float xScale = CCEGLView::sharedOpenGLView()->getFrameSize().width / CCEGLView::sharedOpenGLView()->getDesignResolutionSize().width;
		float yScale = CCEGLView::sharedOpenGLView()->getFrameSize().height / CCEGLView::sharedOpenGLView()->getDesignResolutionSize().height;
		CCRect rc, rc2;
		rc.origin = convertToWorldSpaceAR(ccp(0, 0));
		rc.origin.x += UIManager::Instance()->m_TranslatefPos.x;
		rc.origin.y += UIManager::Instance()->m_TranslatefPos.y;
		rc.origin.x *= xScale;
		rc.origin.y *= yScale;
		rc.size.width = m_width * xScale;
		rc.size.height = m_height * yScale;

		rc2 = UIManager::Instance()->m_nonceClipRect;
		if (rc.size.width == 0 || rc.size.height == 0 || CCRect::CCRectIntersectsRect(rc, rc2))
		{
			setIsDraw(true);
		}
		else
		{
			setIsDraw(false);
		}
	}
	else
	{
		setIsDraw(true);
	}

	Node::visit();


	if (m_clipRect)
	{
		UIBatchRenderer::instance()->flush();
		myGLDisableScissorTest();
	}
	*/
}

void UI::setColor(Color4B color)
{
	m_color = color;

	CCObject* child;
	auto& children = getChildren();
	for (const auto &child : children) {

		UI* p = dynamic_cast<UI*>(child);
		if (p)
		{
			p->setColor(m_color);
		}
	}
}

void UI::setText(const std::string& pszText)
{
	setText(pszText, 24, CCSize(m_width, m_height), tAlignCenterX | tAlignCenterY);
}

void UI::setText(const std::string& strText, int fontSize)
{
	setText(strText, fontSize, CCSize(m_width, m_height), tAlignCenterX | tAlignCenterY);
}

void UI::setText(const std::string& strText, int fontSize, int alignType)
{
	setText(strText, fontSize, CCSize(m_width, m_height), alignType);
}
void UI::setText(int number)
{
	char a[256];
	sprintf(a, "%d", number);

	setText(a, 24, CCSize(m_width, m_height), tAlignCenterX | tAlignCenterY);
}


void UI::setText(const std::string& strText, int fontSize, const CCSize &size, int alignType, bool isAutoNewLine)
{
	setText(strText, fontSize, size, alignType, m_strTextFontName[m_eTextType].c_str(), isAutoNewLine);
}

void		UI::setText(const std::string& strText, int fontSize, const CCSize &size, int alignType, string fontName, bool isAutoNewLine)
{
	if (m_text == strText)
		return;
	m_text = strText;

	if (m_pText)
	{
		m_pText->setText(m_text);
	}
	else
	{
		m_pText = UIText::initUITextWithString(strText, fontSize, size, alignType, fontName, isAutoNewLine);
		addChild(m_pText);
	}

	if (m_type == UI_BUTTON)
	{
		char str[256];
		sprintf(str, "<C=149,212,251>%s</C>", strText.c_str());
		if (m_pTextDown)
		{
			m_pTextDown->setText(str);
			m_pTextDown->setVisible(false);
		}
		else
		{
			m_pTextDown = UIText::initUITextWithString(str, fontSize, size, alignType, fontName, isAutoNewLine);
			m_pTextDown->setVisible(false);
			addChild(m_pTextDown);
		}
	}
}


void UI::setImage(const string& name, bool boundingWH)
{
	if (name == "") return;

	char buf[256];

	sprintf(buf, "UI/LoadingRes/%s", name.c_str());

	//CCTexture2D::setUSED_ANTI_ALIAS( false );
	m_image.m_RenderBatch = ImageCenter::instance().GetRenderBatch(buf);

	//Tyrzhao: Temp Use Two Different File
	if (m_image.m_RenderBatch == NULL)
	{
		sprintf(buf, "UI/LRes/%s", name.c_str());
		m_image.m_RenderBatch = ImageCenter::instance().GetRenderBatch(buf);
	}

	int len = strlen(buf);

	sprintf(&buf[len - 4], "_down.png");

	m_image.m_downRenderBatch = ImageCenter::instance().GetRenderBatch(buf);

	//CCTexture2D::setUSED_ANTI_ALIAS( true );

	if (m_image.m_RenderBatch != NULL)
	{
		//m_image.x1 = -1;
		//m_image.y1 = -1;
		//m_image.x2 = -1;
		//m_image.y2 = -1;

		if (boundingWH)
		{
			m_width = m_image.m_RenderBatch->_SourceSizeX;
			m_height = m_image.m_RenderBatch->_SourceSizeY;
		}
	}

	// load rect9
	{
		sprintf(buf, "UI/%s", name.c_str());

		int len = strlen(buf);
		strcpy(&buf[len - 3], "txt");

		map<string, UIRect9Data>::iterator it = m_sRect9Datatable.find(buf);
		if (it == m_sRect9Datatable.end())
		{
			UIRect9Data r9data;

			ssize_t length;
			unsigned char* pszBuffer = FileUtils::getInstance()->getFileData(buf, "rb", &length);

			if (length <= 0 || pszBuffer == NULL)
			{
				r9data.isHave = false;
				m_sRect9Datatable[buf] = r9data;
				return;
			}


			int n[4] = { 0 };

			int pos = 0;
			for (int i = 0; i < (int)length; i++)
			{
				if ((pszBuffer[i] == ',')
					)
				{
					pos++;

				}
				else if (pszBuffer[i] >= '0' && pszBuffer[i] <= '9')
				{
					n[pos] = n[pos] * 10 + pszBuffer[i] - '0';
				}
			}
			r9data.isHave = true;
			r9data.rect = CCRect(n[0], n[1], n[2] - n[0], n[3] - n[1]);

			m_sRect9Datatable[buf] = r9data;
			if (m_rect9)
				*m_rect9 = CCRect(n[0], n[1], n[2] - n[0], n[3] - n[1]);
			else
				m_rect9 = new CCRect(n[0], n[1], n[2] - n[0], n[3] - n[1]);
			CC_SAFE_DELETE_ARRAY(pszBuffer);
		}
		else
		{
			if (it->second.isHave)
			{
				if (m_rect9)
					*m_rect9 = CCRect(it->second.rect);
				else
					m_rect9 = new CCRect(it->second.rect);
			}
		}
	}

}

extern std::string _fixPathByLanguage(const char* resName);

void UI::setImageByFullPath(const string& name, bool boundingWH)
{
	std::string realTexName = name;
	int pos = realTexName.find_first_of("/");
	if (pos != string::npos)
	{
		string path = realTexName.substr(0, pos);
		if (path == "Origin" || path == "ImageCenter")
		{
			realTexName = _fixPathByLanguage(realTexName.c_str());
		}
	}
#if !( defined WIN32 || defined _WIN32 )
	realTexName = realTexName.substr(0, realTexName.find(".")) + ".pvr.ccz";
#endif
	m_image.m_RenderBatch = ImageCenter::instance().GetRenderBatch(realTexName.c_str());
	if (m_image.m_RenderBatch == NULL && m_image.m_IconName != realTexName)
	{
		if (m_image.m_IconTexture)
		{
			m_image.m_IconTexture->release();
		}
		m_image.m_IconTexture = CCTextureCache::sharedTextureCache()->textureForKey(realTexName.c_str());
		if (m_image.m_IconTexture)
		{
			/*m_image.m_IconName = realTexName;*/
			if (boundingWH)
			{
				m_width = m_image.m_IconTexture->getContentSize().width;
				m_height = m_image.m_IconTexture->getContentSize().height;
			}
			m_image.m_IconTexture->retain();
		}
		else
		{
			m_image.m_IconTexture = CCTextureCache::sharedTextureCache()->addImage(realTexName.c_str());
			if (m_image.m_IconTexture)
			{
				if (boundingWH)
				{
					m_width = m_image.m_IconTexture->getContentSize().width;
					m_height = m_image.m_IconTexture->getContentSize().height;
				}
			}
		}
	}

	m_image.m_downRenderBatch = NULL;
	m_image.m_IconName = realTexName;

	if (boundingWH && m_image.m_RenderBatch)
	{
		m_width = m_image.m_RenderBatch->_SourceSizeX;
		m_height = m_image.m_RenderBatch->_SourceSizeY;
	}

	//if ( m_image.m_texture != NULL )
	//{
	//	m_image.x1 = -1;
	//	m_image.y1 = -1;
	//	m_image.x2 = -1;
	//	m_image.y2 = -1;		
	//}
}


void UI::setIconTex(CCTexture2D* tex, bool boundingWH)
{
	if (m_image.m_IconTexture)
		m_image.m_IconTexture->release();
	m_image.m_IconTexture = tex;
	m_image.m_IconName = "";
	if (m_image.m_IconTexture)
		m_image.m_IconTexture->retain();
	if (boundingWH && m_image.m_IconTexture)
	{
		m_width = m_image.m_IconTexture->getContentSize().width;
		m_height = m_image.m_IconTexture->getContentSize().height;
	}
}


UI* UI::findUI(const string& name)
{
	Ref* child;
	auto& children = getChildren();
	for (const auto &child : children) {

		UI* p = (UI*)child;
		if (p->m_name == name)
		{
			return p;
		}
	}

	return NULL;
}

void UI::setEvent(TEventType msg, TEventFun fun, void* data)
{
	m_eventData[msg].funcEvent = fun;
	m_eventData[msg].pData = data;
}

void UI::setOtherEvent(TEventType msg, TEventFun fun, void* data)
{
	m_otherEventData[msg].funcEvent = fun;
	m_otherEventData[msg].pData = data;
}


void UI::Top()
{
	Node* p = getParent();

	if (p != NULL)
	{
		this->retain();
		p->removeChild(this, false);
		p->addChild(this);
		this->release();
	}

	if (m_bisRoot)
	{
		UIManager::Instance()->topUI(this);
	}
}

void UI::OnEvent(TEventType msg, CCTouch* touch)
{
	if (m_eventData[msg].funcEvent && bIsEnable)
	{
		UIManager::PushEvent(this, m_eventData[msg]);
	}
	if (m_otherEventData[msg].funcEvent && bIsEnable)
	{
		UIManager::PushOtherEvent(this, m_otherEventData[msg]);
	}
	if (m_bIsPlayEffect && msg == EVENT_UP && m_eMusicEffectType < UIMusicType_Button_Num)
	{
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(m_strMusicEffectFileName[m_eMusicEffectType].c_str());
	}
}

UI* UI::loadChild(const string& name)
{
	UI* rt = NULL;

	UIDataGroup group;

	group.Load(name.c_str());

	rt = group.createUIBase();

	if (rt)
	{
		addChild(rt);
	}
	UIManager::Instance()->addItemToChildMap(name, this);
	return rt;
}


Vec2 UI::getCurPos()
{
	return gUIManager.m_curPos;
}

UI* UI::node(void)
{
	UI* ret = new UI();
	ret->autorelease();
	return ret;
}

bool UI::isParentVisible()
{
	UI* pParentUI = this;
	UI* pUITemp = this;
	while (pUITemp != NULL&&pParentUI != NULL)
	{
		pParentUI = pUITemp;
		Node *pParent = pUITemp->getParent();
		if (pParent)
			pUITemp = dynamic_cast<UI*>(pUITemp->getParent());
		else
			pUITemp = NULL;
	}
	return pParentUI->isVisible();
}

Vec2  UI::getRealPos()
{
	Point realPos = getPosition();

	Node *pParent = getParent();
	while (pParent != NULL)
	{
		realPos.x += pParent->getPositionX();
		realPos.y += pParent->getPositionY();
		/*
		UIScrollView *pView = dynamic_cast<UIScrollView*>(pParent);
		if (pView)
		{
			Point pos = pView->getStartPos();
			realPos.x += pos.x;
			realPos.y += pos.y;
		}
		*/
		pParent = pParent->getParent();
	}
	return realPos;
}

bool	UI::isHaveUnKnowChar()
{
	if (m_pText)
	{
		return m_pText->isHaveUnKnowChar();
	}
	return false;
}


UI* UI::SetSubValue(string UIName, string Text)
{
	UI *p = this->findUI(UIName);
	if (!p)	return false;
	p->setText(Text);
	return p;
}


void UI::SetClip(const CCRect& rect)
{
	if (!m_clipRect) m_clipRect = new CCRect();

	*m_clipRect = rect;
	if (m_clipRect->size.width < 0.0f) m_clipRect->size.width = 0.0f;
	if (m_clipRect->size.height < 0.0f) m_clipRect->size.height = 0.0f;
}

EventFunData* UI::getUIEvent(TEventType type)
{
	if (type >= EVENT_DOWN && type < EVENT_SIZE)
		return &m_eventData[type];
	else
		return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////
void UIManager::topUI(UI *ui)
{
	if (!ui->m_bisRoot)
		return;

	for (UIMap::iterator mit = m_uiMap.begin(); mit != m_uiMap.end(); ++mit)
	{
		if (mit->second == ui)
		{
			if (m_stack.size() > 0)
			{
				// find ui
				for (std::vector<UI*>::iterator it = m_stack.begin();
					it != m_stack.end();
					++it)
				{
					if (*it == ui)
					{
						m_stack.erase(it);
						break;
					}
				}
			}

			// push
			UI* old_top = m_stack.size() > 0 ? (*(m_stack.rbegin())) : NULL;

			if (old_top && old_top != ui)
			{
				old_top->onLoseTop();
				//CCLOG("ON LOSE TOP:%s\n", old_top->m_name.c_str());
			}

			// push
			m_stack.push_back(ui);

			// new top
			ui->onGetTop();
			//CCLOG("ON GET TOP:%s\n", ui->m_name.c_str());

			break;
		}
	}
}

void UIManager::onLoseTop(UI *ui)
{
	if (!ui->m_bisRoot)
		return;

	// lose
	UI* old_top = m_stack.size() > 0 ? (*(m_stack.rbegin())) : NULL;

	if (old_top && old_top == ui)
	{
		old_top->onLoseTop();
		//CCLOG("ON LOSE TOP:%s\n", old_top->m_name.c_str());
	}

	// remove
	for (std::vector<UI*>::iterator it = m_stack.begin();
		it != m_stack.end();
		++it)
	{
		if (*it == ui)
		{
			m_stack.erase(it);
			break;
		}
	}

	// cur
	UI* cur_top = m_stack.size() > 0 ? (*(m_stack.rbegin())) : NULL;
	if (cur_top)
	{
		cur_top->onGetTop();
		//CCLOG("nON GET TOP:%s\n", cur_top->m_name.c_str());
	}

}

void UIManager::onGetTop(UI *ui)
{
	// lose
	Node* p = ui->getParent();

	if (p != NULL)
	{
		ui->retain();
		p->removeChild(ui, false);
		p->addChild(ui);
		ui->release();
	}

	this->topUI(ui);
}

void UIManager::clear()
{
	clearChildMap();
	m_uiMap.clear();
	m_iTouchNum = 0;
	m_pOnlyMessage = NULL;
	m_NonceEdit = NULL;
	m_selected.clear();
	m_curTouch = NULL;

	m_stack.clear();
}

UI* UIManager::findUI(const string& name)
{
	map<string, UI*>::iterator it = m_uiMap.find(name);

	if (it != m_uiMap.end())
	{
		return it->second;
	}

	return NULL;
}
string	UIManager::findTopRootName(const string name)
{
	map<string, UI*>::iterator it1 = m_uiMap.find(name);
	if (it1 != m_uiMap.end())
	{
		return name;
	}
	std::map<std::string, ChildUiLocateItem>::iterator it2 = m_mapChildLocatea.find(name);
	if (it2 != m_mapChildLocatea.end())
	{
		return it2->second.parentNameArr[0];
	}
	return "";
}
UI*	UIManager::findUIByName(const string name)
{
	UI*	findTarget = findUI(name);
	if (findTarget == NULL)
	{
		findTarget = findUIInChildMap(name);
	}
	return findTarget;
}
void UI::removeChild(UI* child, bool cleanup)
{
	if (child && child->m_name != "")
	{
		UIManager::Instance()->removeItemFromChildMap(child->m_name);
	}
	Node::removeChild(child, cleanup);
}
//在childmap表查找所需ui
UI* UIManager::findUIInChildMap(const string& name)
{
	std::map<std::string, ChildUiLocateItem>::iterator it = m_mapChildLocatea.find(name);
	if (it != m_mapChildLocatea.end())
	{
		if (!it->second.parentNameArr.empty())
		{
			if (it->second.parentNameArr[0] != "")
			{
				UI*	findTarget = findUI(it->second.parentNameArr[0]);
				if (findTarget)
				{
					int layerNum = it->second.parentNameArr.size();
					for (int i = 1; i < layerNum; i++)
					{
						if (it->second.parentNameArr[i] != "")
						{
							findTarget = findTarget->findUI(it->second.parentNameArr[i]);
							if (findTarget == NULL)
							{
								return NULL;
							}
						}
						else
						{
							return NULL;
						}
					}
					return findTarget->findUI(name);
				}
			}
		}
	}
	return NULL;
}
//在使用loadchild时添加新节点到childmap表
void UIManager::addItemToChildMap(const string name, UI* thisUi)
{
	GameScene* scene = GameScene::GetScene();
	vector<string> tempNameVec;
	if (thisUi)
	{
		tempNameVec.push_back(thisUi->m_name);
		if (thisUi != scene->m_uiNode)
		{
			UI*	findParents = (UI*)(thisUi->getParent());
			while (findParents && findParents != scene->m_uiNode)
			{
				tempNameVec.push_back(findParents->m_name);
				findParents = (UI*)(findParents->getParent());
			}
		}
		ChildUiLocateItem locateItem;
		int layerNum = tempNameVec.size() - 1;
		locateItem.parentNameArr.resize(tempNameVec.size());
		for (int i = layerNum; i >= 0; i--)
		{
			locateItem.parentNameArr[i] = tempNameVec[layerNum - i];
		}
		m_mapChildLocatea[name] = locateItem;
	}
}
//在删除节点时检测表中是否存在，存在则删除
void UIManager::removeItemFromChildMap(const string name)
{
	std::map<std::string, ChildUiLocateItem>::iterator it = m_mapChildLocatea.find(name);
	if (it != m_mapChildLocatea.end())
	{
		m_mapChildLocatea.erase(it);
	}
}
//清理整个节点表
void UIManager::clearChildMap()
{
	m_mapChildLocatea.clear();

}
void UIManager::update(float dt)
{
	if (m_holdTime >= 0) m_holdTime += dt;

#ifdef USE_UI_DRAG
	if( m_holdTime >= 1.0f )
	{
		for( size_t i=0; i<m_selected.size(); ++i )
		{
			if( !m_pDragUI )
			{
				_CreateGragUI( m_selected[i], m_curTouch );
			}
		}
	}
#endif

	Node::update(dt);

	if (m_pOnlyMessage)
	{
		if (!m_pOnlyMessage->isVisible())
		{
			setOnlyMessage(NULL);
		}
	}
	for (map<string, UI*>::iterator i = m_uiMap.begin(); i != m_uiMap.end(); i++)
	{
		if (i->second->isVisible())
		{
			i->second->update(dt);
		}
	}
	
	/*
	for (map<string, UI*>::iterator i = m_uiMap.begin(); i != m_uiMap.end(); i++)
	{
		if (i->second->isRemove())
		{
			CMainUIOpenCloseHandle::instance().DelHandleUI(i->second);
			i->second->removeFromParentAndCleanup(true);
			m_uiMap.erase(i);
			break;
		}
	}

	//////////////////////////////update for zoom in/out////////////////////////////////////////////

	if (sIsFinishChange) {
		Map* pMap = GameScene::GetScene()->GetCurMap();
		CCamera* pCamera = pMap ? pMap->GetCamera() : 0;

		if (sNowScale > _STANDARD_MAX_SCREEN_SCALE)
		{
			sNowScale -= 0.5f*dt;
			if (sNowScale < _STANDARD_MAX_SCREEN_SCALE) {
				sNowScale = _STANDARD_MAX_SCREEN_SCALE;
			}

			if (pCamera)
			{
				gAntoScale = sNowScale;
				pCamera->setSceneScale(sNowScale);
			}
		}
		else if (sNowScale < _STANDARD_MIN_SCREEN_SCALE)
		{
			sNowScale += 0.5f*dt;
			if (sNowScale > _STANDARD_MIN_SCREEN_SCALE) {
				sNowScale = _STANDARD_MIN_SCREEN_SCALE;
			}

			if (pCamera)
			{
				gAntoScale = sNowScale;
				pCamera->setSceneScale(sNowScale);
			}
		}
	}

	
	if (sNowScale != gAntoScale)
	{
		if (sNowScale < gAntoScale)
		{
			sNowScale += 0.5f*dt;
			if (sNowScale >= gAntoScale)
			{
				sNowScale = gAntoScale;
			}
		}
		else
		{
			sNowScale -= 0.5f*dt;
			if (sNowScale <= gAntoScale)
			{
				sNowScale = gAntoScale;
			}
		}

		Map* pMap = GameScene::GetScene()->GetCurMap();
		CCamera* pCamera = pMap ? pMap->GetCamera() : 0;
		if (pCamera)
		{
			pCamera->setSceneScale(sNowScale);
		}
	}
	*/
	//////////////////////////////////////////////////////////////////////////

	list<UIEvent>::iterator it = m_eventList.begin();
	while (it != m_eventList.end())
	{
		if (it->self)
		{
			it->data.funcEvent(it->self, it->data.pData);
		}

		it++;
	}

	m_eventList.clear();

	it = m_otherEventList.begin();
	while (it != m_otherEventList.end())
	{
		if (it->self)
		{
			it->data.funcEvent(it->self, it->data.pData);
		}

		it++;
	}

	m_otherEventList.clear();
}



UI* UIManager::loadUI(const string& name, bool isTop, int zoder)
{
	UI* rt = findUI(name);
	if (rt != NULL) return rt;

	UIDataGroup group;

	group.Load(name.c_str());

	rt = group.createUI(zoder);

	int x = SCREEN_SIZE.width / 2;
	if ((group.align & ALIGN_LEFT) != 0)
	{
		x = 0;
	}
	if ((group.align & ALIGN_HCENTER) != 0)
	{
		x = SCREEN_SIZE.width / 2;
	}
	if ((group.align & ALIGN_RIGHT) != 0)
	{
		x = SCREEN_SIZE.width;
	}

	int y = SCREEN_SIZE.height / 2;
	if ((group.align & ALIGN_TOP) != 0)
	{
		y = SCREEN_SIZE.height;
	}
	if ((group.align & ALIGN_VCENTER) != 0)
	{
		y = SCREEN_SIZE.height / 2;
	}
	if ((group.align & ALIGN_BOTTOM) != 0)
	{
		y = 0;
	}
	rt->setPosition(x, y);

	m_uiMap[name] = rt;

	if (isTop)
	{
		rt->m_bisRoot = true;
		rt->Top();
	}

	return rt;
}


void UIManager::addUI(UI* pUI, bool isTop)
{
	if (pUI)
	{
		if (m_uiMap[pUI->m_name] != NULL)
		{
			return;
		}
		GameScene::GetScene()->m_uiNode->addChild(pUI, 10000);
		m_uiMap[pUI->m_name] = pUI;
	}

	if (isTop)
	{
		pUI->m_bisRoot = true;
		pUI->Top();
	}
}

void UIManager::addUI(UI* pUI, int id, bool isTop)
{
	if (pUI)
	{
		if (m_uiMap[pUI->m_name] != NULL)
		{
			return;
		}
		GameScene::GetScene()->m_uiNode->addChild(pUI, 0);
		m_uiMap[pUI->m_name] = pUI;
	}

	if (isTop)
	{
		pUI->m_bisRoot = true;
		pUI->Top();
	}
}


//*************************
//2012.8.22 by yaf 实现UI多实例
UI* UIManager::loadUIWithIndex(const string& name, int iIndex, bool isTop, int zoder)
{
	char buff[256];
	sprintf(buff, "%s_%d", name.c_str(), iIndex);
	string key = buff;

	UI* rt = findUI(key);
	if (rt != NULL) return rt;

	UIDataGroup group;
	group.Load(name.c_str());

	rt = group.createUI(zoder);

	int x = SCREEN_SIZE.width / 2;
	if ((group.align & ALIGN_LEFT) != 0)
	{
		x = 0;
	}
	if ((group.align & ALIGN_HCENTER) != 0)
	{
		x = SCREEN_SIZE.width / 2;
	}
	if ((group.align & ALIGN_RIGHT) != 0)
	{
		x = SCREEN_SIZE.width;
	}

	int y = SCREEN_SIZE.height / 2;
	if ((group.align & ALIGN_TOP) != 0)
	{
		y = SCREEN_SIZE.height;
	}
	if ((group.align & ALIGN_VCENTER) != 0)
	{
		y = SCREEN_SIZE.height / 2;
	}
	if ((group.align & ALIGN_BOTTOM) != 0)
	{
		y = 0;
	}
	rt->setPosition(x, y);

	m_uiMap[key] = rt;

	if (isTop)
	{
		rt->m_bisRoot = true;
		rt->Top();
	}

	return rt;
}

UI* UIManager::findUI(const string& name, int iIndex)
{
	char buff[256];
	sprintf(buff, "%s_%d", name.c_str(), iIndex);
	string key = buff;

	map<string, UI*>::iterator it = m_uiMap.find(key);
	if (it != m_uiMap.end())
	{
		return it->second;
	}
	return NULL;
}


void UIManager::removeUI(const string& name, int iIndex)
{
	char buff[256];
	sprintf(buff, "%s_%d", name.c_str(), iIndex);
	string key = buff;

	map<string, UI*>::iterator it = m_uiMap.find(key);
	if (it != m_uiMap.end())
	{
		it->second->remove();

		UIManager::Instance()->onLoseTop(it->second);
	}
}

void UIManager::removeUI(const string& name)
{
	map<string, UI*>::iterator it = m_uiMap.find(name);
	if (it != m_uiMap.end())
	{
		it->second->remove();
		UIManager::Instance()->onLoseTop(it->second);
	}
}

UI* UI::setSubImage(int Id, string name)
{
	CCAssert(Id > 0, "");

	UI* p = NULL;

	if (this->getChildByTag(Id))
	{
		p = dynamic_cast<UI*>(getChildByTag(Id));
		if (p)
		{
			if (name == "")
			{
				p->setVisible(false);
				return NULL;
			}
			else
			{
				p->setVisible(true);
			}
		}
	}
	else if (name != "")
	{
		UIData data;

		data.type = UI_BASE;
		data.x = 0;
		data.y = 0;
		data.name = name;
		data.width = 1;
		data.height = 1;


		data.tag = 0;
		data.align = 0;
		data.picName = "";
		data.text = "";


		p = data.createUI();

		addChild(p, 0, Id);


	}

	if (p)
	{
		p->setImageByFullPath(name, true);
		p->setPosition((m_width - p->m_width) / 2, (m_height - p->m_height) / 2);
		p->setIgnoreTouch(true);
	}
	return p;

}

void UI::setVisible(bool visible)
{
	//    CCArray* arr =  this->getChildren();
	//    if (arr)
	//    {
	//        for (int i = 0; i < arr->count(); ++i)
	//        {
	//            Node* node = dynamic_cast<Node*>(arr->objectAtIndex(i));
	//            if(node)
	//            {
	//                node->setVisible(visible);
	//            }
	//        }
	//    }
	// diff
	if (this->isVisible() != visible)
	{
		Node::setVisible(visible);

		// to hide
		if (this->m_bisRoot)
		{
			if (visible)
				UIManager::Instance()->onGetTop(this);
			else
				UIManager::Instance()->onLoseTop(this);
		}

		recursiveResource(visible);
	}
}

void UI::onGetTop()
{
	auto& arr = this->getChildren();
	
	if (&arr)
	{
		for (int i = 0; i < (int)arr.size(); ++i)
		{
			
			UI* node = dynamic_cast<UI*>(arr.at(i));
			if (node)
			{
				node->onGetTop();
			}
		}
	}
	m_isTop = true;
}

void UI::onLoseTop()
{
	auto& arr = this->getChildren();
	if (&arr)
	{
		for (int i = 0; i < (int)arr.size(); ++i)
		{
			UI* node = dynamic_cast<UI*>(arr.at(i));
			if (node)
			{
				node->onLoseTop();
			}
		}
	}
	m_isTop = false;
}

//**************************

void UI::rermoveImag(int Id)
{
	CCAssert(Id > 0, "");

	UI* p = NULL;

	if (this->getChildByTag(Id))
	{
		this->removeChildByTag(Id, true);
	}
}


void UIManager::PushEvent(UI* p, const EventFunData& data)
{
	UIEvent event;

	event.self = p;
	event.data = data;

	gUIManager.m_eventList.push_back(event);
}

void UIManager::PushOtherEvent(UI* p, const EventFunData& data)
{
	UIEvent event;

	event.self = p;
	event.data = data;

	gUIManager.m_otherEventList.push_back(event);
}

void UIManager::RemoveEvent(UI* p)
{
	list<UIEvent>::iterator it = gUIManager.m_eventList.begin();
	while (it != gUIManager.m_eventList.end())
	{
		if (it->self == p)
		{
			it->self = NULL;
		}

		it++;
	}

	vector<UI*>::iterator it2 = gUIManager.m_selected.begin();
	for (; it2 != gUIManager.m_selected.end(); it2++)
	{
		if (*it2 == p)
		{
			gUIManager.m_selected.erase(it2);
			break;
		}
	}

}

void UIManager::RemoveOtherEvent(UI* p)
{
	list<UIEvent>::iterator it = gUIManager.m_otherEventList.begin();
	while (it != gUIManager.m_otherEventList.end())
	{
		if (it->self == p)
		{
			it->self = NULL;
		}

		it++;
	}

	vector<UI*>::iterator it2 = gUIManager.m_selected.begin();
	for (; it2 != gUIManager.m_selected.end(); it2++)
	{
		if (*it2 == p)
		{
			gUIManager.m_selected.erase(it2);
			break;
		}
	}
}


UI*	UIManager::getTouchUI(CCTouch* touch)
{
	GameScene* scene = GameScene::GetScene();
	if (scene == NULL)
		return NULL;
	CCObject* layer;
	UI * pMonopolizeTouch = NULL;
	auto& children = scene->m_uiNode->getChildren();
	for (const auto &layer : children) {

		UI* layer1 = (UI*)layer;
		UI* pUI = NULL;

		if (!layer1->isVisible() || layer1->isIgnoreTouch())
			continue;

		if (!layer1->isInClipRect(touch))
			continue;

		pUI = layer1->getTouchUI(touch);
		if (pUI)
		{
			return pUI;
		}
	}
	return NULL;
}



static bool sCanTouch = true;
static unsigned long sFrontTouchTime = 0;

void UIManager::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
	/*
	cc_timeval now;
	CCTime::gettimeofdayCocos2d(&now, NULL);
	unsigned long nowTime = now.tv_sec * 1000 + now.tv_usec / 1000;
	if (nowTime - sFrontTouchTime >= 300)
	{
		sFrontTouchTime = nowTime;
		sCanTouch = true;
	}
	else
	{
		sCanTouch = false;
		return;
	}
	Vec2 location[2];
	CCSetIterator iter = pTouches->begin();
	bool	isUseDirectCMD = false;

	for (int i = 0; i < 2 && iter != pTouches->end(); iter++, ++i)
	{
		CCTouch* pTouch = (CCTouch*)(*iter);
		location[i] = pTouch->locationInView();

		//如果本次点击被摇杆获取，跳过本次点击
		if (GameScene::GetScene()->m_eGameState == GAME_STATE_GAME)
		{
			if (CDirectHandleUI::instance().onTouchBegin(pTouch))
			{
				isUseDirectCMD = true;
				continue;
			}
			if (CShortPage::instance().getShortCut()
				&& CShortPage::instance().getShortCut()->isCanBegin(pTouch))
			{
				CShortPage::instance().getShortCut()->ccTouchBegan(pTouch, NULL);
				isUseDirectCMD = true;
				continue;
			}

			GuideModule::instance().ccTouchBegan(pTouch, NULL);

		}


		if (i == 0 && pTouches->count() == 1)
		{
			ccTouchBegan(pTouch, pEvent);

			std::set<CCTouchDelegate*>::iterator it = m_TouchHandlers.begin();
			std::set<CCTouchDelegate*>::iterator end = m_TouchHandlers.end();
			while (it != end)
			{
				(*it)->ccTouchBegan(pTouch, pEvent);
				++it;
			}
		}

		UI *pUI = UIManager::Instance()->findUI("main_superskill.ui");
		if (pUI)
		{
			if (pUI->isVisible())
			{
				UIScrollView *pCheatsScrollView = dynamic_cast<UIScrollView*>(pUI->findUI("pageequipmentz1"));;
				if (pCheatsScrollView)
				{
					CCRect rect = pCheatsScrollView->getGlobalRect();
					if (!rect.containsPoint(pTouch->getLocation())) //&& !rect1.containsPoint(dynamic_cast< CCTouch *>(pTouches).getLocation()))
					{
						CGameMainUI::instance()->CloseCheatsList();
					}

				}
			}
		}
	}//遍历取出每个触摸点坐标
	//wcc
	if (!isUseDirectCMD && !Vec2::Vec2EqualToPoint(location[0], Vec2Zero) && !Vec2::Vec2EqualToPoint(location[1], Vec2Zero) && gCanTouchForZoom)
	{
		sIsFinishChange = false;

		float x = location[0].x - location[1].x;
		float y = location[0].y - location[1].y;
		sDistance = sqrtf(x*x + y*y);
	}

	//UI *pUI = this->findUI("main_superskill.ui");
	//if (pUI)
	//{
	//	UI* pUISroll = pUI->findUI("pageequipmentz1");
	//	if (pUISroll)
	//	{
	//		 if(pUISroll->m_rect9->intersectsRect(location[0]));
	//		 {
	//			 CGameMainUI::instance()->CloseCheatsList();
	//		 }
	//	}
	//}
	*/
}

void UIManager::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
	/*
	Vec2 location[2];
	CCSetIterator iter = pTouches->begin();
	bool	isUseDirectCMD = false;


	for (int i = 0; i < 2 && iter != pTouches->end(); iter++, ++i)
	{
		CCTouch* pTouch = (CCTouch*)(*iter);
		location[i] = pTouch->locationInView();

		//如果本次点击被摇杆获取，跳过本次点击
		if (GameScene::GetScene()->m_eGameState == GAME_STATE_GAME)
		{
			if (CDirectHandleUI::instance().onTouchMoved(pTouch))
			{
				isUseDirectCMD = true;
				continue;
			}
			if (CShortPage::instance().getShortCut()
				&& CShortPage::instance().getShortCut()->isBegining())
			{
				CShortPage::instance().getShortCut()->ccTouchMoved(pTouch, NULL);
				isUseDirectCMD = true;
				continue;
			}

			GuideModule::instance().ccTouchMoved(pTouch, NULL);
		}

		if (i == 0 && pTouches->count() == 1)
		{
			if (sCanTouch)
			{
				ccTouchMoved(pTouch, pEvent);

				std::set<CCTouchDelegate*>::iterator it = m_TouchHandlers.begin();
				std::set<CCTouchDelegate*>::iterator end = m_TouchHandlers.end();
				while (it != end)
				{
					(*it)->ccTouchMoved(pTouch, pEvent);
					++it;
				}
			}
		}
	}//遍历取出每个触摸点坐标

	if (!isUseDirectCMD && !Vec2::Vec2EqualToPoint(location[0], Vec2Zero) && !Vec2::Vec2EqualToPoint(location[1], Vec2Zero) && sDistance == 0.0f && gCanTouchForZoom)
	{
		float x = location[0].x - location[1].x;
		float y = location[0].y - location[1].y;
		sDistance = sqrtf(x*x + y*y);
	}


	if (!isUseDirectCMD && !m_NonceEdit && !Vec2::Vec2EqualToPoint(location[0], Vec2Zero) && !Vec2::Vec2EqualToPoint(location[1], Vec2Zero) && !m_selected.size() && gCanTouchForZoom)
	{
		sIsFinishChange = false;

		float x = location[0].x - location[1].x;
		float y = location[0].y - location[1].y;
		float dis = sqrtf(x*x + y*y);
		float ddt = dis - sDistance;

		float scale = ddt / _SPEED_PER_TOUCHES;

		sNowScale += scale;

		if (sNowScale > _LIMITED_MAX_SCREEN_SCALE) {
			sNowScale = _LIMITED_MAX_SCREEN_SCALE;
		}
		if (sNowScale < _LIMITED_MIN_SCREEN_SCALE) {
			sNowScale = _LIMITED_MIN_SCREEN_SCALE;
		}

		Map* pMap = GameScene::GetScene()->GetCurMap();
		CCamera* pCamera = pMap ? pMap->GetCamera() : 0;
		if (pCamera && ddt)
		{
			gAntoScale = sNowScale;
			pCamera->setSceneScale(sNowScale);
		}
	}
	*/
}

void UIManager::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
	/*
	Vec2 location[2];
	CCSetIterator iter = pTouches->begin();
	bool	isUseDirectCMD = false;

	if (sDistance != 0.0f) {
		sDistance = 0.0f;
		sIsFinishChange = true;
	}

	for (int i = 0; i < 2 && iter != pTouches->end(); iter++, ++i)
	{
		CCTouch* pTouch = (CCTouch*)(*iter);
		location[i] = pTouch->getLocationInView();

		//如果本次点击事件被摇杆获取，跳过本次点击
		if (GameScene::GetScene()->m_eGameState == GAME_STATE_GAME)
		{
			if (CDirectHandleUI::instance().onTouchEnd(pTouch))
			{
				isUseDirectCMD = true;
				continue;
			}
			if (CShortPage::instance().getShortCut()
				&& CShortPage::instance().getShortCut()->isBegining())
			{
				CShortPage::instance().getShortCut()->ccTouchEnded(pTouch, NULL);
				isUseDirectCMD = true;
				continue;
			}

			GuideModule::instance().ccTouchEnd(pTouch, NULL);
		}

		if (m_iTouchNum <= 0)
		{
			if (sCanTouch)
			{
				ccTouchEnded(pTouch, pEvent);

				std::set<CCTouchDelegate*>::iterator it = m_TouchHandlers.begin();
				std::set<CCTouchDelegate*>::iterator end = m_TouchHandlers.end();
				while (it != end)
				{
					(*it)->ccTouchEnded(pTouch, pEvent);
					++it;
				}
			}
		}
		m_iTouchNum--;
	}//遍历取出每个触摸点坐标

	GameScene::GetMap()->restHoldTime();
	*/
}


UI*	UI::findUsingUI(const string& uiFileName, const string& uiNodeName)
{
	//CCObject* child;
	auto& children = getChildren();
	for (const auto &child : children) {

		UI* pUI = dynamic_cast<UI*> (child);
		if (pUI && pUI->isVisible())
		{
			if (uiFileName == "")
			{
				if (pUI->m_name == uiNodeName)
				{
					return pUI;
				}
			}
			else
			{
				if (pUI->m_name == uiFileName)
				{
					return pUI->findUsingUI("", uiNodeName);
				}
			}
		}
	}

	//没有查找到，查找子UI
	children = getChildren();
	for (const auto &child : children) {
		UI* pUI = dynamic_cast<UI*> (child);
		if (pUI && pUI->isVisible())
		{
			UI* p = pUI->findUsingUI(uiFileName, uiNodeName);
			if (p) return p;
		}
	}
	return NULL;
}

void UI::setRenderBatch(RenderBatchData* pRenderBatch)
{
	m_image.m_RenderBatch = pRenderBatch;
}

void UI::SetRotateX(float fa)
{
	/*
	float ra = (float)CC_DEGREES_TO_RADIANS(fa);
	float i = sinf(ra) * CCCamera::getZEye();
	float j = cosf(ra) * CCCamera::getZEye();
	getCamera()->setEyeXYZ(0, i, j);
	*/
}

void UI::SetRotateY(float fa)
{
	/*
	float ra = (float)CC_DEGREES_TO_RADIANS(fa);
	float i = sinf(ra) * CCCamera::getZEye();
	float j = cosf(ra) * CCCamera::getZEye();
	getCamera()->setEyeXYZ(i, 0, j);
	*/
}

void UI::SetRotateZ(float fa)
{
	setRotation(fa);
}

void UI::setUIAnchorPoint(float x, float y)
{
	//m_tAnchorPointInPoints = ccp(m_width*x, m_height*y);
	//m_bIsTransformDirty = m_bIsInverseDirty = true;
}

void UI::recursiveResource(bool visible)
{
	if (!visible)
	{
		onReleaseResource();
		CCObject* child;
		auto& children = getChildren();
		for (const auto &child : children) {

			UI* pUI = dynamic_cast<UI*> (child);
			if (pUI)
				pUI->recursiveResource(visible);
		}
	}
	else
	{
		if (isVisible())
			onReLoadResource();
		CCObject* child;
		auto& children = getChildren();
		for (const auto &child : children) {

			UI* pUI = dynamic_cast<UI*> (child);
			if (pUI)
				pUI->recursiveResource(visible);
		}
	}
}

void UI::removeAllChildrenWithCleanup(bool cleanup)
{
	Node::removeAllChildrenWithCleanup(cleanup);
	if (cleanup)
	{
		m_pText = NULL;
		m_pTextDown = NULL;
	}
}

UI*	UIManager::findUsingUI(const string& uiFileName, const string& uiNodeName)
{
	UI *pUI = NULL;
	pUI = findUI(uiFileName);
	if (pUI && pUI->isVisible())
	{
		UI *p = pUI->findUsingUI("", uiNodeName);
		if (p) return p;
	}
	UIMap::iterator it = m_uiMap.begin();
	for (; it != m_uiMap.end(); ++it)
	{
		UI *p = it->second->findUsingUI(uiFileName, uiNodeName);
		if (p) return p;
	}
	return NULL;
}
/*
void UIManager::RemoveTouchDelegate(CCTouchDelegate* pHandler)
{
	std::set<CCTouchDelegate*>::iterator it = m_TouchHandlers.find(pHandler);
	if (it != m_TouchHandlers.end())
	{
		m_TouchHandlers.erase(it);
	}
}
void UIManager::AddTouchDelegate(CCTouchDelegate* pHandler)
{
	m_TouchHandlers.insert(pHandler);
}
*/

#ifdef USE_UI_DRAG

void	UIManager::_CreateGragUI( UI* srcUI, CCTouch * touch )
{
	if( m_pDragUI )
	{
		return;
	}

	if( !srcUI )
		return;
	if( !touch )
		return;
	GameScene* scene = GameScene::GetScene();
	if( !scene )
		return;
	if( !scene->m_uiNode )
		return;

	UIScrollView *pScroll = dynamic_cast<UIScrollView*>(srcUI);
	if( pScroll )
	{
		Vec2 pos = pScroll->getStartPos();
		UI* pUI = pScroll->UI::getTouchUI( pos.x, pos.y, touch );
		if( pUI ) srcUI = pUI;
	}

	if( srcUI->m_pCreateDragFunc )
	{
		m_pDragUI = srcUI->m_pCreateDragFunc( srcUI );
	}

	if( !m_pDragUI )
		return;

	scene->m_uiNode->addChild(m_pDragUI);
	m_pDragUI->m_iDragSrcTag = srcUI->m_iDragSrcTag;
	m_pDragUI->Top();
	Vec2 pos = scene->m_uiNode->convertTouchToNodeSpaceAR(touch);
	m_pDragUI->setPosition( pos.x - m_pDragUI->m_width/2, pos.y - m_pDragUI->m_height/2 );
}

void UIManager::_MoveGragUI( CCTouch *touch )
{
	if( !touch )
		return;
	if( !m_pDragUI )
		return;
	GameScene* scene = GameScene::GetScene();
	if( !scene )
		return;
	if( !scene->m_uiNode )
		return;
	Vec2 pos = scene->m_uiNode->convertTouchToNodeSpaceAR(touch);
	m_pDragUI->setPosition( pos.x - m_pDragUI->m_width/2, pos.y - m_pDragUI->m_height/2 );
}


void	UIManager::_HandleGragEvent( UI* destUI, CCTouch *touch )
{
	if( !m_pDragUI )
		return;
	if( !destUI )
	{
		m_pDragUI->removeFromParentAndCleanup( true );
		m_pDragUI = NULL;
		return;
	}

	UIScrollView *pScroll = dynamic_cast<UIScrollView*>(destUI);
	if( pScroll )
	{
		Vec2 pos = pScroll->getStartPos();
		UI* pUI = pScroll->UI::getTouchUI( pos.x, pos.y, touch );
		if( pUI ) destUI = pUI;
	}

	if( destUI->m_iDragDestTag == m_pDragUI->m_iDragSrcTag &&  destUI->m_pDragFunc )
	{
		destUI->m_pDragFunc( destUI, m_pDragUI, m_pDragUI->m_pDragSrcData );
	}

	m_pDragUI->removeFromParentAndCleanup( true );
	m_pDragUI = NULL;
}

#endif