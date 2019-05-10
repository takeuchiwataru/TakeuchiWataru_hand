//=============================================================================
//
// ロゴ処理 [logo.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _LOGO_H_
#define _LOGO_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLELOGO_TEXTURE "data/TEXTURE/title.png"				// タイトルロゴのテクスチャ
#define RESULTLOGO_TEXTURE "data/TEXTURE/result.png"			// リザルトロゴのテクスチャ
#define TUTORIALLOGO_TEXTURE "data/TEXTURE/tutorial.png"		// チュートリアルロゴのテクスチャ
#define RANKINGLOGO_TEXTURE "data/TEXTURE/ranking.png"			// ランキングロゴのテクスチャ
#define PRESSENTER_TEXTURE "data/TEXTURE/press_enter.png"		// プレスエンターのテクスチャ
#define PRESSENTER_TEXTURE002 "data/TEXTURE/press_enter2.png"	// プレスエンターのテクスチャ
#define PAUSEMAT_TEXTURE	"data/TEXTURE/pause100.png"			// ポーズ台紙のテクスチャ
#define PAUSESELECT_TEXTURE000	"data/TEXTURE/pause000.png"		// CONTINUE
#define PAUSESELECT_TEXTURE001	"data/TEXTURE/pause001.png"		// RETRY
#define PAUSESELECT_TEXTURE002	"data/TEXTURE/pause002.png"		// QUIT
#define FRAME_TEXTURE000 "data/TEXTURE/timer_frame2.png"		// タイマーフレーム
#define FRAME_TEXTURE001 "data/TEXTURE/score_frame.png"			// スコアフレーム

#define LOGO_SIZE_X		(260)
#define LOGO_SIZE_Y		(50)
#define TITLE_SIZE_X	(550)									// タイトル幅
#define TITLE_SIZE_Y	(200)									// タイトル高さ
#define RESULT_SIZE_X	(150)									// リザルト幅
#define RESULT_SIZE_Y	(50)									// リザルト高さ
#define TUTORIAL_SIZE_X	(150)									// チュートリアル幅
#define TUTORIAL_SIZE_Y	(50)									// チュートリアル高さ
#define RANKING_SIZE_X	(200)									// ランキング幅
#define RANKING_SIZE_Y	(50)									// ランキング高さ
#define TITLE_PRESSENTER_SIZE_X	(250)							// プレスエンター幅(タイトル)
#define TUTORIAL_PRESSENTER_SIZE_X	(170)						// プレスエンター幅(チュートリアル)
#define PRESSENTER_SIZE_Y   (50)								// プレスエンター高さ
#define PRESSENTER_COUNTER	(120)								// プレスエンターカウンター
#define PRESSENTER_NONE		(50)								// 押されてない時
#define PRESSENTER_PUSH		(6)									// 押されてる時

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CLogo : public CScene
{
public:
	CLogo(int nPriority);
	~CLogo();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos, float fwidth, float fheight);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	void SetColor(D3DXCOLOR col);					// カラーの設置
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);

private:
	LPDIRECT3DTEXTURE9	m_pTexture;						// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;					// 頂点バッファへのポインタ

protected:
	D3DXVECTOR3				m_pos;						// 位置
	D3DXVECTOR3				m_move;						// 移動
	D3DXCOLOR				m_col;						// 色
}; 
//*****************************************************************************
// タイトルロゴクラスの定義
//*****************************************************************************
class CTitleLogo : public CLogo
{
public:
	CTitleLogo();
	~CTitleLogo();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CTitleLogo * Create(D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// 共有テクスチャのポインタ
};
//*****************************************************************************
// リザルトクラスロゴクラスの定義
//*****************************************************************************
class CResultLogo : public CLogo
{
public:
	typedef enum
	{// リザルトの種類
		RESULTTYPE_CLEAR,
		RESULTTYPE_OVER,
		RESULTTYPE_MAX,
	}RESULTTYPE;

	CResultLogo();
	~CResultLogo();
	HRESULT Init(D3DXVECTOR3 pos, RESULTTYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CResultLogo * Create(D3DXVECTOR3 pos,RESULTTYPE type);
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[RESULTTYPE_MAX];	// 共有テクスチャのポインタ
	RESULTTYPE m_nType;										// リザルトの種類
};
//*****************************************************************************
// チュートリアルロゴクラスの定義
//*****************************************************************************
class CTutorialLogo : public CLogo
{
public:
	CTutorialLogo();
	~CTutorialLogo();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CTutorialLogo * Create(D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// 共有テクスチャのポインタ
};
//*****************************************************************************
// ランキングロゴクラスの定義
//*****************************************************************************
class CRankingLogo : public CLogo
{
public:
	CRankingLogo();
	~CRankingLogo();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CRankingLogo * Create(D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// 共有テクスチャのポインタ
};
//*****************************************************************************
// プレスエンタークラスの定義
//*****************************************************************************
class CPressEnter : public CLogo
{
public:
	typedef enum
	{//プレスエンターの状態
		PRESSENTERSTATE_NONE = 0,
		PRESSENTERSTATE_PUSH,
		PRESSENTERSTATE_MAX
	}PRESSENTERSTATE;

	typedef enum
	{//プレスエンター
		PRESSENTER_TITLE = 0,
		PRESSENTER_TUTORIAL,
		PRESSENTER_MAX
	}PRESSENTER;

	CPressEnter();
	~CPressEnter();
	HRESULT Init(D3DXVECTOR3 pos, PRESSENTER type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CPressEnter * Create(D3DXVECTOR3 pos,PRESSENTER type);
	static HRESULT Load(void);
	static void Unload(void);
	static void DispPressEnter(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[PRESSENTER_MAX];				// 共有テクスチャのポインタ
	PRESSENTERSTATE         m_PressEnterState;			// 状態
	PRESSENTER				m_nType;					// 種類
	int                     m_nCounterPressEnter;		// Drawするタイミング管理カウンター
	static bool             m_bDisp;					// 描画するかしないか
};
#endif
