//=============================================================================
//
// ロゴ処理 [logo.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "logo.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 CTitleLogo::m_pTexture = NULL;
LPDIRECT3DTEXTURE9 CResultLogo::m_apTexture[RESULTTYPE_MAX] = {};
LPDIRECT3DTEXTURE9 CTutorialLogo::m_pTexture = NULL;
LPDIRECT3DTEXTURE9 CRankingLogo::m_pTexture = NULL;
LPDIRECT3DTEXTURE9 CPressEnter::m_apTexture[PRESSENTER_MAX] = {};
bool CPressEnter::m_bDisp = false;

//=============================================================================
// コンストラクタ
//=============================================================================
CLogo::CLogo(int nPriority) : CScene(nPriority)
{
	m_pTexture = NULL;		// テクスチャへのポインタ
	m_pVtxBuff = NULL;		// 頂点バッファへのポインタ

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}
//=============================================================================
// デストラクタ
//=============================================================================
CLogo::~CLogo()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLogo::Init(D3DXVECTOR3 pos)
{
	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	m_pos = pos;

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
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - LOGO_SIZE_X, m_pos.y - LOGO_SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + LOGO_SIZE_X, m_pos.y - LOGO_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - LOGO_SIZE_X, m_pos.y + LOGO_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + LOGO_SIZE_X, m_pos.y + LOGO_SIZE_Y, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 色
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CLogo::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// オブジェクト(自分自身)の破棄
	Release();
}
//=============================================================================
// 更新処理
//=============================================================================
void CLogo::Update(void)
{

}
//=============================================================================
// 描画処理
//=============================================================================
void CLogo::Draw(void)
{
	CManager manager;	//インスタンス

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
// 位置の設置
//=============================================================================
void CLogo::SetPosition(D3DXVECTOR3 pos, float fwidth, float fheight)
{
	m_pos = pos;

	// 頂点情報を設定
	VERTEX_2D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_pos.x - fwidth, m_pos.y - fheight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + fwidth, m_pos.y - fheight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - fwidth, m_pos.y + fheight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + fwidth, m_pos.y + fheight, 0.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//=============================================================================
// テクスチャの割り当て
//=============================================================================
void CLogo::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}
//=============================================================================
// カラーの設置
//=============================================================================
void CLogo::SetColor(D3DXCOLOR col)
{
	m_col = col;

	// 頂点情報を設定
	VERTEX_2D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//=============================================================================
// 頂点バッファの取得
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CLogo::GetVtxBuff(void)
{
	return m_pVtxBuff;
}
//=============================================================================
// コンストラクタ
//=============================================================================
CTitleLogo::CTitleLogo(): CLogo(3)
{

}
//=============================================================================
// デストラクタ
//=============================================================================
CTitleLogo::~CTitleLogo()
{

}
//=============================================================================
// タイトルロゴ初期化処理
//=============================================================================
HRESULT CTitleLogo::Init(D3DXVECTOR3 pos)
{
	CLogo::Init(pos);

	// 位置の設置
	CLogo::SetPosition(pos, TITLE_SIZE_X, TITLE_SIZE_Y);

	return S_OK;
}
//=============================================================================
// タイトルロゴ終了処理
//=============================================================================
void CTitleLogo::Uninit(void)
{
	CLogo::Uninit();
}
//=============================================================================
// タイトルロゴ更新処理
//=============================================================================
void CTitleLogo::Update(void)
{

}
//=============================================================================
// タイトルロゴ描画処理
//=============================================================================
void CTitleLogo::Draw(void)
{
	CLogo::Draw();
}
//=============================================================================
// 生成
//=============================================================================
CTitleLogo * CTitleLogo::Create(D3DXVECTOR3 pos)
{
	CTitleLogo *pTitleLogo = NULL;

	if (pTitleLogo == NULL)
	{
		pTitleLogo = new CTitleLogo;

		if (pTitleLogo != NULL)
		{
			pTitleLogo->Init(pos);
			pTitleLogo->BindTexture(m_pTexture);
		}
	}
	return pTitleLogo;
}
//=============================================================================
// テクスチャの読み込み
//=============================================================================
HRESULT CTitleLogo::Load(void)
{
	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TITLELOGO_TEXTURE, &m_pTexture);

	return S_OK;
}
//=============================================================================
// テクスチャの破棄
//=============================================================================
void CTitleLogo::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
//=============================================================================
// コンストラクタ
//=============================================================================
CResultLogo::CResultLogo() :CLogo(3)
{

}
//=============================================================================
// デストラクタ
//=============================================================================
CResultLogo::~CResultLogo()
{

}
//=============================================================================
// リザルトロゴ初期化処理
//=============================================================================
HRESULT CResultLogo::Init(D3DXVECTOR3 pos, RESULTTYPE type)
{
	CLogo::Init(pos);

	// 位置の設置
	CLogo::SetPosition(pos, RESULT_SIZE_X, RESULT_SIZE_Y);

	m_nType = type;

	return S_OK;
}
//=============================================================================
// リザルトロゴ終了処理
//=============================================================================
void CResultLogo::Uninit(void)
{
	CLogo::Uninit();
}
//=============================================================================
// リザルトロゴ更新処理
//=============================================================================
void CResultLogo::Update(void)
{

}
//=============================================================================
// リザルトロゴ描画処理
//=============================================================================
void CResultLogo::Draw(void)
{
	CLogo::Draw();
}
//=============================================================================
// 生成
//=============================================================================
CResultLogo * CResultLogo::Create(D3DXVECTOR3 pos, RESULTTYPE type)
{
	CResultLogo *pResultLogo = NULL;

	if (pResultLogo == NULL)
	{
		pResultLogo = new CResultLogo;

		if (pResultLogo != NULL)
		{
			pResultLogo->Init(pos,type);
			pResultLogo->BindTexture(m_apTexture[type]);
		}
	}
	return pResultLogo;
}
//=============================================================================
// テクスチャの読み込み
//=============================================================================
HRESULT CResultLogo::Load(void)
{
	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, RESULTLOGO_TEXTURE, &m_apTexture[0]);

	return S_OK;
}
//=============================================================================
// テクスチャの破棄
//=============================================================================
void CResultLogo::Unload(void)
{
	for (int nCntTex = 0; nCntTex < RESULTTYPE_MAX; nCntTex++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}
//=============================================================================
// コンストラクタ
//=============================================================================
CTutorialLogo::CTutorialLogo() :CLogo(3)
{

}
//=============================================================================
// デストラクタ
//=============================================================================
CTutorialLogo::~CTutorialLogo()
{

}
//=============================================================================
// チュートリアルロゴ初期化処理
//=============================================================================
HRESULT CTutorialLogo::Init(D3DXVECTOR3 pos)
{
	CLogo::Init(pos);

	// 位置の設置
	CLogo::SetPosition(pos, TUTORIAL_SIZE_X, TUTORIAL_SIZE_Y);

	return S_OK;
}
//=============================================================================
// チュートリアルロゴ終了処理
//=============================================================================
void CTutorialLogo::Uninit(void)
{
	CLogo::Uninit();
}
//=============================================================================
// チュートリアルロゴ更新処理
//=============================================================================
void CTutorialLogo::Update(void)
{

}
//=============================================================================
// チュートリアルロゴ描画処理
//=============================================================================
void CTutorialLogo::Draw(void)
{
	CLogo::Draw();
}
//=============================================================================
// 生成
//=============================================================================
CTutorialLogo * CTutorialLogo::Create(D3DXVECTOR3 pos)
{
	CTutorialLogo *pTutorial = NULL;

	if (pTutorial == NULL)
	{
		pTutorial = new CTutorialLogo;

		if (pTutorial != NULL)
		{
			pTutorial->Init(pos);
			pTutorial->BindTexture(m_pTexture);
		}
	}
	return pTutorial;
}
//=============================================================================
// テクスチャの読み込み
//=============================================================================
HRESULT CTutorialLogo::Load(void)
{
	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TUTORIALLOGO_TEXTURE, &m_pTexture);

	return S_OK;
}
//=============================================================================
// テクスチャの破棄
//=============================================================================
void CTutorialLogo::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
//=============================================================================
// コンストラクタ
//=============================================================================
CRankingLogo::CRankingLogo() :CLogo(3)
{

}
//=============================================================================
// デストラクタ
//=============================================================================
CRankingLogo::~CRankingLogo()
{

}
//=============================================================================
// ランキングロゴ初期化処理
//=============================================================================
HRESULT CRankingLogo::Init(D3DXVECTOR3 pos)
{
	CLogo::Init(pos);

	// 位置の設置
	CLogo::SetPosition(pos, RANKING_SIZE_X, RANKING_SIZE_Y);

	return S_OK;
}
//=============================================================================
// ランキングロゴ終了処理
//=============================================================================
void CRankingLogo::Uninit(void)
{
	CLogo::Uninit();
}
//=============================================================================
// ランキングロゴ更新処理
//=============================================================================
void CRankingLogo::Update(void)
{

}
//=============================================================================
// ランキングロゴ描画処理
//=============================================================================
void CRankingLogo::Draw(void)
{
	CLogo::Draw();
}
//=============================================================================
// 生成
//=============================================================================
CRankingLogo * CRankingLogo::Create(D3DXVECTOR3 pos)
{
	CRankingLogo *pRanking = NULL;

	if (pRanking == NULL)
	{
		pRanking = new CRankingLogo;

		if (pRanking != NULL)
		{
			pRanking->Init(pos);
			pRanking->BindTexture(m_pTexture);
		}
	}
	return pRanking;
}
//=============================================================================
// テクスチャの読み込み
//=============================================================================
HRESULT CRankingLogo::Load(void)
{
	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, RANKINGLOGO_TEXTURE, &m_pTexture);

	return S_OK;
}
//=============================================================================
// テクスチャの破棄
//=============================================================================
void CRankingLogo::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// コンストラクタ
//=============================================================================
CPressEnter::CPressEnter() : CLogo(3)
{
	m_PressEnterState = PRESSENTERSTATE_NONE;  // 押されていない状態にする
	m_nCounterPressEnter = 0;                  // カウンターは0にする
	m_bDisp = false;                           // 表示はしない
}
//=============================================================================
// デストラクタ
//=============================================================================
CPressEnter::~CPressEnter()
{

}
//=============================================================================
// プレスエンター初期化処理
//=============================================================================
HRESULT CPressEnter::Init(D3DXVECTOR3 pos, PRESSENTER type)
{
	CLogo::Init(pos);

	m_PressEnterState = PRESSENTERSTATE_NONE;  // 押されていない状態にする
	m_nCounterPressEnter = 0;                  // カウンターは0にする
	m_bDisp = false;                           // 表示はしない

	m_nType = type;

	// 位置の設置
	if (m_nType == PRESSENTER_TITLE)
	{// タイトル
		CLogo::SetPosition(pos, TITLE_PRESSENTER_SIZE_X, PRESSENTER_SIZE_Y);
	}
	else if(m_nType == PRESSENTER_TUTORIAL)
	{// チュートリアル
		CLogo::SetPosition(pos, TUTORIAL_PRESSENTER_SIZE_X, PRESSENTER_SIZE_Y);
	}

	return S_OK;
}
//=============================================================================
// プレスエンター終了処理
//=============================================================================
void CPressEnter::Uninit(void)
{
	CLogo::Uninit();
}
//=============================================================================
// プレスエンター更新処理
//=============================================================================
void CPressEnter::Update(void)
{
	// キーボードの取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// ジョイパッドの取得
	CInputJoyPad *pInputJoyPad = CManager::GetJoyPad();

	// モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();

	// フェードの取得
	CFade *pFade = CManager::GetFade();

	// サウンドの取得
	CSound * pSound = CManager::GetSound();

	if (m_nCounterPressEnter >= PRESSENTER_COUNTER)
	{
		m_nCounterPressEnter = 0;
	}

	if (mode == CManager::MODE_TITLE)
	{// タイトルだったら
		if (m_bDisp == true)
		{// 描画する状態ならば
			m_nCounterPressEnter++;
			if (m_PressEnterState != PRESSENTERSTATE_PUSH)
			{// 押された状態以外だったら
				if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
					pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_1) == true ||
					pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_2) == true ||
					pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_3) == true ||
					pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_4) == true)
				{// 対象のボタンが押されたら
					m_PressEnterState = PRESSENTERSTATE_PUSH;	// 押されている状態にする
					pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENETR);
				}
			}
		}
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{// チュートリアルだったら
		if (m_bDisp == true)
		{// 描画する状態ならば
			m_nCounterPressEnter++;
			if (m_PressEnterState != PRESSENTERSTATE_PUSH)
			{// 押された状態以外だったら
				if (pInputKeyboard->GetTrigger(DIK_1) == true ||
					pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_12) == true)
				{// 1 or START押された
					m_PressEnterState = PRESSENTERSTATE_PUSH;	// 押されている状態にする
					pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENETR);
				}
			}
		}
	}
}
//=============================================================================
// プレスエンター描画処理
//=============================================================================
void CPressEnter::Draw(void)
{
	CManager manager;	//インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// 頂点バッファの取得
	LPDIRECT3DVERTEXBUFFER9 VtxBuff = CLogo::GetVtxBuff();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, VtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_apTexture[m_nType]);

	// ポリゴンの描画
	if (m_bDisp == true)
	{// 描画する状態だったら
		if (m_PressEnterState == PRESSENTERSTATE_NONE)
		{// 押されていない状態だったら
			if (m_nCounterPressEnter / PRESSENTER_NONE == 0)
			{// カウンターが一定の値になったら
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
			}
		}
		else if (m_PressEnterState == PRESSENTERSTATE_PUSH)
		{// 押されている状態だったら
			if (m_nCounterPressEnter % PRESSENTER_PUSH == 0)
			{// カウンターが一定の値になったら
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
			}
		}
	}
}
//=============================================================================
// 生成
//=============================================================================
CPressEnter * CPressEnter::Create(D3DXVECTOR3 pos, PRESSENTER type)
{
	CPressEnter *pPressEnter = NULL;

	if (pPressEnter == NULL)
	{
		pPressEnter = new CPressEnter;

		if (pPressEnter != NULL)
		{
			pPressEnter->Init(pos,type);
			pPressEnter->BindTexture(m_apTexture[type]);
		}
	}
	return pPressEnter;
}
//=============================================================================
// テクスチャの読み込み
//=============================================================================
HRESULT CPressEnter::Load(void)
{
	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, PRESSENTER_TEXTURE, &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, PRESSENTER_TEXTURE002, &m_apTexture[1]);

	return S_OK;
}
//=============================================================================
// テクスチャの破棄
//=============================================================================
void CPressEnter::Unload(void)
{
	for (int nCntTex = 0; nCntTex < PRESSENTERSTATE_MAX; nCntTex++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}
//=============================================================================
// プレスエンターロゴを描画するかしないか
//=============================================================================
void CPressEnter::DispPressEnter(void)
{
	m_bDisp = true;
}
