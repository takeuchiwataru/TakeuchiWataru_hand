//=============================================================================
//
// ナンバー処理 [number.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCORE_NUMBER_SIZE (30.0f)
#define SCORE_TEXTURE "data/TEXTURE/number.png"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CNumber
{
public:
	CNumber();
	~CNumber();
	HRESULT Init(D3DXVECTOR3 pos,float width,float height);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetNumber(int nNumber);
	void SetColor(D3DXCOLOR col);

	// 静的メンバ関数
	static CNumber * Create(D3DXVECTOR3 pos, float width, float height,D3DXCOLOR col);
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXVECTOR3				m_pos;			// 位置
	D3DXCOLOR				m_col;			// 色
	float					m_fWidth;		// 幅
	float					m_fHeight;		// 高さ
};
#endif