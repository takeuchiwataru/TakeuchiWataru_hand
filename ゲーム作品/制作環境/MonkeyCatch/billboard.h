//=============================================================================
//
// ビルボードの処理 [billboard.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BILLBOARD_SIZE	(5)									// ビルボードのサイズ

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CBillBoard : public CScene
{// 派生させる時は : public ～を後ろにつけてやる
public:
	CBillBoard();
	~CBillBoard();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos,D3DXVECTOR3 move);
	D3DXVECTOR3 GetPosition(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);

	void SetPosition(D3DXVECTOR3 pos, float fwidth, float fheight);
	void SetColor(D3DXCOLOR col);
	void SetParticle(int nRadius, D3DXVECTOR3 pos);
	void SetParticle2(int nRadius, D3DXVECTOR3 pos);

	// 静的メンバ関数
	static CBillBoard * Create(D3DXVECTOR3 pos);

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ

	D3DXVECTOR3				m_pos;			// 位置
	D3DXVECTOR3				m_move;			// 移動
	D3DXVECTOR3				m_rot;			// 向き
	D3DXCOLOR				m_col;			// 色 
	D3DXMATRIX				m_mtxWorld;		// ワールドマトリックス

};

#endif
