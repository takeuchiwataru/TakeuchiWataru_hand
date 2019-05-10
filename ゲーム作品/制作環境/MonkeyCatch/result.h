//=============================================================================
//
// ���U���g���� [result.h]
// Author : �|���j
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"

//*****************************************************************************
// �O���錾 �C���N���[�h�͂���Ȃ�
//*****************************************************************************
class CRemain;
class CScore;
class CTimer;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CResult
{
public:
	CResult();
	~CResult();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static CRemain *m_pRemain;
	static CTimer *m_pTimer;
	static CScore *m_pScore;
	static CScore *m_pScoreTime;
	static CScore *m_pScoreTotal;
	static CMeshField *m_pMeshField;
	static CCamera *m_pCamera;
	static CLight *m_pLight;
	
	int m_nCounter;		// �`�悷��^�C�~���O
	int m_nTime;		// �J�ڂ���^�C�~���O
};

#endif