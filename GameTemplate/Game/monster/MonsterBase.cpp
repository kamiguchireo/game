#include "stdafx.h"
#include "MonsterBase.h"
#include "SkillList.h"


MonsterBase::MonsterBase()
{
}


MonsterBase::~MonsterBase()
{
}

void MonsterBase::Action()
{
	//残りHPに応じて行動を決める。
	switch (m_stateAI)
	{
	case en_state_Good:
		break;
	
	case en_state_Usually:
		break;
	
	case en_state_Bad:
		break;
	
	case en_state_Death:
		break;
	}
}
