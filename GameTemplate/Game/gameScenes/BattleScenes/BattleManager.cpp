#include "stdafx.h"
#include "BattleManager.h"
#include "BattleScenes.h"
#include "monster/MonsterBase.h"

BattleManager::BattleManager()
{
	m_monsterTeam.resize(job::enjob_Num);
	m_monsterEnemy.resize(job::enjob_Num);
}

BattleManager::~BattleManager()
{
	for (auto p : m_monsterTeam) {
		DeleteGO(p);
	}
	for (auto p : m_monsterEnemy) {
		DeleteGO(p);
	}
	//シャドウキャスター登録をクリア。
	g_graphicsEngine->GetShadowMap()->CasterClear();
}

bool BattleManager::Start()
{
	SetTeams();

	return true;
}

void BattleManager::Update()
{
	if (!m_isBattle) return;
	if (m_battleCoolTime > 0.0f)
	{
		//クールタイムを減算。
		m_battleCoolTime -= g_gameTime.GetFrameDeltaTime();
		return;
	}
	switch (m_battleState)
	{
	case enState_ATB:
		ActiveTimeUpdate();
		break;
	case enState_ACT:
		MonsterAction();
		break;
	case enState_Scoring:
		MonsterScoring();
		break;
	}
}

void BattleManager::SetTeams()
{
	for (auto p : m_monsterTeam) {
		p->SetTeamMenber(m_monsterTeam);
	}
	for (auto p : m_monsterEnemy) {
		p->SetTeamMenber(m_monsterEnemy);
	}
}

void BattleManager::ActiveTimeUpdate()
{
	//行動中なら中断。
	if (m_monsterACTList.size() != 0)return;

	//全員のタイムゲージを加算していく。
	for (int i = 0; i < m_monsterTeam.size(); i++)
	{
		bool is_action = m_monsterTeam[i]->AddATB();

		//ゲージが溜まり切ったらポインタを取得する。　
		if (is_action) {
			m_monsterACTList.push_back({ m_monsterTeam[i], false });
			//ステートを変更。
			m_battleState = enState_ACT;
		}
	}
	for (int i = 0; i < m_monsterEnemy.size(); i++)
	{
		bool is_action = m_monsterEnemy[i]->AddATB();

		//ゲージが溜まり切ったらポインタを取得する。　
		if (is_action) {
			m_monsterACTList.push_back({ m_monsterEnemy[i], true });
			//ステートを変更。
			m_battleState = enState_ACT;
		}
	}
}

void BattleManager::MonsterAction()
{
	if (m_usingSkill == nullptr) {
		m_monsterACT = m_monsterACTList.front();

		if (m_monsterACT.actMonster->IsDeath())
		{
			m_monsterACTList.erase(m_monsterACTList.begin());
			m_monsterACT = { nullptr,false };		//初期化。

			//誰も行動していないなら
			if (m_monsterACTList.size() == 0) {
				m_battleState = enState_ATB;
			}
			else {
				m_battleState = enState_ACT;
			}

			return;
		}

		//SortTeams();

		int skill, target;
		//行動の決定。
		if (m_monsterACT.isEnemy) {		//敵の行動選択。
			m_monsterACT.actMonster->SelectUseSkill(m_monsterTeam,m_monsterEnemy, skill,target);
		}
		else {							//味方の行動選択。
			m_monsterACT.actMonster->SelectUseSkill(m_monsterEnemy,m_monsterTeam, skill, target);
		}
		//スキルの取得。
		int skillTable = (int)(skill / 100);
		int No = skill % 100;
		m_usingSkill = SkillList::GetInstance()->NewSkillData(skillTable, No);
		//使用者、ターゲットの設定。
		m_usingSkill->SetUser(m_monsterACT.actMonster);
		m_usingSkill->SetUseEnemy(m_monsterACT.isEnemy);		//使用者が敵かどうかを設定。
		if (m_monsterACT.isEnemy) {
			if (m_usingSkill->IsAttack()) {
				m_usingSkill->SetTarget(m_monsterTeam[target]);
			}
			else {
				m_usingSkill->SetTarget(m_monsterEnemy[target]);
			}
		}
		else {
			if (m_usingSkill->IsAttack()) {
				m_usingSkill->SetTarget(m_monsterEnemy[target]);
			}
			else {
				m_usingSkill->SetTarget(m_monsterTeam[target]);
			}
		}
	}
	//行動が終わったらステート切り替え。
	else if (!m_usingSkill->GetIsPlay()) {
		//全員のタイムゲージを加算していく。
		for (int i = 0; i < m_monsterTeam.size(); i++)
		{
			m_monsterTeam[i]->StateUpdate();
		}
		for (int i = 0; i < m_monsterEnemy.size(); i++)
		{
			m_monsterEnemy[i]->StateUpdate();
		}
		m_battleState = enState_Scoring;
		m_usingSkill = nullptr;
	}
}

void BattleManager::MonsterScoring()
{
	bool flag = m_monsterACT.actMonster->ACTScoring();

	if (flag) {
		//行動が終わったらポインタにnullを入れる。
		m_monsterACTList.erase(m_monsterACTList.begin());
		m_monsterACT = { nullptr,false };		//初期化。


		int myDeath = 0;
		int eneDeath = 0;

		//デス数をカウントする。
		for (int i = 0; i < m_monsterTeam.size(); i++) {
			if (m_monsterTeam[i]->IsDeath())myDeath++;
		}
		for (int i = 0; i < m_monsterEnemy.size(); i++) {
			if (m_monsterEnemy[i]->IsDeath())eneDeath++;
		}
		//どちらかが全滅すればリザルトへ
		if (myDeath == m_monsterTeam.size()) {
			//バトルシーンに対してステートの変化と勝敗判定を渡す
			m_scenes->SetBattleResult(false);
			SetIsBattle(false);
			return;
		}
		else if (eneDeath == m_monsterEnemy.size()) {
			m_scenes->SetBattleResult(true);
			SetIsBattle(false);
			return;
		}



		//誰も行動していないなら
		if (m_monsterACTList.size() == 0) {
			m_battleState = enState_ATB;
		}
		else {
			m_battleState = enState_ACT;
			//行動前に一定時間止めておく。
			m_battleCoolTime = COOL_TIME;
		}

	}
}
