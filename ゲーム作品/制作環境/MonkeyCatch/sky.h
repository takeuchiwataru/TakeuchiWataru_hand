//=============================================================================
//
// 空処理 [sky.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _SKY_H_
#define _SKY_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SKY_TEXTURE_NAME   "data/TEXTURE/sky000.jpg"	// 読み込むテクスチャファイル名
#define DOME_LENGTH         (100)						// 縦の長さ
#define DOME_SIDE           (50)						// 横の長さ
#define DOME_RADIUS         (1200.0f)					// 半径の長さ
#define DOME_HEIGHT         (2000.0f)					// 高さの長さ
#define BLOCK_LENGTH        (5)							// 縦の分割数
#define BLOCK_SIDE          (10)						// 横の分割数
#define SKY_ROT				(0.00002f)					// 回転量

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CSky : public CScene
{
public:
	CSky();
	~CSky();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	// 静的メンバ関数
	static CSky * Create(D3DXVECTOR3 pos);

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_apVtxBuff[2];	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		// インデックスバッファのポインタ

	D3DXVECTOR3				m_pos;			// 位置
	D3DXVECTOR3				m_rot;			// 向き
	D3DXMATRIX				m_mtxWorld;		// ワールドマトリックス
	D3DXMATRIX		m_mtxRot, m_mtxTrans;	// 計算用マトリックス
	float					m_Length;		// 縦の長さ
	float					m_Side;			// 横の長さ
	int						m_XBlock;		// 横の分割数
	int						m_YBlock;		// 縦の分割数
	int						m_nNumVertex;	// 頂点数
	int						m_nNumIndex;	// インデックス数
	int						m_nNumPolygon;	// ポリゴン数
	float					m_Addtex;		//
};
#endif
