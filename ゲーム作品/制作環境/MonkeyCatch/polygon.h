//=============================================================================
//
// ポリゴン処理 [polygon.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LIFE_SIZE (30.0f)
#define LIFE_TEXTURE "data/TEXTURE/life000.png"					// ライフのテクスチャ

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CPolygon
{
public:
	CPolygon();
	~CPolygon();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetLife(int nLife);

	// 静的メンバ関数
	static CPolygon * Create(D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXVECTOR3				m_pos;			// 位置
};
#endif