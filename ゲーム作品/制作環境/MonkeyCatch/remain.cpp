//=============================================================================
//
// 残機処理 [remain.h]
// Author : 竹内亘
//
//=============================================================================
#include "remain.h"
#include "number.h"
#include "renderer.h"
#include "manager.h"
#include "scene2D.h"
#include "player.h"
#include "enemy.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================

//=============================================================================
// コンストラクタ
//=============================================================================
CRemain::CRemain()
{
	m_nRemain = 0;
}
//=============================================================================
// デストラクタ
//=============================================================================
CRemain::~CRemain()
{
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRemain::Init(D3DXVECTOR3 pos)
{
	m_nRemain = 0;

	// モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{// モードがゲーム
		for (int nCount = 0; nCount < MAX_REMAIN; nCount++)
		{
			m_apNumber[nCount] = new CNumber;

			m_apNumber[nCount]->Init(D3DXVECTOR3(pos.x - (REMAIN_INTERVAL_SIZE * nCount), pos.y, pos.z), REMAIN_SIZE, REMAIN_SIZE);
		}

		// 敵の総数取得
		m_NumEnemy = CEnemy::GetNumEnemy();
		AddRemain(m_NumEnemy);
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (mode == CManager::MODE_RESULT)
	{// モードがリザルト
		for (int nCount = 0; nCount < MAX_REMAIN; nCount++)
		{
			m_apNumber[nCount] = new CNumber;

			m_apNumber[nCount]->Init(D3DXVECTOR3(pos.x - (R_REMAIN_INTERVAL_SIZE * nCount), pos.y, pos.z), R_REMAIN_SIZE, R_REMAIN_SIZE);
		}
	}

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CRemain::Uninit(void)
{
	for (int nCount = 0; nCount < MAX_REMAIN; nCount++)
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
void CRemain::Update(void)
{
	for (int nCount = 0; nCount < MAX_REMAIN; nCount++)
	{
		m_apNumber[nCount]->Update();
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void CRemain::Draw(void)
{
	for (int nCount = 0; nCount < MAX_REMAIN; nCount++)
	{
		m_apNumber[nCount]->Draw();
	}
}
//=============================================================================
// 残機の加算
//=============================================================================
void CRemain::AddRemain(int nValue)
{
	m_nRemain += nValue;

	int nAnswer = 1;
	int nRemain;

	for (int nCount = 0; nCount < MAX_REMAIN; nCount++)
	{
		nRemain = m_nRemain % (nAnswer * REMAIN_DEGIT) / nAnswer;
		m_apNumber[nCount]->SetNumber(nRemain);
		nAnswer *= REMAIN_DEGIT;
	}
}
//=============================================================================
// 色設定
//=============================================================================
void CRemain::SetColor(D3DXCOLOR col)
{
	for (int nCount = 0; nCount < MAX_REMAIN; nCount++)
	{
		m_apNumber[nCount]->SetColor(col);
	}
}

//=============================================================================
// 残機の生成
//=============================================================================
CRemain * CRemain::Create(D3DXVECTOR3 pos)
{
	CRemain *pRemain = NULL;

	if (pRemain == NULL)
	{
		pRemain = new CRemain;

		if (pRemain != NULL)
		{
			pRemain->Init(pos);
		}
	}
	return pRemain;
}
