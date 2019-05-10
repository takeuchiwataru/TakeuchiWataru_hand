//=============================================================================
//
// オブジェクト3D処理 [scene3D.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCENE3D_SIZE	(100)									// ポリゴンのサイズ
#define SCENE3D_TEXTURE "data/TEXTURE/block100.png"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CScene3D : public CScene
{// 派生させる時は : public ～を後ろにつけてやる
public:
	CScene3D();
	~CScene3D();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	float GetHeight(D3DXVECTOR3 pos);

	// 静的メンバ関数
	static CScene3D * Create(D3DXVECTOR3 pos);

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ

	D3DXVECTOR3				m_pos;			// 位置
	D3DXVECTOR3				m_rot;			// 向き
	D3DXVECTOR3				m_move;			// 移動
	D3DXMATRIX				m_mtxWorld;		// ワールドマトリックス
	D3DXMATRIX m_mtxRot, m_mtxTrans;		// 計算用マトリックス

	LPD3DXMESH m_pMesh;						// メッシュ情報へのポインタ
	LPD3DXBUFFER m_pBuffMat;				// マテリアル情報へのポインタ
	DWORD m_nNumMat;						// マテリアル情報の数

	D3DXVECTOR3 m_vecA;
	D3DXVECTOR3 m_vecB;
	D3DXVECTOR3 m_vecC;
	bool m_bLR;			// 右か左か
};
#endif