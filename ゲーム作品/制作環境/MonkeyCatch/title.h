//=============================================================================
//
// �^�C�g������ [title.h]
// Author : �|���j
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CTitle
{
public:
	CTitle();
	~CTitle();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_nTime;		// �J�ڂ���^�C�~���O
	static CMeshField *m_pMeshField;
	static CCamera *m_pCamera;
	static CLight *m_pLight;
};

#endif