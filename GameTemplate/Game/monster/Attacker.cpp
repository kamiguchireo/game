#include "stdafx.h"
#include "Attacker.h"
#include "Skill/SkillList.h"

Attacker::Attacker()
{
}

Attacker::~Attacker()
{
	DeleteGO(m_UI);
}

bool Attacker::Start()
{
	m_model.Init(L"Assets/modelData/testModel.cmo");
	m_model.SetActiveDLFlag(0);
	m_model.SetActiveRLFlag(0);
	
	//アニメーションクリップの読み込み。
	m_animClip[en_anim_Idle].Load(L"Assets/animData/TestIdle.tka");
	m_animClip[en_anim_Idle].SetLoopFlag(true);
	m_animClip[en_anim_Attack].Load(L"Assets/animData/TestAttack.tka");
	m_animClip[en_anim_Attack].SetLoopFlag(false);
	m_animClip[en_anim_Magic].Load(L"Assets/animData/TestMagic.tka");
	m_animClip[en_anim_Magic].SetLoopFlag(false);
	m_animClip[en_anim_Damage].Load(L"Assets/animData/TestDamage.tka");
	m_animClip[en_anim_Damage].SetLoopFlag(false);
	m_animClip[en_anim_Death].Load(L"Assets/animData/TestDeath.tka");
	m_animClip[en_anim_Death].SetLoopFlag(false);

	//アニメーションの設定。
	m_animation.Init(
		m_model,
		m_animClip,
		en_anim_num
	);

	//UIを作る。
	m_UI = NewGO<StatusUI>(0);
	m_UI->SetPosition({ -50.0f,-250.0f,0.0f });

	return true;
}

void Attacker::Update()
{
	switch (m_stateAI)
	{
	case en_state_Death:
		if (!IsDeath) {
			IsDeath = true;
			m_animation.Play(en_anim_Death, 0.3f);
		}

		break;
	default:
		//アニメーションされていないなら。
		if (!m_animation.IsPlaying()) {
			m_animation.Play(en_anim_Idle, 0.3f);
		}

		break;
	}
	//描画処理。
	Draw();
	//アニメーションの更新処理。
	m_animation.Update(1.0f / 30.0f);
}

bool Attacker::BattleAction()
{
	MonsterBase* attack = this;

	bool flag = m_useSkill->UseSkill(attack, m_target);

	if (!m_useSkill->GetIsMagic()) {
		m_animation.Play(en_anim_Attack, 0.3f);
	}
	else if (m_useSkill->GetIsMagic()) {
		m_animation.Play(en_anim_Magic, 0.3f);
	}

	if (m_useSkill != nullptr && flag)
	{
		m_useSkill = nullptr;
		m_target = nullptr;
	}

	return flag;
}

void Attacker::SelectUseSkill(const std::vector<MonsterBase*>& e_team, const std::vector<MonsterBase*>& m_team)
{
	SkillList* skillList = SkillList::GetInstance();

	auto list = e_team;	//ソートするためにリストをコピー。

	for (int i = 0; i < list.size(); i++) {
		for (int j = i; j < list.size(); j++) {
			if (list[i]->GetStatus().HP > list[j]->GetStatus().HP) {
				auto hoge = list[i];
				list[i] = list[j];
				list[j] = hoge;
			}
		}
	}

	int res = rand() % 100;	//適当な乱数。
	int sum = 0;


	int AINum = sizeof(m_AI) / sizeof(*m_AI);
	for (int i = 0; i < AINum; i++) {
		sum += (int)(m_AI[i].rate * 100);
		if (sum > res) {
			int skillTable = (int)(m_AI[i].skillNo / 100);
			int skillNo = m_AI[i].skillNo % 100;
			int targetNo = m_AI[i].target;
			m_useSkill = skillList->GetSkillData(skillTable, skillNo);
			m_target = list[targetNo];
			break;
		}
	}
}

void Attacker::Init(const char* filePath)
{
	FILE* fp = fopen(filePath, "rb");
	if (fp == nullptr) {
		//ファイルが存在しないならデフォルト。
		m_AI[0] = { 0,0,0.25f };
		m_AI[1] = { 0,1,0.1f };
		m_AI[2] = { 0,2,0.15f };
		m_AI[3] = { 1,0,0.25f };
		m_AI[4] = { 1,1,0.1f };
		m_AI[5] = { 1,2,0.15f };

		fclose(fp);
		return;
	}
	fread(m_AI, sizeof(m_AI), 1, fp);

	fclose(fp);
}

void Attacker::Save(const char * filePath)
{
	FILE* fp = fopen(filePath, "wb");

	if (fp == nullptr) {
		fclose(fp);
		return;
	}

	fwrite(m_AI, sizeof(m_AI), 1, fp);

	fclose(fp);
}
