#pragma once
#include "AIBase.h"

/// <summary>
/// GAを回すための簡易AI管理クラス。
/// </summary>

class AISimpleManager : public AIBase
{
public:
	AISimpleManager();
	~AISimpleManager();

	void SetAIData(std::vector<AIData>& copy);

private:

};

