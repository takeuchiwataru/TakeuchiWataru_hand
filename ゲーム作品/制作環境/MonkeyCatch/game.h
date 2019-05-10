//=============================================================================
//
// �Q�[������ [game.h]
// Author : �|���j
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//*****************************************************************************
// �O���錾 �C���N���[�h�͂���Ȃ�
//*****************************************************************************
class CScene3D;
class CMeshField;
class CMeshOrbit;
class CPlayer;
class CEnemy;
class CObject;
class CSky;
class CMeshWall;
class CScore;
class CRemain;
class CLife;
class CTimer;

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CGame
{
public:
	CGame();
	~CGame();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CScene3D * GetScene3D(void);
	static CMeshField * GetMeshField(void);
	static CPlayer * GetPlayer(void);
	static CRemain * GetRemain(void);
	static CScore * GetScore(void);
	static CCamera * GetCamera(void);
	static CTimer * GetTimer(void);

private:
	static CPlayer *m_pPlayer;
	static CScene3D *m_pScene3D;
	static CMeshField *m_pMeshField;
	static CMeshOrbit *m_pMeshOrbit;
	static CScore * m_pScore;
	static CRemain *m_pRemain;
	static CCamera *m_pCamera;
	static CLight *m_pLight;
	static CTimer *m_pTimer;

	int m_nTime;		// �J�ڂ���^�C�~���O

};
#endif