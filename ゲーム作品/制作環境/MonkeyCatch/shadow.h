//=============================================================================
//
// 影処理 [shadow.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
#include "scene3D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SHADOW_TEXTURE "data/TEXTURE/shadow000.jpg"
#define SHADOW_SIZE (25.0f)		// 影のサイズ

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CShadow : public CScene
{
public:
	CShadow();
	~CShadow();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	void DeleteShadow(void);

	// 静的メンバ関数
	static CShadow * Create(D3DXVECTOR3 pos);

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ

	D3DXVECTOR3				m_pos;			// 位置
	D3DXVECTOR3				m_rot;			// 向き
	D3DXVECTOR3				m_move;			// 移動
	D3DXMATRIX				m_mtxWorld;		// ワールドマトリックス
	D3DXMATRIX m_mtxRot, m_mtxTrans;		// 計算用マトリックス
	bool					m_bUse;			// 使用しているかどうか
};

#endif