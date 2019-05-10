//=============================================================================
//
// ポーズ処理 [pause.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"
#include "pauseselect.h"
#include "pausemat.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PAUSESELECT (3)

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CPause
{
public:
	CPause();
	~CPause();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPause * Create(void);

private:
	static CPauseSelect * m_PauseSelect;
	static CPauseMat * m_PauseMat;
};
#endif
