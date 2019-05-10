//=============================================================================
//
// タイマー処理 [timer.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "timer.h"
#include "number.h"
#include "renderer.h"
#include "manager.h"
#include "scene2D.h"
#include "fade.h"
#include "score.h"
#include "game.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
int CTimer::m_nStopTimer = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CTimer::CTimer()
{
	m_nTime = 0;
	m_nTimerCnt = 0;
	m_nFlashCnt = 0;
	m_bTimerStop = false;
}
//=============================================================================
// デストラクタ
//=============================================================================
CTimer::~CTimer()
{
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTimer::Init(D3DXVECTOR3 pos)
{
	m_nTime = 0;
	m_nTimerCnt = 0;
	m_nFlashCnt = 0;
	m_bTimerStop = false;

	// モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();

	for (int nCount = 0; nCount < MAX_TIMER; nCount++)
	{
		m_apNumber[nCount] = new CNumber;

		m_apNumber[nCount]->Init(D3DXVECTOR3(pos.x - (TIME_INTERVAL_SIZE * nCount), pos.y, pos.z), TIMER_SIZE, TIMER_SIZE);
	}


	if (mode == CManager::MODE_GAME)
	{// モードがゲーム
		AddTime(TIMER_GAME);	// タイマー設定
	}
	else
	{// それ以外
		AddTime(TIMER_ZERO);		// タイマー設定
	}

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CTimer::Uninit(void)
{
	for (int nCount = 0; nCount < MAX_TIMER; nCount++)
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
void CTimer::Update(void)
{
	// モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{
		float AlphaCol = 1.0f;

		m_nTimerCnt++;

		if (m_bTimerStop == false)
		{// タイマーが動いていたら
			if (m_nTime <= TIMER_RED)
			{// 30秒以下で赤	
				if (m_nTime <= TIMER_TEN)
				{// 10秒以下で点滅
					m_nFlashCnt++;
					if (m_nFlashCnt <= TIMER_FLASH)
					{
						AlphaCol = 0.0f;
					}
					if (m_nFlashCnt >= TIMER_FLASH2)
					{
						m_nFlashCnt = 0;
					}
				}
				SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, AlphaCol));
			}

			if (m_nTimerCnt >= TIMER_SECOND)
			{	//カウント初期化
				m_nTimerCnt = 0;

				m_nTime -= TIMER_ADD;
				//更新するだけ
				AddTime(TIMER_ZERO);
			}
		}
		if (m_bTimerStop == true)
		{// タイマーが止まったら
			m_nStopTimer = m_nTime;
		}
		if (m_nTime == TIMER_ZERO)
		{// タイマーが0になったら
			// フェードの取得
			CFade *pFade = CManager::GetFade();
			pFade->Set(CManager::MODE_RESULT, pFade->FADE_OUT);
		}
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void CTimer::Draw(void)
{
	for (int nCount = 0; nCount < MAX_TIMER; nCount++)
	{
		m_apNumber[nCount]->Draw();
	}
}
//=============================================================================
// 加算
//=============================================================================
void CTimer::AddTime(int nValue)
{
	m_nTime += nValue;

	int nAnswer = 1;
	int nTime;

	for (int nCount = 0; nCount < MAX_TIMER; nCount++)
	{
		nTime = m_nTime % (nAnswer * TIMER_DEGIT) / nAnswer;
		m_apNumber[nCount]->SetNumber(nTime);
		nAnswer *= TIMER_DEGIT;
	}
}
//=============================================================================
// 色設定
//=============================================================================
void CTimer::SetColor(D3DXCOLOR col)
{
	for (int nCount = 0; nCount < MAX_TIMER; nCount++)
	{
		m_apNumber[nCount]->SetColor(col);
	}
}
//=============================================================================
// タイマーストップ
//=============================================================================
void CTimer::StopTimer(void)
{
	m_bTimerStop = true;

	// スコアの取得
	CScore * pScore = CGame::GetScore();

	// 残った時間分スコア加算
	pScore->AddScore(m_nTime * TIMER_SCORE);
}
//=============================================================================
// 止めた時間の取得
//=============================================================================
int CTimer::GetStopTimer(void)
{
	return m_nStopTimer;
}
//=============================================================================
// 生成
//=============================================================================
CTimer * CTimer::Create(D3DXVECTOR3 pos)
{
	CTimer *pTimer = NULL;

	if (pTimer == NULL)
	{
		pTimer = new CTimer;

		if (pTimer != NULL)
		{
			pTimer->Init(pos);
		}
	}
	return pTimer;
}
