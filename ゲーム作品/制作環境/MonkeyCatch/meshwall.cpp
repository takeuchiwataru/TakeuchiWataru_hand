//=============================================================================
//
// メッシュウォール処理 [meshwall.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "meshwall.h"
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
CMeshWall::CMeshWall()
{
	m_pTexture = NULL;		// テクスチャへのポインタ
	m_pVtxBuff = NULL;		// 頂点バッファへのポインタ
	m_pIdxBuff = NULL;		// インデックスバッファのポインタ

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fHeight = 0.0f;
	m_fWidth = 0.0f;
	m_nVtxIndex = 0;
	m_nIdxIndex = 0;
	m_fPos_X = 0;
	m_fPos_Y = 0;
	m_fXpos = 0;
	m_fYpos = 0;
}
//=============================================================================
// デストラクタ
//=============================================================================
CMeshWall::~CMeshWall()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMeshWall::Init(D3DXVECTOR3 pos)
{
	int nCntVtx = 0;
	int nCntIdx = 0;

	// 各初期化の設定
	m_nNumVertex = VERTEX_MW_KAZU;		// 頂点数
	m_nNumIndex = INDEX_MW_KAZU;		// インデックス数

	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// ポリゴンの位置を設定
	m_pos = pos;

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, MW_TEXTURE, &m_pTexture);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX_MW_KAZU,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// インデックスの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * INDEX_MW_KAZU,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,			// 2バイトのデータ
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// 頂点情報を設定
	VERTEX_3D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntY = 0; nCntY < MAX_MW_Y + 1; nCntY++)
	{// 縦の面の数だけ繰り返し
		for (int nCntX = 0; nCntX < MAX_MW_X + 1; nCntX++)
		{// 横の面の数だけ繰り返し
			pVtx[(nCntY + nCntX) + nCntVtx + m_nVtxIndex].pos = D3DXVECTOR3(m_fPos_X + (nCntX * X_POSSIZE), m_fPos_Y - (nCntY * Y_POSSIZE), 0.0f);
			pVtx[(nCntY + nCntX) + nCntVtx + m_nVtxIndex].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[(nCntY + nCntX) + nCntVtx + m_nVtxIndex].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntY + nCntX) + nCntVtx + m_nVtxIndex].tex = D3DXVECTOR2(0 + (nCntX * 0.5f), 0 + (nCntY * 0.5f));
		}
		// 頂点情報の値を保持
		nCntVtx += MAX_MW_X;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();


	WORD * pIdx;		// インデックスデータへのポインタ

	// インデックスバッファをロックし、インデックスデータへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdxY = 0; nCntIdxY < MAX_MW_Y; nCntIdxY++)
	{
		for (int nCntIdxX = 0; nCntIdxX < MAX_MW_X + 1; nCntIdxX++, nCntIdx++)
		{
			pIdx[0] = nCntIdx + MAX_MW_X + 1;
			pIdx[1] = nCntIdx;

			pIdx += 2;			// pIdxを2個分ずらす

			if (nCntIdxY < MAX_MW_Y && nCntIdxX == MAX_MW_X)
			{
				pIdx[0] = nCntIdx;
				pIdx[1] = nCntIdx + (MAX_MW_X + 1) + 1;

				pIdx += 2;				//pIdxを2個分ずらす
			}
		}
	}

	// インデックスバッファをアンロック
	m_pIdxBuff->Unlock();

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CMeshWall::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// インデックスバッファの解放
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// オブジェクト(自分自身)の破棄
	Release();
}
//=============================================================================
// 更新処理
//=============================================================================
void CMeshWall::Update(void)
{

}
//=============================================================================
// 描画処理
//=============================================================================
void CMeshWall::Draw(void)
{
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

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, VERTEX_MW_KAZU, 0, INDEX_MW_KAZU - 2);
}
//=============================================================================
// テクスチャの割り当て
//=============================================================================
void CMeshWall::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}
//=============================================================================
// 生成
//=============================================================================
CMeshWall * CMeshWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CMeshWall *pMeshWall = NULL;

	if (pMeshWall == NULL)
	{
		pMeshWall = new CMeshWall;

		if (pMeshWall != NULL)
		{
			pMeshWall->Init(pos);
			pMeshWall->m_rot = rot;
		}
	}
	return pMeshWall;
}