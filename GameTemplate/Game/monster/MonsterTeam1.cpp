#include "stdafx.h"
#include "MonsterTeam1.h"


MonsterTeam1::MonsterTeam1()
{
}


MonsterTeam1::~MonsterTeam1()
{
	DeleteGO(m_UI);
}

bool MonsterTeam1::Start()
{
	//cmoファイルの読み込み。
	m_model.Init(L"Assets/modelData/unityChan.cmo", enFbxUpAxisY);
	m_model.SetActiveDLFlag(0);
	m_model.SetActiveRLFlag(0);

	//アニメーションクリップの読み込み。
	m_animClip[en_anim_Idle].Load(L"Assets/animData/idle.tka");
	m_animClip[en_anim_Idle].SetLoopFlag(true);
	m_animClip[en_anim_Attack].Load(L"Assets/animData/run.tka");
	m_animClip[en_anim_Attack].SetLoopFlag(true);
	m_animClip[en_anim_Magic].Load(L"Assets/animData/walk.tka");
	m_animClip[en_anim_Magic].SetLoopFlag(true);
	m_animClip[en_anim_Damage].Load(L"Assets/animData/damage.tka");
	m_animClip[en_anim_Damage].SetLoopFlag(false);
	m_animClip[en_anim_Death].Load(L"Assets/animData/KneelDown.tka");
	m_animClip[en_anim_Death].SetLoopFlag(false);	


	//アニメーションの設定。
	m_animation.Init(
		m_model,
		m_animClip,
		en_anim_num
	);

	m_animation.Play(en_anim_Idle, 0.3f);

	//UIを作る。
	m_UI = NewGO<StatusUI>(0);
	m_UI->SetPosition(m_UIpos);
	m_UI->EnableEnemy();
	m_status.SetUI(m_UI);

	return true;
}

void MonsterTeam1::Update()
{
	//if (m_stateAI == en_state_Death) return;	//死亡時は更新しない。

	//m_position.y = m_activeTime;
	switch (m_status.GetState())
	{
	case en_state_Death:
		if (!m_status.IsDeath()) {
			m_status.SetDeath(true);
			m_animation.Play(en_anim_Death, 0.3f);
		}

		break;
	default:
		//アニメーションされていないなら。
		if (!m_animation.IsPlaying()) {
			m_status.SetDeath(false);
			m_animation.Play(en_anim_Idle, 0.3f);
		}

		break;
	}
	//描画処理。
	Draw();
	//アニメーションの更新処理。
	m_animation.Update(1.0f / 30.0f);
}

