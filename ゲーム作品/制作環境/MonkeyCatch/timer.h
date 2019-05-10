//=============================================================================
//
// タイマー処理 [timer.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// 前方宣言 インクルードはいらない
//*****************************************************************************
class CNumber;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TIMER	(3)				// 最大桁数
#define TIME_INTERVAL_SIZE	(57)	// スコア間隔
#define TIMER_SIZE	(30)			// タイマーサイズ
#define TIMER_GAME	(150)			// ゲームタイマー
#define TIMER_ZERO	(0)				// タイマー0
#define TIMER_DEGIT (10)			// 桁数
#define TIMER_SCORE	(10)			// タイマースコア
#define TIMER_RED	(30)			// 赤くなる時間		
#define TIMER_TEN	(10)			// 10秒以下
#define TIMER_SECOND	(60)		// 秒
#define TIMER_ADD		(1)			// 加算
#define TIMER_FLASH		(8)			// 点滅
#define TIMER_FLASH2	(16)		// 点滅

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CTimer : public CScene
{
public:
	CTimer();
	~CTimer();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddTime(int nValue);
	void SetColor(D3DXCOLOR col);
	void StopTimer(void);
	static int GetStopTimer(void);

	// 静的メンバ関数
	static CTimer * Create(D3DXVECTOR3 pos);

private:
	CNumber * m_apNumber[MAX_TIMER];
	D3DXVECTOR3	m_pos;			// 位置
	int m_nTime;				// タイマー
	int	m_nTimerCnt;			// タイマー用カウント
	int	m_nFlashCnt;			// 点滅カウント
	bool m_bTimerStop;			// 止めるか止めないか
	static int m_nStopTimer;	// 止まった時間保存用
};
#endif