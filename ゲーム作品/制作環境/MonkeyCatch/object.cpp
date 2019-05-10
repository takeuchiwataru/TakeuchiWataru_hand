//=============================================================================
//
// オブジェクト処理 [object.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "object.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"

#include "shadow.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 *CObject::m_pTexture = NULL;
LPD3DXMESH CObject::m_pMesh = NULL;
LPD3DXBUFFER CObject::m_pBuffMat = NULL;
DWORD CObject::m_nNumMat = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CObject::CObject()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
//=============================================================================
// デストラクタ
//=============================================================================
CObject::~CObject()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CObject::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OBJECTTYPE type)
{
	CSceneX::BindModelTex(m_pTexture, m_pMesh, m_pBuffMat, m_nNumMat);
	CSceneX::Init(pos);

	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// オブジェクトのタイプの設定
	SetObjType(CScene::OBJTYPE_OBJECT);

	// モデル初期化
	m_pos = pos;
	m_rot = rot;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = type;

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CObject::Uninit(void)
{
	if (m_pTexture != NULL)
	{// (家)
		for (int nCnt = 0; nCnt < (int)m_nNumMat; nCnt++)
		{// (家具)
			if (m_pTexture[nCnt] != NULL)
			{// テクスチャ破棄
				m_pTexture[nCnt]->Release();
				m_pTexture[nCnt] = NULL;
			}
		}
	}

	// メモリの開放(解体)
	delete[] m_pTexture;
	// NULLを入れる(更地)
	m_pTexture = NULL;

	// オブジェクト(自分自身)の破棄
	Release();
}
//=============================================================================
// 更新処理
//=============================================================================
void CObject::Update(void)
{	

}
//=============================================================================
// 描画処理
//=============================================================================
void CObject::Draw(void)
{
	CSceneX::Draw();
}
//=============================================================================
// 生成
//=============================================================================
CObject * CObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OBJECTTYPE type)
{
	CObject *pObject = NULL;

	if (pObject == NULL)
	{
		pObject = new CObject;

		pObject->Init(pos, rot, type);
		CShadow::Create(D3DXVECTOR3(pos.x, pos.y + OBJ_SHADOWPOS, pos.z));
	}
	return pObject;
}
//=============================================================================
// モデルの読み込み
//=============================================================================
HRESULT CObject::LoadX(void)
{
	D3DXMATERIAL *pMat;							// マテリアルデータへのポインタ

	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX(OBJECT_MODEL_NAME000,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	m_pTexture = new LPDIRECT3DTEXTURE9[m_nNumMat];

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		m_pTexture[nCntMat] = NULL;

		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// テクスチャの生成
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &m_pTexture[nCntMat]);
		}
	}

	return S_OK;
}
//=============================================================================
// モデルの破棄
//=============================================================================
void CObject::UnloadX(void)
{
	// メッシュの開放
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// マテリアルの開放
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}
}
//=============================================================================
// 当たり判定
//=============================================================================
bool CObject::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pVtxMax, D3DXVECTOR3 *pVtxMin)
{
	D3DXVECTOR3 VtxMax = CSceneX::GetMax();
	D3DXVECTOR3 VtxMin = CSceneX::GetMin();

	bool bHit = false;		// 当たったかどうか

	if (pPos->y + pVtxMax->y > m_pos.y + VtxMin.y && pPos->y + pVtxMin->y < m_pos.y + VtxMax.y)
	{// モデルの高さ分
		if (pPos->x + pVtxMax->x > m_pos.x + VtxMin.x && pPos->x + pVtxMin->x < m_pos.x + VtxMax.x)
		{// Z面の判定
			if (pPosold->z + pVtxMin->z >= m_pos.z + VtxMax.z && m_pos.z + VtxMax.z >= pPos->z + pVtxMin->z)
			{// 奧の判定
				pPos->z = pPosold->z;
				bHit = true;
			}
			if (pPosold->z + pVtxMax->z <= m_pos.z + VtxMin.z && m_pos.z + VtxMin.z <= pPos->z + pVtxMax->z)
			{// 手前の判定
				pPos->z = pPosold->z;
				bHit = true;
			}
		}

		if (pPos->z + pVtxMax->z > m_pos.z + VtxMin.z && pPos->z + pVtxMin->z < m_pos.z + VtxMax.z)
		{// X面の判定
			if (pPosold->x + pVtxMin->x >= m_pos.x + VtxMax.x && m_pos.x + VtxMax.x >= pPos->x + pVtxMin->x)
			{// 右面の判定		
				pPos->x = pPosold->x;
				bHit = true;
			}
			if (pPosold->x + pVtxMax->x <= m_pos.x + VtxMin.x && m_pos.x + VtxMin.x <= pPos->x + pVtxMax->x)
			{// 左面の判定
				pPos->x = pPosold->x;
				bHit = true;
			}
		}
	}
	if (pPos->z > m_pos.z + VtxMin.z + pVtxMin->z && pPos->z < m_pos.z + VtxMax.z + pVtxMax->z &&
		pPos->x > m_pos.x + VtxMin.x + pVtxMin->x && pPos->x < m_pos.x + VtxMax.x + pVtxMax->x)
	{// モデルの横
		if (pPosold->y >= m_pos.y + VtxMax.y && pPos->y <= m_pos.y + VtxMax.y)
		{// 上の判定
			pPos->y = pPosold->y;
			bHit = true;
		}
	}
	return bHit;
}
