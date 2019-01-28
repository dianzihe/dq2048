#pragma once

#include <string>
#include "cocos2d.h"

using namespace cocos2d;

enum NETDEALAYTIMES
{
	TIME_GET_FIR = 0,
	TIME_GET_SEC = 1,
	TIME_GET_THR = 2,


	TIME_GET_COUNT,
};
struct NetWork_State 
{
	int times;
	int lastSendTime;
	int dealyTime[TIME_GET_COUNT];
	int aveTime;
};
// ��¼loginserver�ʺ���֤���������
enum Login_LS_Result
{
	Login_LS_Result_Succ		 = 0,				//	�ɹ�
	Login_LS_Result_Fail_UserNameOrPassword = -1,	//	�û����������
	Login_LS_Result_Fail_OtherLogin = -2,			//	�����û���¼���ʺţ������µ�¼
	Login_LS_Result_Fail_Kickout	= -3,			//	��������
	Login_LS_Result_Fail_Freeze		= -4,			//	���˻��Ѿ������ᣬ��ֹ��¼
	Login_LS_Result_Fail_Connect	= -5,			//	��δ���ӵ�¼������
	Login_LS_Result_Fail_Empty		= -6,			//	�û������벻��Ϊ��
	Login_LS_Result_Fail_DbError	= -7,			//	��֤ʧ�ܣ�д���ݿ�ʧ��
	Login_LS_Result_Fail_Unknown	= -8,			//	��֤ʧ�ܣ�δ֪����
	Login_LS_Result_Fail_Exception	= -9,			//	��֤ʧ�ܣ��쳣����
	Login_LS_Result_Fail_Unsupport	= -10,			//	��֤ʧ�ܣ���¼��������֧�ָ�ƽ̨��֤
	Login_LS_Result_Fail_Timeout	= -11,			//	��֤ʧ�ܣ���֤��ʱ
	Login_LS_Result_Fail_Disconnect	= -12,			//	��֤ʧ�ܣ�����Ͽ�
	Login_LS_Result_Fail_Forbidden = -13,				//��֤ʧ�ܣ��ʺű���
	Login_LS_Result_Fail_Version = -14,            //  �汾����
	Login_LS_Result_Fail_Version_LevelUper = -15,            //  �汾����
};

//	��¼GameServer�������
enum Login_GS_Result
{
	Login_GS_Result_Succ		= 0,				//	�ɹ�
	Login_GS_Result_Fail_Full	= -1,				//	����������
	Login_GS_Result_Fail_Check	= -2,				//	�����֤ʧ��
	Login_GS_Result_Fail_Unvalid= -3,				//	��������Ч
	Login_GS_Result_Fail_Connect= -4,				//	����ʧ��
	Login_GS_Result_Fail_ProtVerErr= -5,				//	Э��汾ƥ�����
	Login_GS_Result_Fail_Closed= -6,				//	������ά����
};

//	��ɫ�������
enum CreatePlayer_Result
{
	CreatePlayer_Result_Succ	= 0,				//	�ɹ�
	CreatePlayer_Result_Fail_Full= -1,				//	��ɫ����
	CreatePlayer_Result_Fail_Name_Unvalid = -2,		//	���ֲ��Ϸ�
	CreatePlayer_Result_Fail_Name_Exist = -3,		//	����
	CreatePlayer_Result_Fail_Name_Forbidden =-4     //	�������������ַ�
};
//ѡ���ɫ���
enum SelPlayer_Result
{
	SelPlayerResult_Succ	= 0,				//	�ɹ�
	SelPlayer_Result_PlayerCount_Fail= -1,		//	��ɫ��������1��
	SelPlayer_Result_UserId_Fail = -2,		    //	�˺����޴˽�ɫ
	SelPlayer_Result_Player_IsOnline = -3,		//	��ɫ����
};

enum T91LoginResult
{
    E91LoginResult_NormalAcountLogined,   // normal account logined.
    E91LoginResult_GuestLogined,          // guest login
    E91LoginResult_GuestRegistered,       // guest registered as normal account.
    E91LoginResult_LoginCanceled,
    E91LoginResult_AppIdOrKeyInvalid,
    E91LoginResult_AppIdInvalid,
    E91LoginResult_GuestAcountHasASSOCIATE_91,
    E91LoginResult_UnknowError,
};

enum UCLoginResult
{
    UCLoginResult_Logined = 0,
    UCLoginResult_LoginCanceld,
    UCLoginResult_LoginFaild,
    UCLoginResult_UnknowError,
};



//��ұ�T���ߵ�ԭ��
#define KickoutUser_Reson_ReLogin			0			//�ظ���¼
#define KickoutUser_Reson_GM				1			//GM
#define KickoutUser_Reson_HeartBeat_Timeout	2	//��������ʱ

class GameScene;
class UIEditNew;
class CRunState : public Node
{
public:
	virtual ~CRunState(){}
	virtual void EnterState() {}
	virtual void OnUpdate(float dt) {}
	virtual void OnDraw() {}
	virtual void OnDestroy() {}
};

namespace pk {
	struct PlayerBaseInfo;
}

class UI;
class CLoadingState : public CRunState
{
public:
	CLoadingState();
	virtual ~CLoadingState();

	virtual void EnterState();
	virtual void OnUpdate(float dt);
	virtual void OnDraw();
	virtual void OnDestroy();
	// 0 : from login menu to game
	// 1 : change map loading
	static int getLoadType() { return mType; }
	static void setLoadType( int type ) { mType = type; }
	void exit() { mExit = true; }
	
private:
	static int			mType;
	UI*					mLoadingUI;
	std::string			mImageName;
	unsigned int		mFrameCount;
	bool				mExit;
};


class CGameMainUI;
//class CRolePropertyUI;
class CMissionUI;
class CNPCMissionUI;
class CGameState : public CRunState
{
public:
	CGameState();
	~CGameState();

	virtual void EnterState();
	virtual void OnUpdate(float dt);
	virtual void OnDraw();
	virtual void OnDestroy();
	bool isLoading() const { return mLoadingFrame > 0; }
	void showInGameLoading();
	void beginLoading();
	void endLoading();
	void decideIsToreloading();
	void getNetInfo();
	void showNetStateBtn(int dealyTime);
private:
	int					mLoadingFrame;
	UI*					mLoadingUI;
	std::string			mImageName;
public:
	long mBeKickOutedTime;
	NetWork_State mNetworkstate;
	bool mIsEnterGameFirTime;
	bool mIsFirstTimeLoading;
};