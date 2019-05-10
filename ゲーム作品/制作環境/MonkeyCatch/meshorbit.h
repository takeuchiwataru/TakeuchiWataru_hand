//=============================================================================
//
// メッシュオービット処理 [meshorbit.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _MESHORBIT_H_
#define _MESHORBIT_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// 前方宣言 インクルードはいらない
//*****************************************************************************
class CPlayer;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MO_X (25)															// X(横の数)
#define MAX_MO_Z (1)															// Z(縦の数)
#define MAX_VERTEX	((MAX_MO_X + 1) * (MAX_MO_Z + 1))							// 最大頂点数
#define MAX_POLYGON (((MAX_MO_X * MAX_MO_Z) * 2) + ((MAX_MO_Z - 1) * 4))		// 最大ポリゴン数
#define MAX_OFFSET	(2)															// 両端のオフセット数
#define ORBIT_COLOR	(0.05f)

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CMeshOrbit : public CScene
{// 派生させる時は : public ～を後ろにつけてやる
public:
	CMeshOrbit();
	~CMeshOrbit();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPosition(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	void SetMtxParent(D3DXMATRIX *pMtx);
	D3DXMATRIX GetMtxWorld(void);

	// 静的メンバ関数
	static CMeshOrbit * Create(D3DXVECTOR3 pos);

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ

	D3DXVECTOR3				m_pos;			// 位置
	D3DXVECTOR3				m_rot;			// 向き
	D3DXVECTOR3				m_move;			// 移動
	D3DXMATRIX				m_mtxWorld;		// ワールドマトリックス
	D3DXMATRIX				*m_pMtxParent;	// 親のマトリックスへのポインタ

	D3DXMATRIX m_mtxRot, m_mtxTrans;		// 計算用マトリックス
	D3DXVECTOR3 m_aPosVertex[MAX_VERTEX];	// 算出した頂点座標
	D3DXVECTOR3 m_aOffset[MAX_OFFSET];		// 両端のオフセット
	int m_nNumPos;
	int m_nNumPolygon;
	bool m_bUse;
	float m_fCol;

	static CPlayer *m_pPlayer;
};
#endif