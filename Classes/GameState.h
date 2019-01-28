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
// 登录loginserver帐号验证结果返回码
enum Login_LS_Result
{
	Login_LS_Result_Succ		 = 0,				//	成功
	Login_LS_Result_Fail_UserNameOrPassword = -1,	//	用户名密码错误
	Login_LS_Result_Fail_OtherLogin = -2,			//	已有用户登录次帐号，请重新登录
	Login_LS_Result_Fail_Kickout	= -3,			//	被踢下线
	Login_LS_Result_Fail_Freeze		= -4,			//	该账户已经被冻结，禁止登录
	Login_LS_Result_Fail_Connect	= -5,			//	尚未连接登录服务器
	Login_LS_Result_Fail_Empty		= -6,			//	用户名密码不能为空
	Login_LS_Result_Fail_DbError	= -7,			//	验证失败，写数据库失败
	Login_LS_Result_Fail_Unknown	= -8,			//	验证失败，未知错误
	Login_LS_Result_Fail_Exception	= -9,			//	验证失败，异常错误
	Login_LS_Result_Fail_Unsupport	= -10,			//	验证失败，登录服务器不支持该平台验证
	Login_LS_Result_Fail_Timeout	= -11,			//	验证失败，验证超时
	Login_LS_Result_Fail_Disconnect	= -12,			//	验证失败，网络断开
	Login_LS_Result_Fail_Forbidden = -13,				//验证失败，帐号被封
	Login_LS_Result_Fail_Version = -14,            //  版本过低
	Login_LS_Result_Fail_Version_LevelUper = -15,            //  版本过高
};

//	登录GameServer结果返回
enum Login_GS_Result
{
	Login_GS_Result_Succ		= 0,				//	成功
	Login_GS_Result_Fail_Full	= -1,				//	服务器暴满
	Login_GS_Result_Fail_Check	= -2,				//	身份验证失败
	Login_GS_Result_Fail_Unvalid= -3,				//	服务器无效
	Login_GS_Result_Fail_Connect= -4,				//	链接失败
	Login_GS_Result_Fail_ProtVerErr= -5,				//	协议版本匹配错误
	Login_GS_Result_Fail_Closed= -6,				//	服务器维护中
};

//	角色创建结果
enum CreatePlayer_Result
{
	CreatePlayer_Result_Succ	= 0,				//	成功
	CreatePlayer_Result_Fail_Full= -1,				//	角色已满
	CreatePlayer_Result_Fail_Name_Unvalid = -2,		//	名字不合法
	CreatePlayer_Result_Fail_Name_Exist = -3,		//	重名
	CreatePlayer_Result_Fail_Name_Forbidden =-4     //	名字里有敏感字符
};
//选择角色结果
enum SelPlayer_Result
{
	SelPlayerResult_Succ	= 0,				//	成功
	SelPlayer_Result_PlayerCount_Fail= -1,		//	角色数量大于1个
	SelPlayer_Result_UserId_Fail = -2,		    //	账号下无此角色
	SelPlayer_Result_Player_IsOnline = -3,		//	角色在线
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



//玩家被T下线的原因
#define KickoutUser_Reson_ReLogin			0			//重复登录
#define KickoutUser_Reson_GM				1			//GM
#define KickoutUser_Reson_HeartBeat_Timeout	2	//心跳包超时

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