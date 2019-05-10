//=============================================================================
//
// �`���[�g���A������ [tutorial.h]
// Author : �|���j
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"

//*****************************************************************************
// �O���錾 �C���N���[�h�͂���Ȃ�
//*****************************************************************************
class CPlayer;
class CMeshField;
class CObject;
class CSky;
class CScore;
class CRemain;
class CTimer;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CTutorial
{
public:
	CTutorial();
	~CTutorial();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CMeshField * GetMeshField(void);
	static CPlayer * GetPlayer(void);
	static CRemain * GetRemain(void);
	static CScore * GetScore(void);
	static CCamera * GetCamera(void);


private:
	static CPlayer *m_pPlayer;
	static CMeshField *m_pMeshField;
	static CScore *m_pScore;
	static CRemain *m_pRemain;
	static CCamera *m_pCamera;
	static CLight *m_pLight;
	static CTimer *m_pTimer;
};

#endif