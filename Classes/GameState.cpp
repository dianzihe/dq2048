#include "base.h"
#include "GameState.h"
#include "GameScene.h"
#include "ui.h"
#include "UIEditNew.h"
#include "Text.h"
#include "Task.h"
#include "UIManager.h"

//#include "BattlePointUI.h"

static const char* names[] = { "wp11", "wpd2", "test" };

void CGameState::getNetInfo()
{
	/*
	long long timeRec = CCTime::getMSecond();
	mNetworkstate.dealyTime[mNetworkstate.times] = (int)((timeRec - mNetworkstate.lastSendTime)/2);
	mNetworkstate.times +=1;

	if (mNetworkstate.times == TIME_GET_COUNT || mIsEnterGameFirTime)
	{
		if (mIsEnterGameFirTime)
		{
			mNetworkstate.aveTime = mNetworkstate.dealyTime[TIME_GET_FIR];
		}
		else
		{
			mNetworkstate.aveTime = (mNetworkstate.dealyTime[TIME_GET_FIR] +mNetworkstate.dealyTime[TIME_GET_SEC] + mNetworkstate.dealyTime[TIME_GET_THR])/3;
		}
		mIsEnterGameFirTime = false;
		
		showNetStateBtn(mNetworkstate.aveTime);
		
		mNetworkstate.lastSendTime = 0;
		mNetworkstate.times = TIME_GET_FIR;
		memset(mNetworkstate.dealyTime, 0, sizeof(mNetworkstate.dealyTime));
		mNetworkstate.aveTime = 0;
	}
	*/
}
void CGameState::showNetStateBtn(int dealyTime)
{
	/*
	if (!CGameMainUI::instance()->pNetStateFast 
	||  !CGameMainUI::instance()->pNetStateNormal 
	||  !CGameMainUI::instance()->pNetStateSlow)
		return;

	if (dealyTime >= 0 && dealyTime<=100)
	{
		CGameMainUI::instance()->pNetStateFast->setVisible(true);
		CGameMainUI::instance()->pNetStateNormal->setVisible(false);
		CGameMainUI::instance()->pNetStateSlow->setVisible(false);
	}
	else if (dealyTime > 100 && dealyTime < 300)
	{
		CGameMainUI::instance()->pNetStateFast->setVisible(false);
		CGameMainUI::instance()->pNetStateNormal->setVisible(true);
		CGameMainUI::instance()->pNetStateSlow->setVisible(false);
	}
	else if (dealyTime >= 300)
	{
		CGameMainUI::instance()->pNetStateFast->setVisible(false);
		CGameMainUI::instance()->pNetStateNormal->setVisible(false);
		CGameMainUI::instance()->pNetStateSlow->setVisible(true);
	}
	*/
}
//////////////////////////////////////////////////////////////////////////
CGameState::CGameState() 
: mLoadingFrame( 1 )
, mLoadingUI( NULL )
,mBeKickOutedTime(0)
,mIsEnterGameFirTime(true)
, mIsFirstTimeLoading(true)
{
	mNetworkstate.lastSendTime = 0;
	mNetworkstate.times = 0;
	mNetworkstate.aveTime = 0;
	memset(mNetworkstate.dealyTime, 0, sizeof(mNetworkstate.dealyTime));
}

CGameState::~CGameState()
{
	mBeKickOutedTime = 0;
	OnDestroy();
}
void	ClientToGSOnHeart();

#define		GAME_HEART_TIME	10

static void __ClientToGSOnHeart()
{
	/*
	if( !GameScene::GetScene() ) return;
	static unsigned long timeCounter = 0;
	cc_timeval now;
	CCTime::gettimeofdayCocos2d(&now,NULL);
	if( now.tv_sec-timeCounter >= GAME_HEART_TIME )
	{
		CGameState *pGame =  dynamic_cast<CGameState*>(GameScene::GetScene()->GetState());
		if (pGame) 
			pGame->mNetworkstate.lastSendTime = (int)CCTime::getMSecond();
		pk::U2GS_HeartBeat toServer;
		toServer.Send();
		timeCounter = 0;
		timeCounter = (unsigned long)now.tv_sec;
	}
	*/
}

void	ClientToGSOnHeart()
{
	/*
	if( GameScene::GetScene() && 
		(GameScene::GetScene()->m_eGameState == GAME_STATE_GAME || 
		GameScene::GetScene()->m_eGameState == GAME_STATE_CHARLOBBY || 
		GameScene::GetScene()->m_eGameState == GAME_STATE_CREATEPLAYER || 
		GameScene::GetScene()->m_eGameState == GAME_STATE_LOADING) )
		__ClientToGSOnHeart();
	*/
}

void CGameState::OnDestroy()
{
	log( "CGameState::OnDestroy begin" );

	if ( isLoading() ) {
		endLoading();
	}
	/*
	shituCfg::Instance()->unInit();
	CshituUI::instance().Unload();
	CMutipMine::getInstance().uninit();
	CExppool::Instance().UnInit();
	Variant::instance().Uninitialize();
	LoadingModule::instance().UnInitialize();
	GameScene::GetScene()->GetHeroNavigation()->CancelNavigation();
	//CPlayerEquipment::instance().Uninitialize();
	ItemMgr::instance().Uninitialize();
	SmallBag::instance().release();
	NpcStore::instance().release();
	TokenStore::instance().release();
	ItemBag::instance().release();
	CNpcItemStore::instance().release();
	GameScene::GetActorManager()->onGameStateLeave();
	GameScene::GetScene()->GetCurMap()->unInitialize();
	ChanllengePanelUi::instance().unLoad();
    CNPCMissionUI::Instance()->UnLoad();
	ScenarioModule::instance().UnInitialize();
	CGameMainUI::instance()->UnLoad();
	CChampionRankUI::Instance().UnInit();
	CIconRemindPage::instance().unInit();
	CChampionRankGift::Instance().UnInit();
	CPlayerRankUI::Instance().UnInit();
	RankHistoryUI::Instance().UnInit();
	CChampionMainUI::Instance().UnInit();
	CConSalesBank::instance().unInit();
	CChat::instance().unInit();
	CTaskMgr::Instance()->unLoad();
	XSMUI::instance().unInit();
	CMissionUI::Instance()->unInit();
	CShenMiTianLaoGaoShouZhuZhen::instance().UnInit();
	SkillTips::unInit();
	UICombo::getInstance()->Uninit();
	MessageTips::instance().unInit();
	//CPlayerInfoUI::instance()->unInit();
	CSMCharacter::instance().UnInit();
	CSMCharacterFB::instance().UnInit();
	CPlayerEquipAtrUI::instance()->unInit();
	GameScene::GetHeroNavigation()->UnInitialize();
	SceneInstance::instance().UnInitialize();
	//CHangUpUI::instance()->UnInitialize();
	CBazzarItem::instance().unInit();
	PlayerSkillPanel::GetInstance().UnInit();
	CGameHangUp::GetInstance().UnInit();
	GuideModule::instance().UnInitialize();
	//CMountUI::instance().unInit();
	CHeroMount::instance().unInit();
	CSystemSetUI::instance().Uninitialize();
	CFriendUI::instance()->Uninitialize();
	CGuildUI::instance()->Uninitialize();
	CMAILMgr::Instance()->Uninitialize();
	SkillFilterModule::instance().Uninitialize();
	CItemCD::instance().unInit();
	CShortPage::instance().unInit();
	CShenMiTianLaoUI::instance().UnInit();
	CHeroPet::instance().unInit();
	CWaitingUI::instance().unInit();
	CNumberInputBox::instance().Uninitialize();
	Guild::instance().Uninitialize();
	ItemTipsUI::instance().unInit();
	BuffListUI::instance().unInit();
	CombatModule::instance().CleanAllCombatData();
	CWorldMiniMap::instance().Uninitialize();
	CSystemMessageUI::instance().unInit();
	CPlayerTrade::instance().unInit();
	EquipmentMainUI::instance().UnInit();
	CFriend::Instance()->unInit();
	CItemGetUI::instance().unInit();
	TeamUI::Instance()->unInit();
	CWelfareUI::instance().unInit();
	CCMemoryMonitor::sharedMemoryMonitor()->cleanMemoryWarning();
	CQuestionAndAnswerUI::instance()->uninit();
	ConvoyUI::Instance()->unInitUI();

	CTopUi::instance().uninit();
	CTeam_logicCfg::Instance()->Uninitialize();

	CTargetHeadMenu::instance().unInit();
	CRechargeEditUI::instance().unInit();
	CCheckPlayerUI::instance().unInit();

	EquipmentBagForRefine::instance().UnInit();
	EquipmentBagForQualityUp::instance().UnInit();
	CXSMGiftBag::instance().UnInit();

	CWipeoutUI::instance().unInit();

    OnlineSurveyUI::instance()->uninit();
	CMagicSystemUI::instance().unInit();
	OnlineLotteryUI::shareInstance()->unInit();
	CFriendTeamAdditionUI::instance().unInit();
	CSpecialSkillUI::instance().unInit();
	CGaoShouTopUi::instance().uninit();
	CGaoShouDuiZhanBiaoUI::instance().unInit();
	CBeaconMainUI::instance().uninit();
	CJzfbUI::instance().unInit();
	CHorseGrowUI::instance().unInit();
	MFShareWeiboUI::shareInstance()->unInit();
	CChampionSituation::Instance().unInit();
	CPlayerListUI::Instace().unInit();
	CItemGetWayPanel::Instace().uninit();
	CShop::instance().UnInit();
	StrengthGrowUp::Instance().UnInit();
	CResolveEquipment::Instance().Uninit();
	CSevenDaysGift::instance().uninit();
	COnlineAwardUI::instance().uninit();
	CVitalityConvert::instance().unninit();
	//CBattlePointUI::instance().uninit();
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
	CRechargeUI_IOS::instance().uninit();
#endif

 #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (updateGlobal::getInstance()->m_platformID == Platform_TW_ANDROID)
	{
		CRechargeUI_Android_TW::instance().uninit();
	}
#endif

	CCopyProgressUI::instance().unInit();
	CCopyMapSAUI::instance().unInit();

	CDailyUI::instance().uninit();

	VIPXSMUI::Instance().unInit();

	CBattleUI::instance().unInit();

	CDirectHandleUI::instance().unInit();
	CTaskTeamTabUI::instance().unInit();
	CChampionMainUI::Instance().UnInit();
	ChampionUI::Instance().unInit();
	CLevelGiftUI::instance().unInit();
	CBoundPhone::instance().unInit();
	CPrayUI::instance().uninit();
	CZhuXianTaUI::instance().unInit();
	CPlayingCardUI::instance().unInit();
	CGiftShortUI::instance().unInit();
	CMasterTilTleTip::instance().UnInit();
	FieldBossUi::instance().unLoad();
	shouHuUI *pShouHuUI = shouHuUI::Instance();
	if (NULL != pShouHuUI)
	{
		pShouHuUI->unInit();
	}
	CGrowUpUI::instance().unInit();
	CFlowersUI::instance().unInit();
	CAcupointUI::instance().unInit();

	MFLevelPrompt::shareInstance()->uninit();
	MFLoginQueue::shareInstance()->unInit();
	CSitDown::instance().unInit();
	OpenFunctionUi::instance().unLoad();
	Target::instance().unInit();
	CMainUIOpenCloseHandle::instance().UnInit();
	CBath::instance().unInit();
	//net.closeSocket();
	//net.connect( g_ClientSetup.getLoginIP().c_str(), g_ClientSetup.getLoginPort() );

	INFO_LOG( "CGameState::OnDestroy end" );
    
	UIManager::getInstance()->clear();
	GameScene::GetUI()->removeAllChildrenWithCleanup(true);
	CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
	mIsFirstTimeLoading = true;
	*/
}

static void _LoadEnterGameState()
{
	//GameScene::GetActorManager()->onGameStateEnter();
	log("GameScene::GetActorManager()->onGameStateEnter()");
	//CGameMainUI::instance()->Load();
	log("CGameMainUI::instance()->Load()");
	//ScenarioModule::instance().Initialize();
	log("ScenarioModule::instance().Initialize()");
	//SceneInstance::instance().Initialize();
	log("SceneInstance::instance().Initialize()");
	//GuideModule::instance().Initialize();
	log("GuideModule::instance().Initialize()");
	//CShortPage::instance();
	//CSystemSetUI::instance().load();
	//UpdateMessage::instance().load();
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
	//CRechargeUI_IOS::instance().init();
#endif

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (updateGlobal::getInstance()->m_platformID == Platform_TW_ANDROID)
	{
		CRechargeUI_Android_TW::instance().init();
	}
#endif

	log("CGameMainUI::instance()->Load()");
}

void CGameState::EnterState()
{
	_LoadEnterGameState();
	/*
	CPlayingCardUI::instance().request_state();
	CShop::instance().ShowShopUI( false);
	//////////////////////////////////////////////////////////////////////////
	Variant::instance();
	SmallBag::instance();
	NpcStore::instance();
	TokenStore::instance();
	ItemBag::instance();
	CNpcItemStore::instance();
	CIconRemindPage::instance();
	CChampionMainUI::Instance();
	CChampionRankUI::Instance();
	CChampionRankGift::Instance();
	RankHistoryUI::Instance();
	CPlayerRankUI::Instance();
	CConSalesBank::instance();
	CChat::instance();
	XSMUI::instance();
	//SkillTips::instance();
	//CPlayerInfoUI::instance()->unInit();
	//CHangUpUI::instance()->UnInitialize();
	CBazzarItem::instance();
	CBoundPhone::instance();
	RankHistoryUI::Instance();
	CGameHangUp::GetInstance();
	CExppool::Instance();
	//CMountUI::instance().unInit();
	CFriendUI::instance();
	CGuildUI::instance()->show(false);
	ItemTipsUI::instance();
	BuffListUI::instance();
	CItemGetUI::instance();
	ConvoyUI::Instance();
	CTopUi::instance();
	CShenMiTianLaoUI::instance();
	CTargetHeadMenu::instance();
	CRechargeEditUI::instance();
	CSMCharacterFB::instance();

	TeamUI::Instance();
	CWipeoutUI::instance();

	OnlineSurveyUI::instance();
	CMagicSystemUI::instance();
	OnlineLotteryUI::shareInstance();
	CFriendTeamAdditionUI::instance();
	CSpecialSkillUI::instance();
	CGaoShouTopUi::instance();
	CGaoShouDuiZhanBiaoUI::instance();
	CBeaconMainUI::instance();
	CShenMiTianLaoGaoShouZhuZhen::instance();

	CJzfbUI::instance();
	CHorseGrowUI::instance();
	MFShareWeiboUI::shareInstance();
	CPlayerListUI::Instace();
	CItemGetWayPanel::Instace();

	CCopyProgressUI::instance();
	CCopyMapSAUI::instance();


	VIPXSMUI::Instance();

	CBattleUI::instance();

	CDirectHandleUI::instance();
	CTaskTeamTabUI::instance();

	CLevelGiftUI::instance();
	CPrayUI::instance();
	//CZhuXianTaUI::instance();
	//CGiftShortUI::instance();
	shouHuUI::Instance();
	CGrowUpUI::instance();
	CFlowersUI::instance();
	CSMCharacter::instance();
	CAcupointUI::instance();


	MFLevelPrompt::shareInstance();
	CMasterTilTleTip::instance();
	MFLoginQueue::shareInstance();
	*/

}

void CGameState::OnDraw()
{
	if ( mLoadingFrame > 0 ) {
		--mLoadingFrame;
		if ( mLoadingFrame == 0 ) {
			// mark: loading image just been presented
			mLoadingFrame = -1000;
		}
	}
}

void CGameState::decideIsToreloading()
{
	/*
	CGameState *mPgamestate = dynamic_cast<CGameState*>(GameScene::GetScene()->GetState());
	if (0 != mPgamestate->mBeKickOutedTime)
	{
		long long rebackLoadingTime = CCTime::getMSecond();
		if (rebackLoadingTime - mPgamestate->mBeKickOutedTime >= 500)
		{
			GameScene::GetScene()->ChangeState(GAME_STATE_LOGIN);
			mPgamestate->mBeKickOutedTime = 0;
		}
	}
	*/
}

void CGameState::OnUpdate(float dt)
{		
	/*
	if ( mLoadingFrame != 0 ) {
		if ( mLoadingFrame == -1000 ) {
			// hold the loading image a little period
			if(mIsFirstTimeLoading)
			{
				if ( UpdateTool::AppUpdater::isEnabled() ) {
					UpdateTool::LocalResVersionInfo resVer = UpdateTool::AppUpdater::loadLocalResVersionInfo();
					int val = Variant::instance().getPlayerVariant(PlayerVariant_Index_Is_Update_Package);   
					if( val == 0
					   && UpdateMessage::instance().IsRequestUpdate() )
					{
						//pk::U2GS_StartUpdatePackage downloadResFinish;
						//downloadResFinish.Send();
						UpdateMessage::instance().UpdateRewardUse();
					}
				}
				else
				{
#if defined UPDATE_START
					int val = Variant::instance().getPlayerVariant(PlayerVariant_Index_Is_Update_Package);   
					if( val == 0
					   && UpdateMessage::instance().IsRequestUpdate()
					   && GameScene::GetScene()->m_cp->isFinishedUpdateFullResource() )
					{
						//pk::U2GS_StartUpdatePackage downloadResFinish;
						//downloadResFinish.Send();
						UpdateMessage::instance().UpdateRewardUse();
					}
#endif
				}
			}
			else
			{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
				Sleep(1000);
#else
				sleep(1);
#endif
			}
			endLoading();
			mLoadingFrame = 0;
			mIsFirstTimeLoading = false;
		}
		return;
	}
	*/
	log("CGameState::OnUpdate");

	/*
	CGameMainUI::instance()->update(dt);
	GameScene::GetScene()->GetCurMap()->update(dt);
	GameScene::GetActorManager()->update(dt);
	GameScene::GetScene()->GetHeroNavigation()->Update(dt);
	CombatModule::instance().Update(dt);
	ScenarioModule::instance().Update(dt);
	CNPCMissionUI::Instance()->update(dt);
	GuideModule::instance().Update(dt);
	OpenFunctionUi::instance().update(dt);
	CItemCD::instance().updateCD(dt);
	CDailyUI::instance().check_notify(dt);
	CChat::instance().onTrumpetUpdate(dt);
	CQuestionAndAnswerUI::instance()->update(dt);
	OnlineLotteryUI::shareInstance()->update(dt);
	//CPlayingCardUI::instance().update(dt);
	CCopyMapSAUI::instance().update(dt);
	CPrayUI::instance().update(dt);
	CChampionMainUI::Instance().OnUpdate(dt);
	CGameHangUp::GetInstance().update(dt);
	MFLevelPrompt::shareInstance()->update(dt);
	EquipmentPropup::instance().Update(dt);
	CPlayerEquipAtrUI::instance()->Update(dt);
	ShowKnigtIslandInfo::Instance().Update(dt);
	XSMUI::instance().UpdatePutOn(dt);
	CHorseGrowUI::instance().updatePutOn(dt);
	COnlineAwardUI::instance().update(dt);
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (updateGlobal::getInstance()->m_platformID == Platform_TW_ANDROID)
	{
		CRechargeUI_Android_TW::instance().update(dt);
	}
#endif
*/
	ClientToGSOnHeart();

	//decideIsToreloading();
}


void CGameState::showInGameLoading()
{
	++mLoadingFrame;
	if ( mLoadingFrame == 1 ) {
		beginLoading();
	}
}

void CGameState::beginLoading()
{
	if ( !mLoadingUI ) {
		char imageName[ 256 ];
		int imageIdx = rand();
		imageIdx = imageIdx % 4 + 1;
		sprintf( imageName, "Origin/LoadImage%d.wen", imageIdx );
		if( !(ImageCenter::instance().LoadResource(imageName)) )
		{
			sprintf(imageName, "Origin/LoadImage%d.wen", 1);
			ImageCenter::instance().LoadResource(imageName);
		}
		mImageName = imageName;

		ImageCenter::instance().LoadResource("Origin/GameName.png");
		mLoadingUI = UIManager::getInstance()->loadUI("LoadingUI.ui");
		if ( mLoadingUI ) {
			mLoadingUI->setVisible( true );
		}
		ImageCenter::instance().UnLocalLoadResource( "ImageCenter/Loading.bui" );
	}
}

void CGameState::endLoading()
{
	if ( mLoadingUI ) {
		mLoadingUI->setVisible( false );
		UIManager::getInstance()->removeUI( "LoadingUI.ui" );
		mLoadingUI = NULL;

		ImageCenter::instance().UnLoadResource( mImageName.c_str() );
		ImageCenter::instance().UnLoadResource("Origin/GameName.png");
		mImageName.clear();
	}
	/*
	Map* pMap = GameScene::GetScene()->GetCurMap();
	if(pMap && pMap->isMapPackgeNeedUpdate())
    {
        if(!Variant::instance().getWorldVariantFlag(WorldVariant_Index_1, WorldVariant_Index_1_Active_Update_Resource ))
            UpdateMessage::instance().show(true);
    }
	*/
}


int CLoadingState::mType = -1;

CLoadingState::CLoadingState() : mLoadingUI( NULL ),
mFrameCount( 0 ), mExit( false )
{
}

CLoadingState::~CLoadingState()
{
	ImageCenter::instance().UnLoadResource( mImageName.c_str() );
	mImageName.clear();
	if ( mLoadingUI ) {
		mLoadingUI->setVisible( false );
		UIManager::getInstance()->removeUI( "LoadingUI.ui" );
		mLoadingUI = NULL;
	}
	ImageCenter::instance().UnLoadResource("Origin/GameName.png");
}

void CLoadingState::EnterState()
{
	char imageName[ 256 ];
	int imageIdx = rand();
	imageIdx = imageIdx % 4 + 1;
	sprintf( imageName, "Origin/LoadImage%d.wen", imageIdx );
	if( !(ImageCenter::instance().LoadResource(imageName)) )
	{
		sprintf(imageName, "Origin/LoadImage%d.wen", 1);
		ImageCenter::instance().LoadResource(imageName);
	}
	mImageName = imageName;
		
	mLoadingUI = UIManager::getInstance()->loadUI("LoadingUI.ui");
	if ( mLoadingUI ) {
		mLoadingUI->setVisible( true );
	}
	ImageCenter::instance().UnLocalLoadResource( "ImageCenter/Loading.bui" );
}

void CLoadingState::OnUpdate( float dt )
{
	ClientToGSOnHeart();
}

void CLoadingState::OnDraw()
{
}

void CLoadingState::OnDestroy()
{
}

//EOF

bool	CheckNeedUpdate()
{
	return false;
	/*
	char path[128];
	sprintf(path, "Map/Map21/0_0.wen");
	return !CCFileUtils::sharedFileUtils()->isFileExist(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(path).c_str());
	*/
}
