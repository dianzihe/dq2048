#include "UICombo.h"
#include <string>
//#include "pk_player.h"

using namespace std;

#define FONT_COMBONUM		"Font/5.fnt"
#define FONT_COMBOTEXT		"Font/7.fnt"
#define TEX_BACKGOUND		"UI/LRes/main_lianji.png"
#define TEX_PROCESS1		"UI/LRes/main_lianji1.png"
#define TEX_PROCESS2		"UI/LRes/main_lianji2.png"

const float _LIMITE_COMBO_KILL_TIME			= 3.0f;
const float _LIMITE_COMBO_500KILL_TIME		= 2.0f;
const float _LIMITE_COMBO_1000KILL_TIME		= 1.0f;
const float _LIMITE_COMBO_2000KILL_TIME		= 0.5f;

const float _LIMITE_COMBO_HIT_TIME  = 1.0f;
#define  Player_Effect_Point_x   70.0f
#define  Player_Effect_Point_Space_x 10.0f

static float sNowKillEffectTime;

const string cProcess[TYPE_NUM][LAYERNUM] = {"HITLAY1","HITLAY2","KILLLAY1","KILLLAY2"};

UICombo::UICombo(void)
{
	m_MaxDrpCobTime = 1.0f;
	m_MaxDrpKilTime = 10.0f;
	m_ComboTimes = 0;
	m_KillTimes = 0;
	m_pCurCobSFX = NULL;
	m_pCurKilSFX = NULL;
	m_pCurCobTXT = NULL;
	m_pCurKilTXT = NULL;
	memset(m_pProcess,0,sizeof(UI*)*TYPE_NUM*LAYERNUM);
}

const float xoffset = 60.f;
const float yoffset = 300.f;

UICombo::~UICombo(void)
{
}

UICombo* UICombo::getInstance()
{
	static UICombo m_instance;
	return &m_instance;
}

void UICombo::AddBackground(float x, float y, int type)
{
	int hash = 1<<5;

	UI* process1 = m_pProcess[type][LAYER1];
	UI* process2 = m_pProcess[type][LAYER2];
	
	if (!process1)
	{
		UIData stData;
		stData.align = 4;
		stData.tag = 0;
		stData.height = 10;
		stData.width = 60;
		stData.name = cProcess[type][LAYER1];
		stData.picName = "";
		stData.text = "";
		stData.type = UI_BASE;
		stData.x = 0;
		stData.y = 0;
		process1 = stData.createUI();
		process1->setIgnoreTouch(true);

		process1->setImageByFullPath(TEX_PROCESS1);

		process1->setVisible(false);

		process1->setPosition(x - 20.0f,y - 10.0f);

		UIManager::Instance()->addUI(process1,(int)process1, false);
		//p->addChild(process1,false,hash+type*LAYERNUM+LAYER1);

		m_pProcess[type][LAYER1] = process1;
		
	}

	if (!process2)
	{
		char str[128];
		sprintf(str, "%s%d",TEX_PROCESS2, type);

		UIData stData;
		stData.align = 4;
		stData.tag = 0;
		stData.height = 3;
		stData.width = 52;
		stData.name = cProcess[type][LAYER2];
		stData.picName = "";
		stData.text = "";
		stData.type = UI_BASE;
		stData.x = 1;
		stData.y = 2;
		process2 = stData.createUI();
		process2->setIgnoreTouch(true);

		process2->setImageByFullPath(TEX_PROCESS2);

		process2->setVisible(false);

		UIManager::Instance()->addUI(process2,(int)process1, false);
		//p->addChild(process2,false,hash+type*LAYERNUM+LAYER2);

		process2->setPosition(x - 17.0f,y - 6.0f);

		m_pProcess[type][LAYER2] = process2;		
	}
}


void UICombo::init()
{	

 	AddBackground(SCREEN_SIZE.width - xoffset - 50.0f, SCREEN_SIZE.height - yoffset + 30.0f, TYPE_HIT);
 	AddBackground(SCREEN_SIZE.width - xoffset - 50.0f, SCREEN_SIZE.height - yoffset - 20.0f,TYPE_KILL);
}

void UICombo::Uninit()
{
  	UIManager::Instance()->removeUI(cProcess[TYPE_HIT][LAYER1]);
  	UIManager::Instance()->removeUI(cProcess[TYPE_HIT][LAYER2]);
  	UIManager::Instance()->removeUI(cProcess[TYPE_KILL][LAYER2]);
  	UIManager::Instance()->removeUI(cProcess[TYPE_KILL][LAYER2]);

	memset(m_pProcess,0,sizeof(UI*)*TYPE_NUM*LAYERNUM);
	
	if (m_pCurCobSFX)
	{
		SceneInstance::instance().getSFXModule()->DelSFX(m_pCurCobSFX);
		m_pCurCobSFX = NULL;
	}

	if (m_pCurKilSFX)
	{
		SceneInstance::instance().getSFXModule()->DelSFX(m_pCurKilSFX);
		m_pCurKilSFX = NULL;
	}

	if (m_pCurCobTXT)
	{
		SceneInstance::instance().getSFXModule()->DelSFX(m_pCurCobTXT);
		m_pCurCobTXT = NULL;
	}

	if (m_pCurKilTXT)
	{
		SceneInstance::instance().getSFXModule()->DelSFX(m_pCurKilTXT);
		m_pCurKilTXT = NULL;
	}
}

void UICombo::setKillTimes(int t)
{
	if (t <= 500)
	{
		sNowKillEffectTime = _LIMITE_COMBO_KILL_TIME;
	}
	else if (t <= 1000)
	{
		sNowKillEffectTime =  _LIMITE_COMBO_500KILL_TIME;
	}
	else if(t < 2000)
	{
		sNowKillEffectTime =  _LIMITE_COMBO_1000KILL_TIME;
	}
	else
	{
		sNowKillEffectTime =  _LIMITE_COMBO_2000KILL_TIME;
	}
	
	m_KillTimes = t;
}

void UICombo::Process()
{
	m_MaxDrpCobTime = _LIMITE_COMBO_HIT_TIME;
}

void UICombo::pushHit()
{
	if (m_pCurCobSFX)
	{
		SceneInstance::instance().getSFXModule()->DelSFX(m_pCurCobSFX);
		m_pCurCobSFX = NULL;
	}
	
	m_ComboTimes++;

	char text[10] = "";
	sprintf(text,"%d",m_ComboTimes);

	if (!m_pCurCobTXT)
	{
		m_pCurCobTXT = new ComboSFX();
		CCPoint tmp = CCPoint(SCREEN_SIZE.width - xoffset, SCREEN_SIZE.height - yoffset + 10.0f);
		m_pCurCobTXT->Initialize( tmp, "1", FONT_COMBOTEXT);
		//pSFX->setNoSFX();
		m_pCurCobTXT->SetSFXLocation(eSFXLocation_BackUI);
		m_pCurCobTXT->SetLifeStyle( eLife_Allways );
		SceneInstance::instance().getSFXModule()->AddSFX(m_pCurCobTXT);
	}

	{
		int nCount = strlen(text)-1;
		m_pCurCobSFX = new ComboSFX();

		CCPoint temp = CCPoint(SCREEN_SIZE.width - xoffset - Player_Effect_Point_x-Player_Effect_Point_Space_x*nCount, SCREEN_SIZE.height - yoffset);
		m_pCurCobSFX->Initialize( temp, text, FONT_COMBONUM, true);
		//pSFX->setNoSFX();
		m_pCurCobSFX->SetSFXLocation(eSFXLocation_BackUI);
		m_pCurCobSFX->SetLifeStyle( eLife_Allways );
		SceneInstance::instance().getSFXModule()->AddSFX(m_pCurCobSFX);
	}

}


void UICombo::pushKill()
{
	if (m_pCurKilSFX)
	{
		SceneInstance::instance().getSFXModule()->DelSFX(m_pCurKilSFX);
		m_pCurKilSFX = NULL;
	}

	char text[10] = "";
	sprintf(text,"%d",m_KillTimes);
	int nCount = strlen(text)-1;
	static float pp = 10.0f;
	static float hh = 10.0f;

	if (!m_pCurKilTXT)
	{
		m_pCurKilTXT = new ComboSFX();
		CCPoint temp = CCPoint(SCREEN_SIZE.width - xoffset - 50.0f, SCREEN_SIZE.height - yoffset - 50.0f + hh);
		m_pCurKilTXT->Initialize( temp, "-", FONT_COMBOTEXT);

		m_pCurKilTXT->SetSFXLocation(eSFXLocation_BackUI);
		m_pCurKilTXT->SetLifeStyle( eLife_Allways );
		SceneInstance::instance().getSFXModule()->AddSFX(m_pCurKilTXT);
	}

	{
		m_pCurKilSFX = new ComboSFX();
		CCPoint tmp = CCPoint(SCREEN_SIZE.width - xoffset - Player_Effect_Point_x-Player_Effect_Point_Space_x*nCount, SCREEN_SIZE.height - yoffset - 60.0f);
		m_pCurKilSFX->Initialize( tmp, text, FONT_COMBONUM);

		m_pCurKilSFX->SetSFXLocation(eSFXLocation_BackUI);
		m_pCurKilSFX->SetLifeStyle( eLife_Allways );
		SceneInstance::instance().getSFXModule()->AddSFX(m_pCurKilSFX);
	}
	m_MaxDrpKilTime = sNowKillEffectTime;	
}

void UICombo::update(float dt)
{
	
	if (m_pCurCobSFX)
	{
		m_MaxDrpCobTime -= dt;
		if (m_MaxDrpCobTime > 0.0f)
		{	
			m_pProcess[TYPE_HIT][LAYER1]->setVisible(true);
			m_pProcess[TYPE_HIT][LAYER2]->setVisible(true);
			m_pProcess[TYPE_HIT][LAYER2]->setScaleX(m_MaxDrpCobTime/_LIMITE_COMBO_HIT_TIME);
		}
		else
		{
			SceneInstance::instance().getSFXModule()->DelSFX(m_pCurCobSFX);
			m_pCurCobSFX = NULL;
			SceneInstance::instance().getSFXModule()->DelSFX(m_pCurCobTXT);
			m_pCurCobTXT = NULL;
			
			m_ComboTimes = 0;

			m_pProcess[TYPE_HIT][LAYER2]->setVisible(false);
			m_pProcess[TYPE_HIT][LAYER1]->setVisible(false);
		}
	}

	if (m_pCurKilSFX)
	{
		m_MaxDrpKilTime -= dt;
		if (m_MaxDrpKilTime > 0.0f)
		{
			//m_pBackground[TYPE_KILL]->setVisible(true);
			m_pProcess[TYPE_KILL][LAYER1]->setVisible(true);
			m_pProcess[TYPE_KILL][LAYER2]->setVisible(true);

			if (m_MaxDrpKilTime >= sNowKillEffectTime)
			{
				m_pProcess[TYPE_KILL][LAYER2]->setScaleX(1.0f);
			}
			else
			{
				m_pProcess[TYPE_KILL][LAYER2]->setScaleX(m_MaxDrpKilTime/sNowKillEffectTime);
			}
		}
		else
		{
			SceneInstance::instance().getSFXModule()->DelSFX(m_pCurKilSFX);
			m_pCurKilSFX = NULL;
			SceneInstance::instance().getSFXModule()->DelSFX(m_pCurKilTXT);
			m_pCurKilTXT = NULL;

			m_KillTimes = 0;

			m_pProcess[TYPE_KILL][LAYER1]->setVisible(false);
			m_pProcess[TYPE_KILL][LAYER2]->setVisible(false);
			
		}
	}
}

void	pk::OnGS2U_ContinueKill(GS2U_ContinueKill* value)
{
	UICombo::getInstance()->setKillTimes(value->killNumber);
	UICombo::getInstance()->pushKill();
}


