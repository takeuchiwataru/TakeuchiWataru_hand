//=============================================================================
//
// フェード処理 [fade.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"

#include "renderer.h"
#include "manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FADE_POS_X	(0)                         // フェードの左上Ｘ座標
#define FADE_POS_Y	(0)                         // フェードの左上Ｙ座標
#define FADE_WIDTH	(SCREEN_WIDTH)              // フェードの幅
#define FADE_HEIGHT	(SCREEN_HEIGHT)             // フェードの高さ
#define FADE_TOMEI	(0.05f)						// 透明
#define FADE_NOTTOMEI	(0.02f)					// 不透明
#define FADE_COLOR000	(0.0f)
#define FADE_COLOR001	(1.0f)

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CFade
{
public:
	typedef enum
	{// フェードの状態
		FADE_NONE = 0,		// 何もしていない状態
		FADE_IN,			// フェードイン状態
		FADE_OUT,			// フェードアウト状態
		FADE_MAX
	}FADE;

	CFade();
	~CFade();
	void Init(CManager::MODE modeNext);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CFade * Create();
	static void Set(CManager::MODE modeNext, FADE fade);
	static FADE GetFade(void);

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	static FADE				m_fade;			// フェード状態
	static CManager::MODE	m_modeNext;		// 次のモード
	static D3DXCOLOR		m_colorFade;	// フェード色
};
#endif

 