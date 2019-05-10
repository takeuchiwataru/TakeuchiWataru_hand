//=============================================================================
//
// �c�@���� [remain.h]
// Author : �|���j
//
//=============================================================================
#ifndef _REMAIN_H_
#define _REMAIN_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �O���錾 �C���N���[�h�͂���Ȃ�
//*****************************************************************************
class CNumber;
class CEnemy;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_REMAIN	(2)					// �ő包��
#define REMAIN_INTERVAL_SIZE	(80)	// �c�@�Ԋu
#define REMAIN_SIZE	(45)				// �c�@�T�C�Y
#define R_REMAIN_SIZE	(30)			// �c�@�T�C�Y(���U���g)
#define R_REMAIN_INTERVAL_SIZE	(55)	// �c�@�Ԋu(���U���g)
#define REMAIN_DEGIT	(10)			// ����

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CRemain : public CScene
{
public:
	CRemain();
	~CRemain();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddRemain(int nValue);
	void SetColor(D3DXCOLOR col);

	// �ÓI�����o�֐�
	static CRemain * Create(D3DXVECTOR3 pos);

private:
	CNumber * m_apNumber[MAX_REMAIN];
	int m_nRemain;
	D3DXVECTOR3	m_pos;			// �ʒu
	int m_NumEnemy;				// �G�̑����ۊ�
};
#endif