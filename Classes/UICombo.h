#ifndef _ComboUI_H__
#define _ComboUI_H__

#include "../BaseModule/SFX/ComboSFX.h"
#include "effect.h"
#include "../game/SceneInstance.h"

enum
{
	TYPE_HIT,
	TYPE_KILL,
	TYPE_NUM
};

enum
{
	LAYER1 = 0,
	LAYER2 = 1,
	LAYERNUM
};

class UICombo
{
public:
	static UICombo* getInstance();

	void pushHit();

	void pushKill();

	void Process();

	void update(float dt);

	void Uninit();

	void init();

	void setKillTimes(int num);

private:

	UICombo(void);

	~UICombo(void);

	

	void AddBackground(float x, float y, int type);

	float m_MaxDrpCobTime;

	float m_MaxDrpKilTime;

	ComboSFX* m_pCurCobSFX;

	ComboSFX* m_pCurCobTXT;

	ComboSFX* m_pCurKilSFX;

	ComboSFX* m_pCurKilTXT;

	UI*		  m_pProcess[TYPE_NUM][LAYERNUM];

	int m_KillTimes;

	int m_ComboTimes;

};

#endif