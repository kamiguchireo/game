#pragma once
#include "SkillBase.h"

class SkillHeal : public SkillBase
{
public:
	void SkillSetting();
	void Update();

	bool IsMagic()override
	{
		return true;
	}
	bool IsAttack()override
	{
		return false;
	}

private:
	int WideHeal();
	//prefab::CEffect* m_healEffect = nullptr;

};

