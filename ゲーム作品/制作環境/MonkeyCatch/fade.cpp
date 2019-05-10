//=============================================================================
//
// フェード処理 [fade.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "fade.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
CFade::FADE CFade::m_fade = CFade::FADE::FADE_NONE;
CManager::MODE CFade::m_modeNext = {};
D3DXCOLOR CFade::m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

//=============================================================================
// コンストラクタ
//=============================================================================
CFade::CFade()
{
	m_pTexture = NULL;		// テクスチャへのポインタ
	m_pVtxBuff = NULL;		// 頂点バッファへのポインタ

	m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}
//=============================================================================
// デストラクタ
//=============================================================================
CFade::~CFade()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
void CFade::Init(CManager::MODE modeNext)
{
	// 情報の初期化
	m_fade = FADE_NONE;

	m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報を設定
	VERTEX_2D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 位置
	pVtx[0].pos = D3DXVECTOR3(FADE_POS_X, FADE_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(FADE_WIDTH, FADE_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(FADE_POS_X, FADE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(FADE_WIDTH, FADE_HEIGHT, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 色
	pVtx[0].col = m_colorFade;
	pVtx[1].col = m_colorFade;
	pVtx[2].col = m_colorFade;
	pVtx[3].col = m_colorFade;

	// 頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//=============================================================================
// 終了処理
//=============================================================================
void CFade::Uninit(void)
{
	// テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void CFade::Update(void)
{
	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)
		{// フェードイン状態
			m_colorFade.a -= FADE_TOMEI; // 画面を透明にする
			if (m_colorFade.a <= FADE_COLOR000)
			{// 画面が透明になりきった
				m_colorFade.a = FADE_COLOR000;
				m_fade = FADE_NONE;
			}
		}
		else if (m_fade == FADE_OUT)
		{// フェードアウト状態
			m_colorFade.a += FADE_NOTTOMEI; // 画面を不透明にする
			if (m_colorFade.a >= FADE_COLOR001)
			{// 画面が透明になりきった
				m_colorFade.a = FADE_COLOR001;
				m_fade = FADE_IN;

				// モードの設定
				CManager::SetMode(m_modeNext);
			}
		}
		VERTEX_2D *pVtx;   // 頂点情報へのポインタ

		 // 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラー
		pVtx[0].col = m_colorFade;
		pVtx[1].col = m_colorFade;
		pVtx[2].col = m_colorFade;
		pVtx[3].col = m_colorFade;

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void CFade::Draw(void)
{
	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
//=============================================================================
// フェードの生成
//=============================================================================
CFade * CFade::Create()
{
	CFade *pFade = NULL;

	if (pFade == NULL)
	{
		pFade = new CFade;

		if (pFade != NULL)
		{
			pFade->Init(m_modeNext);
		}
	}
	return pFade;
}
//=============================================================================
// フェードの設定
//=============================================================================
void CFade::Set(CManager::MODE modeNext,FADE fade)
{
	if (m_fade == FADE_NONE)
	{
		m_fade = FADE_OUT;
		m_modeNext = modeNext;
		m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		if (fade == FADE_IN)
		{
			m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		}
	}
}
//=============================================================================
// フェードの取得
//=============================================================================
CFade::FADE CFade::GetFade(void)
{
	return m_fade;
}
