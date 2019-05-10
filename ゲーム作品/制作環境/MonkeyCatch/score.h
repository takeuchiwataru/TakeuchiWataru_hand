//=============================================================================
//
// スコア処理 [score.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// 前方宣言 インクルードはいらない
//*****************************************************************************
class CNumber;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SCORE (8)				// 最大桁数
#define SCORE_INTERVAL_SIZE (55)	// スコア間隔
#define SCORE_SIZE	(30)			// スコアサイズ
#define SCORE_DEGIT	(10)			// 桁数

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CScore : public CScene
{
public:
	CScore();
	~CScore();
	HRESULT Init(D3DXVECTOR3 pos);
	HRESULT InitTotal(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	int GetScore(void);
	void AddScore(int nValue);
	void SetColor(D3DXCOLOR col);

	// 静的メンバ関数
	static CScore * Create(D3DXVECTOR3 pos);
	static CScore * CreateTotal(D3DXVECTOR3 pos);

private:
	CNumber * m_apNumber[MAX_SCORE];
	int m_nScore;
};
#endif