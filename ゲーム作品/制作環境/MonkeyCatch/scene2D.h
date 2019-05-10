//=============================================================================
//
// オブジェクト2D処理 [scene2D.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCENE2D_SIZE	(50)									// ポリゴンのサイズ
#define SCENE2D_TEXTURE "data/TEXTURE/kame.png"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CScene2D : public CScene
{// 派生させる時は : public ～を後ろにつけてやる
public:
	CScene2D();
	~CScene2D();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos,float fwidth,float fheight);
	D3DXVECTOR3 GetPosition(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	void SetColor(D3DXCOLOR col);
	void SetParticle(int nRadius, D3DXVECTOR3 pos);	

	// 静的メンバ関数
	static CScene2D * Create(D3DXVECTOR3 pos);

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ

	D3DXVECTOR3				m_pos;			// 位置
	D3DXVECTOR3				m_move;			// 移動
	D3DXCOLOR				m_col;			// 色 
};
#endif