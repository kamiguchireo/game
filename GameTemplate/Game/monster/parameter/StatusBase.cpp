#include "stdafx.h"
#include "StatusBase.h"

namespace JOB_Status
{
	const Status ATTAKER =
	{
		130,		//HP
		20,			//MP
		50,			//ATK
		10,			//DEF
		5,			//HEAL
		5,			//BUF
		12			//DEX
	};
	const Status HEALER =
	{
		130,		//HP
		50,			//MP
		5,			//ATK
		10,			//DEF
		40,			//HEAL
		10,			//BUF
		10			//DEX
	};
	const Status SUPPOTER =
	{
		130,		//HP
		70,			//MP
		5,			//ATK
		15,			//DEF
		10,			//HEAL
		50,			//BUF
		15			//DEX
	};
}

void CStatusBase::StatusAttaker()
{
	SetStatus(JOB_Status::ATTAKER);
}

void CStatusBase::StatusHealer()
{
	SetStatus(JOB_Status::HEALER);
}

void CStatusBase::StatusSupporter()
{
	SetStatus(JOB_Status::SUPPOTER);
}

int CStatusBase::Healing(int healing)
{
	//死亡時返す。
	if (m_IsDeath) return 0;	
	int res = healing;
	m_status.HP += healing;
	//最大HPを超えた。
	if (m_status.HP > m_statusBase.HP) {
		int diff = m_status.HP - m_statusBase.HP;
		res -= diff;		//超えた分だけ引く。

		m_status.HP = min(m_status.HP, m_statusBase.HP);
	}

	return res;
}

bool CStatusBase::UseMP(const int mp)
{
	if (m_status.MP >= mp)
	{
		m_status.MP -= mp;
		return true;
	}
	return false;
}

int CStatusBase::Monster_Buff(StatusBuff status, float pow, float time)
{
	//死亡時返す。
	if (m_IsDeath) return 0;

	//倍率から1を引いておく。
	pow -= 1.0f;

	//バフをかける。
	switch (status)
	{
	case en_buff_ATK:
		m_buffValues[status] = static_cast<int>(m_statusBase.ATK * pow);	//上昇値の計算。

		break;
	case en_buff_DEF:
		m_buffValues[status] = static_cast<int>(m_statusBase.DEF * pow);	//上昇値の計算。

		break;
	case en_buff_MAT:
		m_buffValues[status] = static_cast<int>(m_statusBase.HEAL * pow);	//上昇値の計算。

		break;
	case en_buff_MDF:
		m_buffValues[status] = static_cast<int>(m_statusBase.BUF * pow);	//上昇値の計算。

		break;
	case en_buff_DEX:
		m_buffValues[status] = static_cast<int>(m_statusBase.DEX * pow);	//上昇値の計算。

		break;
	}
	//バフの効果時間を設定。
	m_buffTimeList[status] = time;
	//バフの効果を反映。
	SumBufAndDebuff(status);

	//効果時間を整数型に
	int res = static_cast<int>(time);

	return res;
}

int CStatusBase::Monster_Debuff(StatusBuff status, float pow, float time)
{
	//死亡時返す。
	if (m_IsDeath) return 0;

	//倍率から1を引いておく。
	pow -= 1.0f;

	//バフをかける。
	switch (status)
	{
	case en_buff_ATK:
		m_debuffValues[status] = static_cast<int>(m_statusBase.ATK * pow);//低下値の計算。

		break;
	case en_buff_DEF:
		m_debuffValues[status] = static_cast<int>(m_statusBase.DEF * pow);//低下値の計算。

		break;
	case en_buff_MAT:
		m_debuffValues[status] = static_cast<int>(m_statusBase.HEAL * pow);//低下値の計算。

		break;
	case en_buff_MDF:
		m_debuffValues[status] = static_cast<int>(m_statusBase.BUF * pow);//低下値の計算。

		break;
	case en_buff_DEX:
		m_debuffValues[status] = static_cast<int>(m_statusBase.DEX * pow);//低下値の計算。

		break;
	}
	//デバフの効果時間を設定。
	m_debuffTimeList[status] = time;
	//デバフの効果を反映。
	SumBufAndDebuff(status);

	//効果時間を整数型に
	int res = static_cast<int>(time);

	return res;
}

void CStatusBase::SumBufAndDebuff(int status)
{
	//バフとデバフを反映したステータスを出す。
	switch (status)
	{
	case en_buff_ATK:
		m_status.ATK = m_statusBase.ATK + m_buffValues[status] + m_debuffValues[status];
		break;
	case en_buff_DEF:
		m_status.DEF = m_statusBase.DEF + m_buffValues[status] + m_debuffValues[status];
		break;
	case en_buff_MAT:
		m_status.HEAL = m_statusBase.HEAL + m_buffValues[status] + m_debuffValues[status];
		break;
	case en_buff_MDF:
		m_status.BUF = m_statusBase.BUF + m_buffValues[status] + m_debuffValues[status];
		break;
	case en_buff_DEX:
		m_status.DEX = m_statusBase.DEX + m_buffValues[status] + m_debuffValues[status];
		break;
	}
}

void CStatusBase::ResetBuff(int i)
{
	//バフによる上昇値を0にする
	m_buffValues[i] = 0;
	//バフの効果時間を0にする。
	m_buffTimeList[i] = 0.0f;

	//ステータスを元のステータスに戻す。(デバフがあるならそれも反映)
	SumBufAndDebuff(i);
}

void CStatusBase::ResetDebuff(int i)
{
	//デバフによる低下値を0にする
	m_debuffValues[i] = 0;
	//デバフの効果時間を0にする。
	m_debuffTimeList[i] = 0;

	//ステータスの低下を元に戻す。
	SumBufAndDebuff(i);
}

void CStatusBase::BuffDebuffTime(const float time)
{
	for (int i = 0; i < en_buff_num; i++) {
		if (m_buffTimeList[i] == 0.0f) continue;

		m_buffTimeList[i] -= time;
		if (m_buffTimeList[i] < 0.0f) {
			ResetBuff(i);
		}
	}
	//デバフの効果時間減少。
	for (int i = 0; i < en_buff_num; i++) {
		if (m_debuffTimeList[i] == 0.0f) continue;

		m_debuffTimeList[i] -= time;
		if (m_debuffTimeList[i] < 0.0f) {
			ResetDebuff(i);
		}
	}
}
