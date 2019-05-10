//=============================================================================
//
// デバッグ表示処理 [debugproc.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "debugproc.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPD3DXFONT CDebugProc::m_pFont = NULL;
char CDebugProc::m_aStr[MAX_STR] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CDebugProc::CDebugProc()
{

}
//=============================================================================
// デストラクタ
//=============================================================================
CDebugProc::~CDebugProc()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
void CDebugProc::Init(void)
{
	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

#ifdef _DEBUG
	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
#endif

}
//=============================================================================
// 終了処理
//=============================================================================
void CDebugProc::Uninit(void)
{
#ifdef _DEBUG
	// デバッグ情報表示用フォントの破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
#endif
}
//=============================================================================
// デバッグ表示処理
//=============================================================================
void CDebugProc::Print(int nNumber, char * fmt, ...)
{
	char cStr[MAX_STR];
	cStr[0] = '\0';
	va_list ap;

	va_start(ap, fmt);
	vsprintf(cStr, fmt, ap);

	if (nNumber == 0)
	{
		wsprintf(&m_aStr[0], &cStr[0]);
	}
	else if (nNumber == 1)
	{
		strcat(&m_aStr[0], &cStr[0]);
	}
	va_end(ap);
}
//=============================================================================
// 描画処理
//=============================================================================
void CDebugProc::Draw(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// テキスト描画
	m_pFont->DrawText(NULL, m_aStr, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}