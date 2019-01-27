#pragma once
#include "UI.h"

//**********************
//UIText  2012.8.16 by yqf
//**********************

enum enTextAlign
{
	tAlignCenterX = 1<<0,
	tAlignLeft = 1<<1,
	tAlignRight = 1<<2,

	tAlignCenterY = 1<<4,
	tAlignTop = 1<<5,
	tAlignBottom = 1<<6
};

struct TextLine
{
	struct TextNode
	{
		float width;
		LabelBMFont *pLabel;
		TextNode()
		{
			width = 0.0f;
			pLabel = NULL;
		};
	};
	float width;
	vector<TextNode> vNode;

	TextLine()
	{
		width = 0.0f;
	}
};

class UIText : public UI
{
	CC_SYNTHESIZE( float, m_fSize, TextSize);//字体大小
	CC_SYNTHESIZE( int, m_iAlignType, TextAlignType);//对齐方式
	CC_SYNTHESIZE( bool, m_bAutoNewLine, AutoNewLine);//自动换行
	CC_SYNTHESIZE( string, m_strFontName, FontName);//字体名字
public:
	UIText();
	virtual ~UIText();

	void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags);

	virtual void update(float dt) override;

	bool	isHaveUnKnowChar();

	void setText( const std::string& strText );

	virtual void setColor( ccColor4B color );

	static UIText * initUITextWithString( const std::string &strText);

	static UIText * initUITextWithString( const std::string &strText, float fTextSize );

	static UIText * initUITextWithString( const std::string &strText, float fTextSize, const CCSize &size , int iAlignType, bool isAutoNewLine = true );

	static UIText * initUITextWithString( const std::string &strText, float fTextSize, const CCSize &size , int iAlignType, string fontName, bool isAutoNewLine = true );

	//创建可格式化的字符串
	inline static void addStrItemToString( string &outString, string inString, Color3B color, bool bNewLine = false )
	{
		char sz[1024];
		if( bNewLine )
			outString += '\n';

		sprintf( sz, "<C=%d,%d,%d>%s</C>", color.r, color.g, color.b, inString.c_str() );
		outString += sz;
	}

	void setTextScale(float scale){mTxScale = scale;}

	float getTextScale(){return mTxScale;}
	
	void setUseAlphaTest(bool alphaTest);

	virtual void onReleaseResource();

	virtual void onReLoadResource();

private:
	bool parseText(const std::string &strText);
	vector<TextLine> vLine;
	float mTxScale;
};