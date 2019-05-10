//=============================================================================
//
// デバッグ表示処理 [debugproc.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_STR (1024)

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CDebugProc
{
public:
	CDebugProc();
	~CDebugProc();

	void Init(void);
	void Uninit(void);

	static void Print(int nNumber,char * fmt, ...);
	static void Draw(void);

private:
	static LPD3DXFONT m_pFont;
	static char m_aStr[MAX_STR];
	//static bool m_bDisplay;			// 表示するかどうか
};

#endif
