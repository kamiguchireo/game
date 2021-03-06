#pragma once
#include "monster/MonsterData.h"

class EvaluationCalculator;
class Fade;
class GASceneUI;

namespace GA {
	enum Job
	{
		en_Attacker,
		en_Healer,
		en_Supporter,
		en_JobNum
	};

	typedef std::vector<AIData> AITable;		//AI情報。
	typedef std::vector<AITable> AITableList;	//AI情報テーブル。
	struct Evaluation	//評価値構造体
	{
		int winRate = 0;			//勝率
		int resultValue = 0;		//行動の評価値
	};
	struct Genetic
	{
		AITableList genetic;		//遺伝子となるAI情報テーブル。
		Evaluation result;			//評価値。
	};
	typedef std::vector<Genetic> GeneticsList;//AI情報テーブルで構成される遺伝子情報。さすがにこれは深すぎるかなぁ？
}

class GAScenes:public IGameObject
{
public:
	GAScenes();
	~GAScenes();
	bool Start()override;
	void Update()override;
private:
	//データを読み込む。
	void LoadAIData(const char* filePath, GA::AITable& ai);
	//完成したAIデータを記録する。
	void SaveAIData(const char* filePath, GA::AITable& ai);
	//初期遺伝子を作成する。
	void FirstGenesCreate();
	//勝率を測定。
	void CalcWinRate();
	//評価値でソートを行う。
	void SortGenes();
	//淘汰の処理。
	void GeneSelection(); 
	//交叉の処理。
	void GenesCrossover();
	//確率を交叉させる。。
	void GeneSwap(GA::AITable& _t1, GA::AITable& _t2);
	//突然変異の処理。
	void Mutation();
	//AIの確率を変動させる
	void GeneMutation(GA::AITable & _table);

private:		//定数。
	static const float RATE_CHANGE;		//初期遺伝子生成時の確率変動幅
	static const int CHANGE_NUM;		//確率変動で作る数。
	const int SELECT_ELETE = 15;		//選択するエリートの数。
	const int MAX_GENERATION = 50;		//繰り返す世代数。
	const float MUTATION_RATE = 0.05f;	//突然変異の確率。

	enum Scene {
		en_FadeIn,
		en_FirstGeneric,
		en_GA,
		en_Calc,
		en_End,
		en_FadeOut
	};

private:		//メンバ変数。
	Fade* m_fade = nullptr;		//フェードクラス。
	GASceneUI* m_ui = nullptr;	//UI管理クラス。
	Scene m_sceneState = en_FadeIn;

	EvaluationCalculator* m_evaluationCalc = nullptr;
	GA::AITable m_myAI[GA::en_JobNum];		//元となるAIのデータ。
	GA::AITable m_enemyAI[GA::en_JobNum];	//敵のAIデータ。
	GA::GeneticsList m_currentGenetics;	//現行世代の遺伝子。
	GA::GeneticsList m_eliteGenetics;		//エリート遺伝子のリスト。
	prefab::CSoundSource* m_bgm = nullptr;	//サウンドソース。
	float m_bgmVol = 0.0f;					//bgmのボリューム。
	const float SOUND_VOL = 0.6f;			//全体的なボリューム。
	int m_geneSize = 0;		//遺伝子のサイズ。
	int m_currentGenerationNum = 0;		//世代数。
	int m_maxWinRate = 0;		//最高遺伝子の勝率(%)。
	float m_aveWinRate = 0.0f;	//現行世代の平均勝率(%)。
	int m_currentCalc = 0;		//現在評価中の遺伝子。
	int m_currentCalcSize = 0;	//現在評価中の世代のサイズ。
	bool m_isError = false;		//読み込みエラーが発生したらtrue。
};

