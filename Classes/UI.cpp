#include "UI.h"
#include "GameScene.h"
#include "UIText.h"
#include "UIAnimation.h"
#include "UIEditNew.h"
#include "UIDrawable.h"
#include "UIScroll.h"
#include "UIManager.h"

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



#ifdef USED_JUMP
int UIManager::m_draged = 0;
#endif
#ifdef _MSC_VER
#pragma warning( disable: 4244 )
#endif


void DrawFillRect(int x = 0, int y = 0, const Rect* pRc = NULL)
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

	log("-------------------UIDataGroup::Load-------------------%s\n",fileName);

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
		log("name:%s x=%d, y=%d, width=%d, height=%d, type=%d", g.data[i].name.c_str(),
			g.data[i].x,
			g.data[i].y,
			g.data[i].width,
			g.data[i].height
		);
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
		p = new UIDrawable();
	}
	else if (type == UI_TEXT)
	{
		p = new UIText();
	}
	else if (type == UI_UISCROLL)
	{
		p = new UIScroll();
	}
	else if (type == UI_UIEDIT)
	{
		p = new UIEditNew();
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
	p->m_name	= name;
	p->setPosition(Vec2(x, -y - height));
	p->m_type	= (TUIType)type;
	p->m_width	= width;
	p->m_height = height;
	p->m_align	= (1 << ((align / 3) + 3)) | (1 << (align % 3));
	p->m_tag	= tag;
	p->setImage(picName, !isSTensile);
	// utf-8 text
	if (_isTextUTF8(text.c_str(), text.length())) {
		p->setText(text);
	} else {
		p->setText(ANSI2UTF8(text));
	}
	p->setIsEnlarge(!!isEnlarge);

	if (type == UI_BUTTON) {
		p->setUIEffectType(UIMusicType_Button_Check);
		p->setPlayEffect(true);
	} else {
		p->setUIEffectType(UIMusicType_Button_Select);
		p->setPlayEffect(false);
	}
	p->init();

	return p;
}

UI* UIDataGroup::createUI(int zoder)
{
	UI* p = createUIBase();
	GameScene::GetUI()->addChild(p, zoder);

	//	GameScene* scene = GameScene::GetScene();
//	scene->m_uiNode->addChild(p, zoder);

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

	for (int i = 0; i < count; i++) {

		if (ui->name == "SelectPlayerBtn0") {
			int a = 0;
			a++;
		}
		//支持多分辨率
		//int uialign = (1<<((ui->align/3)+3)) | (1<<(ui->align%3));
		int uialign = (1 << ((ui->posalign / 3) + 3)) | (1 << (ui->posalign % 3));
		int offsetx = (SCREEN_SIZE.width - BASEWIDTH) / 2;

		if ((uialign & ALIGN_LEFT) != 0 && (align & ALIGN_LEFT) == 0) {
			ui->x -= offsetx;
		}

		if ((uialign & ALIGN_RIGHT) != 0 && (align & ALIGN_RIGHT) == 0) {
			ui->x += offsetx;
		}

		int offsety = (SCREEN_SIZE.height - BASEHIGHT) / 2;
		if ((uialign & ALIGN_TOP) != 0 && (align & ALIGN_TOP) == 0) {
			ui->y -= offsety;
		}

		if ((uialign & ALIGN_BOTTOM) != 0 && (align & ALIGN_BOTTOM) == 0) {
			ui->y += offsety;
		}

		UI* pChild = ui->createUI();

		p->addChild(pChild);

		ui++;
	}

	p->init();
	return p;
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

	m_color = Color4B(255, 255, 255, 255);

	m_rect9 = NULL;

	m_width			= 0;
	m_height		= 0;
	m_nParam64		= 0;

	m_startLineX	= 0;		//start point of line
	m_startLineY	= 0;
	m_endLineX		= 0;		//end point of line
	m_endLineY		= 0;
	m_isReverseTex	= false;

	m_isLine		= false;
	m_isTop			= false;
}

UI::~UI()
{
	if (m_clipRect) {
		delete m_clipRect;
	}

	if (m_rect9) delete m_rect9;

	UIManager::RemoveEvent(this);
	UIManager::RemoveOtherEvent(this);

	if (UIManager::getInstance()->getOnlyMessage() == this) {
		UIManager::getInstance()->setOnlyMessage(NULL);
	}

	m_text = "";
}

bool UI::init()
{
	return true;
}

bool UI::isInClipRect(Touch* touch)
{
	if (!m_clipRect)
		return true;

	// 	Vec2  pos( touch->locationInView().x, touch->locationInView().y );
	// 	pos = CCDirector::sharedDirector()->convertToGL(pos);

	Vec2 r_pos = this->getRealPos();
	Rect rect;
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

UI* UI::getTouchUI(Touch* touch)
{
	return getTouchUI(0, 0, touch);
}

UI* UI::getTouchUI(int beginX, int beginY, Touch* touch)
{
	auto& children = getChildren();
	for (const auto &child : children) {

		UI* p = dynamic_cast<UI*> (child);

		if (!p) continue;

		if (p->isVisible() && !p->isIgnoreTouch())
		{
			UI* pSel = p->getTouchUI(beginX, beginY, touch);

			if (pSel) return pSel;

			Rect rect;
			if (p->m_isEnlarge)
			{
				float w = p->m_width / 4;
				w = w > 32 ? 32 : w;

				float h = p->m_height / 4;
				h = h > 32 ? 32 : h;

				rect = Rect(beginX - w, beginY - h, p->m_width + (w * 2), p->m_height + (h * 2));
			}
			else
			{
				rect = Rect(beginX, beginY, p->m_width, p->m_height);
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
	for (int i = 0; i < (int)UIManager::getInstance()->m_selected.size(); ++i)
	{
		if (this == UIManager::getInstance()->m_selected[i])
		{
			return UIManager::getInstance()->m_holdTime;
		}
	}
	return -1;
}

void UI::update(float t)
{
	if (m_pUpdateFun && m_pUpdateFun(this, t)) {
		return;
	}

	auto& children = getChildren();
	for (const auto &child : children) {
		Node* p = dynamic_cast<Node*>(child);
		if (p && p->isVisible()) {
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

Rect UI::getGlobalRect()
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

	Rect rect;
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

	if (m_bIsMasking) {
		if (!m_MaskingBack) {
			m_MaskingBack = ImageCenter::instance().GetRenderBatch("UI/LRes/NPCCover2.png");
		}
		//if (m_MaskingBack)
		//	DrawImage(m_MaskingBack, -SCREEN_SIZE.width / 2, -SCREEN_SIZE.height / 2 - 100, SCREEN_SIZE.width, SCREEN_SIZE.height + 100);
	}

	RenderBatchData* nRenderBatch = m_image.m_RenderBatch;

	if (m_type == UI_BUTTON || m_type == UI_BTN_feature) {
		if (m_down) {
			if (m_image.m_downRenderBatch != NULL) {
				//don't use it if no need
				if (nRenderBatch != NULL) {
					needReCalDownTex();
				}

				nRenderBatch = m_image.m_downRenderBatch;
			} else {
				offY -= 2;
			}

			if (m_pText) m_pText->setVisible(false);
			if (m_pTextDown) m_pTextDown->setVisible(true);
		} else {
			if (m_pText) m_pText->setVisible(true);
			if (m_pTextDown) m_pTextDown->setVisible(false);
		}
	}

	// 	Rect rect = this->getGlobalRect();
	// 
	// 	rect.origin = Vec2Zero;
	// 	Vec2 rt = rect.origin;
	// 	rt.x += rect.size.width;
	// 	rt.y += rect.size.height;
	// 
	// 	ccDrawRect(rect.origin, rt);

	if (nRenderBatch != NULL) {
		GLProgram* p = NULL;
		if (m_onlyUseVertexColor) {
			p = ShaderCache::getInstance()->programForKey(GLProgram::SHADER_NAME_POSITION_U_COLOR);
		} else {
			p = ShaderCache::getInstance()->programForKey(GLProgram::SHADER_NAME_POSITION_TEXTURE_U_COLOR);
		}

		//UIBatchRenderer::instance()->setShader(p);

		//add line flag
		if (!m_isReverseTex) {
			/*
			if (m_rect9)
				DrawImage9(nRenderBatch, 0, offY, m_width, m_height, m_color, *m_rect9);
			else
				DrawImage(nRenderBatch, 0, offY, m_width, m_height, m_color);
				*/
		} else {
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

	if (m_image.m_IconTexture != NULL) {
		//DrawImage(m_image.m_IconTexture, 0, offY, m_width, m_height, m_color);
	}

	if (m_clipRect) {
#ifdef  UI_BATCH_RENDERER
		UIBatchRenderer::instance()->flush();
#endif
	}


#ifdef DEBUG_DRAW
	Vec2 pos = getPosition();
	if( m_width > 0 && m_height >0 )
		FillRect( RectMake( 0, 0, m_width, m_height), ccc4(255,255,255,255), true);
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
		Rect rc, rc2;
		rc.origin = convertToWorldSpaceAR(ccp(0, 0));
		rc.origin.x += UIManager::Instance()->m_TranslatefPos.x;
		rc.origin.y += UIManager::Instance()->m_TranslatefPos.y;
		rc.origin.x *= xScale;
		rc.origin.y *= yScale;
		rc.size.width = m_width * xScale;
		rc.size.height = m_height * yScale;

		rc2 = UIManager::Instance()->m_nonceClipRect;
		if (rc.size.width == 0 || rc.size.height == 0 || Rect::RectIntersectsRect(rc, rc2))
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

	auto& children = getChildren();
	for (const auto &child : children) {

		UI* p = dynamic_cast<UI*>(child);
		if (p) {
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

	setText(a, 24, Size(m_width, m_height), tAlignCenterX | tAlignCenterY);
}


void UI::setText(const std::string& strText, int fontSize, const CCSize &size, int alignType, bool isAutoNewLine)
{
	setText(strText, fontSize, size, alignType, m_strTextFontName[m_eTextType].c_str(), isAutoNewLine);
}

void UI::setText(const std::string& strText, int fontSize, const CCSize &size, int alignType, string fontName, bool isAutoNewLine)
{
	if (m_text == strText)
		return;
	m_text = strText;

	if (m_pText) {
		m_pText->setText(m_text);
	} else {
		m_pText = UIText::initUITextWithString(strText, fontSize, size, alignType, fontName, isAutoNewLine);
		addChild(m_pText);
	}

	if (m_type == UI_BUTTON) {
		char str[256];
		sprintf(str, "<C=149,212,251>%s</C>", strText.c_str());
		if (m_pTextDown) {
			m_pTextDown->setText(str);
			m_pTextDown->setVisible(false);
		} else {
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
	if (m_image.m_RenderBatch == NULL) {
		sprintf(buf, "UI/LRes/%s", name.c_str());
		m_image.m_RenderBatch = ImageCenter::instance().GetRenderBatch(buf);
	}

	int len = strlen(buf);

	sprintf(&buf[len - 4], "_down.png");

	m_image.m_downRenderBatch = ImageCenter::instance().GetRenderBatch(buf);

	//CCTexture2D::setUSED_ANTI_ALIAS( true );

	if (m_image.m_RenderBatch != NULL) {
		//m_image.x1 = -1;
		//m_image.y1 = -1;
		//m_image.x2 = -1;
		//m_image.y2 = -1;

		if (boundingWH) {
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
		if (it == m_sRect9Datatable.end()) {
			UIRect9Data r9data;

			ssize_t length;
 			unsigned char* pszBuffer = FileUtils::getInstance()->getFileData(buf, "rb", &length);

			if (length <= 0 || pszBuffer == NULL) {
				r9data.isHave = false;
				m_sRect9Datatable[buf] = r9data;
				return;
			}

			int n[4] = { 0 };

			int pos = 0;
			for (int i = 0; i < (int)length; i++) {
				if ((pszBuffer[i] == ',') ) {
					pos++;
				} else if (pszBuffer[i] >= '0' && pszBuffer[i] <= '9') {
					n[pos] = n[pos] * 10 + pszBuffer[i] - '0';
				}
			}
			r9data.isHave = true;
			r9data.rect = Rect(n[0], n[1], n[2] - n[0], n[3] - n[1]);

			m_sRect9Datatable[buf] = r9data;
			if (m_rect9)
				*m_rect9 = Rect(n[0], n[1], n[2] - n[0], n[3] - n[1]);
			else
				m_rect9 = new Rect(n[0], n[1], n[2] - n[0], n[3] - n[1]);
			CC_SAFE_DELETE_ARRAY(pszBuffer);
		} else {
			if (it->second.isHave) {
				if (m_rect9)
					*m_rect9 = Rect(it->second.rect);
				else
					m_rect9 = new Rect(it->second.rect);
			}
		}
	}

}

extern std::string _fixPathByLanguage(const char* resName);

void UI::setImageByFullPath(const string& name, bool boundingWH)
{
	std::string realTexName = name;
	int pos = realTexName.find_first_of("/");
	if (pos != string::npos) {
		string path = realTexName.substr(0, pos);
		if (path == "Origin" || path == "ImageCenter") {
			realTexName = _fixPathByLanguage(realTexName.c_str());
		}
	}
#if !( defined WIN32 || defined _WIN32 )
	realTexName = realTexName.substr(0, realTexName.find(".")) + ".pvr.ccz";
#endif
	m_image.m_RenderBatch = ImageCenter::instance().GetRenderBatch(realTexName.c_str());
	if (m_image.m_RenderBatch == NULL && m_image.m_IconName != realTexName) {
		if (m_image.m_IconTexture) {
			m_image.m_IconTexture->release();
		}
		m_image.m_IconTexture = TextureCache::getInstance()->textureForKey(realTexName.c_str());
		if (m_image.m_IconTexture) {
			/*m_image.m_IconName = realTexName;*/
			if (boundingWH) {
				m_width = m_image.m_IconTexture->getContentSize().width;
				m_height = m_image.m_IconTexture->getContentSize().height;
			}
			m_image.m_IconTexture->retain();
		} else {
			m_image.m_IconTexture = TextureCache::getInstance()->addImage(realTexName.c_str());
			if (m_image.m_IconTexture) {
				if (boundingWH) {
					m_width = m_image.m_IconTexture->getContentSize().width;
					m_height = m_image.m_IconTexture->getContentSize().height;
				}
			}
		}
	}

	m_image.m_downRenderBatch = NULL;
	m_image.m_IconName = realTexName;

	if (boundingWH && m_image.m_RenderBatch) {
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
	auto& children = getChildren();
	for (const auto &child : children) {

		UI* p = (UI*)child;
		if (p->m_name == name) {
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

	if (p != NULL) {
		this->retain();
		p->removeChild(this, false);
		p->addChild(this);
		this->release();
	}

	if (m_bisRoot) {
		UIManager::getInstance()->topUI(this);
	}
}

void UI::OnEvent(TEventType msg, Touch* touch)
{
	if (m_eventData[msg].funcEvent && bIsEnable) {
		UIManager::PushEvent(this, m_eventData[msg]);
	}
	if (m_otherEventData[msg].funcEvent && bIsEnable) {
		UIManager::PushOtherEvent(this, m_otherEventData[msg]);
	}
	if (m_bIsPlayEffect && msg == EVENT_UP && m_eMusicEffectType < UIMusicType_Button_Num) {
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(m_strMusicEffectFileName[m_eMusicEffectType].c_str());
	}
}

UI* UI::loadChild(const string& name)
{
	UI* rt = NULL;

	UIDataGroup group;

	group.Load(name.c_str());

	rt = group.createUIBase();

	if (rt) {
		addChild(rt);
	}
	UIManager::getInstance()->addItemToChildMap(name, this);
	return rt;
}


Vec2 UI::getCurPos()
{
	return UIManager::getInstance()->m_curPos;
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
	while (pUITemp != NULL&&pParentUI != NULL) {
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
	while (pParent != NULL) {
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


void UI::SetClip(const Rect& rect)
{
	if (!m_clipRect) m_clipRect = new Rect();

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



void UI::removeChild(UI* child, bool cleanup)
{
	if (child && child->m_name != "") {
		UIManager::getInstance()->removeItemFromChildMap(child->m_name);
	}
	Node::removeChild(child, cleanup);
}


UI* UI::setSubImage(int Id, string name)
{
	CCAssert(Id > 0, "");

	UI* p = NULL;

	if (this->getChildByTag(Id)) {
		p = dynamic_cast<UI*>(getChildByTag(Id));
		if (p) {
			if (name == "") {
				p->setVisible(false);
				return NULL;
			} else {
				p->setVisible(true);
			}
		}
	} else if (name != "") {
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

	if (p) {
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
	if (this->isVisible() != visible) {
		Node::setVisible(visible);

		// to hide
		if (this->m_bisRoot) {
			if (visible)
				UIManager::getInstance()->onGetTop(this);
			else
				UIManager::getInstance()->onLoseTop(this);
		}

		recursiveResource(visible);
	}
}

void UI::onGetTop()
{
	auto& arr = this->getChildren();
	
	if (&arr) {
		for (int i = 0; i < (int)arr.size(); ++i) {
			
			UI* node = dynamic_cast<UI*>(arr.at(i));
			if (node) {
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


UI*	UI::findUsingUI(const string& uiFileName, const string& uiNodeName)
{
	//CCObject* child;
	auto& children = getChildren();
	for (const auto &child : children) {

		UI* pUI = dynamic_cast<UI*> (child);
		if (pUI && pUI->isVisible()) {
			if (uiFileName == "") {
				if (pUI->m_name == uiNodeName) {
					return pUI;
				}
			} else {
				if (pUI->m_name == uiFileName) {
					return pUI->findUsingUI("", uiNodeName);
				}
			}
		}
	}

	//没有查找到，查找子UI
	children = getChildren();
	for (const auto &child : children) {
		UI* pUI = dynamic_cast<UI*> (child);
		if (pUI && pUI->isVisible()) {
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
	if (!visible) {
		onReleaseResource();
		auto& children = getChildren();
		for (const auto &child : children) {

			UI* pUI = dynamic_cast<UI*> (child);
			if (pUI)
				pUI->recursiveResource(visible);
		}
	} else {
		if (isVisible())
			onReLoadResource();
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
