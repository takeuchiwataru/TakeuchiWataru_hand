//=============================================================================
//
// メッシュフィールド処理 [meshfield.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHFILED_POS	(2500.0f)											// ポリゴンのサイズ
#define MAX_MF_X (250)														// X(横の数)
#define MAX_MF_Z (250)														// Z(縦の数)
#define VERTEX_KAZU ((MAX_MF_X + 1) * (MAX_MF_Z + 1))							// 頂点数
#define INDEX_KAZU ((((MAX_MF_X + 1) * 2) * (MAX_MF_Z)) + ((MAX_MF_Z - 1) * 2))	// インデックス数
#define POLYGON_KAZU (((MAX_MF_X * MAX_MF_Z) * 2) + ((MAX_MF_Z - 1) * 4))		// ポリゴン数// ポリゴン数
#define MF_TEXTURE ("data/TEXTURE/field001.jpg")

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CMeshField : public CScene
{
public:
	CMeshField();
	~CMeshField();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	float GetHeight(D3DXVECTOR3 pos);
	void Nor(void);
	void SetHeight(D3DXVECTOR3 pos, float fValue, float fRange);
	void SaveMeshFiled(void);
	void LoadMeshFiled(void);

	// 静的メンバ関数
	static CMeshField * Create(D3DXVECTOR3 pos);

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		// インデックスバッファのポインタ

	D3DXVECTOR3				m_pos;			// 位置
	D3DXVECTOR3				m_rot;			// 向き
	D3DXVECTOR3				m_move;			// 移動
	D3DXMATRIX				m_mtxWorld;		// ワールドマトリックス
	D3DXMATRIX m_mtxRot, m_mtxTrans;		// 計算用マトリックス

	LPD3DXMESH m_pMesh;						// メッシュ情報へのポインタ
	LPD3DXBUFFER m_pBuffMat;				// マテリアル情報へのポインタ
	DWORD m_nNumMat;						// マテリアル情報の数

	int m_nNumVertex;				// 頂点数
	int m_nNumIndex;				// インデックス数
	int m_nNumPolygon;				// ポリゴン数

	D3DXVECTOR3 m_vecA;
	D3DXVECTOR3 m_vecB;
	D3DXVECTOR3 m_vecC;

	D3DXVECTOR3 m_aPos[9];
};
#endif
