//=============================================================================
//
// �^�C�}�[���� [timer.h]
// Author : �|���j
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �O���錾 �C���N���[�h�͂���Ȃ�
//*****************************************************************************
class CNumber;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_TIMER	(3)				// �ő包��
#define TIME_INTERVAL_SIZE	(57)	// �X�R�A�Ԋu
#define TIMER_SIZE	(30)			// �^�C�}�[�T�C�Y
#define TIMER_GAME	(150)			// �Q�[���^�C�}�[
#define TIMER_ZERO	(0)				// �^�C�}�[0
#define TIMER_DEGIT (10)			// ����
#define TIMER_SCORE	(10)			// �^�C�}�[�X�R�A
#define TIMER_RED	(30)			// �Ԃ��Ȃ鎞��		
#define TIMER_TEN	(10)			// 10�b�ȉ�
#define TIMER_SECOND	(60)		// �b
#define TIMER_ADD		(1)			// ���Z
#define TIMER_FLASH		(8)			// �_��
#define TIMER_FLASH2	(16)		// �_��

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CTimer : public CScene
{
public:
	CTimer();
	~CTimer();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddTime(int nValue);
	void SetColor(D3DXCOLOR col);
	void StopTimer(void);
	static int GetStopTimer(void);

	// �ÓI�����o�֐�
	static CTimer * Create(D3DXVECTOR3 pos);

private:
	CNumber * m_apNumber[MAX_TIMER];
	D3DXVECTOR3	m_pos;			// �ʒu
	int m_nTime;				// �^�C�}�[
	int	m_nTimerCnt;			// �^�C�}�[�p�J�E���g
	int	m_nFlashCnt;			// �_�ŃJ�E���g
	bool m_bTimerStop;			// �~�߂邩�~�߂Ȃ���
	static int m_nStopTimer;	// �~�܂������ԕۑ��p
};
#endif