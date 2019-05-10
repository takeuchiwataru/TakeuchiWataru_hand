//=============================================================================
//
// 空処理 [sky.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "sky.h"
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
CSky::CSky()
{
	m_pTexture = NULL;		// テクスチャへのポインタ
	m_apVtxBuff[0] = NULL;	// 頂点バッファへのポインタ
	m_apVtxBuff[1] = NULL;	// 頂点バッファへのポインタ
	m_pIdxBuff = NULL;		// インデックスバッファのポインタ

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Length = 0;
	m_Side = 0;
	m_XBlock = 0;
	m_YBlock = 0;
	m_Addtex = 0.0f;		
}
//=============================================================================
// デストラクタ
//=============================================================================
CSky::~CSky()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSky::Init(D3DXVECTOR3 pos)
{	
	// 各初期化の設定
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Length = DOME_LENGTH;
	m_Side = DOME_SIDE;
	m_XBlock = BLOCK_SIDE;
	m_YBlock = BLOCK_LENGTH;

	m_nNumVertex = (m_XBlock + 1) * (m_YBlock + 1);		// 頂点数
	m_nNumIndex = ((m_XBlock + 1) * (m_YBlock + 1)) + (2 * (m_YBlock - 1)) + (m_XBlock + 1) * (m_YBlock - 1);		// インデックス数
	m_nNumPolygon = ((m_XBlock * m_YBlock) * 2) + ((m_YBlock - 1) * 4);	// ポリゴン数

	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// ポリゴンの位置を設定
	m_pos = pos;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,SKY_TEXTURE_NAME,&m_pTexture);

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_apVtxBuff[0],
		NULL);

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_apVtxBuff[0]->Lock(0, 0, (void**)&pVtx, 0);

	float fAngle = 0;   // 角度
	float fRadius = 0;  // 半径

	fAngle = 0.0f;							// 角度は0に
	fRadius = cosf(0.0f) * DOME_RADIUS;		// 半径を設定

	for (int nCntV = 0; nCntV < m_YBlock + 1; nCntV++)
	{// 垂直方向の分割数だけ繰り返し
		for (int nCntH = 0; nCntH < m_XBlock + 1; nCntH++)
		{// 水平方向の分割数だけ繰り返し
			pos.x = sinf(-fAngle * D3DX_PI) * fRadius;
			pos.y = sinf(0.15f * nCntV) * DOME_HEIGHT - 500.0f;
			pos.z = cosf(-fAngle * D3DX_PI) * fRadius;

			pVtx[nCntH].pos = pos;
			pVtx[nCntH].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
			pVtx[nCntH].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[nCntH].tex = D3DXVECTOR2(0.0f + 1.0f / (m_XBlock / 2), 0.0f + (1.0f * nCntV));

			fAngle += 1.0f / (m_XBlock / 2);	// 角度を進める

			if (fAngle >= 1.0f)
			{// 倍率が1を超えた
				fAngle = -1.0f;
			}
		}
		fRadius = cosf(0.26f * (nCntV + 1)) * DOME_RADIUS;	// 半径を徐々に減少させる
		fAngle = 0.0f;										// 角度は0に
		pVtx += m_XBlock + 1;								// 頂点の先頭の番号を進める
	}

	// 頂点バッファをアンロックする
	m_apVtxBuff[0]->Unlock();


	WORD *pIdx;      // インデックスデータへのポインタ
	int nCntIdx = 0; // インデックス番号

	// インデックスバッファをロックし,インデックスデータへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdxY = 0; nCntIdxY < m_YBlock; nCntIdxY++)
	{// 縦の分割数分繰り返し
		for (int nCntIdxX = 0; nCntIdxX < m_XBlock + 1; nCntIdxX++, nCntIdx++)
		{// 横の分割数+１繰り返し
		 // 上下の二つ分インデックス番号を設定
			pIdx[0] = nCntIdx + m_XBlock + 1; // 下側
			pIdx[1] = nCntIdx;                    // 上側

			pIdx += 2;  // 2つ分進める
			if (nCntIdxY < m_YBlock && nCntIdxX == m_XBlock)
			{// 1 , 下側が縦の分割数の最下層ではない
			 // 2 , 横の分割数分設定が終わった
				pIdx[0] = nCntIdx;						// 上側
				pIdx[1] = nCntIdx + (m_XBlock + 1) + 1;	// 次の下側

				pIdx += 2;	// 2つ分進める
			}
		}
	}

	// インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CSky::Uninit(void)
{
	// 頂点バッファの破棄
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_apVtxBuff[nCnt] != NULL)
		{
			m_apVtxBuff[nCnt]->Release();
			m_apVtxBuff[nCnt] = NULL;
		}
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
void CSky::Update(void)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_apVtxBuff[0]->Lock(0, 0, (void**)&pVtx, 0);

	m_Addtex += SKY_ROT;  // 加える分を進める

	for (int nCntV = 0; nCntV <m_YBlock + 1; nCntV++)
	{// 垂直方向の分割数だけ繰り返し
		for (int nCntH = 0; nCntH <m_XBlock + 1; nCntH++)
		{//水平方向の分割数だけ繰り返し
		 // テクスチャ座標
			pVtx[nCntH].tex = D3DXVECTOR2(0.0f + (nCntH * (1.0f /m_XBlock)) + m_Addtex, 0.0f - (nCntV * (1.0f /m_YBlock)));
		}
		pVtx +=m_XBlock + 1;       // 頂点の先頭の番号を進める
	}

	// 頂点バッファをアンロックする
	m_apVtxBuff[0]->Unlock();

	m_rot.y += SKY_ROT;
}
//=============================================================================
// 描画処理
//=============================================================================
void CSky::Draw(void)
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
	pDevice->SetStreamSource(0, m_apVtxBuff[0], 0, sizeof(VERTEX_3D));

	// インデックスデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);
}
//=============================================================================
// テクスチャの割り当て
//=============================================================================
void CSky::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}
//=============================================================================
// 生成
//=============================================================================
CSky * CSky::Create(D3DXVECTOR3 pos)
{
	CSky *pSky = NULL;

	if (pSky == NULL)
	{
		pSky = new CSky;

		if (pSky != NULL)
		{
			pSky->Init(pos);
		}
	}
	return pSky;
}
