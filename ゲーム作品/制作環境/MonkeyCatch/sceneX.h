//=============================================================================
//
// オブジェクトX処理 [sceneX.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _SCENEX_H_
#define _SCENEX_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CSceneX : public CScene
{// 派生させる時は : public ～を後ろにつけてやる
public:
	CSceneX();
	~CSceneX();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Draw2(void);
	void SetPosition(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	D3DXVECTOR3 GetPosition(void);
	void BindModelTex(LPDIRECT3DTEXTURE9 *pTexture,LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD pNumMat);
	void BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD pNumMat);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	D3DXVECTOR3 GetMax(void);
	D3DXVECTOR3 GetMin(void);

	// 静的メンバ関数
	static CSceneX * Create(D3DXVECTOR3 pos);

private:
	LPDIRECT3DTEXTURE9		*m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファへのポインタ

	D3DXVECTOR3				m_pos;				// 位置
	D3DXVECTOR3				m_rot;				// 向き
	D3DXVECTOR3				m_move;				// 移動

	D3DXMATRIX				m_mtxWorld;			// ワールドマトリックス
	D3DXMATRIX m_mtxRot, m_mtxTrans;			// 計算用マトリックス
	D3DMATERIAL9			m_matDef;			// 現在のマテリアル保存用
	D3DXVECTOR3 m_VtxMin, m_VtxMax;				// モデルの最小値、最大値

	LPD3DXMESH		m_pMesh;					// メッシュ情報へのポインタ
	LPD3DXBUFFER	m_pBuffMat;					// マテリアル情報へのポインタ
	DWORD			m_nNumMat;					// マテリアル情報の数
};
#endif