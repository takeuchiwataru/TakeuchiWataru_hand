//=============================================================================
//
// オブジェクトX処理 [sceneX.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "sceneX.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================

//=============================================================================
// コンストラクタ
//=============================================================================
CSceneX::CSceneX()
{
	//m_pTexture = NULL;		// テクスチャへのポインタ
	m_pVtxBuff = NULL;		// 頂点バッファへのポインタ

	m_pMesh = NULL;
	m_pBuffMat = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

}
//=============================================================================
// デストラクタ
//=============================================================================
CSceneX::~CSceneX()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSceneX::Init(D3DXVECTOR3 pos)
{
	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	int nNumVtex;			// 頂点数
	DWORD sizeFvF;			// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;			// 頂点バッファへのポインタ

	// モデル初期化
	m_pos = pos;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_VtxMin = D3DXVECTOR3(10000, 10000, 10000);
	m_VtxMax = D3DXVECTOR3(-10000, -10000, -10000);

	// 頂点数を取得
	if (m_pMesh != NULL)
	{
		nNumVtex = m_pMesh->GetNumVertices();

		// 頂点フォーマットのサイズを取得
		sizeFvF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

		// 頂点バッファをロック
		m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtex; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;
			if (m_VtxMin.x >= vtx.x)
			{
				m_VtxMin.x = vtx.x;
			}
			if (m_VtxMin.y >= vtx.y)
			{
				m_VtxMin.y = vtx.y;
			}
			if (m_VtxMin.z >= vtx.z)
			{
				m_VtxMin.z = vtx.z;
			}

			if (m_VtxMax.x <= vtx.x)
			{
				m_VtxMax.x = vtx.x;
			}
			if (m_VtxMax.y <= vtx.y)
			{
				m_VtxMax.y = vtx.y;
			}
			if (m_VtxMax.z <= vtx.z)
			{
				m_VtxMax.z = vtx.z;
			}
			pVtxBuff += sizeFvF;
		}
		// 頂点バッファをアンロック
		m_pMesh->UnlockVertexBuffer();
	}
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CSceneX::Uninit(void)
{

}
//=============================================================================
// 更新処理
//=============================================================================
void CSceneX::Update(void)
{

}
//=============================================================================
// 描画処理(Xファイルにマテリアルが存在)
//=============================================================================
void CSceneX::Draw(void)
{
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;							// マテリアルデータへのポインタ

	CManager manager;	//インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&m_mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&m_mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// マテリアル設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		if (pMat[nCntMat].pTextureFilename != NULL)
		{// テクスチャある
			pDevice->SetTexture(0, m_pTexture[nCntMat]);
		}
		else
		{
			pDevice->SetTexture(0, NULL);
		}

		// モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}
//=============================================================================
// 描画処理(普通)
//=============================================================================
void CSceneX::Draw2(void)
{
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;							// マテリアルデータへのポインタ

	CManager manager;	//インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&m_mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&m_mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// マテリアル設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		pDevice->SetTexture(0, NULL);

		// モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// 生成
//=============================================================================
CSceneX * CSceneX::Create(D3DXVECTOR3 pos)
{
	CSceneX *pSceneX = NULL;

	if (pSceneX == NULL)
	{
		pSceneX = new CSceneX;

		if (pSceneX != NULL)
		{
			pSceneX->Init(pos);
		}
	}
	return pSceneX;
}
//=============================================================================
// 位置の設置
//=============================================================================
void CSceneX::SetPosition(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;

	m_rot = rot;
}
//=============================================================================
// 位置の取得
//=============================================================================
D3DXVECTOR3 CSceneX::GetPosition(void)
{
	return m_pos;
}
//=============================================================================
// モデルの割り当て(テクスチャ有り)
//=============================================================================
void CSceneX::BindModelTex(LPDIRECT3DTEXTURE9 *pTexture,LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD pNumMat)
{
	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	m_pMesh = pMesh;
	m_pBuffMat = pBuffMat;
	m_nNumMat = pNumMat;
	m_pTexture = pTexture;
}
//=============================================================================
// モデルの割り当て(テクスチャなし)
//=============================================================================
void CSceneX::BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD pNumMat)
{
	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	m_pMesh = pMesh;
	m_pBuffMat = pBuffMat;
	m_nNumMat = pNumMat;
}

//=============================================================================
// 頂点バッファの取得
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CSceneX::GetVtxBuff(void)
{
	return m_pVtxBuff;
}
//=============================================================================
// 最大値取得
//=============================================================================
D3DXVECTOR3 CSceneX::GetMax(void)
{
	return m_VtxMax;
}
//=============================================================================
// 最小値取得
//=============================================================================
D3DXVECTOR3 CSceneX::GetMin(void)
{
	return m_VtxMin;
}


