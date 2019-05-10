//=============================================================================
//
// スコア処理 [score.h]
// Author : 竹内亘
//
//=============================================================================
#include "score.h"
#include "number.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================

//=============================================================================
// コンストラクタ
//=============================================================================
CScore::CScore()
{
	m_nScore = 0;
}
//=============================================================================
// デストラクタ
//=============================================================================
CScore::~CScore()
{
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScore::Init(D3DXVECTOR3 pos)
{
	for (int nCount = 0; nCount < MAX_SCORE; nCount++)
	{
		m_apNumber[nCount] = new CNumber;

		m_apNumber[nCount]->Init(D3DXVECTOR3(pos.x - (SCORE_INTERVAL_SIZE * nCount), pos.y, pos.z), SCORE_SIZE, SCORE_SIZE);
	}

	m_nScore = 0;

	return S_OK;
}
//=============================================================================
// 初期化処理(合計スコア用)
//=============================================================================
HRESULT CScore::InitTotal(D3DXVECTOR3 pos)
{
	for (int nCount = 0; nCount < MAX_SCORE; nCount++)
	{
		m_apNumber[nCount] = new CNumber;

		m_apNumber[nCount]->Init(D3DXVECTOR3(pos.x - (90 * nCount), pos.y, pos.z), 50, 50);
	}

	m_nScore = 0;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScore::Uninit(void)
{
	for (int nCount = 0; nCount < MAX_SCORE; nCount++)
	{
		m_apNumber[nCount]->Uninit();

		// メモリの開放
		delete m_apNumber[nCount];

		// NULLを入れる
		m_apNumber[nCount] = {};
	}

	// オブジェクト(自分自身)の破棄
	Release();
}
//=============================================================================
// 更新処理
//=============================================================================
void CScore::Update(void)
{
	for (int nCount = 0; nCount < MAX_SCORE; nCount++)
	{
		m_apNumber[nCount]->Update();
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void CScore::Draw(void)
{
	for (int nCount = 0; nCount < MAX_SCORE; nCount++)
	{
		m_apNumber[nCount]->Draw();
	}
}
//=============================================================================
// スコアの取得
//=============================================================================
int CScore::GetScore(void)
{
	return m_nScore;
}
//=============================================================================
// スコアの加算
//=============================================================================
void CScore::AddScore(int nValue)
{
	m_nScore += nValue;

	int nAnswer = 1;
	int nScore;

	for (int nCount = 0; nCount < MAX_SCORE; nCount++)
	{
		nScore = m_nScore % (nAnswer * SCORE_DEGIT) / nAnswer;
		m_apNumber[nCount]->SetNumber(nScore);
		nAnswer *= SCORE_DEGIT;
	}
}
//=============================================================================
// 色設定
//=============================================================================
void CScore::SetColor(D3DXCOLOR col)
{
	for (int nCount = 0; nCount < MAX_SCORE; nCount++)
	{
		m_apNumber[nCount]->SetColor(col);
	}
}
//=============================================================================
// スコアの生成
//=============================================================================
CScore * CScore::Create(D3DXVECTOR3 pos)
{
	CScore *pScore = NULL;

	if (pScore == NULL)
	{
		pScore = new CScore;

		if (pScore != NULL)
		{
			pScore->Init(pos);
		}
	}
	return pScore;
}
//=============================================================================
// スコアの生成(合計)
//=============================================================================
CScore * CScore::CreateTotal(D3DXVECTOR3 pos)
{
	CScore *pScore = NULL;

	if (pScore == NULL)
	{
		pScore = new CScore;

		if (pScore != NULL)
		{
			pScore->InitTotal(pos);
		}
	}
	return pScore;
}
