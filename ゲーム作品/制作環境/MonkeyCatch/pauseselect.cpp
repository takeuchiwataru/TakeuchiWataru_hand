//=============================================================================
//
// ポーズ項目処理 [pauseselect.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "pauseselect.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "pause.h"
#include "sound.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 CPauseSelect::m_apTexture[RESTART_MAX] = {};
CPauseSelect::RESTART CPauseSelect::m_Restart = RESTART_CONTINUE;

//=============================================================================
// コンストラクタ
//=============================================================================
CPauseSelect::CPauseSelect() : CScene(7)
{
	m_nSelect = 0;
	for (int nCntPauseSelect = 0; nCntPauseSelect < MAX_PAUSE_SELECT; nCntPauseSelect++)
	{
		m_apLogo[nCntPauseSelect] = {};
		m_aCol[nCntPauseSelect] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}
}
//=============================================================================
// デストラクタ
//=============================================================================
CPauseSelect::~CPauseSelect()
{

}
//=============================================================================
// ポーズ項目初期化処理
//=============================================================================
HRESULT CPauseSelect::Init(D3DXVECTOR3 pos)
{
	for (int nCntPauseSelect = 0; nCntPauseSelect < MAX_PAUSE_SELECT; nCntPauseSelect++)
	{
		m_apLogo[nCntPauseSelect] = new CLogo(7);

		m_apLogo[nCntPauseSelect]->Init(D3DXVECTOR3(pos.x, pos.y - PAUSESELECT_INTERVAL + (nCntPauseSelect * PAUSESELECT_INTERVAL), 0.0f));
		m_apLogo[nCntPauseSelect]->BindTexture(m_apTexture[nCntPauseSelect]);

		m_aSelect[nCntPauseSelect] = PAUSESELECT_NONE;
		m_aCol[nCntPauseSelect] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}

	m_aSelect[0] = PAUSESELECT_SELECT;

	return S_OK;
}
//=============================================================================
// ポーズ項目終了処理
//=============================================================================
void CPauseSelect::Uninit(void)
{

	for (int nCntPauseSelect = 0; nCntPauseSelect < MAX_PAUSE_SELECT; nCntPauseSelect++)
	{
		m_apLogo[nCntPauseSelect]->Uninit();
		m_apLogo[nCntPauseSelect] = NULL;
	}
	// オブジェクト(自分自身)の破棄
	Release();

}
//=============================================================================
// ポーズ項目更新処理
//=============================================================================
void CPauseSelect::Update(void)
{
	// キーボードの取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// ジョイパッドの取得
	CInputJoyPad *pInputJoyPad = CManager::GetJoyPad();

	// サウンドの取得
	CSound * pSound = CManager::GetSound();

	// フェードの取得
	CFade *pFade = CManager::GetFade();

	// 選択項目移動処理
	if (pInputKeyboard->GetTrigger(DIK_S) == true || pInputKeyboard->GetTrigger(DIK_DOWN) == true ||
		pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_23) == true ||
		pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_31) == true)
	{// S or ↓　or 左スティック↓ or 十字キー↓が押されたら
		m_aSelect[m_nSelect] = PAUSESELECT_NONE;
		m_nSelect = (m_nSelect + 1) % MAX_PAUSE_SELECT;
		m_aSelect[m_nSelect] = PAUSESELECT_SELECT;
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SELECT);
	}

	if (pInputKeyboard->GetTrigger(DIK_W) == true || pInputKeyboard->GetTrigger(DIK_UP) == true ||
		pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_21) == true ||
		pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_29) == true)
	{// W or ↑ or 左スティック↑ or 十字キー↑が押されたら
		m_aSelect[m_nSelect] = PAUSESELECT_NONE;
		m_nSelect = (m_nSelect + (MAX_PAUSE_SELECT - 1)) % MAX_PAUSE_SELECT;
		m_aSelect[m_nSelect] = PAUSESELECT_SELECT;
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SELECT);
	}

	for (int nCntPauseSelect = 0; nCntPauseSelect < MAX_PAUSE_SELECT; nCntPauseSelect++)
	{
		if (m_aSelect[nCntPauseSelect] == PAUSESELECT_SELECT)
		{// 選択されているとこ
			m_aCol[m_nSelect] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else
		{// 選択されていないとこ
			m_aCol[m_nSelect] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		if (m_apLogo[nCntPauseSelect] != NULL)
		{
			m_apLogo[nCntPauseSelect]->SetColor(m_aCol[m_nSelect]);
		}
	}
	// 画面遷移
	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
		pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_1) == true ||
		pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_2) == true ||
		pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_3) == true ||
		pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_4) == true)
	{// 対象のボタンが押されたら
		switch (m_nSelect)
		{
		case RESTART_CONTINUE:
			m_Restart = RESTART_CONTINUE; // ゲーム再開状態に
			break;
		case RESTART_RETRY:
			m_Restart = RESTART_RETRY;    // ゲームやり直し状態に
			break;
		case RESTART_QUIT:
			m_Restart = RESTART_QUIT;     // タイトル遷移状態に
			break;
		}
	}
}
//=============================================================================
// ポーズ項目描画処理
//=============================================================================
void CPauseSelect::Draw(void)
{

}
//=============================================================================
// 項目の取得
//=============================================================================
CPauseSelect::RESTART CPauseSelect::GetRstart(void)
{
	return m_Restart;
}
//=============================================================================
// 生成
//=============================================================================
CPauseSelect * CPauseSelect::Create()
{
	CPauseSelect *pPauseSelect = NULL;

	if (pPauseSelect == NULL)
	{
		pPauseSelect = new CPauseSelect;

		pPauseSelect->Init(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
	}
	return pPauseSelect;
}
//=============================================================================
// テクスチャの読み込み
//=============================================================================
HRESULT CPauseSelect::Load(void)
{
	CManager manager;	// インスタンス

						// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, PAUSESELECT_TEXTURE000, &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, PAUSESELECT_TEXTURE001, &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, PAUSESELECT_TEXTURE002, &m_apTexture[2]);

	return S_OK;
}
//=============================================================================
// テクスチャの破棄
//=============================================================================
void CPauseSelect::Unload(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < MAX_PAUSE_SELECT; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}
