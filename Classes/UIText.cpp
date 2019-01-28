#include "UIText.h"
#include "UITextDefine.h"
#include "GameScene.h"
#include "ColorCfg.h"
/*
#include "GraphicsExtension.h"

#include "Text.h"
#include "TextDef.h"
#include "ActorManager.h"
#include "Hero.h"
#include "Guild.h"
#include "HeroPet.h"
#include "ItemCfg.h"
#include "Item.h"
#include "ItemBag.h"
#include "MapCfg.h"
#include "SkillCfg.h"
#include "MonsterCfg.h"
*/
//#define DEBUG_DRAW
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "LabelBMFont.h"
#include "utf.h"
#else
//#include "../../label_nodes/LabelBMFont.h"
//#include "../../support/utf.h"
#endif

#define MS_FONT_GAME_SIZE 27
#define MS_FONT_GAME_SCALE 1.3f

UIText::UIText() :
m_fSize(24), m_iAlignType(tAlignCenterX | tAlignCenterY),    //对齐方式，默认居中
m_bAutoNewLine(true)
{
	m_strFontName = FONT_BMP_24;
	mTxScale	  = MS_FONT_GAME_SCALE;
}

UIText::~UIText() 
{
	vLine.clear();
}

bool UIText::parseText(const std::string &strText) 
{
	m_fSize = MS_FONT_GAME_SIZE;
	char tmp[1024];
	vector<ccColor3B> vColor;

	vLine.clear();
	TextLine newLine;
	vLine.push_back(newLine);

	bool bIsComputeWidth = (m_width <= 0); //是否计算宽度
	bool bIsComputeHeight = (m_height <= 0); //是否计算高度

	string strReal = "";
	char *pStr = new char[strText.length() + 1];
	strcpy(pStr, strText.c_str());
	pStr[strText.length()] = '\0';
	for (int i = 0, j = 0; i < (int)strlen(pStr) + 1; ++i)
	{
		char c = pStr[i];
		if (c == '<') 
		{
			j = i + 1;
			for (; i < (int)strlen(pStr) + 1; ++i) 
			{
				c = pStr[i];
				if (c == '>') 
				{
					strncpy(tmp, pStr + j, i - j);
					tmp[i - j] = '\0';
					j = i + 1;

					if (strcmp(tmp, TL_PLAYER_NAME) == 0) 
					{
						/*						
						CHero *pHero = GameScene::GetActorManager()->GetHero();
						if (pHero) 
						{
							strReal += pHero->GetName();
						}
						*/

					} 
					else if (strcmp(tmp, TL_GUILD_NAME) == 0) 
					{
						/*
						CHero *pHero = GameScene::GetActorManager()->GetHero();
						if (pHero && pHero->getGuildID()) 
						{
							strReal +=Guild::instance().getGuildBaseInfo().strGuildName;
						}
						*/
					} 
					else if (strcmp(tmp, TL_FACTION_NAME) == 0) 
					{
						/*
						CHero *pHero = GameScene::GetActorManager()->GetHero();
						if (pHero) 
						{
							switch (pHero->getFaction()) 
							{
							case CAMP_TIANJI:
								//strReal += CText::GetText(CTextDef::OCC_CAMP_TIANJI);
								break;

							case CAMP_XUANZONG:
								//strReal += CText::GetText(CTextDef::OCC_CAMP_XUANZONG);
								break;
							}
						}
						*/
					} 
					else if (strcmp(tmp, TL_PLAYER_SEX) == 0) 
					{
						/*
						CHero *pHero = GameScene::GetActorManager()->GetHero();
						if (pHero) 
						{
							switch (pHero->getSex()) 
							{
							case Player_Sex_Man:
								strReal += CText::GetText(CTextDef::PLAYER_SEX_MAN);
								break;

							case Player_Sex_Woman:
								strReal += CText::GetText(CTextDef::PLAYER_SEX_WOMEN);
								break;
							}
						}
						*/
					} 
					else if (strcmp(tmp, TL_PET_NAME) == 0) 
					{
						/*
						PlayerPetData *pPet =CHeroPet::instance().FindPlayerPetData(CHeroPet::instance().getOutFightPetId());
						if (pPet) 
						{
							char str[256];
							sprintf( str, "[%s]", pPet->name.c_str());
							strReal += str;
						}
						*/
					} 
					else if (strcmp(tmp, TL_MATE_NAME) == 0) 
					{
					}
					else if (tmp[0] == TIL_MONSTER_NAME)
					{
						/*
						strtok(tmp, "=,");
						char *p = strtok(NULL, "=,");
						int itemId = p ? atoi(p) : 0;
						const MonsterData* pMonster = MonsterCfg::instance().getMonsterCfgData(itemId);
						if (pMonster) 
						{
							char str[256];
							sprintf( str, "[%s]",pMonster->name.c_str());
							strReal+=str;
						}
						*/
					}
					else if (tmp[0] == TIL_MAP_NAME)
					{
						/*
						strtok(tmp, "=,");
						char *p = strtok(NULL, "=,");
						int mapId = p ? atoi(p) : 0;
						const MapData* pMap = MapCfg::instance().getMapCfgData(mapId);
						if (pMap) 
						{
							char str[256];
							sprintf( str, "[%s]",pMap->name.c_str());
							strReal+=str;
						}
						*/
					}
					else if (tmp[0] == TIL_SKILL_NAME)
					{
						/*
						strtok(tmp, "=,");
						char *p = strtok(NULL, "=,");
						int skillId = p ? atoi(p) : 0;
						const SkillInfo* pSkill = SkillCfg::instance().getSkillCfgData(skillId);
						if (pSkill) 
						{
							char str[256];
							string skillName = SkillCfg::instance().getName(pSkill);
							sprintf( str, "[%s]",skillName.c_str());
							strReal+=str;
						}
						*/
					}
					/*else if (tmp[0] == TIL_WORLDBOSS_NAME)
					{

					}*/
					else if (tmp[0] == TIL_ITEM_NAME)
					{
						/*
						strtok(tmp, "=,");
						char *p = strtok(NULL, "=,");
						int itemId = p ? atoi(p) : 0;
						const ItemData* pItem = ItemCfg::instance().findItemDataByID(itemId);
						if (pItem) 
						{
							char str[256];
							ccColor3B color = ColorCfg::instance().getQualityColorCfgData(pItem->m_quality)->color;
							sprintf( str, "<C=%d,%d,%d>[%s]</C>", color.r, color.g, color.b, pItem->m_name.c_str());
							strReal+=str;
						}
						*/
					} 
					////////////////////////????????????????????//////////////////////
					else if( tmp[0] == TIL_EQUIP_NAME )
					{
						/*
						strtok(tmp, "=,");
						char *p =  strtok(NULL, "=,");
						int equipId = p ? atoi( p ) : 0;
						p = strtok(NULL, "=,");
						int quality = p ? atoi( p ) : 0;
						ccColor3B color = ccc3(255,255,255);
						const ColorData *pColorData	= ColorCfg::instance().getQualityColorCfgData(quality);
						if( pColorData )
						{
							color = pColorData->color;
						}
						//const SEquipmentItem *pData = CEquipmentCfg::GetInstance().GetById(equipId);
						const ItemData* pData = ItemCfg::instance().findItemDataByID(equipId);
						if( pData != NULL )
						{
							char str[256];
							sprintf(str, "<C=%d,%d,%d>[%s]</C>", color.r, color.g, color.b, pData->m_name.c_str());
							strReal+=str;
						}
						*/
					}
					else if( tmp[0] == TIL_ADDRESS_NEWLINE )
					{
						strReal+='\n';
					}
					else 
					{
						strReal += "<";
						strReal += tmp;
						strReal += ">";
					}
					break;
				}
			}
		} else 
		{
			strReal += c;
		}
	}
	SAFE_DELETE_ARRAY(pStr);
	pStr = new char[strReal.length() + 1];
	strcpy(pStr, strReal.c_str());
	pStr[strReal.length()] = '\0';

	string strLine = "";

	for (int i = 0, j = 0; i < (int)strlen(pStr) + 1; ++i) 
	{
		char c = pStr[i];
		if (c == '<') 
		{
			if (i != 0 && i != j) 
			{
				strncpy(tmp, pStr + j, i - j);
				tmp[i - j] = '\0';

				if (strlen(tmp))
				{
					TextLine::TextNode node;
					node.pLabel = LabelBMFont::create(tmp, m_strFontName.c_str());
					node.pLabel->setScale(getTextScale());
					node.width = node.pLabel->getContentSize().width*getTextScale();
					node.pLabel->setAnchorPoint(Vec2(0,0));
					if (vColor.size())
						node.pLabel->setColor(vColor.back());
					else
						node.pLabel->setColor(ccc3(255, 255, 255));

					vLine.back().width += node.width;
					vLine.back().vNode.push_back(node);
				}
			}

			j = i + 1;
			for (; i < (int)strlen(pStr) + 1; ++i) 
			{
				c = pStr[i];
				if (c == '>') 
				{
					strncpy(tmp, pStr + j, i - j);
					tmp[i - j] = '\0';
					j = i + 1;

					if (tmp[0] == TL_COLOR) 
					{
						strtok(tmp, "=,");
						ccColor3B color;
						char *p = strtok(NULL, "=,");
						color.r = p ? atoi(p) : 255;
						p = strtok(NULL, "=,");
						color.g = p ? atoi(p) : 255;
						p = strtok(NULL, "=,");
						color.b = p ? atoi(p) : 255;
						vColor.push_back(color);
					}
					if (strcmp(tmp, TL_COLOR_END) == 0 && vColor.size()) 
					{
						vColor.pop_back();
					}

					if (tmp[0] == TL_COLOR_ID) 
					{
						strtok(tmp, "=,");
						ccColor3B color;
						char *p = strtok(NULL, "=,");
						int colorId = p ? atoi(p) : 0;
						const ColorData *pData = ColorCfg::instance().getColorCfgData(colorId);
						if (pData) 
						{
							color = pData->color;
						} else 
						{
							color = ccc3(255, 255, 255);
						}
						vColor.push_back(color);
					}
					if (strcmp(tmp, TL_COLOR_ID_END) == 0 && vColor.size()) 
					{
						vColor.pop_back();
					}
					break;
				}
			}
		} 
		else if (c == '\n') 
		{
			strncpy(tmp, pStr + j, i - j);
			tmp[i - j] = '\0';

			if (strlen(tmp)) 
			{
				TextLine::TextNode node;
				node.pLabel = LabelBMFont::create(tmp, m_strFontName.c_str());
				node.pLabel->setScale(getTextScale());
				node.width = node.pLabel->getContentSize().width*getTextScale();
				node.pLabel->setAnchorPoint(Vec2(0,0));
				if (vColor.size())
					node.pLabel->setColor(vColor.back());
				else
					node.pLabel->setColor(ccc3(255, 255, 255));

				vLine.back().width += node.width;
				vLine.back().vNode.push_back(node);
			}

			strLine = "";
			TextLine newLine;
			vLine.push_back(newLine);
			j = i + 1;
		} 
		else if (c == '\0') 
		{
			strncpy(tmp, pStr + j, i - j);
			tmp[i - j] = '\0';
			if (strlen(tmp)) 
			{
				TextLine::TextNode node;
				node.pLabel = LabelBMFont::create(tmp, m_strFontName.c_str());
				node.pLabel->setScale(getTextScale());
				node.width = node.pLabel->getContentSize().width*getTextScale();
				node.pLabel->setAnchorPoint(Vec2(0,0));
				if (vColor.size())
					node.pLabel->setColor(vColor.back());
				else
					node.pLabel->setColor(ccc3(255, 255, 255));

				strLine += tmp;
				vLine.back().width += node.width;
				vLine.back().vNode.push_back(node);
			}
		} 
		else 
		{
			float fTextWidth = m_fSize;
			if (strLine.size()) 
			{
 				LabelBMFont *pTempLable = LabelBMFont::create(strLine.c_str(), m_strFontName.c_str());
 				fTextWidth += pTempLable->getContentSize().width;
 				pTempLable->release();
				/*
				CCBMFontConfiguration* config = FNTConfigLoadFile(m_strFontName.c_str());
				if (!config)
					fTextWidth +=0;
				else
				{
					int len = cc_utf8_strlen(strLine.c_str(), -1);
					unsigned short* sString = NULL;
					sString = cc_utf16_from_utf8(strLine.c_str());

					tFontDefHashElement *element = NULL;
					for (size_t n = 0; n < (size_t)len; ++n)
					{
						unsigned int key = sString[n];
						HASH_FIND_INT(config->m_pFontDefDictionary, &key, element);
						if (!element)
						{
							key = LabelBMFont::m_sUnknowSting[0];
							HASH_FIND_INT(config->m_pFontDefDictionary, &key, element);
							if( !element ) 
								continue;
						}
						ccBMFontDef fontDef = element->fontDef;

						fTextWidth += fontDef.xAdvance*getTextScale();
					}
					delete [] sString;
				}*/
			}
			//换行
			if (fTextWidth >= m_width) 
			{

				if (bIsComputeWidth || !m_bAutoNewLine) 
				{
				} 
				else 
				{
					strncpy(tmp, pStr + j, i - j);
					tmp[i - j] = '\0';

					if (strlen(tmp)) 
					{
						TextLine::TextNode node;
						node.pLabel = LabelBMFont::create(tmp, m_strFontName.c_str());
						node.pLabel->setScale(getTextScale());
						node.width = node.pLabel->getContentSize().width*getTextScale();
						node.pLabel->setAnchorPoint(Vec2(0,0));
						if (vColor.size())
							node.pLabel->setColor(vColor.back());
						else
							node.pLabel->setColor(ccc3(255, 255, 255));

						vLine.back().width += node.width;
						vLine.back().vNode.push_back(node);
						j = i;
					}
					strLine = "";
					TextLine newLine;
					vLine.push_back(newLine);
				}
			}

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			//3个数字段表示一个汉字，FontSize为每个汉字的宽度
			// 汉字

			if (c > 0x00 && c <= 0x7F)//处理单字节UTF8字符（英文字母、数字）
			{
				strLine+=pStr[i];
			}
			else if (((c) & 0xE0) == 0xC0) //处理双字节UTF8字符
			{
				// 汉字
				strLine+=pStr[i];
				strLine+=pStr[i+1];
				//跳到下一个文字/字符开始
				i+=1;
			}
			else if (((c) & 0xF0) == 0xE0) //处理三字节UTF8字符
			{
				// 汉字
				strLine+=pStr[i];
				strLine+=pStr[i+1];
				strLine+=pStr[i+2];
				//跳到下一个文字/字符开始
				i+=2;
			}
//#else
//			//计算自动换行
//			if (c < 0) 
//			{
//				// 汉字
//				strLine += pStr[i];
//				strLine += pStr[i + 1];
//				strLine += pStr[i + 2];
//				//跳到下一个文字/字符开始
//				i += 2;
//			} else 
//			{
//				strLine += pStr[i];
//			}
//#endif
		}
	}

	//计算line与node位置
	float posX, posY;
	float fTextHeight = vLine.size() * m_fSize;
	if (bIsComputeHeight) 
	{
		m_height = (int)fTextHeight;
	}
	switch (m_iAlignType & 0xf0) 
	{
	case tAlignCenterY:
		posY = m_height / 2 - fTextHeight / 2 + fTextHeight - m_fSize;
		break;
	case tAlignTop:
		posY = m_height - m_fSize;
		break;
	case tAlignBottom:
		posY = fTextHeight - m_fSize;
		break;
	default: 
		{
			if (vLine.size() % 2 == 0)
				posY = m_height / 2 + fTextHeight / 2 - m_fSize;
			else
				posY = m_height / 2 - fTextHeight / 2;
		}
		break;
	};
	if (bIsComputeWidth || !m_bAutoNewLine) 
	{
		m_width = 0;
		for (vector<TextLine>::iterator itLine = vLine.begin();itLine != vLine.end(); ++itLine) 
		{
			if( itLine->width > m_width )
			{
				m_width = (int)itLine->width;
			}
		}
	}
	for (vector<TextLine>::iterator itLine = vLine.begin();itLine != vLine.end(); ++itLine) 
	{
		switch (m_iAlignType & 0xf) 
		{
		case tAlignCenterX:
			posX = m_width / 2 - itLine->width / 2;
			break;
		case tAlignLeft:
			posX = 5;
			break;
		case tAlignRight:
			posX = m_width - itLine->width;
			break;
		default:
			posX = m_width / 2.0f;
			break;
		}
		for (vector<TextLine::TextNode>::iterator itNode =itLine->vNode.begin(); itNode != itLine->vNode.end();++itNode) 
		{
			if (itNode->pLabel) 
			{
				itNode->pLabel->setPosition(Vec2(posX, posY));
				addChild(itNode->pLabel);
				posX += itNode->width;
			}
		}
		posY -= m_fSize;
	}

	SAFE_DELETE_ARRAY(pStr);
	return true;
}

void UIText::setText(const std::string &strText) 
{
	if (strText != m_text) 
	{
		m_text = strText;
		removeAllChildrenWithCleanup(true);
		setIgnoreTouch(true); //忽略点击
		parseText(m_text);
	}
}

void UIText::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
	UI::draw(renderer, transform, flags);

#ifdef DEBUG_DRAW
	Vec2oint pos = getRealPos();
	if( m_width > 0 && m_height >0 )
		FillRect( CCRectMake( 0, 0, m_width, m_height), ccc4(255,255,255,255), true);
#endif
}

void UIText::update(float t) 
{
	Node::update(t);
}

bool	UIText::isHaveUnKnowChar()
{
	auto& children = getChildren();
	for (const auto &child : children) {

		LabelBMFont *pLabel = dynamic_cast<LabelBMFont*>(child);
		if( pLabel) 
		{
			//if( pLabel->isHaveUnKnowChar() )
			//	return true;
		}
	}
	return false;
}


UIText * UIText::initUITextWithString( const std::string &strText )
{
	return initUITextWithString( strText, 18, Size::ZERO, tAlignCenterX | tAlignCenterY );
}

UIText * UIText::initUITextWithString(const std::string &strText,float fTextSize) 
{
	return initUITextWithString(strText, fTextSize, Size::ZERO, tAlignCenterX | tAlignCenterY);
}

void UIText::setColor(Color4B color) 
{
	m_color = color;

	Color3B c = ccc3(m_color.r, m_color.g, m_color.b);

	if (m_color.a > 255)
		m_color.a = 255;

	m_color = color;

	auto& children = getChildren();
	for (const auto &child : children) {
		UI* p = dynamic_cast<UI*>(child);
		if (p && p->isVisible()) 
		{
			p->setColor(m_color);
		} 
		else 
		{
			LabelBMFont *pLabel = dynamic_cast<LabelBMFont*>(child);
			if (pLabel) 
			{
				pLabel->setOpacity(m_color.a);
			}
		}
	}
}

UIText * UIText::initUITextWithString(const std::string &strText,float fTextSize, const CCSize &size, int iAlignType,bool isAutoNewLine) 
{
	return initUITextWithString(strText, fTextSize, size, iAlignType, FONT_BMP_24, isAutoNewLine );
}

UIText * UIText::initUITextWithString( const std::string &strText, float fTextSize, const CCSize &size , int iAlignType, string fontName, bool isAutoNewLine )
{
	UIText *pText = new UIText();
	pText->autorelease();
	pText->setIgnoreTouch(true); //忽略点击
	pText->m_iAlignType = iAlignType; //文字对齐方式
	pText->setTextSize(100); //文字大小
	pText->m_width = (int)size.width;
	pText->m_height = (int)size.height;
	pText->setContentSize(size);
	pText->m_text = strText;
	pText->m_bAutoNewLine = isAutoNewLine;
	pText->m_strFontName = _fixFontPath( fontName.c_str() );

	if (strText.size()) 
	{
		pText->parseText(strText);
	}
	return pText;
}


void UIText::setUseAlphaTest(bool alphaTest) 
{
	vector<TextLine>::iterator it = vLine.begin();
	vector<TextLine>::iterator end = vLine.end();
	for (; it != end; ++it) 
	{
		TextLine& tl = *it;

		for (int i = 0; i < (int)tl.vNode.size(); ++i) 
		{
			if (tl.vNode[i].pLabel) 
			{
				//tl.vNode[i].pLabel->setUseAlphaTest(alphaTest);
			}
		}
	}
}

void UIText::onReleaseResource( )
{
	removeAllChildrenWithCleanup(true);
	vLine.clear();
	m_pText = NULL;
	m_pTextDown = NULL;
}

void UIText::onReLoadResource( )
{
	parseText(m_text);
}
