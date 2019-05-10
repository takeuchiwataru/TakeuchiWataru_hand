//=============================================================================
//
// モデル処理 [model.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CModel
{// 派生させる時は : public ～を後ろにつけてやる
public:
	CModel();
	~CModel();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPos(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	void BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD pNumMat);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	void SetParent(CModel * pModel);
	D3DXMATRIX &GetMtxWorld(void);
	void SetRot(D3DXVECTOR3 rot);
	void SetPos(D3DXVECTOR3 pos);

	// 静的メンバ関数
	static CModel * Create(D3DXVECTOR3 pos);

private:
	// Xファイル管理に必要な要数
	LPDIRECT3DTEXTURE9		m_pTexture;			// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファへのポインタ

	D3DXVECTOR3				m_pos;				// 位置
	D3DXVECTOR3				m_rot;				// 向き
	D3DXVECTOR3				m_move;				// 移動
	D3DXMATRIX				m_mtxWorld;			// ワールドマトリックス

	LPD3DXMESH		m_pMesh;					// メッシュ情報へのポインタ
	LPD3DXBUFFER	m_pBuffMat;					// マテリアル情報へのポインタ
	DWORD			m_nNumMat;					// マテリアル情報の数

	D3DXVECTOR3 m_VtxMinModel, m_VtxMaxModel;	// モデルの最小値、最大値

	CModel * m_pParent;							// 親モデルのポインタ
};
#endif