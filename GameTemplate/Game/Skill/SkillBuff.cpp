#include "stdafx.h"
#include "SkillBuff.h"

void SkillBuff::SkillSetting()
{
	//エフェクトの再生。
	skillEffect = NewGO<prefab::CEffect>(0);
	skillEffect->Play(L"Assets/effect/chant1.efk");
	CVector3 effectPos = GetEffectPos(en_PosUser);
	skillEffect->SetPosition(effectPos);
	//skillEffect->SetScale(CVector3::One() * 50.0f);

	//アニメーションの再生。
	m_user->AnimationMagic();
}

void SkillBuff::Update()
{
	if (m_target->IsDead()) {
		DeleteGO(this);
		return;
	}

	if (!skillEffect->IsPlay()) {
		if (m_effectPaths.size() > m_playEffectNum)
		{
			if (!m_isWide)
			{
				skillEffect = NewGO<prefab::CEffect>(0);
				skillEffect->Play(m_effectPaths[m_playEffectNum]);
				CVector3 efPos = GetEffectPos(m_effectPosFlag[m_playEffectNum]);
				skillEffect->SetPosition(efPos);
			}
			else
			{
				auto list = m_target->GetTeamMenber();

				for (int i = 0; i < list.size(); i++)
				{
					//メンバー全てをターゲットする。
					m_target = list[i];
					skillEffect = NewGO<prefab::CEffect>(0);
					skillEffect->Play(m_effectPaths[m_playEffectNum]);
					CVector3 efPos = GetEffectPos(m_effectPosFlag[m_playEffectNum]);
					skillEffect->SetPosition(efPos);
				}
			}

			m_playEffectNum++;
		}
		else
		{
			//効果値。
			int res = 0;
			//バフをかける。
			if (!m_isWide) {
				//効果時間を計算。
				int result = m_user->GetStatusManager().GetStatus().MAT * 5.0f;
				res = m_target->Monster_Buff(m_status, skillPower, result);
			}
			else {	//全体にかける。
				res = WideBuff();
			}

			//効果値を記録。
			m_user->SetDamageResult(res);
			//クールタイムの設定。
			m_user->SetCoolTime(coolTime);

			skillEffect = nullptr;
			DeleteGO(this);
		}
	}
}

int SkillBuff::WideBuff()
{
	//効果時間の計算。
	int result = m_user->GetStatusManager().GetStatus().MAT * 5.0f;
	//チームメンバーを取得。
	auto list = m_target->GetTeamMenber();

	int res = 0;
	for (int i = 0; i < list.size(); i++) {
		//全体にバフをかける。
		res += list[i]->Monster_Buff(m_status, skillPower, result);
	}
	res /= list.size();		//上昇値の平均をとる。

	return res;
}
