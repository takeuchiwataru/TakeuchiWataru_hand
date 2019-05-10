//=============================================================================
//
// ポーズ項目処理 [pauseselect.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _PAUSESELECT_H_
#define _PAUSESELECT_H_

#include "main.h"
#include "logo.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PAUSESELECT_WIDTH (150)
#define PAUSESELECT_HEIGHT (50)
#define MAX_PAUSE_SELECT (RESTART_MAX)
#define PAUSESELECT_INTERVAL (150)

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CPauseSelect : public CScene
{
public:
	typedef enum
	{// 状態
		PAUSESELECT_NONE = 0,   // 選択されていない状態
		PAUSESELECT_SELECT,     // 選択されている状態
		PAUSESELECT_MAX
	}PAUSESELECT;

	typedef enum
	{// 項目
		RESTART_CONTINUE,  // コンティニュー
		RESTART_RETRY,     // ゲームリトライ
		RESTART_QUIT,      // タイトル遷移
		RESTART_MAX
	}RESTART;

	CPauseSelect();
	~CPauseSelect();

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static RESTART GetRstart(void);

	// 静的メンバ関数
	static CPauseSelect * Create();
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[RESTART_MAX];	// 共有テクスチャのポインタ
														// RESTART m_Restart;								// 遷移する先
	static RESTART m_Restart;
	RESTART m_nType;									// 種類
	PAUSESELECT m_aSelect[MAX_PAUSE_SELECT];
	D3DXCOLOR m_aCol[MAX_PAUSE_SELECT];
	int m_nSelect;										// 選択
	float m_fCol;										// 色の変化に使用
	CLogo * m_apLogo[MAX_PAUSE_SELECT];

};
#endif
