//=============================================================================
//
// フレーム処理 [logo.cpp]
// Author : 竹内亘
//
//=============================================================================
#ifndef _FRAME_H_
#define _FRAME_H_

#include "main.h"
#include "logo.h"

//*****************************************************************************
// フレームクラスの定義
//*****************************************************************************
class CFrame : public CLogo
{
public:
	typedef enum
	{// フレームの種類
		FRAMETYPE_TIMER,
		FRAMETYPE_SCORE,
		FRAMETYPE_MAX,
	}FRAMETYPE;

	CFrame();
	~CFrame();
	HRESULT Init(D3DXVECTOR3 pos,FRAMETYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CFrame * Create(D3DXVECTOR3 pos,FRAMETYPE type);
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[FRAMETYPE_MAX];	// 共有テクスチャのポインタ
	FRAMETYPE m_nType;										// フレームの種類
	float m_fWidth;											// 幅
	float m_fHeight;										// 高さ
};
#endif
