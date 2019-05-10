//=============================================================================
//
// 残機処理 [remain.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _REMAIN_H_
#define _REMAIN_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// 前方宣言 インクルードはいらない
//*****************************************************************************
class CNumber;
class CEnemy;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_REMAIN	(2)					// 最大桁数
#define REMAIN_INTERVAL_SIZE	(80)	// 残機間隔
#define REMAIN_SIZE	(45)				// 残機サイズ
#define R_REMAIN_SIZE	(30)			// 残機サイズ(リザルト)
#define R_REMAIN_INTERVAL_SIZE	(55)	// 残機間隔(リザルト)
#define REMAIN_DEGIT	(10)			// 桁数

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CRemain : public CScene
{
public:
	CRemain();
	~CRemain();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddRemain(int nValue);
	void SetColor(D3DXCOLOR col);

	// 静的メンバ関数
	static CRemain * Create(D3DXVECTOR3 pos);

private:
	CNumber * m_apNumber[MAX_REMAIN];
	int m_nRemain;
	D3DXVECTOR3	m_pos;			// 位置
	int m_NumEnemy;				// 敵の総数保管
};
#endif