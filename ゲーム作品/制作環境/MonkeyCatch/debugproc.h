//=============================================================================
//
// �f�o�b�O�\������ [debugproc.h]
// Author : �|���j
//
//=============================================================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_STR (1024)

//*****************************************************************************
// �N���X�̒�`
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
	//static bool m_bDisplay;			// �\�����邩�ǂ���
};

#endif
