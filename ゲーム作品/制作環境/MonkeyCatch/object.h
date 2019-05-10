//=============================================================================
//
// オブジェクト処理 [object.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"
#include "sceneX.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define OBJECT_MODEL_NAME000 "data/MODEL/tree3.x"
#define OBJ_SHADOWPOS (0.3f)	// 影が変にならないようにしとくやつ

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CObject : public CSceneX
{// 派生させる時は : public ～を後ろにつけてやる
public:

	typedef enum
	{// オブジェクトの種類
		OBJECTTYPE_TREE = 0,
		OBJECTTYPE_MAX
	}OBJECTTYPE;

	CObject();
	~CObject();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OBJECTTYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pVtxMax, D3DXVECTOR3 *pVtxMin);

	// 静的メンバ関数
	static CObject * Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OBJECTTYPE type);
	static HRESULT LoadX(void);
	static void UnloadX(void);

	// 静的メンバ変数
	static LPDIRECT3DTEXTURE9		*m_pTexture;	// テクスチャへのポインタ
	static LPD3DXMESH		m_pMesh;				// メッシュ情報へのポインタ
	static LPD3DXBUFFER		m_pBuffMat;				// マテリアル情報へのポインタ
	static DWORD			m_nNumMat;				// マテリアル情報の数

private:
	D3DXVECTOR3				m_pos;					// 位置
	D3DXVECTOR3				m_rot;					// 向き
	D3DXVECTOR3				m_move;					// 移動
	D3DXMATRIX				m_mtxWorld;				// ワールドマトリックス
	OBJECTTYPE				m_type;					// 種類
	D3DXMATRIX m_mtxRot, m_mtxTrans;			// 計算用マトリックス

};
#endif