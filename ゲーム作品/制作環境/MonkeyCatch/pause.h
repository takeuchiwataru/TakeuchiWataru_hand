//=============================================================================
//
// �|�[�Y���� [pause.h]
// Author : �|���j
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"
#include "pauseselect.h"
#include "pausemat.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PAUSESELECT (3)

//*****************************************************************************
// �N���X�̒�`
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
