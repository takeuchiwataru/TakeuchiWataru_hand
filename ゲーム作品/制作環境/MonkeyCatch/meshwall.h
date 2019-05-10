//=============================================================================
//
// メッシュウォール処理 [meshwall.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MW_TEXTURE ("data/TEXTURE/block100.png")
#define TEX_POS_X_INIT			(1.0f)								// テクスチャ座標Uの初期位置
#define TEX_POS_Y_INIT			(1.0f)								// テクスチャ座標Vの初期位置
#define TEX_LEFT				(0.0f)								// テクスチャ座標U左
#define TEX_RIGHT				(1.0f)								// テクスチャ座標U右
#define TEX_TOP					(0.0f)								// テクスチャ座標V上
#define TEX_BOT					(1.0f)								// テクスチャ座標V下
#define X_POSSIZE				(25)								// Xの大きさ
#define Y_POSSIZE				(25)								// Zの大きさ
#define MAX_MW_X				(32)								// Xの幅
#define MAX_MW_Y			 	(2)									// Yの幅
#define VERTEX_MW_KAZU ((MAX_MW_X + 1) * (MAX_MW_Y + 1))							// 頂点数
#define INDEX_MW_KAZU (((MAX_MW_X + 1) * (MAX_MW_Y + 1) +((MAX_MW_Y - 1) * 2) + (MAX_MW_X + 1) * (MAX_MW_Y - 1)))	// インデックス数
#define MAX_MESHWALL (4)													// メッシュオールの最大数

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CMeshWall : public CScene
{
public:
	CMeshWall();
	~CMeshWall();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	// 静的メンバ関数
	static CMeshWall * Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;		// インデックスバッファのポインタ

	D3DXVECTOR3				m_pos;			// 位置
	D3DXVECTOR3				m_rot;			// 向き
	D3DXVECTOR3				m_move;			// 移動
	D3DXMATRIX				m_mtxWorld;		// ワールドマトリックス
	D3DXMATRIX m_mtxRot, m_mtxTrans;		// 計算用マトリックス
	float					m_fHeight;		// 高さ
	float					m_fWidth;		// 幅
	int						m_nVtxIndex;	
	int						m_nIdxIndex;
	float					m_fPos_X;
	float					m_fPos_Y;
	float					m_fXpos;
	float					m_fYpos;

	LPD3DXMESH m_pMesh;						// メッシュ情報へのポインタ
	LPD3DXBUFFER m_pBuffMat;				// マテリアル情報へのポインタ
	DWORD m_nNumMat;						// マテリアル情報の数

	int m_nNumVertex;						// 頂点数
	int m_nNumIndex;						// インデックス数
	int m_nNumPolygon;						// ポリゴン数
};
#endif
