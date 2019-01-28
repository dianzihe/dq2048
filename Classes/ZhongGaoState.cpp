#include "ZhongGaoState.h"
#include "GameScene.h"
#include "UIManager.h"
//#include "LoginBack.h"

CZhongGaoState::CZhongGaoState(void)
: m_pMainUI(NULL)
{
}

CZhongGaoState::~CZhongGaoState(void)
{
	OnDestroy();
}

#define		ShowAndHide_Time		0.5f
void CZhongGaoState::EnterState()
{
	log("=========CZhongGaoState::EnterState");
	ImageCenter::instance().LoadResource("Origin/zhonggao.png");
	m_pMainUI = UIManager::getInstance()->loadUI("Zhonggao.ui");
	if( !m_pMainUI ) {
		GameScene::GetScene()->ChangeState(GAME_STATE_CUTSCENE);
		return;
	}

	m_pMainUI->setVisible(true);
	m_pMainUI->setColor(ccc4(255,255,255, 0 ));
	m_fTime = ShowAndHide_Time * 3;

	if( IsCHTOnly() ) {
		m_pMainUI->setVisible( false );
		m_fTime = -1.0f;
	}
}

void CZhongGaoState::OnUpdate(float dt)
{
	log("=========CZhongGaoState::OnUpdate");
	if (dt > 1.0f/60.0f) {
		dt = 1.0f/60.0f;
	}

	if( !m_pMainUI ) {
		GameScene::GetScene()->ChangeState(GAME_STATE_CUTSCENE);
		return;
	}

	if( IsCHTOnly() ) {
		m_fTime = -1.0f;
	} else if( m_fTime > 2*ShowAndHide_Time ) {
		m_pMainUI->setColor(ccc4(255,255,255, ( GLubyte )( (3*ShowAndHide_Time - m_fTime)/ShowAndHide_Time * 255 )));
	} else {
		m_fTime = -1.0f;
		//只支持繁体
		if ( IsCHTOnly() ) {
			ChangeLanguage( LT_CHT, true );
			GameScene::GetScene()->ChangeState(GAME_STATE_CUTSCENE);
		} else if( /*IsCHSOnly()*/ true )	//临时修改，不显示语言选择界面，直接直接用简体
		{
			ChangeLanguage( LT_CHS, true );
			GameScene::GetScene()->ChangeState(GAME_STATE_CUTSCENE);
		} else {
			/*
			extern ClientSetup g_ClientSetup;
			switch ( g_ClientSetup.getDefaultLanguage() )
			{
			case DLT_None:
				OpenSelectLanguageTypeUI();
				break;
			case DLT_CHS:
				ChangeLanguage( LT_CHS, false );
				GameScene::GetScene()->ChangeState(GAME_STATE_CUTSCENE);
				break;
			case DLT_CHT:
				ChangeLanguage( LT_CHT, false );
				GameScene::GetScene()->ChangeState(GAME_STATE_CUTSCENE);
				break;
			}
			*/
		}
		return;
	}
	m_fTime -= dt;
}

void CZhongGaoState::OnDraw()
{
}

void CZhongGaoState::OnDestroy()
{
	ImageCenter::instance().UnLoadResource("Origin/zhonggao.png");
	UIManager::getInstance()->removeUI("Zhonggao.ui");
	//LoginBack::instance();
	m_pMainUI = NULL;
}

void OnSelectLanguageBtnUP( UI* ui, void* data )
{
	int type = (int)data;
	Language ltType = LT_CHS;
	switch( type )
	{
	case DLT_CHS:
		ltType = LT_CHS;
		break;

	case DLT_CHT:
		ltType = LT_CHT;
		break;
	}
	ChangeLanguage( ltType, true );
	//extern ClientSetup g_ClientSetup;
	//g_ClientSetup.setDefaultLanguage( type );
	//g_ClientSetup.saveToFile( ClientSetupFileName );
	GameScene::GetScene()->ChangeState(GAME_STATE_CUTSCENE);
	UIManager::getInstance()->removeUI( "ChooseLanguage.ui" );
}

void	CZhongGaoState::OpenSelectLanguageTypeUI()
{
	ImageCenter::instance().LoadResource("ImageCenter/HRes.bui");
	ImageCenter::instance().LoadResource("ImageCenter/LRes.bui");
	UI * pSelectUI = UIManager::getInstance()->loadUI( "ChooseLanguage.ui" );
	if( !pSelectUI )
	{
		GameScene::GetScene()->ChangeState(GAME_STATE_CUTSCENE);
		return;
	}

	UI*	pCHSBtn = pSelectUI->findUI( "Simplified1" );
	pCHSBtn->setEvent( EVENT_UP, OnSelectLanguageBtnUP, (void*)DLT_CHS );
	UI*	pCHTBtn = pSelectUI->findUI( "Traditional1" );
	pCHTBtn->setEvent( EVENT_UP, OnSelectLanguageBtnUP, (void*)DLT_CHT );
}
